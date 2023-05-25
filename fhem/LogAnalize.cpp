/*
 *
 *  Copyright (C) 2015 Jürg Müller, CH-5524
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see http://www.gnu.org/licenses/ .
 */

// Filtert aus einem log-File, das mit ComfortSoft und CS_Bruecke erstellt wurde,
// für die Wärmepumpe spezifischen Angaben heraus und speichert sie in fhem_tab.inc.
// Diese Angaben sollten im File 50_Elster.pm eingesetzt werden.

#include <string.h>

#include "VList.h"
#include "NUtils.h"
#include "KElsterTable.h"

#if 1

int main(int argc, const char * argv[])
{
  KSortItemList list1;
  KSortItemList list;
  const char * Name = "cs_log.txt";
  bool scan_input = true;
  
  if (argc == 2)
    Name = argv[1];
  
  if (!list1.ReadFile(Name))
  {
    printf("\nFile '%s' not found\n\n", Name);
    printf("Use: ./log_analize <filename>\n\n");
    printf("Where \"file\" is:\n"
           "  cs-log file (generated by CS-Bruecke),\n"
           "  scan-log (generated by can_scan)\n"
           "For log-file (generated by can_logger) use the program \"log_to_scan\"\n");
    return -1;
  }

  for (int i = list1.GetCount(); i >= 0; i--)
  {
    const char * str = list1.GetKey(i);
    if (str)
    {
      if (strstr(str, "Elster ") == str)
      {
        scan_input = false;
        list.AppendKey(str);
      } else
      if (strstr(str, "0x"))  // { 0x180, 0x0008, 0x00c8},
      {
        bool Ok;
        unsigned short recv_id;
        unsigned short elster_idx;
        unsigned short value;
        const char * p = str;
        while (*p != 'x')
          p++;
        
        p++;
        recv_id = NUtils::GetHex(p);
        Ok = !strncmp(p, ", 0x", 4) &&
             !(recv_id & 0xf878);
        if (Ok)
        {
          p += 4;
          elster_idx = NUtils::GetHex(p);
          Ok = !strncmp(p, ", 0x", 4);
        }
        if (Ok)
        {
          p += 4;
          value = NUtils::GetHex(p);
          Ok = *p == '}';
        }
        const ElsterIndex * el = NULL;
        if (Ok)
          el = GetElsterIndex(elster_idx);
        if (Ok && el &&
            value != 0 && (value & 0xff00) != 0x8000 &&
            recv_id != 0)
        {
          char st[128];
          sprintf(st, "Elster %3.3x.%4.4x.%4.4x: %s", recv_id, elster_idx, value, el->Name);
          list.AppendKey(st);
        }        
      }
    }
  }
  
  char new_name[1024];
  const char * file_name = NUtils::ExtractFilename(Name);
  
  new_name[0] = 0;
  if (Name != file_name)
  {
    strncpy(new_name, Name, strlen(Name) - strlen(file_name));
    new_name[strlen(Name) - strlen(file_name)] = 0;
  }
  strcat(new_name, "sorted_");
  strcat(new_name, file_name);
  
  list.Sort();
  
 // if (!list.SaveFile(new_name))
 //   printf("'%s' not saved\n", new_name);
  
  KStream tab;
  KStream set_tab;
  bool first = true;
  bool set_first = true;
  KSortItemList Keys;

  tab.AppendString("my %Elster_gets = (\n");
  set_tab.AppendString("my %Elster_sets = (\n");

  for (int i = 0; i < (int) list.GetCount(); i++)
  {
    const char * p = list.GetKey(i);
    if (p)
    {
      char dev[16];
      char id[16];
      char longdev[32];
      char str[512];
      char temp[256];
      bool sub = false;
      
      if (strstr(p, "= not available"))
        continue;
      
      p += 7;
      strncpy(dev, p, 3);
      dev[3] = 0;
      strncpy(id, p+4, 4);
      id[4] = 0;
      if (dev[0] == '0')
        strcpy(longdev, "$direct");
      else
      if (!strcmp(dev, "180"))
        strcpy(longdev, "$kessel");
      else
      if (dev[0] == '3' && dev[1] == '0')
      {
        strcpy(longdev, "$bedien_");
        strcat(longdev, dev + 2);
        sub = true;
      } else
      if (!strcmp(dev, "480"))
        strcpy(longdev, "$manager");
      else
      if (!strcmp(dev, "500"))
        strcpy(longdev, "$heiz");
      else
      if (dev[0] == '6' && dev[1] == '0')
      {
        strcpy(longdev, "$mischer_");
        strcat(longdev, dev + 2);
        sub = true;
      } else
        strcpy(longdev, dev);
      
      strcpy(temp, p+15);
      int i = 0;
      while (temp[i] && temp[i] != ' ')
        i++;
      temp[i] = 0;
      if (sub)
      {
        sprintf(temp + strlen(temp), "_%c", dev[2]);
      }
      
      const char * p_id = id;
      unsigned short elster_index = NUtils::GetHex(p_id);
      const ElsterIndex * ei = GetElsterIndex(elster_index);
      const ElsterIndex * ei1 = GetElsterIndex(elster_index+1);
      if (ei && ei1)
      {
        if (ei->Type == et_default && ei1->Type == et_double_val)
          continue;
      
        if (ei->Type == et_double_val && ei1->Type == et_triple_val)
          continue;
      }
      
      bool can_change = false;
      bool dont_use = false;
      const char * p = id;
      unsigned id_int = NUtils::GetHex(p);
      p = dev;
      unsigned dev_int = NUtils::GetHex(p);

      if (0x0b00 <= id_int && id_int <= 0x0bff) // Fehlerfeld
      {
        dont_use = dev_int != 0x180;
      } else
      if ((0x1400 <= id_int && id_int <= 0x14c2) || // Heizprog.
          (0x1500 <= id_int && id_int <= 0x15c2))
      {
        can_change = (dev_int & 0xfff0) == 0x300;
        dont_use = !can_change;
      } else
      if (0x1700 <= id_int && id_int <= 0x17ff) // Warmwasserprog.
      {
        can_change = dev_int == 0x180;
        dont_use = !can_change;
      } else
      switch (id_int)
      {
        case 0x005: // Raumtemp. Tag Soll
        case 0x008: // Raumtemp. Nacht Soll
          can_change = (dev_int & 0xfff0) == 0x300;
          dont_use = !can_change;
          break;

        case 0x013: // Warmwasser Tag Soll
        case 0xa06: // Warmwasser Nacht Soll

        case 0x112: // Betriebsart

        case 0x122: // Tag
        case 0x123: // Monat
        case 0x124: // Jahr
        case 0x125: // Stunde
        case 0x126: // Minute
        case 0x127: // Sekunde
            
        case 0x11b: case 0x11c: case 0x11d: // Ferienanfang
        case 0x11e: case 0x11f: case 0x120: // Ferienende
            
        //case 0x135: // Warmwasserbetrieb
        case 0x27e: // Warmwasser ECO
            
          
        case 0xfdb4: // Sommerbetrieb
        case 0xfdb9:
        case 0xfdbf: // Party Dauer
           can_change = dev_int == 0x180;
           dont_use = !can_change;
          break;

        case 0x2f3: // ANFORDERUNG_NACHHEIZUNG

        case 0x003: // SPEICHERSOLLTEMP
        case 0x004: // VORLAUFSOLLTEMP
        case 0x00c: // AUSSENTEMP
        case 0x00d: // SAMMLERISTTEMP
        case 0x00e: // SPEICHERISTTEMP
        case 0x00f: // VORLAUFISTTEMP
        case 0x016: // RUECKLAUFISTTEMP
        case 0x022: // HYSTERESEZEIT
        case 0x053: // HZK_PUMPE
        case 0x063: // KESSELSTATUS
        case 0x064: // SAMMLER_PUMPE
        case 0x113: // SPRACHE
        case 0x11a: // ESTRICHFUNKTION
        case 0x135: // WARMWASSERMODE
        case 0x181: // MAX_WW_TEMP
        case 0x1ac: // BIVALENTPARALLELTEMPERATUR_HZG
        case 0x1ad: // BIVALENTPARALLELTEMPERATUR_WW
        case 0x1ae: // BIVALENZALTERNATIVTEMPERATUR_HZG
        case 0x1af: // BIVALENZALTERNATIVTEMPERATUR_WW
        case 0x1b7: // AUSSENTEMPERATUR_WARMWASSER
        case 0x1bf: // SW_AUSSENTEMP
        case 0x1c0: // FESTWERT
        case 0x1ca: // LAUFZEIT_SOLAR
        case 0x1cb: // LAUFZEIT_2WE
        case 0x1d4: // QUELLE_IST
        case 0x1d5: // PUFFERSOLL
        case 0x1d6: // WPVORLAUFIST
        case 0x1e8: // MAX_HEIZUNG_TEMP
        case 0xa00: // AUSSEN_FROSTTEMP
        case 0xfda8:// FEHLERANZAHL
        case 0xfdaa:// BUSKONTROLLE
        case 0xfdaf:// DAUERLAUF_PUFFERLADEPUMPE
        case 0xfdb0:// SCHALTWERKDYNAMIKZEIT
        case 0xfdb1:// STILLSTANDZEIT
        case 0xfdb2:// PUMPENZYKLEN
        case 0xfdb3:// GEBAEUDEART
        case 0xfdba:// ZWEITER_WE_STATUS
          dont_use = dev_int != 0x180;
          break;

        case 0x011: // RAUMISTTEMP
        case 0x028: // MAX_TEMP_KESSEL
        case 0x058: // MISCHER_ZU_
        case 0x10e: // HEIZKURVE
          dont_use = (dev_int & 0xfff0) != 0x300;
          break;

        case 0x808: // ABTAUZEIT_VERD1
        case 0x809: // ABTAUZEIT_VERD2
        case 0x91b: // EL_AUFNAHMELEISTUNG_WW_TAG_KWH
        case 0x91d: // EL_AUFNAHMELEISTUNG_WW_SUM_MWH
        case 0x91f: // EL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH
        case 0x921: // EL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH
        case 0x923: // WAERMEERTRAG_2WE_WW_TAG_KWH
        case 0x925: // WAERMEERTRAG_2WE_WW_SUM_MWH
        case 0x927: // WAERMEERTRAG_2WE_HEIZ_TAG_KWH
        case 0x929: // WAERMEERTRAG_2WE_HEIZ_SUM_MWH
        case 0x92b: // WAERMEERTRAG_WW_TAG_KWH
        case 0x92d: // WAERMEERTRAG_WW_SUM_MWH
        case 0x92f: // WAERMEERTRAG_HEIZ_TAG_KWH
        case 0x931: // WAERMEERTRAG_HEIZ_SUM_MWH
          dont_use = dev_int != 0x500;
          break;

        case 0x180: // CODENUMMER
        case 0x00a: // DATUM
        case 0x25f: // FLAECHENKUEHLUNG
        case 0x010: // GERAETEKONFIGURATION
        case 0x262: // HYSTERESE_FLAECHE
        case 0x0fe: //
        case 0xfdde:// KESSELFOLGE2_10
        case 0x7fc: // LZ_VERD_1_HEIZBETRIEB
        case 0x7ff: // LZ_VERD_1_KUEHLBETRIEB
        case 0x802: // LZ_VERD_1_WW_BETRIEB
        case 0x1a2: // MAX_WASSERDRUCK
        case 0x7a4: // MESSSTROM_HOCHDRUCK
        case 0xfde0:// MONAT_SOMMER_BEGIN
        case 0x1b0: // QUELLENSOLLTEMPERATUR
        case 0x261: // RAUMSOLL_FLAECHE
        case 0x199: // SOFTWARE_NUMMER
        case 0x19a: // SOFTWARE_VERSION
        case 0x1b1: // SOLLTEMP_ANZEIGE_0_1
        case 0x1b2: // SOLLTEMP_ANZEIGE_0_2
        case 0x1b3: // SOLLTEMP_ANZEIGE_0_3
        case 0x1b4: // SOLLTEMP_ANZEIGE_1_1
        case 0x1b5: // SOLLTEMP_ANZEIGE_1_2
        case 0x1b6: // SOLLTEMP_ANZEIGE_1_3
        case 0x05a: // SPEICHER_STATUS
        case 0x026: // SPERRZEIT
        case 0xfddf:// TAG_SOMMER_BEGIN
        case 0xfde1:// TAG_SOMMER_ENDE
        case 0x706: // TEST_OBJEKT_197
        case 0x708: // TEST_OBJEKT_199
        case 0x006: // RAUMSOLLTEMP_II
        case 0x709: //
        case 0x70a: // TEST_OBJEKT_201
        case 0x70b: // TEST_OBJEKT_202
        case 0x70c: // TEST_OBJEKT_203
        case 0x68e: // TEST_OBJEKT_77
        case 0x6a4: // TEST_OBJEKT_99
        case 0x02b: // TN
        case 0x009: // UHRZEIT
        case 0x260: // VORLAUFSOLL_FLAECHE
        case 0xfdb8:// WAERMEMENGE
        case 0x121: // WOCHENTAG
        case 0x263: // WWKORREKTUR
        case 0x10d: // CODENUMMER
          dont_use = dev_int != 0x180;
          break;

        case 0x075: // FEUCHTE
        case 0x007: // RAUMSOLLTEMP_III
        case 0x012: // VERSTELLTE_RAUMSOLLTEMP
          dont_use = (dev_int & 0xfff0) != 0x300;
          break;

        case 0x1be: // ESTRICH_HALTEN_MAX_TEMPERATUR
        case 0x1bc: // ESTRICH_HALTEN_SOCKELTEMPERATUR
        case 0x1bd: // ESTRICH_MAX_TEMPERATUR
        case 0x1bb: // ESTRICH_SOCKELTEMPERATUR
        case 0x1ba: // ESTRICH_STEIGUNG_PRO_TAG
        case 0x183: // FERNBEDIENUNGSZUORDNUNG
        case 0x25e: // HYSTERESE_GEBLAESE
        case 0x288: // KUEHLDYNAMIK_FLAECHE
        case 0x289: // KUEHLDYNAMIK_GEBLAESE
        case 0x25d: // RAUMSOLL_GEBLAESE
        case 0x10f: // RAUMEINFLUSS
        case 0x294: // WE1_TYP
          dont_use = dev_int != 0x480;
          break;

        default:
          break;
      }

      if (!dont_use || !scan_input)
      {
        bool exists = Keys.FindItem(temp) >= 0;
        if (exists)
          printf("%s ist doppelt\n", temp);
        else
          Keys.AppendKey(temp);

        if (!first)
        {
          tab.AppendString(",\n");
        }
        if (exists)
          tab.AppendString("# ");
          
        sprintf(str, "  \"%s\"   => { EIdx => \"%s\", ID => \"%s\" } ",
                temp, id, dev);
        tab.AppendString(str);
        first = false;
        if (can_change)
        {
          if (!set_first)
            set_tab.AppendString(",\n");
          set_tab.AppendString(str);
          set_first = false;
        }
      }
    }
  }
  tab.AppendString("\n);\n");
  set_tab.AppendString("\n);\n\n");
  set_tab.AppendString(tab.GetCharMemory());

  new_name[0] = 0;
  if (Name != file_name)
  {
    strncpy(new_name, Name, strlen(Name) - strlen(file_name));
    new_name[strlen(Name) - strlen(file_name)] = 0;
  }
  
  strcat(new_name, "fhem_tab.inc");
  if (set_tab.SaveFile(new_name))
    printf("\nFile '%s' saved\n\n", new_name);
  
  return 0;
}

#endif

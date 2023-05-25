my $ElsterVersion = "0.1";
################################################################################
# This script implements an interface to a Stiebel-Eltron Heatpump using the
# CAN bus. Supported interfaces are: SocketCAN, USBtin and Lawicel CANUSB/232.
#
# Do not use the set function to write to the pump unless you know what you are
# doing.
################################################################################
#
#  Einige Teile sind von WPL15 (Radiator / Hartmut Schmidt) kopiert worden. 
#
#  Copyright (C) 2015 Jürg Müller, CH-5524
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation version 3 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program. If not, see http://www.gnu.org/licenses/ .
#
################################################################################
#
# Vorgehen bei älteren Wärmepumpen-Managern (vor WPM 3):
#
#  - "totalen Scan" erstellen (siehe can_scan)
#  - den Scan in CS_Bruecke laden und den Simulationsmodus verwenden
#  - in CS_Bruecke das log-File aktivieren
#  - ComfortSoft verwenden. Alle Seiten/Geräte (kurz alles) einmal anwählen.
#    Alle Daten sollten so in das log-File gelangen.
#  - "LogAnalize" verwenden:
#    LogAnalize ../win/cs_log.txt
#    dabei entsteht das File "fhem_tab.inc"
#  - "fhem_tab.inc" in diesem File in die Strukturen "Elster_gets" und
#    "Elster_sets" ersetzen.
#  - fertig!
#
################################################################################
#
# Rasperry Pi:
#
#  sudo cp 50_Elster.pm   /opt/fhem/FHEM
#  sudo cp elster_perl.pm /opt/fhem/FHEM
#  sudo cp elster_perl.so /opt/fhem/FHEM
#  sudo /etc/init.d/fhem start     (für neue Installationen nicht mehr nötig)
#
# debug: perl -d fhem.pl fhem.cfg 
################################################################################
#
# Im Browser Fhem starten:
#  http://localhost:8083/fhem
#
# fhem:
#  reload 50_Elster
#  define Stiebel Elster can0
#

package main;
use strict;
use warnings;
use Time::HiRes qw(gettimeofday);
use feature ":5.10";
use SetExtensions;
use elster_perl; # SWIG wrapper zu elster_perl.so (C++-Modul)
use Data::Dumper;

my $direkt    = "000";
my $kessel    = "180";
my $bedien_1  = "301";
my $bedien_2  = "302";
my $manager   = "480";
my $heiz      = "500";
my $mischer_1 = "601";
my $mischer_2 = "602";

my $sender_id = "680";
my $trace_all = 1;

my $sniffed_set = 0;

#
# Mit LogAnalize.exe automatisch erstellte Tabellen:
#
my %Elster_sets = (
  "EINSTELL_SPEICHERSOLLTEMP"   => { EIdx => "0013", ID => "180" } ,
  "PROGRAMMSCHALTER"   => { EIdx => "0112", ID => "180" } ,
  "FERIENANFANG_TAG"   => { EIdx => "011b", ID => "180" } ,
  "FERIENANFANG_MONAT"   => { EIdx => "011c", ID => "180" } ,
  "FERIENANFANG_JAHR"   => { EIdx => "011d", ID => "180" } ,
  "FERIENENDE_TAG"   => { EIdx => "011e", ID => "180" } ,
  "FERIENENDE_MONAT"   => { EIdx => "011f", ID => "180" } ,
  "FERIENENDE_JAHR"   => { EIdx => "0120", ID => "180" } ,
  "TAG"   => { EIdx => "0122", ID => "180" } ,
  "MONAT"   => { EIdx => "0123", ID => "180" } ,
  "JAHR"   => { EIdx => "0124", ID => "180" } ,
  "STUNDE"   => { EIdx => "0125", ID => "180" } ,
  "MINUTE"   => { EIdx => "0126", ID => "180" } ,
  "SEKUNDE"   => { EIdx => "0127", ID => "180" } ,
  "WW_ECO"   => { EIdx => "027e", ID => "180" } ,
  "EINSTELL_SPEICHERSOLLTEMP2"   => { EIdx => "0a06", ID => "180" } ,
  "W_WASSERPROG_1_MO"   => { EIdx => "1710", ID => "180" } ,
  "W_WASSERPROG_1_MO_SCHALT_2"   => { EIdx => "1711", ID => "180" } ,
  "W_WASSERPROG_1_MO_SCHALT_3"   => { EIdx => "1712", ID => "180" } ,
  "W_WASSERPROG_1_DI"   => { EIdx => "1720", ID => "180" } ,
  "W_WASSERPROG_1_DI_SCHALT_2"   => { EIdx => "1721", ID => "180" } ,
  "W_WASSERPROG_1_DI_SCHALT_3"   => { EIdx => "1722", ID => "180" } ,
  "W_WASSERPROG_1_MI"   => { EIdx => "1730", ID => "180" } ,
  "W_WASSERPROG_1_MI_SCHALT_2"   => { EIdx => "1731", ID => "180" } ,
  "W_WASSERPROG_1_MI_SCHALT_3"   => { EIdx => "1732", ID => "180" } ,
  "W_WASSERPROG_1_DO"   => { EIdx => "1740", ID => "180" } ,
  "W_WASSERPROG_1_DO_SCHALT_2"   => { EIdx => "1741", ID => "180" } ,
  "W_WASSERPROG_1_DO_SCHALT_3"   => { EIdx => "1742", ID => "180" } ,
  "W_WASSERPROG_1_FR"   => { EIdx => "1750", ID => "180" } ,
  "W_WASSERPROG_1_FR_SCHALT_2"   => { EIdx => "1751", ID => "180" } ,
  "W_WASSERPROG_1_FR_SCHALT_3"   => { EIdx => "1752", ID => "180" } ,
  "W_WASSERPROG_1_SA"   => { EIdx => "1760", ID => "180" } ,
  "W_WASSERPROG_1_SA_SCHALT_2"   => { EIdx => "1761", ID => "180" } ,
  "W_WASSERPROG_1_SA_SCHALT_3"   => { EIdx => "1762", ID => "180" } ,
  "W_WASSERPROG_1_SO"   => { EIdx => "1770", ID => "180" } ,
  "W_WASSERPROG_1_SO_SCHALT_2"   => { EIdx => "1771", ID => "180" } ,
  "W_WASSERPROG_1_SO_SCHALT_3"   => { EIdx => "1772", ID => "180" } ,
  "SOMMERBETRIEB"   => { EIdx => "fdb4", ID => "180" } ,
  "AUTOMATIK_WARMWASSER"   => { EIdx => "fdb9", ID => "180" } ,
  "DYNAMIK"   => { EIdx => "fdbf", ID => "180" } ,
  "RAUMSOLLTEMP_I_1"   => { EIdx => "0005", ID => "301" } ,
  "RAUMSOLLTEMP_NACHT_1"   => { EIdx => "0008", ID => "301" } ,
  "HEIZPROG_1_MO_1"   => { EIdx => "1410", ID => "301" } ,
  "HEIZPROG_1_MO_SCHALT_2_1"   => { EIdx => "1411", ID => "301" } ,
  "HEIZPROG_1_MO_SCHALT_3_1"   => { EIdx => "1412", ID => "301" } ,
  "HEIZPROG_1_DI_1"   => { EIdx => "1420", ID => "301" } ,
  "HEIZPROG_1_DI_SCHALT_2_1"   => { EIdx => "1421", ID => "301" } ,
  "HEIZPROG_1_DI_SCHALT_3_1"   => { EIdx => "1422", ID => "301" } ,
  "HEIZPROG_1_MI_1"   => { EIdx => "1430", ID => "301" } ,
  "HEIZPROG_1_MI_SCHALT_2_1"   => { EIdx => "1431", ID => "301" } ,
  "HEIZPROG_1_MI_SCHALT_3_1"   => { EIdx => "1432", ID => "301" } ,
  "HEIZPROG_1_DO_1"   => { EIdx => "1440", ID => "301" } ,
  "HEIZPROG_1_DO_SCHALT_2_1"   => { EIdx => "1441", ID => "301" } ,
  "HEIZPROG_1_DO_SCHALT_3_1"   => { EIdx => "1442", ID => "301" } ,
  "HEIZPROG_1_FR_1"   => { EIdx => "1450", ID => "301" } ,
  "HEIZPROG_1_FR_SCHALT_2_1"   => { EIdx => "1451", ID => "301" } ,
  "HEIZPROG_1_FR_SCHALT_3_1"   => { EIdx => "1452", ID => "301" } ,
  "HEIZPROG_1_SA_1"   => { EIdx => "1460", ID => "301" } ,
  "HEIZPROG_1_SA_SCHALT_2_1"   => { EIdx => "1461", ID => "301" } ,
  "HEIZPROG_1_SA_SCHALT_3_1"   => { EIdx => "1462", ID => "301" } ,
  "HEIZPROG_1_SO_1"   => { EIdx => "1470", ID => "301" } ,
  "HEIZPROG_1_SO_SCHALT_2_1"   => { EIdx => "1471", ID => "301" } ,
  "HEIZPROG_1_SO_SCHALT_3_1"   => { EIdx => "1472", ID => "301" } ,
  "RAUMSOLLTEMP_I_2"   => { EIdx => "0005", ID => "302" } ,
  "RAUMSOLLTEMP_NACHT_2"   => { EIdx => "0008", ID => "302" } ,
  "HEIZPROG_1_MO_2"   => { EIdx => "1410", ID => "302" } ,
  "HEIZPROG_1_MO_SCHALT_2_2"   => { EIdx => "1411", ID => "302" } ,
  "HEIZPROG_1_MO_SCHALT_3_2"   => { EIdx => "1412", ID => "302" } ,
  "HEIZPROG_1_DI_2"   => { EIdx => "1420", ID => "302" } ,
  "HEIZPROG_1_DI_SCHALT_2_2"   => { EIdx => "1421", ID => "302" } ,
  "HEIZPROG_1_DI_SCHALT_3_2"   => { EIdx => "1422", ID => "302" } ,
  "HEIZPROG_1_MI_2"   => { EIdx => "1430", ID => "302" } ,
  "HEIZPROG_1_MI_SCHALT_2_2"   => { EIdx => "1431", ID => "302" } ,
  "HEIZPROG_1_MI_SCHALT_3_2"   => { EIdx => "1432", ID => "302" } ,
  "HEIZPROG_1_DO_2"   => { EIdx => "1440", ID => "302" } ,
  "HEIZPROG_1_DO_SCHALT_2_2"   => { EIdx => "1441", ID => "302" } ,
  "HEIZPROG_1_DO_SCHALT_3_2"   => { EIdx => "1442", ID => "302" } ,
  "HEIZPROG_1_FR_2"   => { EIdx => "1450", ID => "302" } ,
  "HEIZPROG_1_FR_SCHALT_2_2"   => { EIdx => "1451", ID => "302" } ,
  "HEIZPROG_1_FR_SCHALT_3_2"   => { EIdx => "1452", ID => "302" } ,
  "HEIZPROG_1_SA_2"   => { EIdx => "1460", ID => "302" } ,
  "HEIZPROG_1_SA_SCHALT_2_2"   => { EIdx => "1461", ID => "302" } ,
  "HEIZPROG_1_SA_SCHALT_3_2"   => { EIdx => "1462", ID => "302" } ,
  "HEIZPROG_1_SO_2"   => { EIdx => "1470", ID => "302" } ,
  "HEIZPROG_1_SO_SCHALT_2_2"   => { EIdx => "1471", ID => "302" } ,
  "HEIZPROG_1_SO_SCHALT_3_2"   => { EIdx => "1472", ID => "302" } 
);

my %Elster_gets = (
  "BUSFEHLER"   => { EIdx => "0a26", ID => "000" } ,
  "SPEICHERSOLLTEMP"   => { EIdx => "0003", ID => "180" } ,
  "VORLAUFSOLLTEMP"   => { EIdx => "0004", ID => "180" } ,
  "AUSSENTEMP"   => { EIdx => "000c", ID => "180" } ,
  "SAMMLERISTTEMP"   => { EIdx => "000d", ID => "180" } ,
  "SPEICHERISTTEMP"   => { EIdx => "000e", ID => "180" } ,
  "VORLAUFISTTEMP"   => { EIdx => "000f", ID => "180" } ,
  "EINSTELL_SPEICHERSOLLTEMP"   => { EIdx => "0013", ID => "180" } ,
  "RUECKLAUFISTTEMP"   => { EIdx => "0016", ID => "180" } ,
  "HYSTERESEZEIT"   => { EIdx => "0022", ID => "180" } ,
  "HZK_PUMPE"   => { EIdx => "0053", ID => "180" } ,
  "KESSELSTATUS"   => { EIdx => "0063", ID => "180" } ,
  "SAMMLER_PUMPE"   => { EIdx => "0064", ID => "180" } ,
  "PROGRAMMSCHALTER"   => { EIdx => "0112", ID => "180" } ,
  "SPRACHE"   => { EIdx => "0113", ID => "180" } ,
  "ESTRICHFUNKTION"   => { EIdx => "011a", ID => "180" } ,
  "FERIENANFANG_TAG"   => { EIdx => "011b", ID => "180" } ,
  "FERIENANFANG_MONAT"   => { EIdx => "011c", ID => "180" } ,
  "FERIENANFANG_JAHR"   => { EIdx => "011d", ID => "180" } ,
  "FERIENENDE_TAG"   => { EIdx => "011e", ID => "180" } ,
  "FERIENENDE_MONAT"   => { EIdx => "011f", ID => "180" } ,
  "FERIENENDE_JAHR"   => { EIdx => "0120", ID => "180" } ,
  "TAG"   => { EIdx => "0122", ID => "180" } ,
  "MONAT"   => { EIdx => "0123", ID => "180" } ,
  "JAHR"   => { EIdx => "0124", ID => "180" } ,
  "STUNDE"   => { EIdx => "0125", ID => "180" } ,
  "MINUTE"   => { EIdx => "0126", ID => "180" } ,
  "SEKUNDE"   => { EIdx => "0127", ID => "180" } ,
  "WARMWASSERMODE"   => { EIdx => "0135", ID => "180" } ,
  "MAX_WW_TEMP"   => { EIdx => "0181", ID => "180" } ,
  "BIVALENTPARALLELTEMPERATUR_HZG"   => { EIdx => "01ac", ID => "180" } ,
  "BIVALENTPARALLELTEMPERATUR_WW"   => { EIdx => "01ad", ID => "180" } ,
  "BIVALENZALTERNATIVTEMPERATUR_HZG"   => { EIdx => "01ae", ID => "180" } ,
  "BIVALENZALTERNATIVTEMPERATUR_WW"   => { EIdx => "01af", ID => "180" } ,
  "AUSSENTEMPERATUR_WARMWASSER"   => { EIdx => "01b7", ID => "180" } ,
  "SW_AUSSENTEMP"   => { EIdx => "01bf", ID => "180" } ,
  "FESTWERT"   => { EIdx => "01c0", ID => "180" } ,
  "LAUFZEIT_SOLAR"   => { EIdx => "01ca", ID => "180" } ,
  "LAUFZEIT_2WE"   => { EIdx => "01cb", ID => "180" } ,
  "QUELLE_IST"   => { EIdx => "01d4", ID => "180" } ,
  "PUFFERSOLL"   => { EIdx => "01d5", ID => "180" } ,
  "WPVORLAUFIST"   => { EIdx => "01d6", ID => "180" } ,
  "MAX_HEIZUNG_TEMP"   => { EIdx => "01e8", ID => "180" } ,
  "WW_ECO"   => { EIdx => "027e", ID => "180" } ,
  "AUSSEN_FROSTTEMP"   => { EIdx => "0a00", ID => "180" } ,
  "EINSTELL_SPEICHERSOLLTEMP2"   => { EIdx => "0a06", ID => "180" } ,
  "FEHLERFELD_0"   => { EIdx => "0b00", ID => "180" } ,
  "FEHLERFELD_1"   => { EIdx => "0b01", ID => "180" } ,
  "FEHLERFELD_2"   => { EIdx => "0b02", ID => "180" } ,
  "FEHLERFELD_3"   => { EIdx => "0b03", ID => "180" } ,
  "FEHLERFELD_4"   => { EIdx => "0b04", ID => "180" } ,
  "FEHLERFELD_5"   => { EIdx => "0b05", ID => "180" } ,
  "FEHLERFELD_6"   => { EIdx => "0b06", ID => "180" } ,
  "FEHLERFELD_7"   => { EIdx => "0b07", ID => "180" } ,
  "FEHLERFELD_8"   => { EIdx => "0b08", ID => "180" } ,
  "FEHLERFELD_9"   => { EIdx => "0b09", ID => "180" } ,
  "FEHLERFELD_10"   => { EIdx => "0b0a", ID => "180" } ,
  "FEHLERFELD_11"   => { EIdx => "0b0b", ID => "180" } ,
  "FEHLERFELD_12"   => { EIdx => "0b0c", ID => "180" } ,
  "FEHLERFELD_13"   => { EIdx => "0b0d", ID => "180" } ,
  "FEHLERFELD_14"   => { EIdx => "0b0e", ID => "180" } ,
  "FEHLERFELD_15"   => { EIdx => "0b0f", ID => "180" } ,
  "FEHLERFELD_16"   => { EIdx => "0b10", ID => "180" } ,
  "FEHLERFELD_17"   => { EIdx => "0b11", ID => "180" } ,
  "FEHLERFELD_18"   => { EIdx => "0b12", ID => "180" } ,
  "FEHLERFELD_19"   => { EIdx => "0b13", ID => "180" } ,
  "FEHLERFELD_20"   => { EIdx => "0b14", ID => "180" } ,
  "FEHLERFELD_21"   => { EIdx => "0b15", ID => "180" } ,
  "FEHLERFELD_22"   => { EIdx => "0b16", ID => "180" } ,
  "FEHLERFELD_23"   => { EIdx => "0b17", ID => "180" } ,
  "FEHLERFELD_24"   => { EIdx => "0b18", ID => "180" } ,
  "FEHLERFELD_25"   => { EIdx => "0b19", ID => "180" } ,
  "FEHLERFELD_26"   => { EIdx => "0b1a", ID => "180" } ,
  "FEHLERFELD_27"   => { EIdx => "0b1b", ID => "180" } ,
  "FEHLERFELD_28"   => { EIdx => "0b1c", ID => "180" } ,
  "FEHLERFELD_29"   => { EIdx => "0b1d", ID => "180" } ,
  "FEHLERFELD_30"   => { EIdx => "0b1e", ID => "180" } ,
  "FEHLERFELD_31"   => { EIdx => "0b1f", ID => "180" } ,
  "FEHLERFELD_32"   => { EIdx => "0b20", ID => "180" } ,
  "FEHLERFELD_33"   => { EIdx => "0b21", ID => "180" } ,
  "FEHLERFELD_34"   => { EIdx => "0b22", ID => "180" } ,
  "FEHLERFELD_35"   => { EIdx => "0b23", ID => "180" } ,
  "FEHLERFELD_36"   => { EIdx => "0b24", ID => "180" } ,
  "FEHLERFELD_37"   => { EIdx => "0b25", ID => "180" } ,
  "FEHLERFELD_38"   => { EIdx => "0b26", ID => "180" } ,
  "FEHLERFELD_39"   => { EIdx => "0b27", ID => "180" } ,
  "FEHLERFELD_40"   => { EIdx => "0b28", ID => "180" } ,
  "FEHLERFELD_41"   => { EIdx => "0b29", ID => "180" } ,
  "FEHLERFELD_42"   => { EIdx => "0b2a", ID => "180" } ,
  "FEHLERFELD_43"   => { EIdx => "0b2b", ID => "180" } ,
  "FEHLERFELD_44"   => { EIdx => "0b2c", ID => "180" } ,
  "FEHLERFELD_45"   => { EIdx => "0b2d", ID => "180" } ,
  "FEHLERFELD_46"   => { EIdx => "0b2e", ID => "180" } ,
  "FEHLERFELD_47"   => { EIdx => "0b2f", ID => "180" } ,
  "FEHLERFELD_48"   => { EIdx => "0b30", ID => "180" } ,
  "FEHLERFELD_49"   => { EIdx => "0b31", ID => "180" } ,
  "FEHLERFELD_50"   => { EIdx => "0b32", ID => "180" } ,
  "FEHLERFELD_51"   => { EIdx => "0b33", ID => "180" } ,
  "FEHLERFELD_52"   => { EIdx => "0b34", ID => "180" } ,
  "FEHLERFELD_53"   => { EIdx => "0b35", ID => "180" } ,
  "FEHLERFELD_54"   => { EIdx => "0b36", ID => "180" } ,
  "FEHLERFELD_55"   => { EIdx => "0b37", ID => "180" } ,
  "FEHLERFELD_56"   => { EIdx => "0b38", ID => "180" } ,
  "FEHLERFELD_57"   => { EIdx => "0b39", ID => "180" } ,
  "FEHLERFELD_58"   => { EIdx => "0b3a", ID => "180" } ,
  "FEHLERFELD_59"   => { EIdx => "0b3b", ID => "180" } ,
  "FEHLERFELD_60"   => { EIdx => "0b3c", ID => "180" } ,
  "FEHLERFELD_61"   => { EIdx => "0b3d", ID => "180" } ,
  "FEHLERFELD_62"   => { EIdx => "0b3e", ID => "180" } ,
  "FEHLERFELD_63"   => { EIdx => "0b3f", ID => "180" } ,
  "FEHLERFELD_64"   => { EIdx => "0b40", ID => "180" } ,
  "FEHLERFELD_65"   => { EIdx => "0b41", ID => "180" } ,
  "FEHLERFELD_66"   => { EIdx => "0b42", ID => "180" } ,
  "FEHLERFELD_67"   => { EIdx => "0b43", ID => "180" } ,
  "FEHLERFELD_68"   => { EIdx => "0b44", ID => "180" } ,
  "FEHLERFELD_69"   => { EIdx => "0b45", ID => "180" } ,
  "FEHLERFELD_70"   => { EIdx => "0b46", ID => "180" } ,
  "FEHLERFELD_71"   => { EIdx => "0b47", ID => "180" } ,
  "FEHLERFELD_72"   => { EIdx => "0b48", ID => "180" } ,
  "FEHLERFELD_73"   => { EIdx => "0b49", ID => "180" } ,
  "FEHLERFELD_74"   => { EIdx => "0b4a", ID => "180" } ,
  "FEHLERFELD_75"   => { EIdx => "0b4b", ID => "180" } ,
  "FEHLERFELD_76"   => { EIdx => "0b4c", ID => "180" } ,
  "FEHLERFELD_77"   => { EIdx => "0b4d", ID => "180" } ,
  "FEHLERFELD_78"   => { EIdx => "0b4e", ID => "180" } ,
  "FEHLERFELD_79"   => { EIdx => "0b4f", ID => "180" } ,
  "FEHLERFELD_80"   => { EIdx => "0b50", ID => "180" } ,
  "FEHLERFELD_81"   => { EIdx => "0b51", ID => "180" } ,
  "FEHLERFELD_82"   => { EIdx => "0b52", ID => "180" } ,
  "FEHLERFELD_83"   => { EIdx => "0b53", ID => "180" } ,
  "FEHLERFELD_84"   => { EIdx => "0b54", ID => "180" } ,
  "FEHLERFELD_85"   => { EIdx => "0b55", ID => "180" } ,
  "FEHLERFELD_86"   => { EIdx => "0b56", ID => "180" } ,
  "FEHLERFELD_87"   => { EIdx => "0b57", ID => "180" } ,
  "FEHLERFELD_88"   => { EIdx => "0b58", ID => "180" } ,
  "FEHLERFELD_89"   => { EIdx => "0b59", ID => "180" } ,
  "FEHLERFELD_90"   => { EIdx => "0b5a", ID => "180" } ,
  "FEHLERFELD_91"   => { EIdx => "0b5b", ID => "180" } ,
  "FEHLERFELD_92"   => { EIdx => "0b5c", ID => "180" } ,
  "FEHLERFELD_93"   => { EIdx => "0b5d", ID => "180" } ,
  "FEHLERFELD_94"   => { EIdx => "0b5e", ID => "180" } ,
  "FEHLERFELD_95"   => { EIdx => "0b5f", ID => "180" } ,
  "FEHLERFELD_96"   => { EIdx => "0b60", ID => "180" } ,
  "FEHLERFELD_97"   => { EIdx => "0b61", ID => "180" } ,
  "FEHLERFELD_98"   => { EIdx => "0b62", ID => "180" } ,
  "FEHLERFELD_99"   => { EIdx => "0b63", ID => "180" } ,
  "FEHLERFELD_100"   => { EIdx => "0b64", ID => "180" } ,
  "FEHLERFELD_101"   => { EIdx => "0b65", ID => "180" } ,
  "FEHLERFELD_102"   => { EIdx => "0b66", ID => "180" } ,
  "FEHLERFELD_103"   => { EIdx => "0b67", ID => "180" } ,
  "FEHLERFELD_104"   => { EIdx => "0b68", ID => "180" } ,
  "FEHLERFELD_105"   => { EIdx => "0b69", ID => "180" } ,
  "FEHLERFELD_106"   => { EIdx => "0b6a", ID => "180" } ,
  "FEHLERFELD_107"   => { EIdx => "0b6b", ID => "180" } ,
  "FEHLERFELD_108"   => { EIdx => "0b6c", ID => "180" } ,
  "FEHLERFELD_109"   => { EIdx => "0b6d", ID => "180" } ,
  "FEHLERFELD_110"   => { EIdx => "0b6e", ID => "180" } ,
  "FEHLERFELD_111"   => { EIdx => "0b6f", ID => "180" } ,
  "FEHLERFELD_112"   => { EIdx => "0b70", ID => "180" } ,
  "FEHLERFELD_113"   => { EIdx => "0b71", ID => "180" } ,
  "FEHLERFELD_114"   => { EIdx => "0b72", ID => "180" } ,
  "FEHLERFELD_115"   => { EIdx => "0b73", ID => "180" } ,
  "FEHLERFELD_116"   => { EIdx => "0b74", ID => "180" } ,
  "FEHLERFELD_117"   => { EIdx => "0b75", ID => "180" } ,
  "FEHLERFELD_118"   => { EIdx => "0b76", ID => "180" } ,
  "FEHLERFELD_119"   => { EIdx => "0b77", ID => "180" } ,
  "FEHLERFELD_120"   => { EIdx => "0b78", ID => "180" } ,
  "FEHLERFELD_121"   => { EIdx => "0b79", ID => "180" } ,
  "FEHLERFELD_122"   => { EIdx => "0b7a", ID => "180" } ,
  "FEHLERFELD_123"   => { EIdx => "0b7b", ID => "180" } ,
  "FEHLERFELD_124"   => { EIdx => "0b7c", ID => "180" } ,
  "FEHLERFELD_125"   => { EIdx => "0b7d", ID => "180" } ,
  "FEHLERFELD_126"   => { EIdx => "0b7e", ID => "180" } ,
  "FEHLERFELD_127"   => { EIdx => "0b7f", ID => "180" } ,
  "FEHLERFELD_128"   => { EIdx => "0b80", ID => "180" } ,
  "FEHLERFELD_129"   => { EIdx => "0b81", ID => "180" } ,
  "FEHLERFELD_130"   => { EIdx => "0b82", ID => "180" } ,
  "FEHLERFELD_131"   => { EIdx => "0b83", ID => "180" } ,
  "FEHLERFELD_132"   => { EIdx => "0b84", ID => "180" } ,
  "FEHLERFELD_133"   => { EIdx => "0b85", ID => "180" } ,
  "FEHLERFELD_134"   => { EIdx => "0b86", ID => "180" } ,
  "FEHLERFELD_135"   => { EIdx => "0b87", ID => "180" } ,
  "FEHLERFELD_136"   => { EIdx => "0b88", ID => "180" } ,
  "FEHLERFELD_137"   => { EIdx => "0b89", ID => "180" } ,
  "FEHLERFELD_138"   => { EIdx => "0b8a", ID => "180" } ,
  "FEHLERFELD_139"   => { EIdx => "0b8b", ID => "180" } ,
  "W_WASSERPROG_1_MO"   => { EIdx => "1710", ID => "180" } ,
  "W_WASSERPROG_1_MO_SCHALT_2"   => { EIdx => "1711", ID => "180" } ,
  "W_WASSERPROG_1_MO_SCHALT_3"   => { EIdx => "1712", ID => "180" } ,
  "W_WASSERPROG_1_DI"   => { EIdx => "1720", ID => "180" } ,
  "W_WASSERPROG_1_DI_SCHALT_2"   => { EIdx => "1721", ID => "180" } ,
  "W_WASSERPROG_1_DI_SCHALT_3"   => { EIdx => "1722", ID => "180" } ,
  "W_WASSERPROG_1_MI"   => { EIdx => "1730", ID => "180" } ,
  "W_WASSERPROG_1_MI_SCHALT_2"   => { EIdx => "1731", ID => "180" } ,
  "W_WASSERPROG_1_MI_SCHALT_3"   => { EIdx => "1732", ID => "180" } ,
  "W_WASSERPROG_1_DO"   => { EIdx => "1740", ID => "180" } ,
  "W_WASSERPROG_1_DO_SCHALT_2"   => { EIdx => "1741", ID => "180" } ,
  "W_WASSERPROG_1_DO_SCHALT_3"   => { EIdx => "1742", ID => "180" } ,
  "W_WASSERPROG_1_FR"   => { EIdx => "1750", ID => "180" } ,
  "W_WASSERPROG_1_FR_SCHALT_2"   => { EIdx => "1751", ID => "180" } ,
  "W_WASSERPROG_1_FR_SCHALT_3"   => { EIdx => "1752", ID => "180" } ,
  "W_WASSERPROG_1_SA"   => { EIdx => "1760", ID => "180" } ,
  "W_WASSERPROG_1_SA_SCHALT_2"   => { EIdx => "1761", ID => "180" } ,
  "W_WASSERPROG_1_SA_SCHALT_3"   => { EIdx => "1762", ID => "180" } ,
  "W_WASSERPROG_1_SO"   => { EIdx => "1770", ID => "180" } ,
  "W_WASSERPROG_1_SO_SCHALT_2"   => { EIdx => "1771", ID => "180" } ,
  "W_WASSERPROG_1_SO_SCHALT_3"   => { EIdx => "1772", ID => "180" } ,
  "FEHLERANZAHL"   => { EIdx => "fda8", ID => "180" } ,
  "BUSKONTROLLE"   => { EIdx => "fdaa", ID => "180" } ,
  "DAUERLAUF_PUFFERLADEPUMPE"   => { EIdx => "fdaf", ID => "180" } ,
  "SCHALTWERKDYNAMIKZEIT"   => { EIdx => "fdb0", ID => "180" } ,
  "STILLSTANDZEIT"   => { EIdx => "fdb1", ID => "180" } ,
  "PUMPENZYKLEN"   => { EIdx => "fdb2", ID => "180" } ,
  "GEBAEUDEART"   => { EIdx => "fdb3", ID => "180" } ,
  "SOMMERBETRIEB"   => { EIdx => "fdb4", ID => "180" } ,
  "AUTOMATIK_WARMWASSER"   => { EIdx => "fdb9", ID => "180" } ,
  "ZWEITER_WE_STATUS"   => { EIdx => "fdba", ID => "180" } ,
  "DYNAMIK"   => { EIdx => "fdbf", ID => "180" } ,
  "VORLAUFSOLLTEMP_1"   => { EIdx => "0004", ID => "301" } ,
  "RAUMSOLLTEMP_I_1"   => { EIdx => "0005", ID => "301" } ,
  "RAUMSOLLTEMP_NACHT_1"   => { EIdx => "0008", ID => "301" } ,
  "RAUMISTTEMP_1"   => { EIdx => "0011", ID => "301" } ,
  "MAX_TEMP_KESSEL_1"   => { EIdx => "0028", ID => "301" } ,
  "MISCHER_ZU_1"   => { EIdx => "0058", ID => "301" } ,
  "HEIZKURVE_1"   => { EIdx => "010e", ID => "301" } ,
  "HEIZPROG_1_MO_1"   => { EIdx => "1410", ID => "301" } ,
  "HEIZPROG_1_MO_SCHALT_2_1"   => { EIdx => "1411", ID => "301" } ,
  "HEIZPROG_1_MO_SCHALT_3_1"   => { EIdx => "1412", ID => "301" } ,
  "HEIZPROG_1_DI_1"   => { EIdx => "1420", ID => "301" } ,
  "HEIZPROG_1_DI_SCHALT_2_1"   => { EIdx => "1421", ID => "301" } ,
  "HEIZPROG_1_DI_SCHALT_3_1"   => { EIdx => "1422", ID => "301" } ,
  "HEIZPROG_1_MI_1"   => { EIdx => "1430", ID => "301" } ,
  "HEIZPROG_1_MI_SCHALT_2_1"   => { EIdx => "1431", ID => "301" } ,
  "HEIZPROG_1_MI_SCHALT_3_1"   => { EIdx => "1432", ID => "301" } ,
  "HEIZPROG_1_DO_1"   => { EIdx => "1440", ID => "301" } ,
  "HEIZPROG_1_DO_SCHALT_2_1"   => { EIdx => "1441", ID => "301" } ,
  "HEIZPROG_1_DO_SCHALT_3_1"   => { EIdx => "1442", ID => "301" } ,
  "HEIZPROG_1_FR_1"   => { EIdx => "1450", ID => "301" } ,
  "HEIZPROG_1_FR_SCHALT_2_1"   => { EIdx => "1451", ID => "301" } ,
  "HEIZPROG_1_FR_SCHALT_3_1"   => { EIdx => "1452", ID => "301" } ,
  "HEIZPROG_1_SA_1"   => { EIdx => "1460", ID => "301" } ,
  "HEIZPROG_1_SA_SCHALT_2_1"   => { EIdx => "1461", ID => "301" } ,
  "HEIZPROG_1_SA_SCHALT_3_1"   => { EIdx => "1462", ID => "301" } ,
  "HEIZPROG_1_SO_1"   => { EIdx => "1470", ID => "301" } ,
  "HEIZPROG_1_SO_SCHALT_2_1"   => { EIdx => "1471", ID => "301" } ,
  "HEIZPROG_1_SO_SCHALT_3_1"   => { EIdx => "1472", ID => "301" } ,
  "RAUMSOLLTEMP_I_2"   => { EIdx => "0005", ID => "302" } ,
  "RAUMSOLLTEMP_NACHT_2"   => { EIdx => "0008", ID => "302" } ,
  "RAUMISTTEMP_2"   => { EIdx => "0011", ID => "302" } ,
  "MAX_TEMP_KESSEL_2"   => { EIdx => "0028", ID => "302" } ,
  "MISCHER_ZU_2"   => { EIdx => "0058", ID => "302" } ,
  "HEIZKURVE_2"   => { EIdx => "010e", ID => "302" } ,
  "HEIZPROG_1_MO_2"   => { EIdx => "1410", ID => "302" } ,
  "HEIZPROG_1_MO_SCHALT_2_2"   => { EIdx => "1411", ID => "302" } ,
  "HEIZPROG_1_MO_SCHALT_3_2"   => { EIdx => "1412", ID => "302" } ,
  "HEIZPROG_1_DI_2"   => { EIdx => "1420", ID => "302" } ,
  "HEIZPROG_1_DI_SCHALT_2_2"   => { EIdx => "1421", ID => "302" } ,
  "HEIZPROG_1_DI_SCHALT_3_2"   => { EIdx => "1422", ID => "302" } ,
  "HEIZPROG_1_MI_2"   => { EIdx => "1430", ID => "302" } ,
  "HEIZPROG_1_MI_SCHALT_2_2"   => { EIdx => "1431", ID => "302" } ,
  "HEIZPROG_1_MI_SCHALT_3_2"   => { EIdx => "1432", ID => "302" } ,
  "HEIZPROG_1_DO_2"   => { EIdx => "1440", ID => "302" } ,
  "HEIZPROG_1_DO_SCHALT_2_2"   => { EIdx => "1441", ID => "302" } ,
  "HEIZPROG_1_DO_SCHALT_3_2"   => { EIdx => "1442", ID => "302" } ,
  "HEIZPROG_1_FR_2"   => { EIdx => "1450", ID => "302" } ,
  "HEIZPROG_1_FR_SCHALT_2_2"   => { EIdx => "1451", ID => "302" } ,
  "HEIZPROG_1_FR_SCHALT_3_2"   => { EIdx => "1452", ID => "302" } ,
  "HEIZPROG_1_SA_2"   => { EIdx => "1460", ID => "302" } ,
  "HEIZPROG_1_SA_SCHALT_2_2"   => { EIdx => "1461", ID => "302" } ,
  "HEIZPROG_1_SA_SCHALT_3_2"   => { EIdx => "1462", ID => "302" } ,
  "HEIZPROG_1_SO_2"   => { EIdx => "1470", ID => "302" } ,
  "HEIZPROG_1_SO_SCHALT_2_2"   => { EIdx => "1471", ID => "302" } ,
  "HEIZPROG_1_SO_SCHALT_3_2"   => { EIdx => "1472", ID => "302" } ,
  "ACCESS_EEPROM"   => { EIdx => "0030", ID => "480" } ,
  "SPEICHERBEDARF"   => { EIdx => "005f", ID => "480" } ,
  "STILLSTANDZEIT_0"   => { EIdx => "01cc", ID => "480" } ,
  "STILLSTANDZEIT_1"   => { EIdx => "01cd", ID => "480" } ,
#   "ZWEITER_WE_STATUS"   => { EIdx => "fdab", ID => "480" } ,
  "WP_EVU"   => { EIdx => "fdac", ID => "480" } ,
  "VERDAMPFERTEMP"   => { EIdx => "0014", ID => "500" } ,
  "SPEICHER_STATUS"   => { EIdx => "005a", ID => "500" } ,
  "ABTAUUNGAKTIV"   => { EIdx => "0061", ID => "500" } ,
  "HEISSGAS_TEMP"   => { EIdx => "0265", ID => "500" } ,
  "ANZEIGE_HOCHDRUCK"   => { EIdx => "07a6", ID => "500" } ,
  "ANZEIGE_NIEDERDRUCK"   => { EIdx => "07a7", ID => "500" } ,
  "LZ_VERD_1_HEIZBETRIEB"   => { EIdx => "07fc", ID => "500" } ,
  "LZ_VERD_2_HEIZBETRIEB"   => { EIdx => "07fd", ID => "500" } ,
  "LZ_VERD_1_2_HEIZBETRIEB"   => { EIdx => "07fe", ID => "500" } ,
  "LZ_VERD_1_WW_BETRIEB"   => { EIdx => "0802", ID => "500" } ,
  "LZ_VERD_2_WW_BETRIEB"   => { EIdx => "0803", ID => "500" } ,
  "LZ_VERD_1_2_WW_BETRIEB"   => { EIdx => "0804", ID => "500" } ,
  "LZ_DHC12"   => { EIdx => "0805", ID => "500" } ,
  "ABTAUZEIT_VERD1"   => { EIdx => "0808", ID => "500" } ,
  "ABTAUZEIT_VERD2"   => { EIdx => "0809", ID => "500" } ,
  "EL_AUFNAHMELEISTUNG_WW_TAG_KWH"   => { EIdx => "091b", ID => "500" } ,
  "EL_AUFNAHMELEISTUNG_WW_SUM_MWH"   => { EIdx => "091d", ID => "500" } ,
  "EL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH"   => { EIdx => "091f", ID => "500" } ,
  "EL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH"   => { EIdx => "0921", ID => "500" } ,
  "WAERMEERTRAG_2WE_WW_TAG_KWH"   => { EIdx => "0923", ID => "500" } ,
  "WAERMEERTRAG_2WE_WW_SUM_MWH"   => { EIdx => "0925", ID => "500" } ,
  "WAERMEERTRAG_2WE_HEIZ_TAG_KWH"   => { EIdx => "0927", ID => "500" } ,
  "WAERMEERTRAG_2WE_HEIZ_SUM_MWH"   => { EIdx => "0929", ID => "500" } ,
  "WAERMEERTRAG_WW_TAG_KWH"   => { EIdx => "092b", ID => "500" } ,
  "WAERMEERTRAG_WW_SUM_MWH"   => { EIdx => "092d", ID => "500" } ,
  "WAERMEERTRAG_HEIZ_TAG_KWH"   => { EIdx => "092f", ID => "500" } ,
  "WAERMEERTRAG_HEIZ_SUM_MWH"   => { EIdx => "0931", ID => "500" } ,
#   "MAX_TEMP_KESSEL_2"   => { EIdx => "0028", ID => "602" } ,
  "TN_2"   => { EIdx => "002b", ID => "602" } ,
  "BRENNER_2"   => { EIdx => "0052", ID => "602" } ,
  "DCF_2"   => { EIdx => "0056", ID => "602" } ,
  "MISCHER_AUF_2"   => { EIdx => "0057", ID => "602" } 
);

my $sniff = 0;
my $oldValue = 'empty';

#####################################################
# Elster_Initialize($)                              #
# Parameter hash                                    #
#####################################################
sub Elster_Initialize($)
{
  my ($hash) = @_;
  
  $hash->{ReadyFn} = "Elster_Ready";
  $hash->{ReadFn}  = "Elster_Read";
  $hash->{DefFn}   = "Elster_Define";
  $hash->{UndefFn} = "Elster_Undef";
  $hash->{GetFn}   = "Elster_Get";
  $hash->{SetFn}   = "Elster_Set";
  $hash->{AttrFn}  = "Elster_Attr";
  $hash->{AttrList}= $readingFnAttributes;
}

######################################################
# Elster_define                                      #
# Parameter hash and configuration                   #
######################################################
sub Elster_Define($$)
{
  my ($hash, $def) = @_;

  my @a = split("[ \t][ \t]*", $def);
  my $name = $a[0];
  return "use syntax: \"define\" <name> \"Elster\" <device>" if(@a != 3);

  my $dev  = $a[2];
  $hash->{DeviceName} = $dev; # can0 / ttyABC / com3
  $hash->{INTERVAL} = 10;
  elster_perl::setdev($dev);
  # Adaptertyp hier definieren!
  # Default device:
  #   Linux: can0 / SocketCAN
  #   Win: COM1 / can232
  # elster_perl::set_can232(); # USBtin
  # elster_perl::set_cs();     # für das optische Interface
  # elster_perl::toggle_trace();
  
  if (!elster_perl::initcan())
  {
    Log 1, elster_perl::geterrormsg();

    return undef;
  }
  
  $readyfnlist{"$name.$dev"} = $hash;
  $hash->{STATE} = "connected";
  #DoTrigger($name, "CONNECTED");
  #$hash->{FD} = 8;
  #$selectlist{"$name.$dev"} = $hash;
  
  return;
}

######################################################
# ElsterUndef - Close connection                     #
######################################################
sub Elster_Undef($$)
{
  my ($hash, $arg) = @_;

  elster_perl::undef();

  return;
}

######################################################
#  sub Elster_Get                                    #
#  parameter name                                    #
######################################################
sub Elster_Get($@)
{
  my ( $hash, @a ) = @_;
  return "\"get Elster\" needs at least one argument" if ( @a < 2 );

  my $name = shift @a;
  my $opt = shift @a;
  my $cmdhash = $Elster_gets{$opt};
  if (!$cmdhash)
  {
    my @cList = keys %Elster_gets;
    return "Unknown argument $opt, choose one of \n" . join(" ", @cList);
  }

  my $Elster_str = $sender_id." ".$cmdhash->{ID}." ".$cmdhash->{EIdx};
  my $res = elster_perl::getstring($Elster_str);
  
  if ($trace_all)
  {
    Log3 ($name,3,"getstring($Elster_str): $res");
  }
  if (length($res) > 0)
  {
    readingsSingleUpdate($hash, "$opt", "$res", 1);
  } else {
    my $err = elster_perl::geterrormsg();
    readingsSingleUpdate($hash, "err", "$opt: $err", 1);  
  }
  return;
}

######################################################
#  sub Elster_Set                                    #
#  parameter name value                              #
# !!!!!Use with care!!!!! This can damage the pump!!!#
######################################################
sub Elster_Set($@)
{
  my ( $hash, @a ) = @_;
  return "\"set Elster\" needs at least an argument" if ( @a < 2 );

  my $name = shift @a;
  my $opt = shift @a;
  my $value = join("", @a);
  my $cmdhash = $Elster_sets{$opt};
  if(!$cmdhash)
  {
    my @cList = keys %Elster_sets;
    return "Unknown argument $opt, choose one of \n" . join(" ", @cList);
  }

  my $Elster_str = $sender_id." ".$cmdhash->{ID}." ".$cmdhash->{EIdx}." ".$value;
  my $res = elster_perl::setstring($Elster_str);
  my $true;
  
  if ($res)
  {
    $true = "true";
  } else {
    $true = "false";
  }
  
  if ($trace_all)
  {
    Log3 ($name,3,"setstring($Elster_str): $true");
  }
  readingsSingleUpdate($hash, "set result", "$true", 1);
  
  if ($res)
  {
    Elster_Get($hash, $name, $opt );
  }
  return;
}

sub Elster_Attr($)
{
}

sub Elster_Read($)
{
  my ($hash) = @_;
  my @cList = keys %Elster_gets;
  
  if ($sniffed_set != 1)
  {
    return;
  }
  my $buf = elster_perl::getsniffedvalue();
  while (length($buf) > 10)
  {
    my @arr = split /\s+/, $buf;
    my $recv_id = shift @arr;
    my $elster_idx = shift @arr;
    my $value = join(" ", @arr);

    my $newValue = 'id:' . $recv_id . ' idx:' . $elster_idx . ' val:' . $value;
  
    #printf "elster_read_: $recv_id $elster_idx $value\n";
    $buf = elster_perl::getsniffedvalue();
    for my $key (@cList)
    {
      my $el = $Elster_gets{$key};
      if ($el)
      {
        #printf "id: $el->{ID} $el->{EIdx}\n";
        if (hex($el->{ID}) == hex($recv_id) &&
            hex($el->{EIdx}) == hex($elster_idx) &&
            $oldValue != $newValue)
        {
          #printf "single update: $key $value\n";
          $oldValue = $newValue;
          readingsSingleUpdate($hash, "$key", "$value", 1);
          last;
        }
      }
    }
    $buf = elster_perl::getsniffedvalue();
  }
  return;
}

sub Elster_Ready($)
{
  my ($hash) = @_;
  
  if (!$sniff)
  {
    my @cList = keys %Elster_gets;
  
    for my $key (@cList)
    {
      my $el = $Elster_gets{$key};
      
      elster_perl::setsniffedframe($el->{ID}." ".$el->{EIdx})
    }
    $sniff = 1;
  }
  
  $sniffed_set = 1;
  return 1;
}

######################################################
#                                                    #
# Exceptions in elster_perl.so sollten damit abge-   #
# fangen werden. (funktioniert nicht!)               #
#                                                    #
######################################################

my $internalHash;

local $SIG{__WARN__} = sub
{
  my $message = shift;
  
  if (!defined($internalHash)) {
    Log 3, "EXCEPTION in Elster: '$message'";
  }
  else
  {
    Log3 $internalHash->{NAME},3, "EXCEPTION in Elster: '$message'";
  }
};

1;
=pod
=begin html

<a name="Elster"></a>
<h3>Elster</h3>
<ul>
  Elster module: communicates through the CAN bus or the optical interface with
  a Stiebel Eltron heatpump. <br>
  CAN Adapter: SocketCAN, USB2CAN, USBtin, or Lawicel CANUSB/232.<br>
  Should work on all Stiebel Eltron devices. <br>
  Please, define the Adapter type in "50_Elster.pm" first.<br>
  <br><br>

  <a name="Elsterdefine"></a>
  <b>Define</b>
  <code>define &lt;name&gt; Elster &lt;device&gt;</code> <br>
  <br>Example for Linux: <code>define Stiebel Elster ttyACM0</code><br>
  <br>Example for Windows: <code>define Stiebel Elster COM3<br></code>
  <br>
</ul>

=end html

=begin html_DE

<a name="Elster"></a>
<h3>Elster</h3>
<ul>
  Elster Modul: Kommuniziert &uuml;ber den CAN-Bus oder das optische Interface
  mit einer Stiebel-Eltron W&auml;rmepumpe. <br>
  CAN Adapter: SocketCAN, USB2CAN, USBtin oder Lawicel CANUSB/232. <br>
  Sollte mit allen Stiebel-Eltron W&auml;rmepumpen laufen.<br>
  Bitte defineren Sie zuerst den Adapertyp in "50_Elster.pm".<br>
  <br><br>

  <a name="Elsterdefine"></a>
  <b>Define</b>
  <code>define &lt;name&gt; Elster &lt;device&gt;</code><br>
  <br>Beispiel f&uuml;er Linux: <code>define Stiebel Elster ttyACM0</code><br>
  <br>Beispiel f&uuml;er Windows: <code>define Stiebel Elster COM3<br></code>
  <br>
</ul>
 
=end html_DE
=cut



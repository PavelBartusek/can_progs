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
  "HEIZPROG_1_SO_SCHALT_3_1"   => { EIdx => "1472", ID => "301" } 
);

my %Elster_gets = (
  "SPEICHERSOLLTEMP"   => { EIdx => "0003", ID => "180" } ,
  "VORLAUFSOLLTEMP"   => { EIdx => "0004", ID => "180" } ,
  "GERAETE_ID"   => { EIdx => "000b", ID => "180" } ,
  "AUSSENTEMP"   => { EIdx => "000c", ID => "180" } ,
  "SPEICHERISTTEMP"   => { EIdx => "000e", ID => "180" } ,
  "VORLAUFISTTEMP"   => { EIdx => "000f", ID => "180" } ,
  "GERAETEKONFIGURATION"   => { EIdx => "0010", ID => "180" } ,
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
  "BIVALENTPARALLELTEMPERATUR_WW"   => { EIdx => "01ad", ID => "180" } ,
  "BIVALENZALTERNATIVTEMPERATUR_WW"   => { EIdx => "01af", ID => "180" } ,
  "QUELLENSOLLTEMPERATUR"   => { EIdx => "01b0", ID => "180" } ,
  "AUSSENTEMPERATUR_WARMWASSER"   => { EIdx => "01b7", ID => "180" } ,
  "SW_AUSSENTEMP"   => { EIdx => "01bf", ID => "180" } ,
  "FESTWERT"   => { EIdx => "01c0", ID => "180" } ,
  "LAUFZEIT_WP1"   => { EIdx => "01c4", ID => "180" } ,
  "LAUFZEIT_WP2"   => { EIdx => "01c5", ID => "180" } ,
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
  "WP_STATUS"   => { EIdx => "fdae", ID => "180" } ,
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
  "GERAETE_ID_1"   => { EIdx => "000b", ID => "301" } ,
  "RAUMISTTEMP_1"   => { EIdx => "0011", ID => "301" } ,
  "MAX_TEMP_KESSEL_1"   => { EIdx => "0028", ID => "301" } ,
  "MISCHER_ZU_1"   => { EIdx => "0058", ID => "301" } ,
  "HEIZKURVE_1"   => { EIdx => "010e", ID => "301" } ,
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
  "FEHLERMELDUNG"   => { EIdx => "0001", ID => "480" } ,
#   "GERAETE_ID"   => { EIdx => "000b", ID => "480" } ,
#   "GERAETEKONFIGURATION"   => { EIdx => "0010", ID => "480" } ,
  "STILLSTANDZEIT_0"   => { EIdx => "01cc", ID => "480" } ,
  "STILLSTANDZEIT_1"   => { EIdx => "01cd", ID => "480" } ,
#   "GERAETE_ID"   => { EIdx => "000b", ID => "500" } ,
#   "GERAETEKONFIGURATION"   => { EIdx => "0010", ID => "500" } ,
  "ANFORDERUNG_NACHHEIZUNG"   => { EIdx => "02f3", ID => "500" } ,
#   "GERAETE_ID"   => { EIdx => "000b", ID => "501" } ,
#   "GERAETEKONFIGURATION"   => { EIdx => "0010", ID => "501" } ,
#   "ANFORDERUNG_NACHHEIZUNG"   => { EIdx => "02f3", ID => "501" } ,
#   "GERAETE_ID_1"   => { EIdx => "000b", ID => "601" } 
);

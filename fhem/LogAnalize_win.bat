del *.obj
del LogAnalize.exe

bcc32 -D__WINDOWS__ -D__CONSOLE__ -D__CAN__ -I.. ^
  ../KStream.cpp ^
  ../NUtils.cpp ^
  ../VList.cpp ^
  ../NCanUtils.cpp ^
  ../KElsterTable.cpp ^
  LogAnalize.cpp

del *.obj
del *.tds
ren kstream.exe LogAnalize.exe

del *fhem*.inc

LogAnalize cs_log_hajo.txt
ren fhem_tab.inc fhem_tab_hajo.inc 

LogAnalize cs_log_radiator.txt
ren fhem_tab.inc fhem_tab_radiator.inc 

LogAnalize cs_log_WPL33.txt
ren fhem_tab.inc fhem_tab_WPL33.inc
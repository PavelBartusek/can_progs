
set MinGW=C:\MinGW
set mysql_flag=


%MinGW%\bin\g++ -c -Wall -D__WINDOWS__ -D__CAN__ -D__CONSOLE__ %1.cpp -o obj\%1.obj


set PATH=C:\cerbero\build-tools\bin;%PATH%

flatc -o lib/include/fbs --cpp doc/common.fbs --include-prefix fbs 
flatc -o lib/include/fbs --cpp doc/livestreamer.fbs --include-prefix fbs 
flatc -o lib/include/fbs --cpp doc/webstreamer.fbs --include-prefix fbs 

flatc -o test/schema -s doc/common.fbs --include-prefix fbs
flatc -o test/schema -s doc/livestreamer.fbs --include-prefix fbs
flatc -o test/schema -s doc/webstreamer.fbs --include-prefix fbs
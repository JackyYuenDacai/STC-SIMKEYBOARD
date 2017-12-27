copy SIMKB.exe C:\Windows\System32\SIMKB.exe
runas /user:Administrator "schtasks /create /tn SimuKB /tr SIMKB.exe /sc ONSTART "
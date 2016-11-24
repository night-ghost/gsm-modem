gsm-modem is a telemetry modem VIA SIM800 GSM module. 

based on idea in http://diydrones.com/group/telemetry-over-cellular-ip/forum/topics/adding-gprs-telemetry-to-walkera-x350-pro

differences:

* it uses small Arduino Nano instead of huge and heavy Arduino Mega
* much more robust initialization and response parsing for GSM module
* it never hangs forever
* Config tool to set parameters - address, port, APN
* auto-baudrate on FC side
* it parses stream from FC so knows what happens
* it connect to server only after receiving of valid MAVlink packet so it not eats money in debugging

Server-side:

mkfifo /tmp/proxypipe
nc -ukl 8888 0</tmp/proxypipe | nc -ulk 7777 1> /tmp/proxypipe &

when modem connected to 8888, MP can connect to 7777 and listen all data from UAV
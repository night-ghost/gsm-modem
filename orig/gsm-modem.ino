/*
* debug_Comm_Mega.ino
*
http://diydrones.com/group/telemetry-over-cellular-ip/forum/topics/adding-gprs-telemetry-to-walkera-x350-pro


You'll need to interface the modem with your Arduino. 

TX from the modem's TTL port to RX1 on the Arduino, RX on the modem's TTL port to TX1 on the Arduino, 
Shared GND connecting the Modem with the Arduino and the power source and finally the VIN to your power source. 
The modem I'm using works with 5 to 20 v so I supply it from the X350 Pro's 11.1v battery. 

To handle loss of connection, you'll need to connect Pin 2 of your Arduino to the PWRKEY / PWRKEY_OUT pin on your modem 
and then connect Pin 3 of your Arduino to the Reset Pin of the Arduino

*/
///@file debug_Comm.ino
///@brief AT-command compatible cellular/GPRS/IP modem initialization & communication for SIM900D -> Arduino -> ArduCopter
///@author Robert Haddad
///@date 14-09-2014

#define LED 12 // All Good LED Turns green when data is being sent over UDP Connection
#define ERR 11 // Error LED Turns Red when error is encountered, turns off if all is okay

#define powerKey 2 // Set PWRKEY pin
#define resetPin 3 // Set reset pin

boolean isReady = false; // GSM modem is Ready to receive AT Commands flag
int count = 0; // Counter for instances of an error (For monitor/debug only);
String checker = ""; // String to hold debug output

// Setup procedure
void setup()
{
    pinMode(LED, OUTPUT); // All Good LED
    pinMode(ERR, OUTPUT); // Warning LED
    digitalWrite(LED,LOW); // Set All Good LED to Off
    digitalWrite(ERR,LOW); // Set Warning LED to Off
    digitalWrite(resetPin, HIGH); // Set Reset Pin

    pinMode(powerKey, OUTPUT);

    // Set Data Rate for debug Ports
    debug.begin(57600); // Start USB Port debug @ 57,600 Baud Rate (For Monitor/Debug only)
    gsm.begin(57600); // Start GSM Module debug Communication @ 57,600 Baud Rate
    pinMode(resetPin, OUTPUT);

    // Wait 3 Seconds for power on (Add Auto on feature through GSM module power button pin
    startUPWait();

    // Initializing GSM Module
    debug.println("Initializing...");
    gsm.print("AT\r");
    while(!isReady){
	if(debug.available()){
	    String a=debug.readString();
	    gsm.print(a);
	}

	if(gsm.available()){
	    String b=gsm.readString();
	    
	    if(b.indexOf("+CREG: 1") > 0){// || b.indexOf("OK") > 0){
		debug.println("SIM900 Ready & Loading");
		isReady = true;
		while(!sendATCommand("AT V1 E1 X1 S0=0","OK",100)); // Set error response and do not pickup on ring
		while(!sendATCommand("AT+CREG=2","OK",100)); // Set various notice messages and parameters
		debug.println("Registered: Starting Configuration");
		while(!sendATCommand("AT+CMEE=2","OK",100));
		while(!sendATCommand("AT+CR=1","OK",100));
		while(!sendATCommand("AT+CRC=1","OK",100));
		while(!sendATCommand("AT+CSNS=4","OK",100));
		while(!sendATCommand("AT+CSMINS=1","OK",100));
		while(!sendATCommand("AT+CSCLK=0","OK",100));
		while(!sendATCommand("AT+CIURC=1","OK",100));
		while(!sendATCommand("AT+CGEREP=2","OK",100));
		while(!sendATCommand("AT+CIPMUX=0","OK",100)); // Single channel communication (ie only one socket can be opened)
		while(!sendATCommand("AT+CIPMODE=1","OK",100)); // Transparent bridge mode
		while(!sendATCommand("AT+CIPCCFG=8,10,400,0","OK",100)); // GPRS params
		gsm.print("AT+CMUX=0,0,4,32768,10,3,30,10,2\r"); // GPRS/IP params
		//0 4 127/32768 3 2
		delay(2000);
		while(!sendATCommand("AT+CGATT?","OK",1000)); // Make sure GPRS is Attached
		while(!sendATCommand("AT+CSTT= \"internet\",\"\",\"\"","OK",1000)); // AT+CSTT="APN","username","password" - login to service provider/carrier
		debug.println("APN Set");
		while(!sendATCommand("AT+CIICR","OK",1000)); // Connect!
		while(!sendATCommand("AT+CIFSR",".",100)); // Get IP address (for info only);
		//while(!sendATCommand("AT+CIPCTL=1","+CREG: 1",100)); // Set port to listen. Not needed
		while(!sendATCommand("AT+CLPORT=\"UDP\",8888","OK",100)); // Prep UDP Port 8888
		debug.println("Connecting to UDP Server");
		while(!sendATCommand("AT+CIPSTART=\"UDP\",\"drone.dyndns.org\",8888","OK",1000)); // AT+CIPSTART="protocol","ip address or domain","port #"
		//while(!sendATCommand("AT+CIPSERVER=1,8888","OK",1000)); // TCP/UDP Server
		digitalWrite(LED,HIGH); // Turn on All Good LED
		digitalWrite(ERR,LOW); // Turn off Warning LED
		delay(1000);
		digitalWrite(LED,LOW);
		debug.end(); // Stop USB debug communication
		MAVlink.begin(57600); // Start Autopilot Communication @ 57,600 Baud Rate

/*
//* First test if everything is okay 

=> AT

<= AT //* This should come back. SIM900 default is to echo back commands you enter 

<= OK //* This string should tell you all is well

=>AT+CPIN? //*This is to check if SIM is unlocked. This sample assumes unlocked SIMs

<= +CPIN: READY //* If your response contains this, then it means SIM is unlocked and ready

=>AT+CREG? //*This checks if SIM is registered or not

<=+CREG: 0,1 //*This string in the response indicates SIM is registered

=>AT+CGATT? //*Check if GPRS is attached or not

<=+CGATT: 1 //*A response containing this string indicates GPRS is attached

=>AT+CIPSHUT //*Reset the IP session if any

<=SHUT OK //*This string in the response represents all IP sessions shutdown.

=>AT+CIPSTATUS //*Check if the IP stack is initialized

<=STATE: IP INITIAL //*This string in the response indicates IP stack is initialized

=>AT+CIPMUX=0 //*To keep things simple, I’m setting up a single connection mode

<=OK //*This string indicates single connection mode set successfully at SIM 900

=>AT+CSTT= “APN”, “UNAME”, “PWD” //*Start the task, based on the SIM card you are using, you need to know the APN, username and password for your service provider

<= OK //*This response indicates task started successfully

=> AT+CIICR //*Now bring up the wireless. Please note, the response to this might take some time

<=OK //*This text in response string indicates wireless is up

=>AT+CIFSR //*Get the local IP address. Some people say that this step is not required, but if I do not issue this, it was not working for my case. So I made this mandatory, no harm.

<= xxx.xxx.xxx.xxx //*If previous command is successful, you should see an IP address in the response

=>AT+CIPSTART= “TCP” , “www.vishnusharma.com”, “80” //*Start the connection, TCP, domain name, port

<= CONNECT OK //*This string in the response indicates TCP connection established

=>AT+CIPSEND //*Request initiation of data sending (the request)

<= > //*The response should be the string “>” to indicate, type your data to send

=> xxxxxx //*Just type anything for now

=>#026 //*Now type the sequence #026. This tells the terminal.exe to send the hex code 0x1a (which is Ctrl+Z) to indicate end of data sending

<= xxxxxxxxxx //*You should get some response back from the server…it would generally be a complain that the request string was not valid…but that is a different subject…you have established the connection

//*To close the connection

=>AT+CIPSHUT //*Request shutting down of the current connections

<=SHUT OK //*Indicates shutdown successful


*/


/*
String dyndns = "members.dyndns.org";
String hostname = "YOUR DYNAMIC HOST NAME"; // IE drone.dyndns.org
String userpwdb64 = "BASE64 Username & Password"; // username:password

String currentIP = "YOUR CURRENT IP ADDRESS";
debug.print("AT+CIPSEND\r");
debug.print("GET /nic/update?hostname=" + hostname + "&myip=" + currentIP + "&wildcard=NOCHG&mx=NOCHG&backmx=NOCHG HTTP/1.0\r");
debug.print("Host: " + dyndns + "\r");
debug.print("Authorization: Basic " + userpwdb64 + "\r");
debug.print("User-Agent: arduinodydns - 1.1\r");
debug.print("\x1A\r");

*/
	    } else {
		digitalWrite(ERR,HIGH); // Turn on Warning LED
		digitalWrite(LED,LOW); // Turn off All Good LED
		debug.print(b);
	    }
	}
    }
}

// AT Command Sender
boolean sendATCommand(String input, String output, int wait){ // AT Command, Expected Result, Delay before issue command
    boolean resp = false;
    debug.print(input + "\r");
    gsm.print(input + "\r");
    delay(wait);
    while(!resp){
	if(gsm.available()){
	    String b=gsm.readString();
	    debug.println(b);
	    gsm.read();
	    debug.read();
	    if(b.indexOf(output) > 0){ // True if expected result is returned
	        resp = true;
	        count = 0;
	        digitalWrite(ERR,LOW); // Warning LED OFF
	        return resp;
	    } else if(b.indexOf("DEACT") > 0){
		if(powerUpOrDown()){
		    digitalWrite(resetPin,LOW);
		}
	    } else {
		digitalWrite(ERR,HIGH); // Warning LED ON
		debug.print(b);
		count++;
		return resp;
	    }
	}
    }
}

//Start Up Wait Period with LEDs
void startUPWait(){
    digitalWrite(LED,HIGH);
    digitalWrite(ERR,LOW);
    delay(500);
    digitalWrite(LED,LOW);
    digitalWrite(ERR,HIGH);
    delay(500);
    digitalWrite(LED,HIGH);
    digitalWrite(ERR,LOW);
    delay(500);
    digitalWrite(LED,LOW);
    digitalWrite(ERR,HIGH);
    delay(500);
    digitalWrite(LED,HIGH);
    digitalWrite(ERR,HIGH);
    delay(1000);
    digitalWrite(LED,LOW);
    digitalWrite(ERR,LOW);
}

// Main Loop
void loop(){
    // Relay All GSM Module communication to Autopilot and USB (USB for monitor/debug only)
    if(gsm.available()){
	char b=gsm.read();
	MAVlink.write(b);
	// Check For Disconnection
	checker += b;
	if(checker.indexOf("\n") > 0 || checker.indexOf("\r") > 0){
	    if(checker.indexOf("DEACT") > 0){
		if(powerUpOrDown()){
		    digitalWrite(3,LOW);
		}
	    }
	    checker = "";
	}
	digitalWrite(ERR,HIGH);
	digitalWrite(ERR,LOW);
	debug.print(b);
    }

    // Relay all Autopilot communication to GSM module and USB (USB for monitor/debug only)
    if(MAVlink.available()){
	digitalWrite(LED,HIGH);
	while(MAVlink.available()){
	    char c=MAVlink.read();
	    gsm.write(c);
	    debug.print(c);
	}
	digitalWrite(LED,LOW);
    }
}

boolean powerUpOrDown(){
    boolean powered = false;
    digitalWrite(powerKey,LOW);
    delay(1000);
    digitalWrite(powerKey,HIGH);
    delay(2000);
    digitalWrite(powerKey,LOW);
    delay(3000);
    powered = true;
    return powered;
}


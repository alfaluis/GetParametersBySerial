#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial) {}
  mySerial.begin(19200);
}

void checkDataReceived ( String );
boolean startConfigMode ( String );
boolean endConfigMode ( String );

// the loop routine runs over and over again forever:
String inputString = "";
String data [4] = {"", "", "", ""};
boolean receiveMode = false;
boolean stringComplete = false;
String prefixDetect [6] = { "AT", "END", "text=", "bright=", "update=", "color="};

void loop() {
	if (mySerial.available()) mySerialEvent();
	if (!receiveMode && stringComplete) {
		delay(1000);
	}
	else if (receiveMode) {
		for (int i = 0; i < 4; i ++){
			if (!data[i].equals("")) {
				Serial.print(prefixDetect[i + 2] + " ");
				Serial.println(data[i]);
			}
		}
	}

	else {
		Serial.println("Nothing");
	}
	delay(1000);
}

void serialEvent() {
  if (!receiveMode) {
    while (Serial.available()) {
      char a = Serial.read();
      if (!(a == '\r' || a == '\n')) {
        inputString += a;
      }
    }
    receiveMode = startConfigMode(inputString);
    if (receiveMode) {
      Serial.println("OK");
    }
    inputString = "";
  }
  else {
    while (Serial.available()) {
      char a = Serial.read();
      if (!(a == '\r' || a == '\n')) {
      	inputString += a;
      }
    }
    checkDataReceived(inputString);

    if (endConfigMode(inputString)) {
    	Serial.println("OK");
    	receiveMode = false;
    }
    inputString = "";
  }
}

// serial event for software serial connection. Port defined by user (example: port 10 and 11)
void mySerialEvent(){
	if (!receiveMode) {
	    while (Serial.available()) {
	      char a = Serial.read();
	      if (!(a == '\r' || a == '\n')) {
	        inputString += a;
	      }
	    }
	    receiveMode = startConfigMode(inputString);
	    if (receiveMode) {
	      Serial.println("OK");
	    }
	    inputString = "";
	  }
	  else {
	    while (Serial.available()) {
	      char a = Serial.read();
	      if (!(a == '\r' || a == '\n')) {
	      	inputString += a;
	      }
	    }
	    checkDataReceived(inputString);

	    if (endConfigMode(inputString)) {
	    	Serial.println("OK");
	    	receiveMode = false;
	    }
	    inputString = "";
	  }
}

boolean startConfigMode(String stringReceived){
  //int indexString = stringReceived.equals(prefixDetect[0]);
  return stringReceived.equals(prefixDetect[0]);
}

boolean endConfigMode(String stringReceived){
  //int indexString = stringReceived.equals(prefixDetect[1]);
  return stringReceived.equals(prefixDetect[1]);
}


void checkDataReceived(String stringReceived) {

	for (int i = 2; i < 6; i ++) {
		if (stringReceived.startsWith(prefixDetect[i]) ) {
			int idx = stringReceived.indexOf(prefixDetect[i]);
			data[i - 2] = stringReceived.substring(idx + prefixDetect[i].length());
			Serial.println("OK");
		}
	}
}

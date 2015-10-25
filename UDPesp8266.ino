#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESP8266CarByCmmc";
const char *password = "1234567890";

char packetBuffer[1500]; //buffer to hold incoming packet
int count = 0;

float vaules = 0;
float vaulesR = 0;
float vaulesL = 0;

float x, y, z, iy;
//
unsigned long timeStar, timeEnd;
boolean readStar = true;
boolean readStop = false;
boolean testRate = true;

int counts = 0;

//const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

//int sensorValue = 0;        // value read from the pot
//
WiFiUDP Udp;

void setup() {


  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(16, OUTPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //WiFi.mode(WIFI_AP_STA);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress destinationIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(destinationIP);

  if ( Udp.begin(80) > 0) {
    Serial.print("UDP star port : ");
    Serial.println(Udp.localPort());
  } else {
    Serial.println("Error");
  }


}

void loop() {
  // put your main code here, to run repeatedly:

  while (true) {

    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    //
    if (packetSize == 0 ) {

      unsigned long ts = timeStar;
      timeEnd = millis();
      if ((timeEnd - ts) > 500) {


        analogWrite(12, 0); // IN1
        analogWrite(13, 0); // IN2
        analogWrite(14, 0); // IN3
        analogWrite(16, 0); // IN4

        analogWrite(4, 0); // ENA(IN1)
        analogWrite(5, 0); // ENB(IN3)
        // readStar = true;
        // readStop = false;
      }

    }
    //

    if (packetSize > 0 )
    {

      // analogWrite(16,0);
      // if (readStar == true) {
      timeStar = millis();
      // readStar = false;
      // readStop = true;
      // }
      timeEnd = millis();
      /*
          timeEnd = millis();
          counts++;
        */
      /* Serial.print("Received packet of size ");
       Serial.println(packetSize);
       Serial.print("From ");
       IPAddress remoteIp = Udp.remoteIP();
       Serial.print(remoteIp);
       Serial.print(", port ");
       Serial.println(Udp.remotePort());
      */
      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 255);
      if (len > 0) packetBuffer[len] = 0;

      // Serial.print("Contents:");
      //Serial.println(packetBuffer);
      String b = packetBuffer;
      // Serial.println(b);
      //get data x y z
      x = b.substring((b.indexOf('x') + 1), b.indexOf('y')).toInt();
      y = b.substring((b.indexOf('y') + 1), b.indexOf('z')).toInt();
      z = b.substring((b.indexOf('z') + 1), b.indexOf('!')).toInt();
      //show data
      Serial.print("x :");
      Serial.println(x);
      Serial.print("y :");
      Serial.println(y);
      Serial.print("z :");
      Serial.println(z);
      //drive motor L - R
      //go forward IN1 and IN3 High
      //back ward IN2 and IN4 high

      //do go forward if x<0
      if (x <= -1) {

        vaules = map(abs(x), 0, 5, 0, 1023);
        if (vaules > 1023) {
          vaules = 1023;
        }
        // vaules = vaules*200;
        //
        iy = map(abs(y), 0, 5, 0, 1023);
        if (iy > 1023) {
          iy = 1023;
        }
        //

        //
        vaulesR = vaules;
        vaulesL = vaules;


        // check y for trun
        if (y <= 1) // trun R  low valueR
        {
          vaulesL = vaulesL - iy;
        

        }

        if (y >= -1) // trun L  low valueL
        {
          vaulesR = vaulesR - iy;
        

        }
        analogWrite(12, 1023); // IN1  F //L
        analogWrite(13, 0); // IN2    //L
        analogWrite(14, 1023); // IN3 F// R
        analogWrite(16, 0); // IN4    //R
        //fix bug
        if (vaulesR <= 1) {
          vaulesR = 0;
        }

        if (vaulesR >= 1023) {
          vaulesR = 1023;
        }

        if (vaulesL <= 1) {
          vaulesL = 0;
        }

        if (vaulesL >= 1023) {
          vaulesL = 1023;
        }
        //end fix bug

        analogWrite(4, vaulesR); // ENA(IN1)
        analogWrite(5, vaulesL); // ENB(IN3)
      }// end go forward

      // do back ward
      if (x >= 1)
      {
        vaules = map(abs(x), 0, 5, 0, 1024);
        if (vaules > 1023) {
          vaules = 1023;
        }
        // vaules = vaules*200;
        //
        iy = map(abs(y), 0, 5, 0, 1023);
        if (iy > 1023) {
          iy = 1023;
        }
        //
        vaulesR = vaules;
        vaulesL = vaules;


        // check y for trun
        if (y <= 1) // trun R  low valueR
        {
          vaulesL = vaulesL - iy;
          
        }

        if (y >= -1) // trun L  low valueL
        {
          vaulesR = vaulesR - iy;
          
        }
        analogWrite(12, 0); // IN1   //L
        analogWrite(13, 1023); // IN2  B  //L
        analogWrite(14, 0); // IN3 // R
        analogWrite(16, 1023); // IN4   B //R
        //fix bug
        if (vaulesR <= 1) {
          vaulesR = 0;
        }

        if (vaulesR >= 1023) {
          vaulesR = 1023;
        }

        if (vaulesL <= 1) {
          vaulesL = 0;
        }

        if (vaulesL > 1023) {
          vaulesL = 1023;
        }
        //end fix bug
        analogWrite(4, vaulesR); // ENA(IN1)
        analogWrite(5, vaulesL); // ENB(IN3)
      }//end back ward

      //
       if (x > -1 && x < 1 ){
        //
        vaules = map(abs(x), 0, 5, 0, 1023);
        if (vaules > 1023) {
          vaules = 1023;
        }
        // vaules = vaules*200;
        //
        iy = map(abs(y), 0, 5, 0, 1023);
        if (iy > 1023) {
          iy = 1023;
        }
        //
        vaulesR = vaules;
        vaulesL = vaules;
            // check y for trun
        if (y <= 1) // trun R  low valueR
        {
       
            vaulesR = iy;
          
        }

        if (y >= -1) // trun L  low valueL
        {
          
         
            vaulesL = iy;
          
        
       }
       analogWrite(12, 1023); // IN1  F //L
        analogWrite(13, 0); // IN2    //L
        analogWrite(14, 1023); // IN3 F// R
        analogWrite(16, 0); // IN4    //R

        //end fix bug
        analogWrite(4, vaulesR); // ENA(IN1)
        analogWrite(5, vaulesL); // ENB(IN3)
       }
        //show data
      Serial.print("vaules :");
      Serial.println(vaules);
      Serial.print("vaulesL :");
      Serial.println(vaulesL);
      Serial.print("vaulesR :");
      Serial.println(vaulesR);
      //drive motor L - R
      //
      // int i  = b.toInt();
      // i = abs(i);
      // analogWrite(16,i*25);

      //Udp.flush();
      /*if(i = 1){
        analogWrite(16,64);
      }
       if(i = 2){
        analogWrite(16,128);
      }
       if(i = 3){
        analogWrite(16,180);
      }
       if(i = 4){
        analogWrite(16,255);
      }*/

      /*if((timeEnd - timeStar) > 1000 && readStop == true ){

          Serial.print("Packet per 1 sec :");
          Serial.println(counts);
          counts = 0;
          testRate = false;
      }
      */
    }
  }

}
/*boolean readBatt() {
  boolean x = true;

  // read the analog in value:// RA=22k RB=1k  7.6V = 305 // 40 per 1V // stop work add Vbatt = 6.5V(3.25V per cell) = 40*6.25 = 250
  sensorValue = analogRead(analogInPin);

  // print the results to the serial monitor:
  //Serial.print("sensor = " );
  //Serial.println(sensorValue);
  if (sensorValue < 250) {
    x = false;
  }
  //delay(50);
  return x;

}*/


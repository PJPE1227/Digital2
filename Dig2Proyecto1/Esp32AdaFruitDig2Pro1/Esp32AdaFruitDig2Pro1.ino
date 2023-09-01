#include "config.h"

char incoming[14];
int cont = 0;
char thermo[5];
String temp;
char RTStime[8];
String RTS;
char bol[1];
int light = 0;

AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *watchstuff = io.feed("time");
AdafruitIO_Feed *testbutton = io.feed("testbutton");

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  delay(5000);
}

void loop() {
  io.run();
  RTS = "";
  if(Serial2.available()>0){
    if(Serial2.read() == 'x'){
      Serial2.readBytesUntil(10, thermo, 5); 
      Serial2.readBytesUntil(10, RTStime, 8);
      Serial2.readBytesUntil(10, bol, 1 );
      if(bol[0] == '0'){
        light = 0;
      }else{
        light = 1;
      }
      temp = thermo;
      for(int i = 0; i < 8; i++){
        RTS = RTS + RTStime[i];
      }
      Serial.println(temp);
      Serial.println(RTS);
      Serial.println(light);
      if(cont == 2){
        temperature->save(temp);
        watchstuff ->save(RTS);
        testbutton->save(light);
        cont = 0;
      }
      cont++;
    }
  }
  /*if(light != NULL){
    testbutton->save(light);
  }*/
}
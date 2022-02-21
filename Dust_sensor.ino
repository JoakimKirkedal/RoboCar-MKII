

/* BEFORE RUNNING THE CODE: (Works only for group's 2 robocar and sensor)
RED wire to VCC (5V)
BROWN wire to GND
YELLOW wire to pin 13 
*/
const char* dust_topic = "eaaa/itek/e21a/car2/dust";
const char* smoke_topic = "eaaa/itek/e21a/car2/smoke";

// variables for the SMOKE particles
int P10;
int oldP10;
int P25;
int oldP25;
int P100;
int oldP100;

//variables for the DUST particles
int D10;
int oldD10;
int D25;
int oldD25;
int D100;
int oldD100;

//character array to store the bytes
char dust[30];

#include <WiFi.h>  //new
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);



void dust_sensor() {

if (Serial2.read() == 0x42 && Serial2.read() == 0x4D){  // when initializing
  int message = 0;
for(int i = 0; i<30; i++){
  while(!Serial2.available());
    dust[message++]= Serial2.read(); // the sensor detects a change in the air and the Smoke/Dust functions are triggered

}}
//client.publish("eaaa
Smoke(); //checking and displaying SMOKE particles
Dust(); //checking and displaying DUST particles
}


void Smoke(){
oldP10 = P10; // when the loop starts again, if the particles haven't change it won't print the value again
P10 = (dust[2]*256)+dust[3]; // particles stored in a character array (bytes)
if (oldP10 != P10){
  DataToMsg_smoke(smoke_topic, P10);
Serial.print("Smoke particles (1 micrometer or less): "); 
Serial.print(P10);
Serial.print(" ug/m3"); // dust and smoke unit
Serial.println(""); // just a blank line
}
 
oldP25 = P25;
P25 = (dust[4]*256)+dust[5];
if (oldP25 != P25){
  DataToMsg_smoke(smoke_topic, P25);
Serial.print("Smoke particles (2.5 micrometer or less): ");
Serial.print(P25);
Serial.print(" ug/m3");
Serial.println("");
}
oldP100 = P100;
P100 = (dust[6]*256)+dust[7];
if (oldP100 != P100){
  DataToMsg_smoke(smoke_topic, P100);
Serial.print("Smoke particles (10 micrometer or less): ");
Serial.print(P100);
Serial.print(" ug/m3");
Serial.println("");
}}

void DataToMsg_dust(const char* dust_topic, int Value){
  String temp = String(Value);
  char Pub_Msg[temp.length() +1];
  temp.toCharArray(Pub_Msg, temp.length() +1);
  client.publish(dust_topic, Pub_Msg);
}

void DataToMsg_smoke(const char* smoke_topic, int Value){
  String temp = String(Value);
  char Pub_Msg[temp.length() +1];
  temp.toCharArray(Pub_Msg, temp.length() +1);
  client.publish(smoke_topic, Pub_Msg);
}
void Dust(){

oldD10 = D10;
D10 = (dust[2]*256)+dust[3];
if (oldD10 != D10){
  DataToMsg_dust(dust_topic, D10);
  
Serial.print("Dust particles (1 micrometer or less): ");
Serial.print(D10);
Serial.print(" ug/m3");
Serial.println("");

}
oldD25 = D25;
D25 = (dust[4]*256)+dust[5];
if (oldD25 != D25){
  DataToMsg_dust(dust_topic, D25);
Serial.print("Dust particles (2.5 micrometer or less): ");
Serial.print(D25);
Serial.print(" ug/m3");
Serial.println("");

}
oldD100 = D100;
D100 = (dust[6]*256)+dust[7];
if (oldD100 != D100){
  DataToMsg_dust(dust_topic, D100);
Serial.print("Dust particles (10 micrometer or less): ");
Serial.print(D100);
Serial.print(" ug/m3");
Serial.println("");
}}

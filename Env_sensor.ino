
/*!
 * @file getVocIndex.ino
 * @brief Read the environmental VOC index.  Range: 0-500;
 * @n Experimental phenomena: read environmental VOC index once per second and print the value in serial port
 *
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2020-12-18
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_SGP40
 */
 //This is to include the SGP40 Libary.
#include <DFRobot_SGP40.h>
DFRobot_SGP40 sgp40;
//#include <WiFi.h>  //new
//#include <PubSubClient.h>
//WiFiClient espClient;
//PubSubClient client(espClient);
int SGP40_old;
int SGP40_new;
/* 
 * Method 1: pass in the specified I2C object address 
 * #include <Wire.h>
 * DFRobot_SGP40    mySgp40(&Wire);

 * Method 2: use the default I2C object&Wire
 * I2C Default Address：0x59
 */

#include <Wire.h>
TwoWire wire2 (0);
DFRobot_SGP40    mySgp40(&wire2);
//DFRobot_SGP40    mySgp40;

// We used pins 33 and 32 for this sensor since they're available.
// SDA(D) pin goes to pin 33 on the ESP32, GREEN
// SCL(C) pin goes to pin 32 on the ESP32, BLUE


void environment_sensor() {
  /* 
   * Get VOC index 
   * VOC index can directly indicate the condition of air quality. The larger the value, the worse the air quality
   *    0-100，no need to ventilate,purify
   *    100-200，no need to ventilate,purify
   *    200-400，ventilate,purify
   *    400-500，ventilate,purify intensely
   * Return VOC index, range: 0-500
   */
  uint16_t index = mySgp40.getVoclndex();
  SGP40_old = SGP40_new;
  SGP40_new = index;
  char dispsize[100];
    
  if (SGP40_old != SGP40_new){
  //Serial.print("vocIndex = ");
  //Serial.println(index);
  snprintf(dispsize, 100, "%d", SGP40_new);
  client.publish("eaaa/itek/e21a/car2/air", dispsize);
  }
  //delay(1000);
}


void callback2(char* topic, byte* payload, unsigned int length) {
Serial.print("Message arrived [");
Serial.print(topic);
Serial.print("] ");
for (int i = 0; i < length; i++) {
Serial.print((char)payload[i]);
}

}

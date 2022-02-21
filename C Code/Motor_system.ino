/*
  Converted to esp32 listening on
  Basic ESP8266 MQTT example
  This sketch demonstrates the capabilities of the pubsub library in
  combination
  with the ESP8266 board/library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
  it receives. NB - it assumes the received payloads are strings not
  binary
  - If the first character of the topic "inTopic" is an 1, switch ON the
  ESP Led,
  else switch it off
  It will reconnect to the server if the connection is lost using a
  blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how
  to
  achieve the same result without blocking the main loop.
  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences
  -> Additional Boards Manager URLs":
  http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the
  ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/
//#include <ESP8266WiFi.h>   commented
#include <WiFi.h>  //new
#include <PubSubClient.h>
#define Right_Mot_Forw 12
#define Right_Mot_Backw 14
#define Left_Mot_Forw 15
#define Left_Mot_Backw 13
int BUILTIN_LED = 2;  //new
// Update these with values suitable for your network.
//const char* ssid = "ITEK 2nd";
//const char* password = "Four_Sprints_F21v";
const char* mqtt_server = "10.120.0.230";
const char* inTopic = "eaaa/itek/e21a/car2";
//WiFiClient espClient;
//PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback1(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == 'B') {
    digitalWrite(Left_Mot_Forw, HIGH);
    //Serial.println(digitalRead(Left_Mot_Forw));
    digitalWrite(Left_Mot_Backw, LOW);
    //Serial.println(digitalRead(Left_Mot_Backw));
    digitalWrite(Right_Mot_Forw, HIGH);
    //Serial.println(digitalRead(Right_Mot_Forw));
    digitalWrite(Right_Mot_Backw, LOW);
    //Serial.println(digitalRead(Right_Mot_Backw));

  }
  if ((char)payload[0] == 'F') {
    digitalWrite(Left_Mot_Forw, LOW);
    //Serial.println(digitalRead(Left_Mot_Forw));
    digitalWrite(Left_Mot_Backw, HIGH);
    //Serial.println(digitalRead(Left_Mot_Backw));
    digitalWrite(Right_Mot_Forw, LOW);
    //Serial.println(digitalRead(Right_Mot_Forw));
    digitalWrite(Right_Mot_Backw, HIGH);
    //Serial.println(digitalRead(Right_Mot_Backw));
  }
  if ((char)payload[0] == 'S') {
    digitalWrite(Left_Mot_Forw, LOW);
    //Serial.println(digitalRead(Left_Mot_Forw));
    digitalWrite(Left_Mot_Backw, LOW);
    //Serial.println(digitalRead(Left_Mot_Backw));
    digitalWrite(Right_Mot_Forw, LOW);
    //Serial.println(digitalRead(Right_Mot_Forw));
    digitalWrite(Right_Mot_Backw, LOW);
    //Serial.println(digitalRead(Right_Mot_Backw));
  }


  if ((char)payload[0] == 'R') {
    digitalWrite(Right_Mot_Forw, LOW);
    //Serial.println(digitalRead(Right_Mot_Forw));
    digitalWrite(Right_Mot_Backw, HIGH);
    //Serial.println(digitalRead(Left_Mot_Backw));
    digitalWrite(Left_Mot_Forw, LOW);
    //Serial.println(digitalRead(Left_Mot_Forw));
    digitalWrite(Left_Mot_Backw, LOW);
    //Serial.println(digitalRead(Left_Mot_Backw));

  }
  if ((char)payload[0] == 'L') {
    digitalWrite(Right_Mot_Forw, LOW);
    //Serial.println(digitalRead(Right_Mot_Forw));
    digitalWrite(Right_Mot_Backw, LOW);
    //Serial.println(digitalRead(Right_Mot_Backw));
    digitalWrite(Left_Mot_Forw, LOW);
    //Serial.println(digitalRead(Left_Mot_Forw));
    digitalWrite(Left_Mot_Backw, HIGH);
    //Serial.println(digitalRead(Left_Mot_Backw));
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void motor_system() {


 
  
  
  if (!client.connected()) {
    reconnect();
  }
  long now = millis();
  client.loop();
  if (millis() - now > 7)
  {
    Serial.print("duration of  mqtt.loop()  is  : milisec  ->");
    Serial.println(millis() - now);
  }
  now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}

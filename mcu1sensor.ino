#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Servo.h>

const char* ssid = "My ASUS";                   // wifi ssid
const char* password =  "qwertyuiop";         // wifi password
const char* mqttServer = "192.168.43.207";    // IP adress Raspberry Pi
const int mqttPort = 1883;
const char* mqttUser = "username";      // if you don't have MQTT Username, no need input
const char* mqttPassword = "qwerty";   // if you don't have MQTT Password, no need input

const char* topic = "test2";

int trigPin=5;
int echoPin=4;

long duration;
Servo servo;

int dist;
String distance_str;
char distance[50];

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(2);
  
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}

void mqtt_connect(){
  
  client.setServer(mqttServer, mqttPort);
  //client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client1", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }
  }

void loop() { 
   
    for (int i=0;i<=180;i++){
      servo.write(i);
      delay(20);
      dist=calculateDistance();
      distance_str = String(dist);
      distance_str.toCharArray(distance,distance_str.length()+1);
    
      if(dist<50){
        mqtt_connect();
        client.publish(topic, distance);
        client.publish(topic, "MCU#2");
        //client.subscribe(topic);
        client.disconnect();
      }
      Serial.print(distance);
      Serial.print(" ");
    }
    
    for (int i=180;i>=0;i--){
      servo.write(i);
      delay(20);
      dist=calculateDistance();
      distance_str = String(dist);
      distance_str.toCharArray(distance,distance_str.length()+1);
      
      if(dist<50){
        mqtt_connect();
        client.publish(topic, distance);
        client.publish(topic, "MCU#2");
        //client.subscribe(topic);
        client.disconnect();
      }
      Serial.print(distance);
      Serial.print(" ");
    }
    
    client.loop();
  
}

int calculateDistance()
{
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration=pulseIn(echoPin,HIGH);
  dist= duration*0.034/2;
  return dist;
}  

  #include <PubSubClient.h>
  #include <stdlib.h>
  #include <ESP8266WiFi.h>
  #include <SoftwareSerial.h>
  const char* ssid = "Hue Tang 2";
  const char* password = "duatienday";
  const char* mqtt_server = "broker.mqtt-dashboard.com";
  uint16_t PORT = 1883;
//  const char* mqtt_server = "eefab35a990f4c88963ab8a93196f0be.s1.eu.hivemq.cloud";
//  uint16_t PORT = 8884;

  unsigned long timer;
  float temp;
  String sData = "";

  WiFiClient espClient;
  PubSubClient client(espClient);
  SoftwareSerial swSerial(D2, D3); // TX RX
void setup_wifi()
{
delay(10);
Serial.print("connecting to");
 Serial.println(ssid);
 WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED)
 {
 delay(500);
 Serial.print("-");
 }

Serial.println();
 Serial.println("WiFi Connected");
 Serial.println("WiFi got IP");
 Serial.println();
 Serial.println(WiFi.localIP());
}

void swSerialTransfer(String code, String value) {
    swSerial.print(code);
    swSerial.print(":");
    swSerial.print(value);
    swSerial.print("#");
}

void callback(char* topic, byte* payload, unsigned int length)
{

  Serial.print("Message arrived : ");
  Serial.print(String(topic));
  Serial.print(" : ");
  String value;
  for (int i = 0; i < length; i++){
    value += (char)payload[i];
  }
  Serial.print(value);
  Serial.println();
  if (String(topic) == "smarthome/01/R1") swSerialTransfer("R1",value);
  if (String(topic) == "smarthome/01/R2") swSerialTransfer("R2",value);
  if (String(topic) == "smarthome/01/R3") swSerialTransfer("R3",value);
  if (String(topic) == "smarthome/01/H1") swSerialTransfer("H1",value);
  if (String(topic) == "smarthome/01/G1") swSerialTransfer("G1",value);
}

void reconnect()
{
  
  while(!client.connected()){
  Serial.println("Attempting MQTT connection");
  if(client.connect("smarthomeDevice01"))
    {
    Serial.println("Connected");
    client.subscribe("smarthome/01/status");
    client.subscribe("smarthome/01/MF58");
    client.subscribe("smarthome/01/LDR");
    client.subscribe("smarthome/01/R1");
    client.subscribe("smarthome/01/R2");
    client.subscribe("smarthome/01/R3");
    client.subscribe("smarthome/01/H1");
    Serial.print("subscribed!");
    client.publish("smarthome/01/status", "smarthomeDevice01 connected");
    }
  else
    {
    Serial.print("Failed, rc = ");
    Serial.print(client.state());
    Serial.println("Waiting for 5 seconds to try again");
    delay(5000);
     }
 }
}

void setup()
{
 
 //pinMode(2, OUTPUT);
 Serial.begin(115200);
 swSerial.begin(9600);
 setup_wifi();
 client.setServer(mqtt_server, PORT);
 client.setCallback(callback);
 reconnect();
}

void loop()
{
  while (swSerial.available()){
    String code = swSerial.readStringUntil(':');
    char value[10];
    swSerial.readStringUntil('#').toCharArray(value, 10);
    Serial.print(code);
    Serial.print(":");
    Serial.println(value);
    if (code == "T1") client.publish("smarthome/01/MF58", value);
    if (code == "L1") client.publish("smarthome/01/LDR",  value);
    if (code == "R1") client.publish("smarthome/01/R1",  value);
    if (code == "R2") client.publish("smarthome/01/R2",  value);
    if (code == "R3") client.publish("smarthome/01/R3",  value);
    if (code == "H1") client.publish("smarthome/01/H1",  value);
    if (code == "G1") client.publish("smarthome/01/G1",  value);
  }
  if (millis() - timer > 1000) {
    timer = millis();
  }
  if(!client.connected())
  {
    reconnect();
    Serial.print("disconnected");
  }
  client.loop();
  
}

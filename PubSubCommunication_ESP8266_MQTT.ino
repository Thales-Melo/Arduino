/*
 Basic ESP8266 MQTT example

 NEED to install: PubSub library 

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "Hello World" to the topic "pic1-gusthaliel/msg" every two seconds
  - subscribes to the topic "pic1-gusthaliel/led", printing out any messages
    it receives. It assumes the received payloads are strings not binary
  - If the first character of the topic "pic1-gusthaliel/led" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define BME_ID 0x76 //BMP I2C REGISTER
#define LED_BUILTIN 2  //For node_MCU Lolin esp8266

Adafruit_BMP280 bmp;

// Update these with values suitable for your network.
// ###################### Trocar aqui!!! ##################### 
const char* ssid = "PIC2-2.4G";
const char* password = "engcomp@ufes";
//Free mqtt server for testing
const char* mqtt_server = "broker.mqtt-dashboard.com";
//Local MQTT server - Tested using mosquitto mqtt for windows and linux
//const char* mqtt_server = "192.168.137.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
float temp_value, pres_value;

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


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(LED_BUILTIN, LOW);   // Change for nodeMCU Lolin
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } 
  else {
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    digitalWrite(LED_BUILTIN, HIGH);   // Change for nodeMCU Lolin
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
      client.publish("pic1-gusthaliel/msg", "Hello World!!");
      // ... and resubscribe
      client.subscribe("pic1-gusthaliel/led");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);     // Just for ESP8266 nodeMCU Lolin
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  if(!bmp.begin(BME_ID)){ //SE O SENSOR NÃO FOR INICIALIZADO NO ENDEREÇO I2C 
    Serial.println(F("Sensor BMP280 não foi identificado! Verifique as conexões.")); 
    while(1); //SEMPRE ENTRE NO LOOP
  }
}

void loop() {
  if (!client.connected()) {
     reconnect();
  }
  client.loop();

  Serial.print(F("Temperatura: ")); /*If you use F() you can move constant strings to the program memory instead of the ram. 
                                      This will take up space that will decrease the amount of 
                                      other code you can write. But it will free up dynamic ram.
                                    */
  temp_value = bmp.readTemperature();
  Serial.print(temp_value); 
  Serial.println(" *C (Grau Celsius)"); 
  
  Serial.print(F("Pressão: ")); 
  pres_value = bmp.readPressure();
  Serial.print(pres_value); //IMPRIME NO MONITOR SERIAL A PRESSÃO
  Serial.println(" Pa (Pascal)"); 
  
  Serial.println("-----------------------------------");

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    sprintf (msg, "%f;%f", temp_value, pres_value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("pic1-gusthaliel/temp", msg);
  }
}

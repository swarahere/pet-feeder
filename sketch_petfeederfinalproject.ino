#include <UbidotsESPMQTT.h>
#include <ESP32Servo.h>

#define RELAY 2
#define SERVO_PIN 13  // Servo ka pin

#define TOKEN "BBUS-mKqfeexWXEpA5Gfdma395KxXktnqkt" // Ubidots TOKEN
#define WIFISSID "swara"  // WiFi SSID
#define WIFIPASS "9876543210"  // WiFi Password

Ubidots client(TOKEN);
Servo myServo;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  bool command = *payload - '0';  // Convert char to int
  Serial.print("Command: ");
  Serial.println(command);
  
  digitalWrite(RELAY, command);
  if (command == 1) {
    Serial.println("Turning Servo to 90 degrees...");
    myServo.write(90);  // Servo 90 degree ghoomega
    delay(1000); // Thoda rukne de, dekh raha hai kya ho raha hai
} else {
    Serial.println("Turning Servo to 0 degrees...");
    myServo.write(0);  // Wapas 0 degree par
    delay(1000);
    digitalWrite(RELAY, LOW);
    Serial.println("Feeding done. Servo back to 0.");
}

}

void setup() {
  Serial.begin(9600);
  Serial.println("Init... T4_Smart_Home");
  pinMode(RELAY, OUTPUT);
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Start position
  
  Serial.print("Connecting to SSID: ");
  Serial.print(WIFISSID);
  client.wifiConnection(WIFISSID, WIFIPASS);
  Serial.println("Done");
  
  Serial.println("Initializing Ubidots Connection...");
  client.ubidotsSetBroker("industrial.api.ubidots.com");
  client.setDebug(true);
  client.begin(callback);
  client.ubidotsSubscribe("pet-feeder","feed");
  Serial.println("Done");
}

void loop() {
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe("pet-feeder","feed");
  }
  client.loop();
  delay(1000);
}
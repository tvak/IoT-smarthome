#include "ESP8266WiFi.h"  // including lib for ESP WiFi functionality
#include "DHT.h"    // including DHT lib for DTH sensor(temperature&humidity)
#define DHTPIN 0    // what digital pin we're connected to
#define FAN_PIN 2  // define pin used for fan

// Defining DHT sensor type that we're using!
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);  // setting dht

const char host[] = "192.168.43.55";  // our gateway ip

// Using mobile hotspot for testing
const char* MY_SSID = "OnePlus3 tt";  // wifi ssid
const char* MY_PWD =  "12345678";     // wifi password

int ledPin = LED_BUILTIN; // We are using buildin led for demonstrating light logic
String automation = "ON";

WiFiClient client;    // creating client for sending data to gateway
WiFiServer server(80);  // setting server for receiving commands (light on/off, fan on/off)

#define SENSING_MILLIS 30000  // interval for sensing activities(temperature, humidity, luminance)
unsigned long previousMillis = 0; // variable that is used in timer

void setup()
{
  Serial.begin(115200); // begin serial in 115200 baud
  dht.begin();  // begin dht
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD); // Connect to wifi network(in this case mobile hotspot)
  Serial.println("going into wl connect");

  while (WiFi.status() != WL_CONNECTED) //not connected,  ...waiting to connect
    {
      delay(1000);
      Serial.print(".");
    }
  Serial.println("wl connected");
  Serial.println("");
  Serial.println("Credentials accepted! Connected to wifi\n ");
  Serial.println("");

  // Start the server in node
  server.begin();
  Serial.println("Server started");
 
  // Print the controllers IP address which is used for sending commands(controlling lights and fan)
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // we are using internal led to simulate "lights" so we set it here
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  // and set correct defined digital pin to fan
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis(); // "starting" timer by setting current millis in variable
  if(currentMillis - previousMillis > SENSING_MILLIS){ // compare current millis to previous one(first time it's 0 (line22)) if interval(30s)(line21) is up make measurements
    previousMillis = currentMillis; // "reset" timer

    // Read humidity
    int Humidity = dht.readHumidity();
    // Read temperature as Celsius
    int Temperature_Cel = dht.readTemperature();
    // Read luminance
    int luminance = analogRead(0);
    // Light status is off when program starts
    String lightstatus = "OFF";
    // Same as fan status
    String fanstatus = "OFF";
    // Check if automation is selected
    if (automation=="ON"){
      // if it is then check temperature and turn on fan when it's too high
      if (Temperature_Cel > 23){
        Serial.println("Too hot! Fan on!");
        digitalWrite(FAN_PIN, HIGH);
        fanstatus = "ON";
      }else{
        digitalWrite(FAN_PIN, LOW);
        fanstatus = "OFF";
      }
      // Automation for ligths. If there are dark(luminance<200) when measurements are done(30s interval) turning on light else light=off.
      if (luminance <200){
        Serial.println("It's dark! Lights on");
        digitalWrite(ledPin, LOW);
        lightstatus = "ON";
      }else{
        Serial.println("No need for light! Ligths off");
        digitalWrite(ledPin, HIGH);
        lightstatus = "OFF";
      }
    }
    // Check if any reads failed and exit early (to try again).
    if (isnan(Humidity) || isnan(Temperature_Cel) || isnan(luminance))
    {
      Serial.println("Failed to read from some sensor!");
      return;
    }
    // Printing measurements in serial
    Serial.print("Humidity: ");
    Serial.print(Humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(Temperature_Cel);
    Serial.print(" *C ");
    Serial.print("Luminance:");
    Serial.println(luminance);
    Serial.print("Light:");
    Serial.println(lightstatus);
    Serial.print("Fan:");
    Serial.println(fanstatus);
    Serial.print("Automation:");
    Serial.println(automation);

    Serial.println("\nStarting connection to server..."); 
    // if you get a connection, report back via serial:
    // Using WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
  
    // This will send the get request to the server with measurement data
    client.print("GET /smarthome/dataCollector.php?Temperature="+(String) Temperature_Cel
            +"&Humidity="+(String) Humidity
            +"&Luminance="+(String) luminance
            +"&Light="+(String) lightstatus
            +"&Fan="+(String) fanstatus
            +"&Automation="+(String) automation+ " HTTP/1.1\r\n" +"Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

    // Read all the lines of the reply from server and print them to Serial (at this point we don't have any response in server side)
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println("closing connection");
    }
   // Here we implement logic for commands received from wifi to control lights
   // Check if a client has connected
   WiFiClient client2 = server.available();
   if (!client2) {
     return;
   }
   // Wait until the client sends some data
   Serial.println("new client");
   while(!client2.available()){
     delay(1);
   }
   // Read the first line of the request
   String request = client2.readStringUntil('\r');
   Serial.println(request);
   client2.flush();
   // Match the request
   int value = digitalRead(ledPin);
   if (request.indexOf("/AUTOMATION=ON") != -1)  {
     automation = "ON";
   }
   if (request.indexOf("/AUTOMATION=OFF") != -1)  {
     automation = "OFF";
   }
   if (request.indexOf("/LED=ON") != -1)  {
     digitalWrite(ledPin, LOW);
     value = LOW;
   }
   if (request.indexOf("/LED=OFF") != -1)  {
     digitalWrite(ledPin, HIGH);
     value = HIGH;
   } 
   if (request.indexOf("/FAN=ON") != -1)  {
     digitalWrite(FAN_PIN, HIGH);
   }
   if (request.indexOf("/FAN=OFF") != -1)  {
     digitalWrite(FAN_PIN, LOW);
   }
   delay(1);
   Serial.println("Client disonnected");
   Serial.println("");
}

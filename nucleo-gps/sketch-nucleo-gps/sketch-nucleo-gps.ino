#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

boolean connected = false;
// Die serielle Verbindung zum GPS Modul
SoftwareSerial gpsSerial(3, 4);

SoftwareSerial loraSerial(7, 6);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  loraSerial.begin(9600);
  
  printLoraConfig();
  //Join lora network
  joinLoraNetwork();
  
}

void loop() {
  //Serial.println(getGPSLocation());
  printGPSOutput(1000);
  Serial.println("");
  delay(10000);
}

String getGPSLocation()
{
  
  feedGPS(1000);
  String locText = "Latitude: ";
  locText += gps.location.lat();
  locText += ", Longitude: ";
  locText += gps.location.lng();
  return locText;
}

static void feedGPS(unsigned long ms)
{
  unsigned long start = millis();
  gpsSerial.listen();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

void joinLoraNetwork()
{
  loraSerial.write("AT+JOIN");
  printLoraOutput(10000);
}

void printLoraConfig()
{
  loraSerial.write("AT+ID");
  printLoraOutput(1000);
}

void sendLoraMessage(String msg)
{
  String message = "AT+MSG=\"";
  message += msg;
  message += "\""; 
  char p[message.length()+1];
  message.toCharArray(p, message.length()+1);
  loraSerial.write(p);
  printLoraOutput(1000);
}

void printLoraOutput(unsigned long ms)
{
  unsigned long start = millis();
  loraSerial.listen();
  do
  {
    while (loraSerial.available()) {
      Serial.write(loraSerial.read());
    }
  } while (millis() - start < ms);
}

void printGPSOutput(unsigned long ms)
{
  unsigned long start = millis();
  gpsSerial.listen();
  do 
  {
    while (gpsSerial.available())
      Serial.write(gpsSerial.read());
  } while (millis() - start < ms);
}

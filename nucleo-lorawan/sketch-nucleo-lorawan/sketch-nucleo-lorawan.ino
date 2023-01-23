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
  
  
  //Join lora network

  resetLora();
  
  setLoraConfigOTAA();

  //printLoraConfig();
  
  joinLoraNetwork();
  
}

void loop() {
  //Serial.println(getGPSLocation());
  String s = getGPSLocation();
  Serial.println(s);
  
  //printGPSOutput(1000);
  sendLoraMessage(s);
  Serial.println("");
  delay(10000);
}

String getGPSLocation()
{
  
  feedGPS(1000);
  //String locText = "Latitude: ";
  //locText += gps.location.lat();
  //locText += ", Longitude: ";
  //locText += gps.location.lng();
  String msg = buildMessage(String(gps.location.lat(), 6), String(gps.location.lng(), 6), String(gps.altitude.value(), 6), String(gps.satellites.value()), String(gps.date.value()),String(gps.time.value()), "20");
  
  return msg;
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
  //printLoraOutput(10000);
  String result = getLoraOutput(6000);
  printToSerial(result);
  if(result.indexOf("Join failed") >= 0){
    joinLoraNetwork();
  }
}

void resetLora()
{
  loraSerial.write("AT+FDEFAULT");
  printLoraOutput(1000);

  //loraSerial.write("AT+RESET");
  //printLoraOutput(1000);

  delay(5000);
}

void setLoraConfigABP()
{
  loraSerial.write("AT+MODE=LWABP");
  printLoraOutput(1000);
  
  loraSerial.write("AT+ID=DEVADDR,\"260B2DB1\"");
  printLoraOutput(1000);
  
  loraSerial.write("AT+KEY=APPSKEY,\"BB9749ABD0B2A2ACE74D0FCEED6069CE\"");
  printLoraOutput(1000);

  loraSerial.write("AT+KEY=NWKSKEY,\"20D7E7CEC1859B1DAFE5479332589E39\"");
  printLoraOutput(1000);

  loraSerial.write("AT+DR=EU868");
  printLoraOutput(1000);

  loraSerial.write("AT+DR=3");
  printLoraOutput(1000);

  loraSerial.write("AT+CH=NUM,0-2");
  printLoraOutput(1000);

  
}

void setLoraConfigOTAA()
{ 
  loraSerial.write("AT+KEY=APPKEY, \"2C11FE5BED9F7936C00FA46A7A4F8991\"");
  printLoraOutput(1000);

  loraSerial.write("AT+DR=EU868");
  printLoraOutput(1000);

  //loraSerial.write("AT+CH=NUM,0-7");
  //printLoraOutput(1000);

  loraSerial.write("AT+MODE=LWOTAA");
  printLoraOutput(1000);
}

void printLoraConfig()
{
  loraSerial.write("AT+MODE");
  printLoraOutput(1000);
  
  loraSerial.write("AT+ID");
  printLoraOutput(1000);
  
  loraSerial.write("AT+DR=SCHEME");
  printLoraOutput(1000);

  loraSerial.write("AT+LW=VER");
  printLoraOutput(1000);
}

String buildMessage(String lati, String longi, String alti, String sat, String dat, String timestamp, String measure)
{
  String msg = lati + "|" + longi + "|" + alti +"|"+ sat + "|" + dat + "|" + timestamp + "|" + measure;
  return msg;
}

void sendLoraMessage(String msg)
{
  String message = "AT+CMSG=\"";
  message += msg;
  message += "\""; 
  char p[message.length()+1];
  message.toCharArray(p, message.length()+1);
  loraSerial.write(p);
  printLoraOutput(30000);
}

void printLoraOutput(unsigned long ms)
{
  printToSerial(getLoraOutput(ms));
}

void printToSerial(String output)
{
  char p[output.length()+1];
  output.toCharArray(p, output.length()+1);
  Serial.write(p);
}

String getLoraOutput(unsigned long ms)
{
  String result = "";
  unsigned long start = millis();
  loraSerial.listen();
  do
  {
    while (loraSerial.available()) {
      result += loraSerial.readString();
    }
  } while (millis() - start < ms);
  return result;
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

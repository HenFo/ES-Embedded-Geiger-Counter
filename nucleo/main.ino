#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

boolean connected = false;
// Die serielle Verbindung zum GPS Modul
SoftwareSerial gpsSerial(PA9, PA10);

SoftwareSerial loraSerial(PA2, PA3);

const String AppKey = "CDAE2B1CCAE402758B6E5CB59D06580D";
const String DR = "EU868";

uint counter = 0;
unsigned long lastCount;

void setup() {
  lastCount = millis();
  pinMode(PC7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PC7), callback, FALLING); 
   
  gpsSerial.begin(9600);
  loraSerial.begin(9600);
  
  setLoraConfigOTAA();
  
  joinLoraNetwork();
  
}

void loop() {
  
  String s = getMessage();
  sendLoraMessage(s);
  delay(3000);
}

void callback(){
  unsigned long newCount = millis();
  if (newCount > lastCount + 100){
    counter ++;
    lastCount = newCount;
  }
}

String getMessage()
{
  
  feedGPS(1000);
  String latString = String(gps.location.lat(), 6);
  String longString = String(gps.location.lng(), 6);
  String altString = String(gps.altitude.value(), 6);
  String satString = String(gps.satellites.value());
  String dateString = String(gps.date.value());
  String timeString = String(gps.time.value());
  String measureString = String(counter);
  
  String msg = buildMessage(latString, longString, altString, satString, dateString, timeString, measureString);
  
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
    connected = false;
    while(!connected)
    {
        write(loraSerial, "AT+JOIN");
        
        String result = read(loraSerial, 6000);
        print(result);

        if(result.indexOf("Network joined") >= 0 || result.indexOf("Joined already") >= 0){
            connected = true;
        }
        delay(5000);
    }
}

void resetLora()
{
  write(loraSerial, "AT+FDEFAULT");
  printLoraOutput(1000);
  delay(5000);
}

void setLoraConfigABP()
{
  write(loraSerial, "AT+MODE=LWABP");
  printLoraOutput(1000);
  
  write(loraSerial, "AT+ID=DEVADDR,\"260B2DB1\"");
  printLoraOutput(1000);
  
  write(loraSerial, "AT+KEY=APPSKEY,\"BB9749ABD0B2A2ACE74D0FCEED6069CE\"");
  printLoraOutput(1000);

  write(loraSerial, "AT+KEY=NWKSKEY,\"20D7E7CEC1859B1DAFE5479332589E39\"");
  printLoraOutput(1000);

  write(loraSerial, "AT+DR=EU868");
  printLoraOutput(1000);

  write(loraSerial, "AT+DR=3");
  printLoraOutput(1000);

  write(loraSerial, "AT+CH=NUM,0-2");
  printLoraOutput(1000);

}

void setLoraConfigOTAA()
{ 
  write(loraSerial, fill("AT+KEY=APPKEY, \"%1\"",AppKey));
  printLoraOutput(1000);

  write(loraSerial, fill("AT+DR=%1", DR));
  printLoraOutput(1000);
  
  write(loraSerial, "AT+MODE=LWOTAA");
  printLoraOutput(1000);
}

void printLoraConfig()
{
  write(loraSerial, "AT+MODE");
  printLoraOutput(1000);
  
  write(loraSerial, "AT+ID");
  printLoraOutput(1000);
  
  write(loraSerial, "AT+DR=SCHEME");
  printLoraOutput(1000);

  write(loraSerial, "AT+LW=VER");
  printLoraOutput(1000);
}

String buildMessage(String lati, String longi, String alti, String sat, String dat, String timestamp, String measure)
{
  String msg = lati + "|" + longi + "|" + alti +"|"+ sat + "|" + dat + "|" + timestamp + "|" + measure;
  return msg;
}

void sendLoraMessage(String msg)
{
    if(connected){
        write(loraSerial, fill("AT+CMSG=\"%1\"", msg));
        printLoraOutput(10000);
    }
}

void printLoraOutput(unsigned long ms)
{
    print(read(loraSerial, ms));
}

void printGPSOutput(unsigned long ms)
{
    print(read(loraSerial, ms));
}

String fill(String base, String param1)
{
    base.replace("%1", param1);
    return base;
}

void write(SoftwareSerial serial, String msg)
{
    msg += "\n";
    char p[msg.length()+1];
    msg.toCharArray(p, msg.length()+1);
    serial.write(p);
}


void print(String msg)
{
    // char p[msg.length()+1];
    // msg.toCharArray(p, msg.length()+1);
    // Serial.write(p);
    // write(gpsSerial, p);
}

String read(SoftwareSerial serial, unsigned long ms)
{
    String result = "";
    unsigned long start = millis();
    serial.listen();
    do
    {
        while (serial.available()) {
        result += serial.readString();
        }
    } while (millis() - start < ms);
    return result;
}
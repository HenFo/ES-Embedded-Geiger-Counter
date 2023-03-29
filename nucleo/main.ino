#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

// gibt an ob Verbindung zum Lora-Gateway besteht
boolean connected = false;

// UART-Verbindung zum GPS Modul
SoftwareSerial gpsSerial(PA9, PA10);
// UART-Verbindung zum Lora Modul
SoftwareSerial loraSerial(PA2, PA3);

// LORA Konfiguration
const String AppKey = "CDAE2B1CCAE402758B6E5CB59D06580D";
const String DR = "EU868";

// Zähler für SBM20 Messwert
uint counter = 0;
// Zeitstempel für letztes Inkrement
unsigned long lastCount;

// setup Funktion wird bei Programmstart ausgeführt
void setup() {
  // initialisiere Variable
  lastCount = millis();

  // Definition des Pins für SBM20 Messwerte und registrierung der callback Funktion bei fallender Flanke
  pinMode(PC7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PC7), callback, FALLING); 
   
  // Initialisierung der UART-Verbindungen
  gpsSerial.begin(9600);
  loraSerial.begin(9600);
  
  // Setzen der Lora-Konfiguration
  setLoraConfigOTAA();
  
  // Verbindung zum Lora-Gateway aufbauen
  joinLoraNetwork();
  
}

// loop Funktion
void loop() {
  // Zusammenstellung der zu versendenen Nachricht
  String s = getMessage();
  // Senden der Nachricht an Lora-Gateway
  sendLoraMessage(s);
  // Warten bis nächster Messwert gesendet werden soll
  delay(3000);
}

// callback Funktion die ausgeführt wird, wenn neuer Messwert von SBM20 kommt
void callback(){
  // aktuellen Zeitstempel auslesen
  unsigned long newCount = millis();
  // wenn letzter messwert mehr als 100 ms her ist hochzählen
  // (um Schwankungen bei Überbrücken zu ignorieren, da SBM20 Röhre nicht funktionsfähig)
  if (newCount > lastCount + 100){
    // Messwert inkrementieren
    counter ++;
    // zeitstempel überschreiben
    lastCount = newCount;
  }
}

// getMessage Funktion gibt einen String mit allen Daten für den Versand zurück
String getMessage()
{
  // akteulle GPS-Daten laden
  feedGPS(1000);
  // GPS-Daten und Messwert auslesen
  String latString = String(gps.location.lat(), 6);
  String longString = String(gps.location.lng(), 6);
  String altString = String(gps.altitude.value(), 6);
  String satString = String(gps.satellites.value());
  String dateString = String(gps.date.value());
  String timeString = String(gps.time.value());
  String measureString = String(counter);
  
  // Daten in einem String kodieren
  String msg = buildMessage(latString, longString, altString, satString, dateString, timeString, measureString);
  
  //kodierten String zurückgeben
  return msg;
}

// feedGPS Funktion ruft die aktuellen GPS-Daten vom GPS-Modul ab
static void feedGPS(unsigned long ms)
{
  // aktuellen Zeitstempel auslesen
  unsigned long start = millis();
  // lese vom auf GPS-Modul
  gpsSerial.listen();
  
  // wiederhole bis threshold erreicht wurde
  do 
  {
    // wiederhole solange bis keine Daten mehr verfügbar sind
    while (gpsSerial.available())
      // lesen und zwischenspeichern der aktuellen Daten von GPS-Modul 
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

// joinLoraNetwork Funktion baut Verbindung zum Lora-Netzwerk auf
void joinLoraNetwork()
{
    connected = false;
    // wiederhole bis Verbindung besteht
    while(!connected)
    {
        // sende Befehl zum Joinen
        write(loraSerial, "AT+JOIN");
        
        // warte 6s für Rückmeldung
        String result = read(loraSerial, 6000);

        // werte Rückmeldung aus
        if(result.indexOf("Network joined") >= 0 || result.indexOf("Joined already") >= 0){
            // unterbreche Schleife wenn Verbindung besteht
            connected = true;
        }
        // warte 5s
        delay(5000);
    }
}

// resetLora Funktion setzt Lora-Chip auf Werkseinstellung zurück
void resetLora()
{
  // sende Befehl für Factory Reset
  write(loraSerial, "AT+FDEFAULT");
  delay(5000);
}

// setLoraConfigOTAA Funktion setzt Konfiguration für Lora-Verbindung
void setLoraConfigOTAA()
{ 
  // setze AppKey aus TTN Einrichtung
  write(loraSerial, fill("AT+KEY=APPKEY, \"%1\"",AppKey));
  delay(1000);

  // setze DR Schema aus TTN Einrichtung
  write(loraSerial, fill("AT+DR=%1", DR));
  delay(1000);

  // setze Modus auf OTAA
  write(loraSerial, "AT+MODE=LWOTAA");
  delay(1000);
}

// buildMessage Funktion fasst alle Daten in einen String mit Trennzeichen zusammen
String buildMessage(String lati, String longi, String alti, String sat, String dat, String timestamp, String measure)
{
  String msg = lati + "|" + longi + "|" + alti +"|"+ sat + "|" + dat + "|" + timestamp + "|" + measure;
  return msg;
}

// sendLoraMessage Funktion sendet Nachricht an Lora-Gateway
void sendLoraMessage(String msg)
{
  // sende NAchricht nur wenn Verbindung besteht
    if(connected){
        // Befehl zum senden der Nachricht
        write(loraSerial, fill("AT+CMSG=\"%1\"", msg));
    }
}

// fill Funktion Hilfe für Ersetzung von Wildcards in Strings
String fill(String base, String param1)
{
    // ersetze Zeichen %1 durch param
    base.replace("%1", param1);
    return base;
}

// write Funktion schreibt String Nachricht über UART-Verbindung
void write(SoftwareSerial serial, String msg)
{
    // füge Linebreak am Ende hinzu
    msg += "\n";
    // konvertierung in charArray
    char p[msg.length()+1];
    msg.toCharArray(p, msg.length()+1);
    // schreiben des charArray
    serial.write(p);
}
// read Funktion liest String von UART-Verbindung
String read(SoftwareSerial serial, unsigned long ms)
{
    
    String result = "";
    unsigned long start = millis();
    // lese von angegebener Verbindung
    serial.listen();
    // warte mindestens angegeben ms bis Nachricht anliegt und lese Nachricht
    do
    {
        while (serial.available()) {
        result += serial.readString();
        }
    } while (millis() - start < ms);
    return result;
}
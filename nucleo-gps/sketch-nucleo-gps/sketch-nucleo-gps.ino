#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

// Die serielle Verbindung zum GPS Modul
SoftwareSerial ss(3, 4);

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
}

void loop() {
  while (ss.available() > 0) {
    //Serial.println(ss.read());
    int data = ss.read();
    //Serial.write(data);
    gps.encode(data);
    //if (gps.location.isUpdated()) {
      // Breitengrad mit 3 Nachkommastellen
      Serial.print("Breitengrad= ");
      Serial.print(gps.location.lat(), 3);
      // Längengrad mit 3 Nachkommastellen
      Serial.print(" Längengrad= ");
      Serial.println(gps.location.lng(), 3);
    //}
  }
}

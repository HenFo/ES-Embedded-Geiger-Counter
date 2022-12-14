# Beispiel: Auslesen von GPS-Daten
## Komponenten
- Nucleo F070RB Evaluation Board
- ublox neo-6m GPS Empfänger mit Antenne

## Beschreibung
Dieses Beispiel liest die Werte des GPS-Moduls über den Serialport aus und zeigt die gelieferten Koordinaten auf dem Serialmonitor an.

Hinweis: Für einen GPS-Empfang sollte das Gerät unter freiem Himmel liegen. Wenn ein GPS Signal gefunden wurde, blinkt die LED auf dem GPS-Modul.

## Verkabelung
- (neo-6N) VCC -> (Nucleo) 5V
- (neo-6N) Tx -> (Nucleo) D3
- (neo-6N) Rx -> (Nucleo) D4
- (neo-6N) GND -> (Nucleo) GND

## benötigte Libraries
- Nutzung der Arduino IDE für Nucleo: https://www.instructables.com/Quick-Start-to-STM-Nucleo-on-Arduino-IDE/
  - Neuer Link für den BoardManager: https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
- SoftwareSerial
- TinyGPS

## Ergebnis
![serial-output](https://github.com/HenFo/ES-Embedded-Geiger-Counter/blob/main/nucleo-gps/serial-output.png)

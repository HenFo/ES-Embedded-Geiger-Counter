# Anforderungen

## Bewertungskriterien
- Es wurde ein Schaltplan entworfen, der mindestens eine LoRa Anbindung, eine Ansteuerung für eine SBM-20 Röhre, ein GPS-Modul und einen µC (o.Ä.) enthält
- Konzept zur Energieversorgung wurde eingeplant. (Netzteil, Akku, etc.) Ggf. Abschätzung der Akkulaufzeit.
- EMV- und ESD-gerechtes Design wurde berücksichtigt
- Das Layout wurde entsprechend des Schaltplans entwickelt und bestellt.
- Die Platine wurde bestückt und die Funktion verifiziert
- Ein Gehäuse wurde entworfen und gefertigt.
- Bonus für bestimmte Konzepte darüber hinaus.

- Die Architektur der Firmware wurde entworfen und dokumentiert.
- Die grundlegende Software zur Inbetriebnahme von Mikroprozessor und Peripherie (Timer, ADC, etc.) wurde erstellt.
- Die Software wurde ressourceneffizient entworfen (Nutzung von Interrupts, DMA, Sleep/Wait-States wo sinnvoll).
- Sicherheitsvorrichtungen des Mikrocontrollers wurden sinnvoll genutzt (Watchdog, Brownout detetction, etc.).
- Ein Testkonzept für die Software wurde entwickelt.
- Die Firmware kompiliert frei von Fehlern und Warnings (-Wall), ist lauffähig und entspricht der Spezifikation.
- Ein Codingstyle wurde spezifiziert und eingehalten.
- Bonus für bestimmte Konzepte darüber hinaus.

- Gemessene Daten können aus dem TTN empfangen, gespeichert und visualisiert werden.

- Eine Versionsverwaltung wurde eingesetzt.
- Das Projekt wurde dokumentiert.
- Es wurde ein detaillierter Projektplan erstellt inkl. Anforderungsanalyse, Arbeitspaketen und Meilensteinen.
- Es haben regelmäßige Projekttreffen stattgefunden, in
denen
    - Zuständigkeiten eindeutig zugeordnet wurden
    - Arbeitsgruppen festgelegt wurden
    - die Arbeitsgruppen ihren Fortschritt präsentieren und
    auf Abweichungen vom Projektplan hinweisen
    - der Fortschritt festgehalten wurde (soll vs. ist)
    - der Projektplan angepasst wurde
    - Änderungen im Projektplan nachvollziehbar
    dokumentiert wurden
- Es wurden regelmäßige Design-Reviews mit Unterstützung von anderen Projektgruppen durchgeführt und dokumentiert.
- Termine und Fristen wurden eingehalten. Hardwarebestellung 2022/12/01 Abschlusspräsentation 2023/02/xx Abgabe der Dokumentation 2023/03/xx

## Anforderungen Hardware
- Für das Hardwarelayout soll ein Schaltplan entworfen werden
- Das Hardwarelayout soll erstellt werden, sodass ein Board bestellt werden kann.
- Das Board soll einen Mikroprozessor STM32F070RB beinhalten
- Das Board soll einen Lora Chip Wio-E5 STM32WLE5JC mit Antenne beinhalten
- Das Board soll einen GPS-Modul ublox neo 6m mit Antenne beinhalten
- Das Board soll einen Geigerzähler (https://github.com/SensorsIot/Geiger-Counter-RadiationD-v1.1-CAJOE-) beinhalten
- Das Board soll eine Stromversorgung (Akku+Solar?) beinhalten
- (Das Board soll LEDs beinhalten (An, GPS-empfang, Lora Connected))
- Das Board soll ein ein Gehäuse passen, wobei die Antennen extern angebracht werden sollen

## Anforderungen Software
- 
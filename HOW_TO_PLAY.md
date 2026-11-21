# So spielst du — Nexus-Core-Gamemode

> [!NOTE]
> Diese Doku wurde optisch aufbereitet, ohne technische Fakten, Pfade, Ports oder Projektregeln zu verändern.
> Fokus lag auf Lesbarkeit, Struktur, Übersichten und deutlicherer Orientierung.

## ✅ Wichtig zu wissen

- Technische Fakten, Befehle, Pfade und Projektregeln bleiben bewusst unverändert.
- Port- und Systemtrennung bleiben konsistent, insbesondere für den 7780-Contract und den Dini-first-Pfad.
- Die optische Aufwertung dient nur der Lesbarkeit, nicht der inhaltlichen Interpretation.

---
## 🧭 Schnellüberblick

| Bereich | Inhalt |
| --- | --- |
| Dokument | `HOW_TO_PLAY.md` |
| Typ | Spieler-Guide |
| Fokus | Spielerfluss und Server-Workflow |
| Wichtige Pfade | `gamemodes/`, `Compiler/samp03/`, `tools/` |

---
## 🔧 1. Ich starte auf dem Live-Server – und die Welt ist sofort lebendig

Der erste Login ist nicht nur ein kurzer „Hallo“-Moment. Der Moment des Einsteigens ist ein klarer Übergang in eine tiefe RP-Welt. Ich steige in die Welt ein und merke sofort: Das ist kein Standard-Singleplayer-Spawn, kein sinnloser CJ-Default-Fallback und kein simpler Sandbox-Feldzug. Dieses Projekt ist eine echte, modulare Reallife-Welt mit klaren Behörden, Staatssystemen, Jobs, Fraktionskrieg, Besitz, Fahrzeugen und Wirtschaft.

Die genaue Laufzeitdatei der Welt liegt hier:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Und die Serverkonfiguration muss genau auf denselben Namen zeigen:

```cfg
gamemode0 gtasa_reallife 1
```

Das ist nicht nur ein technisches Detail, sondern die Grundlage dafür, dass ich in der richtigen, echten Welt starte und nicht in einen Default-State oder Standardspawn falle.

---

## 🏗️ 2. Das Drei-Staaten-System: Politik, Grenzen, Gesetze und Kassen

Das erste, was ich direkt spüre, ist die politische Struktur der Welt. Der Gamemode arbeitet mit einem klaren Drei-Staaten-System. Die Staaten sind nicht nur Labels auf der Karte; sie sind die Grundlage für Gesetze, Wirtschaft, Kassen und Verhalten.

### 📘 Die drei Staaten

- Los Santos (LS) – urbaner Großstadtbereich, Wirtschaft, Verwaltung und hohes Aktivitätsniveau
- San Fierro (SF) – industrieller, logistisch geprägter Bereich mit Fokus auf Produktion und Infrastruktur
- Las Venturas (LV) – luxuriöses, touristisches und sicherheitsorientiertes Wirtschaftsgebiet

Die Logik wird in den Modulen rund um `state_system.inc`, `state_identity_system.inc`, `state_spawn_system.inc`, `state_governance.inc` und `state_budget_system.inc` umgesetzt. Das heißt: Meine Zugehörigkeit zu einem Staat ist ein echter in-game-Status und kein bloßes Dekor.

### 📘 Staatskassen und Treasuries

Jeder Staat besitzt seine eigene Verwaltung, seine eigene Staatskasse und seine eigene wirtschaftliche Identität. Diese Schicht ist entscheidend, weil sie die Welt in echte politische und finanzielle Gewichte aufteilt.

Eine Staatskasse ist nicht nur eine Zahl im UI. Sie steuert:

- Steuern
- öffentliche Ausgaben
- Regierungseinnahmen
- Wirtschaftsbalance
- regionale Preis- und Job-Logik

Wenn ich als Spieler Geld verdiene, miete ein Haus, kaufe ein Fahrzeug, zahle Gebühren oder bekomme Lohn, ist das Teil dieser breiten Ökonomie. Ein Staat ist also nicht nur ein geografisches Label; er ist ein aktives Finanz- und Rechtsgebilde.

### ✅ Gesetze, Regeln und grenzorientierte Logik

Die Welt ist in drei politische Räume organisiert, und jeder dieser Räume hat seine eigene Identität. Das heißt:

- Jobs können staatlich gebunden sein
- Spielerausgaben wirken regional ab
- Fraktionsstrategien basieren auf Territorien und Einfluss
- Stadtverwaltung und Gesetzeslogik hängen mit Staatsfunktionen zusammen

Das ist der Grund, warum der Server sich wie eine echte, geordnete Welt und nicht wie eine statische Sandbox anfühlt.

---

## 📘 3. Charaktererstellung, Auth und Spawn-Flow

Beim ersten Verbinden beginnt sofort der Auth- und Spawn-Prozess. Ich lande nicht einfach irgendwo im Standard-CJ-Flow. Die Basis für den ersten Kontakt mit der Welt liegt im Auth-Modul `login_system.inc` und den zugehörigen `tutorial`- und `camera`-Systemen.

### 📘 Der erste Verbindungsflow

Wenn ich mich zum ersten Mal einlogge oder einen neuen Charakter erstelle, läuft das so:

1. Verbindung zum Server wird aufgebaut
2. Auth-Status wird aktiv gesetzt
3. State-Daten werden vorbereitet
4. Login-/Register-Overlay erscheint
5. ich muss eine Identität erzeugen oder einloggen
6. danach erfolgt die Auswahl des Heimatstaats
7. der Spieler wird an den passenden Spawn-Punkt gesetzt

Das System ist bewusst textdraw- und UI-basiert und nicht bloß auf Chat-Kommandos reduziert. Das macht die ersten Schritte sauber und hilft, dass der Charakter wirklich als Teil der Welt auftritt.

### 📘 Spawn und Heimatstaat

Der Spawn ist nicht willkürlich. Der Server zieht die Heimatstadt aus dem State-System. Das ist ein entscheidender Unterschied zwischen einem reinen Test-Server und einem echten Reallife-Gamemode. Der State-Mechanismus verknüpft: 

- Spielerhome-state
- aktive Staatszuordnung
- spawn- und city-Logik
- Wirtschaft und Jobs

Das bedeutet: Ich kann nicht einfach an irgendeinem zufälligen Punkt auftauchen und sofort als „warum auch immer“ spielen. Ich komme in die richtige State-Struktur und nehme die Welt so wahr, wie sie durch den Staat bestimmt ist.

### 📘 Tutorial und erste Onboarding-Schritte

Nach dem Login kommt das Tutorial. Dieses System ist nicht nur „ein paar Hinweise“, sondern ein strukturiertes Intro in die Spielwelt. Der Server führt mich Schritt für Schritt durch:

- Wie spielt man auf dem Server?
- Welche Zustände und Staaten gibt es?
- Wie arbeite ich mit JobCenter, Geld und UI?
- Wie interagiere ich mit Gebäuden und Verwaltungsstellen?
- Wie werden Fahrzeuge, Häuser und Inventar aktiviert?

Das ist der Leitsatz der ersten Stunde auf dem Server: Ein Spieler soll nicht blind durch die Welt laufen, sondern sofort in die Grundsysteme hineingeführt werden.

---

## 🏗️ 4. City Halls und Regierungssysteme

Die Welt wirkt durch ihre Verwaltungssysteme real. Der Server enthält separate Module für City Hall, government, state governance und municipal system. Diese Ordnungen sind Kern des RP-Settings.

### 📘 City Halls als Zentrum der Verwaltung

Wenn ich zu einer City Hall gehe, betrete ich kein bloßes dekoratives Gebäude, sondern das Zentrum der Staatsfunktion. Dort ist die Welt verwaltbar, dokumentierbar und politisch sichtbar.

In den Systemen ist der City Hall-Layer auf die drei Staaten verteilt:

- Los Santos Rathaus
- San Fierro Rathaus
- Las Venturas Rathaus

Diese Gebäude sind nicht nur Landmarken, sondern die administrative Schicht des Spiels. Der Staat zeigt sich dort als Ordnung, als Verwaltung und als sichtbare Repräsentation der politischen Struktur.

### 📘 Staatsverwaltung und Treasuries

Die Regierung ist nicht einfach eine statische Liste, sondern ein aktives wirtschaftliches und verwaltungstechnisches System. Dazu zählen:

- Staatskassen
- Budgetverteilung
- Verwaltung von Projekten und Steuern
- öffentliche Richtlinien und Rollen
- Business- und District-Management

Ein Zustand, ein Staat, eine öffentliche Kasse und die Verwaltung sind nicht getrennte Dinge; sie bilden zusammen das staatliche Rückgrat. Das beeinflusst nicht nur Regierungskämpfe, sondern auch den Spielalltag.

### 🧭 Lizenzen, Dokumente und offizielle Infrastruktur

Die Architektur des Projekts klingt nach einem RP-System mit offizieller Verwaltung. Das ist klar in den Projekten `city_hall_system.inc`, `government_system.inc`, `state_governance.inc` und verwandten Modulen zu sehen. Es gibt ein Gefühl von „öffentlicher Ordnung“ und „staatlicher Infrastruktur“, das den Server von bloßer Sandbox-Entwicklung unterscheidet.

Das bedeutet für mich als Spieler:

- ich sehe Verwaltungsräume, Behördensysteme und staatlichen Verlauf
- ich kann an Verwaltungsprozesse glauben
- die ökonomische und politische Welt ist durch Regeln und Zustände verbunden

---

## 📘 5. JobCenter, Jobs und die reale Wirtschaft

Jobs sind der treibende Motor der Langlebigkeit auf diesem Server. Das Job-System ist klar modular und in `job_center.inc`, `jobs_core.inc`, `state_jobs.inc`, `dynamic_job_system.inc` und `mission_system.inc` aufgebaut.

### 📘 Das JobCenter

Das JobCenter ist der zentrale Einstieg in die legalen Einnahmequellen. Hier kann ich als Spieler:

- Jobs auswählen
- Aufgaben annehmen
- Verdienst und Fortschritt verfolgen
- Staaten und Aufgaben vergleichen
- die Aktivitätslogik im Spielfluss verstehen

Das JobCenter ist nicht nur eine statische Liste; es ist das eigentliche Interface für die legale Karriere. Es bündelt die Aufgaben und ist der sichtbare Zugang zur Wirtschaft der Welt.

### 📘 Dynamische Aufgaben und Checkpoints

Wenn ich einen Job annehme, startet der Server eine Logik mit klaren Aufgaben, Zielpunkten und Fortschrittskontrolle. Der Prozess ist typischerweise:

1. Job wird gewählt
2. das System lädt den passenden Task
3. ein Checkpoint oder Ziel wird gesetzt
4. ich bewege mich zur Stelle
5. Aufgabe wird beendet
6. Lohn, XP und fortlaufender Fortschritt werden validiert

Das macht die Jobs spannend und dynamisch. Sie sind nicht nur „steht still und kassiert“, sondern körperlich und zeitlich in der Umgebung verankert. So wirkt die Arbeit viel realistischer und in der Welt verankert.

### 📘 Geld, Preise und Marktlogik

Die Wirtschaft ist aus mehreren Ebenen aufgebaut:

- Cash direkt am Spieler
- Bank- und Staatskassen
- Verkäufe und Marktpreise
- lokale Shop- und Händlerlogik
- regionale Unterschiede in den Staaten

Das macht den Verlauf natürlich. Ich verdiene Geld, gebe Geld aus, kaufe Waren, zahle Gebühren, investiere in Besitztümer und beeinflusse dabei die Welt. Diese Ökonomie funktioniert nicht isoliert, sondern in Verbindung mit Staat, Verwaltung und Spielerverhaltenslogik.

---

## 📘 6. Fraktionen, Gangs und TextWars

Die Fraktionslogik ist eines der mit Abstand beeindruckendsten Systeme. Der Gamemode ist zu großen Teilen auf Gruppen, Einfluss und Turf-Strategie ausgelegt.

### 🏗️ Fraktionen und Gangsysteme

Ich kann als Spieler nicht nur ein Bürger sein, sondern auch Teil einer Fraktion und damit Teil eines größeren Einflussnetzwerks. Das ist das Herzstück der sozialen Konfliktlogik. Fraktionen tragen Einfluss, kontrollieren Räume und beeinflussen die Beziehung zwischen den Staaten.

In diesen Systemen liegt die Idee, dass bestimmte Gruppen dominieren, attackieren, verteidigen und Territorien beeinflussen. Das macht den Server nicht nur mehr „RP“, sondern auch strategisch und politisch spannender.

### 📘 TextWars als strategische Kriegsoberfläche

`text_wars.inc` ist ein wichtige Schnittstelle zwischen UI, Faction-Logik und Territorial-Kriegen. Es arbeitet als Textdraw- und Interaktionsoberfläche, auf der ich den Kriegs- und Influence-Status einer Fraktion oder eines Gebiets nachvollziehen kann.

Die wichtigsten Elemente sind:

- Statusanzeige
- Gebiete/Territorien
- Angriffsmöglichkeiten
- Truppenstatus
- Übersichten über Fraktionsereignisse

Das wirkt wie ein strategisches War-Board, das die Fronten und den Konfliktstatus auf einen Blick sichtbar macht. Es ist keine bloße Fancy-UI, sondern ein funktionaler Bestandteil der Fraktions- und Gebietspolitik.

### ⚠️ Warum das wichtig ist

Wenn ich als Spieler in die Welt einsteige, dann sind nicht nur meine persönlichen Aufgaben wichtig. Ich kann Teil eines größeren Konflikts werden:

- Fraktionen kontrollieren Straßen, Immobilien oder Gebiete
- Gang-Konflikte formen die Stadt
- State-Politik zieht sich über die Fraktionslogik
- Spieler müssen Entscheidungen strategisch treffen

Das ist genau das, was den Server als echte Reallife-Welt wahrnehmbar macht.

---

## 🏗️ 7. Untergrundsysteme: Drogen, Schmuggel und illegale Netzwerke

Nicht alles in der Welt ist legal. Das Projekt hat ein starkes illegales Marktsystem. Das ist ein wesentlicher Teil der RP-Atmosphäre und enthält echte wirtschaftliche Abgründe.

### 📘 Drogenproduktion und -handel

Das Drogenmodul gruppiert die illegale Wirtschaft in verschiedene Bereiche:

- Produktion
- Lagerung
- Verkauf
- Preise je nach Stadt und Staat
- Player- und Fraktionsbesitz

Das Produkt ist nicht bloß konzeptionell, sondern als echtes Spielsystem am Server integriert. Spieler können Stoffe kaufen, produzieren, tauschen und durch illegale Netze bewegen.

### 📘 Illegale Netzwerke und Risikofaktoren

Illegale Aktivitäten sind nie ohne Risiko. Das System erzeugt sogenannte thermische oder kriminalitätsbezogene Zustände, wobei der Server den Beteiligten ein entsprechendes Gewicht zuordnet. Dadurch wird der Untergrund nicht bloß als „extra-late-night-mode“ behandelt, sondern als echte Gegenwelt zur legalen Wirtschaft.

Das ist für mich als Spieler ein großer Unterschied:

- Legale Jobs geben stabilen Verlauf und Sicherheit
- illegale Wirtschaft bringt hohe Gewinne, aber auch Risiken
- Good cop / bad cop-Dynamik wird dadurch wahrer

---

## 📘 8. Housing, Inventar und Safe-Management

Besitz ist ein zentrales Thema. Ich will nicht nur laufen und Geld verdienen, sondern auch Häuser besitzen, Lager nutzen, Gegenstände verwalten und mein eigenes System aufbauen.

### 📘 Häuser und Immobilien

Das Housing-System modelliert Eigentum als echte Serverstruktur. Das umfasst:

- Hausbesitz
- Lager- und Storage-Systeme
- Fähigkeiten im Umgang mit Besitzobjekten
- Eigentumsrechte und Zugriffskontrollen

Ein Haus ist nicht bloß ein Marker auf der Karte. Es ist Teil der RP-Identität und ein Wirtschaftsobjekt.

### 📘 Safes und hidden Storage

Die Safe- und Storage-Module sind besonders wichtig, weil sie eine kleine, aber tragende Schicht der Eigentumslogik bilden. Ich kann Gegenstände lagern, sichern und kontrollieren. Das ist der Kern dessen, was viele Spieler als „realistische Inventarperspektive“ wahrnehmen.

Das System ist unabhängig von der Gross-UI und wird durch `ui_hub.inc` und das Inventory-System ergänzt. Als Spieler merkt man es im Alltag als:

- Lager- und Safe-Zugriff
- Geheiminventar
- sichere Gegenstandsverwaltung
- Verwaltung von Haus- und Eigentumsinhalten

### 🧩 ui_hub: die zentrale Übergangspunkt-UI

Das UI Hub wird zu einem zentralen Tätigkeits- und Wissenszentrum. Es bündelt wichtige Informationen und Funktionen. Die Welt ist dadurch nicht nur funktional, sondern gut lesbar. Als Spieler sehe ich über dieses Modul wie auf einen Hub, der verschiedene Bereiche miteinander verbindet:

- Inventar
- Staat
- Jobs
- Besitz
- Fahrzeuge
- Fraktions- und War-Status

Das ist das Raumsystem der live-aktiven Welt.

---

## 🏗️ 9. Fahrzeugsystem, Schlüssel, Motor und UBB

Fahrzeuge sind keine bloßen Spawn-Objekte, sondern echte Besitz- und Verlaufsmechaniken. Das Fahrzeugsystem ist das, was die Welt mobil macht.

### 📘 Fahrzeugbesitz und Schlüssel

Fahrzeuge haben reale Eigenschaften:

- Besitzerzuordnung
- Schlüssel-/Access-Logik
- Fahrzeugstatus
- Garage- und Spawn-Verwaltung
- Mobilität im RP-Alltag

Das Fahrzeugsystem funktioniert nicht als simplistische `CreateVehicle()`-Maschine, sondern als echtes Ownership- und Activity-System. Ich kann ein Fahrzeug besitzen, verwenden, reparieren, absichern und im Alltag einsetzen.

### 📊 Motorzustand, Engine-State und Parken

Die Motor- und Fahrzeugzustand-Logik ist dabei besonders wichtig. Die UBB-Mechanik bestimmt nicht nur, ob ein Fahrzeug fährt, sondern auch, wie es in der Welt als Besitz, Ressource und transportierte Last funktioniert. Das System ist Teil der Nutzungskette:

- Motor an/aus
- Parkplatzlogik
- Fahrzeugsystem-Checks
- Schlüssel-/Zugangsstatus
- garagen- und statebasierte Verwaltung

Das macht Fahrzeuge zu einem echten System innerhalb des RP-Ökosystems.

### 📘 UBB und Fahrzeugmanagement

Das `ubb_vehicle_system.inc` ist der Teil, der tiefergehende Fahrzeugverwaltung kapselt. Es wird mit Tuning, Ownership, Garage, Fahrzeugstatus und Objektlogik zusammengeführt. Das heißt: Das Auto ist nicht nur ein Transportmittel, sondern ein wichtiger Teil der Besitz-, Quests- und Wirtschaftsschicht.

---

## 📘 10. Warum die Welt so „lebendig“ wirkt

Wenn ich auf diesem Server spiele, dann empfinde ich die Welt nicht als einzelne Systeme, sondern als zusammenhängendes RP-Netzwerk. Jede Schicht hängt mit der nächsten zusammen:

- State-System bildet politische Identität
- Government-System bildet Verwaltung und Recht
- Economy-System bildet Einnahmen und Preise
- Jobs-System bildet Aktivität und Lohn
- Faction-System bildet Einfluss und Konflikte
- Housing-System bildet Besitz und Sicherheit
- Inventory-System bildet Lager und Ressourcen
- Vehicle-System bildet Mobilität und private Infrastruktur
- UI-Hub macht alles nutzbar und sichtbar

Das ergibt eine Welt, die funktioniert und nicht nur dekorativ wirkt. Ich als Spieler bin nicht auf einzelne Features reduziert, sondern Teil eines legitimen, zusammenhängenden Spielsystems.

---

## 🔧 11. Wenn ich den Server in einem Satz beschreiben müsste

Nexus-Core-Gamemode ist ein modularer 3-Staaten-Reallife-Server, auf dem ich als Bürger, Arbeiter, Gangmitglied, Fraktionsmitglied oder Geschäftsmann in einer politischen, wirtschaftlichen und sozialen Welt lebe – mit Jobs, Staat, Eigentum, Faction-Konflikten, illegalen Netzwerken und echten Fahrzeug- und Besitzmechaniken.

Das ist nicht bloß ein Modus. Das ist ein vollwertiges Rollenspiel-Ökosystem, das durch die modularen Kernsysteme aus state, jobs, economy, factions, housing, inventory, vehicles und ui zusammenhält.

Und genau diese Struktur ist der Grund, warum die Serverruntime sauber auf `gtasa_reallife.amx` ausgerichtet sein muss. Denn nur dadurch kann die Welt korrekt starten und ich als Spieler wirklich in die richtige, vollständige Reallife-Welt hineinspawnen statt in einen Standard- oder Fallback-Blueberry-Zustand.

Das Projekt enthält außerdem Logiken für den Schwarzmarkt, den Transport und die illegale Verteilung in Modulen wie `contraband_system.inc`, `trade_logistics.inc` und anderen Crime-/Commerce-Systemen. Damit lassen sich:

- versteckte Güter
- Schmuggelrouten
- illegale Handelswege
- geheime Warenströme

modellieren.

Der Server ist also bewusst so gebaut, dass Kriminalität kein separates Nebenprodukt ist, sondern ein eigener Wirtschaftszweig mit Risiken, Druck und Konsequenzen.

---

## 📘 8. Häuser, Inventar und Safes

Häuser und Inventar sind Kernbestandteile der Progression. In diesem Server ist das tägliche Leben so wichtig wie Jobs und Konflikte.

### 📘 Häuser

Die Wohnungslogik liegt in:

- `gamemodes/modules/housing/house_system.inc`
- `gamemodes/modules/housing/house_storage_system.inc`

Der Server definiert Häuser mit Eigentümer, Preis, Zustand, Innenraum und Lock-Status. Häuser können gekauft, übertragen und verwaltet werden. Das ist einer der wichtigsten RP-Grundpfeiler: Ein Spieler ist nicht nur eine Figur im Raum, sondern jemand mit Besitz, Sicherheit und persönlichem Fortschritt.

Das System umfasst:

- Hauskauf-Checks
- Bankvalidierung
- Staatliche Transaktionen bei Verkauf
- Eigentümer-Tracking
- Locks und Verwaltung

### 📘 Hause safes und Lager

`HouseStorageSystem` verwaltet Haus-Safes und Lagercontainer. Es unterstützt mehrere Speicherplätze, Items, Metadaten und Geld-Lager. Spieler können Gegenstände lagern, wieder aufnehmen und in ihrem Zuhause sichern. Das macht den Server realistischer, weil der Spieler nicht ständig alles in der Tasche tragen muss.

### 🏗️ Inventar-System

Das Inventar sitzt in `gamemodes/modules/inventory/inventory_system.inc` und wird über `ui_hub.inc` dargestellt. Das ist die tägliche Spieloberfläche für:

- Inventar ansehen
- Items nutzen
- Items wegwerfen
- Transfers zwischen Spielern
- Geld-/Items in Safes sichern
- andere Alltagssysteme verwalten

Das UI Hub wirkt wie ein modulares Dashboard. Es bündelt die wichtigsten Prozesse und macht die Welt lesbar und nutzbar. Es soll den Alltag im Server gleichzeitig leicht und tief machen.

---

## 🏗️ 9. Fahrzeugsysteme und UBB

Fahrzeuge sind zentral für die Identität des Servers. Das Projekt beinhaltet ein allgemeines Fahrzeugsystem sowie ein fortgeschritteneres UBB-Fahrzeugmodell.

### 📊 Fahrzeugbesitz und Zustand

Das Basis-Fahrzeugsystem liegt in `gamemodes/modules/vehicles/vehicle_system.inc`. Fahrzeuge werden verwaltet mit:

- Modell
- Zustand
- Besitzer
- Preis
- Tankfüllung
- Fahrzeugtyp

Das System erlaubt Fahrzeugerstellung, Eigentümerzuweisung, Tankstatusänderungen und state-aware Logik.

### 🏗️ UBB-Fahrzeugsystem

Die fortgeschrittene Schicht liegt in `gamemodes/modules/vehicles/ubb_vehicle_system.inc`. Dieses System behandelt Eigentümer, Schlüssel, Fahrzeugstatus und Persistenz. Es unterstützt unter anderem:

- Fahrzeugregister
- Auf-/Zuklappen, sperren
- Zustandsänderungen
- Tank-/Fuel-Updates
- Eigentümer-Checks
- Persistenz im Backend

Das ist deutlich mehr als nur ein temporäres `CreateVehicle()`. Fahrzeuge sind echte Besitzobjekte mit Ownership- und State-Logik.

### 📘 Praktischer Fahrzeugfluss

Im echten Spiel sieht das so aus:

- ich kaufe oder finde ein Fahrzeug
- es gehört mir im Rahmen der Spielerdaten
- ich sperre oder entsperre es
- ich tanke es
- ich park es korrekt und verwalte es
- ich nutze es für Jobs, Illegalität, Reisen und Alltag

Die Fahrzeuge sind mit dem Rest der Welt verknüpft: Jobs nutzen sie, Kriegslogik benötigt Mobilität, Häuser liefern Kontext und Player-Progression hängt oft an Fahrzeugzugang und Besitz ab.

---

## 🧩 10. Das UI Hub – das Bedienzentrum des Spielers

`ui_hub.inc` ist das zentrale Spielerdashboard. Es ist ein dialogbasiertes Menü, das die wichtigsten Systeme bündelt:

- Inventar
- Handy-/Phone-Interaktion
- Business-Logik
- Ammu-Nation
- Mission Board
- Garage / Tuning
- Job Center
- Text Wars
- State Control / Staatliche Übersichten
- Spieler-Dashboard

Das ist das operative Herz des Servers. Statt unübersichtlicher roher Commands bekommt der Spieler ein strukturiertes Interface, das die Welt verständlich und nutzbar macht.

`UiHub_Ready()` sorgt dafür, dass das Menü nur aktiv wird, wenn das System wirklich bereit ist. So wird verhindert, dass fehlerhafte Schnittstellen oder uninitialisierte Zustände im Spiel auftauchen.

---

## 📘 11. Wie die Welt im echten Spiel wirkt

Wenn ich auf diesem Server spiele, fühlt sich das wie eine vollständige RP-Stadt an, aufgebaut in Schichten:

- Ich logge mich ein und wähle meinen Staat.
- Ich registriere meinen Charakter und bekomme meine Heimatstadt.
- Ich gehe ins JobCenter und verdiene Geld.
- Ich kaufe Waren, Häuser und Fahrzeuge.
- Ich arbeite legale Jobs oder gehe in die kriminelle Seite.
- Ich interagiere mit Fraktionen, Gangs und Territorien.
- Ich verwalte mein Inventar und meine Safes.
- Ich bewege mich durch die Stadt als Bürger, Arbeiter, Gangster oder Organisator.

Das ist keine einzelne Spielmechanik, sondern eine modulare Lebenswelt. Die Welt besteht aus Wirtschaft, Regierung, Recht, Krieg, Eigentum, Fraktionssystemen und Progression, alles verbunden durch das State- und Lifecycle-System.

---

## 📘 12. Kurzfazit für den Spieler

Wenn ich den Server in einem Satz erklären müsste, dann so:

Ich spiele nicht nur einen Modus, sondern lebe in einer staatlich geprägten Reallife-Stadt, in der ich meinen Staat wähle, meinen Charakter aufbaue, legal arbeite, Geld verdiene, Häuser und Fahrzeuge besitze, Fraktionen mitgestalte und in einer Welt überlebe, in der Wirtschaft, Recht und Territorium wirklich zählen.

Das ist das Kernprinzip von Nexus-Core-Gamemode.

---

## 📊 13. Finaler Stand vor der Live-Validierung

Der aktuelle Spiel- und Server-Stand ist auf einen stabilen, konsistenten Source-Level ausgerichtet, aber die finale Freigabe bleibt explizit ein Live-Validation-Gate auf der Ubuntu-VM. Die Kernregel bleibt: keine ungebremste Live-Startfreigabe auf Basis eines reinen Source-Checks, sondern echte Verifikation mit Logs, DB-Status, Dini-Persistenz und Clientflow.

### 📘 13.1 Was bereits stabilisiert wurde

- Login-/Register-Flow bleibt menu-first und nicht chat-abhängig
- State-/Home-State-Auswahl bleibt auf gültige Werte und nicht auf Default-Null-Werte ausgelegt
- Spawns laufen über den kontrollierten Pfad und nicht durch wiederholte Fallbacks
- Camera-Reset und controls werden sauber wieder freigegeben
- UI und HUD behalten die erwünschte Zonierung, mit freier unterer linker Zone für Radar/Mobile-Controls
- Dini-/SQLite-Persistenz bleibt im Projekt als kritischer Pfad dokumentiert
- Runtime-Hotpaths wurden reduziert, damit sie innerhalb der qemu-i386-Umgebung nicht unnötig belastend werden

### 🖥️ 13.2 Was noch nicht als live-freigegeben gilt

- echtes Login-/Register-/Spawn-Verhalten auf einer laufenden SA:MP-/OMP-Instanz
- reale State- und Home-State-Checks in `gtasa_reallife.db`
- reale scriptfiles-/Dini-Validierung nach Crash oder Reconnect
- echte Client- und HUD-Tests unter realem UI-/Mobil-Layout

### 🔧 13.3 Freigabe- und Startkriterien

Die Live-Validierung darf erst dann als erfolgreich gelten, wenn alle folgenden Punkte in der echten Ubuntu-VM erfüllt sind:

- `gtasa_reallife.amx` wird per `qemu-i386` Build sauber erzeugt
- `server_log.txt` und `tools/log_watcher.py` zeigen keinen kritischen Start- oder Loop-Fehler
- `data/db/gtasa_reallife.db` zeigt konsistente `state_id` / `home_state` / `player_state`-Werte
- `Compiler/samp03/scriptfiles/` enthält die erwarteten Ordner mit korrekten Berechtigungen
- Der vollständige Connect/Onboarding/Tutorial/State/Spawn-/Disconnect-Lebenszyklus funktioniert real und ohne doppelte Trigger

Damit bleibt die Spielbeschreibung des Projekts korrekt, aber die Release-Freigabe steht weiterhin unter der echten Live-Validierung und nicht bloß unter dem Source-Status.

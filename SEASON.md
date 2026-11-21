# Season-System für Nexus-Core-Gamemode

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
| Dokument | `SEASON.md` |
| Typ | Season-/Eventsystem |
| Fokus | Zyklus, Wirtschaft und Spielereignisse |
| Wichtige Pfade | `gamemodes/`, `Compiler/samp03/`, `tools/` |

---
## 🧭 Überblick

Das Season-System ergänzt den Gamemode um einen zyklischen Jahreszeiten-Loop, der die Welt, die Wirtschaft, das Gameplay und die sozialen Events über längere Zeiträume hinweg verändert. Es soll nicht nur ein visueller Wechsel sein, sondern ein echtes Spielsystem mit Auswirkungen auf Wetter, Jobs, Produktion, Fraktionsaktivitäten, Item-Verfügbarkeit, NPC-Verhalten, Fahrzeugnutzung und saisonale Ereignisse.

Die Idee ist bewusst modular: Die Saison selbst ist kein isoliertes Feature, sondern ein übergeordnetes Layer-System, das mit Core-, State-, Economy-, Job- und Faction-Logik interagiert. Dadurch kann die Welt des Servers über mehrere Wochen oder Monate dynamisch „reifen“, ohne dass der Gamemode in monolithische Speziallogik ausartet.

## 🧭 Ziel des Systems

Das Season-System verfolgt fünf Kernziele:

1. Dynamische Welt-Varianz
   - Jede Saison verändert die Grundstimmung und die Spielumgebung.
   - Die Welt wirkt nicht statisch, sondern saisonal lebendig.

2. Wirtschaftliche Tiefe
   - Ressourcen, Jobs und Preise ändern sich je nach Saison.
   - Dies erzeugt langfristige RP-Strategien und wirtschaftliche Entscheidungen.

3. Long-Term-Progression
   - Saisonale Events, Belohnungen und Meilensteine sorgen für wiederkehrende Motivation.
   - Spieler erleben einen fortlaufenden Jahreszyklus statt eines statischen Starts.

4. Modulare Erweiterbarkeit
   - Neue Jahreszeiten, Events, Gegenstände und Belohnungen lassen sich ohne Umbau des gesamten Gamemodes ergänzen.

5. Server-Identität
   - Ein klarer Season-Zyklus macht den Server spürbar „lebendig“ und unterscheidet ihn von reinem Sandbox-Gameplay.

## ✅ Grundprinzip

Die Saison ist ein globaler Zustand des Servers. Sie umfasst:

- aktuelle Saison
- Jahreszeit-Startzeit
- verbleibende Dauer
- aktive Event-Liste
- saisonale Boni/Mali
- globale Effekte
- state-spezifische Anpassungen

Diese Daten sollten zentral im Core- oder State-Layer verwaltet werden und dann von betroffenen Modulen abgefragt werden.

## 📘 Saisonzyklus

Ein sinnvoller Serverzyklus umfasst vier Jahreszeiten:

### 📘 1. Frühling

**Ziel:** Erneuerung, Aktivität, Wachstum und erste Nachsaisonphase.

**Weltgefühl:**
- angenehmes Wetter
- grüne Umgebung
- mehr Besucher auf Straßen und Plätzen
- erhöhte Aktivität in Jobs mit Liefer-/Transport-/Baucharakter

**Spielmechanik:**
- erhöhte Häufigkeit von Ernte-, Landwirtschafts-, Liefer- und Outdoor-Jobs
- mehr Events rund um Festivals, Märkte und Stadtreinigungen
- saisonal passende Texturen und Wetterwechsel

**Wirtschaftliche Auswirkung:**
- Preise für Nahrungsmittel, Getränkefertigung und Räumungs-/Reinigungsdienstleistungen können leicht sinken
- Lieferanforderungen steigen

---

### 📘 2. Sommer

**Ziel:** Hochsaison, Tourismus, Aktivität und ein stärkeres öffentliches Leben.

**Weltgefühl:**
- hohe Temperaturen
- mehr Touristen, mehr Straßenverkehr, mehr öffentliche Veranstaltungen
- stärkeres Nachtleben, Clubs, Festivals und Außenaktivitäten

**Spielmechanik:**
- mehr Events mit Publikum, Geselligkeit, Musik, Gondeln, Booten, Festivals und Stadtauftritten
- Jobs mit direktem Player-Flow profitieren: Taxi, Delivery, Transport, Security, Resort-Services
- Fahrzeugnutzung steigt aufgrund von längeren Reisewegen und hoher Nachfrage

**Wirtschaftliche Auswirkung:**
- höhere Preise für Luxusgüter, Lebensmittel, Getränke, Lifestyle-Items und Tourismus-Services
- erhöhte Nachfrage in Hotels, Clubs, Restaurants und Freizeitbetrieben

---

### 📘 3. Herbst

**Ziel:** Übergang, Vorbereitung, Rückzug, Stabilisierung, saisonale Kürzungen.

**Weltgefühl:**
- kühleres Wetter
- weniger öffentliche Aktivität im Freien
- Fokus auf Innenräume, Jobs und langwierige Aufgaben

**Spielmechanik:**
- verstärkte Logistik-, Lager-, Bau- und Instandhaltungsaktivitäten
- mehr Jobs mit Materialfluss, Reparatur, Kisten, Transport und Überwachung
- Saisonale Events mit Ernte, Sondereinsätzen, Risiko-/Rettungsmissionen

**Wirtschaftliche Auswirkung:**
- Material- und Reparaturkosten können steigen
- Industrie- und Lagermodule gewinnen an Bedeutung
- bestimmte Luxusgüter verlieren an Popularität

---

### 📘 4. Winter

**Ziel:** Härte, Stabilität, Eskalation, Sicherheits- und Überlebensgefühl.

**Weltgefühl:**
- kaltes Wetter, Schnee, dunklere Straßen, geschlossene Freizeitbereiche
- Aufenthalt in Innenräumen und auf kontrollierten Gebieten wird wichtiger
- Gefahrensituation und Logistik werden spürbar anspruchsvoller

**Spielmechanik:**
- erhöhte Risiken bei Outdoor- oder Fahraktivitäten
- Warm-Up-/Heizungs-/Verpflegungssysteme können relevant werden
- Jobs im Sicherheits-, Transport-, Notfall- und Überwachungssektor gewinnen an Gewicht

**Wirtschaftliche Auswirkung:**
- Verbrauchsgüter und Heiz-/Versorgungsangebote werden wichtiger
- Preise für bestimmte Outdoor- und Luxus-Services sinken
- Sicherheits-, EMS- und Logistikmodule bekommen saisonale Priorität

## 📘 Saison-Definition

Eine Saison sollte als struktureller Zustand modelliert werden, nicht als bloßes String-Feld. Ein gutes Datenmodell sieht ungefähr so aus:

```pawn
enum E_SEASON
{
    E_SEASON_NONE,
    E_SEASON_SPRING,
    E_SEASON_SUMMER,
    E_SEASON_AUTUMN,
    E_SEASON_WINTER
};

new g_CurrentSeason = E_SEASON_SPRING;
new g_SeasonStartTimestamp = 0;
new g_SeasonDuration = 0;
new g_SeasonRotationActive = 1;
```

Optional zusätzlich:

```pawn
enum E_SEASON_STATE
{
    E_SEASON_TYPE,
    E_SEASON_WEEK,
    E_SEASON_EVENT_COUNT,
    E_SEASON_REWARD_TIER,
    E_SEASON_BONUS_MONEY,
    E_SEASON_BONUS_REPUTATION
};
```

Die genaue Struktur hängt davon ab, ob die Saison einfach ein Serverstatus ist oder ein richtiges Progressionssystem mit Manager, Belohnungen und Datenbankpersistenz.

## 📘 Zeitlicher Ablauf

Eine praxisnahe Umsetzung ist ein Server-Zyklus mit fixer Dauer, etwa:

- 1 Saison = 7 Tage realer Spielzeit
- 1 Jahreszyklus = 28 Tage
- Wechsel erfolgt automatisch oder durch Scheduled Event

Alternativ kann die Dauer auch durch eine konfigurierbare Einstellung gesteuert werden:

```pawn
#define SEASON_DURATION_DAYS 7
#define SEASON_ROTATION_HOURS 168
```

Diese Dauer kann je nach Server-Tempo angepasst werden. Wichtig ist, dass der Verlauf für Spieler nachvollziehbar ist und nicht zufällig wirkt.

## 📘 Globale Effekt-Mechanik

Das Season-System sollte nicht nur den Zustand ändern, sondern auch die Modullogik beeinflussen. Beispiele:

### 📘 1. Wetter
- Winter: Schnee, kältere Luft, trübe Tage
- Sommer: klare Tage, starke Sonne, mehr externe Events

### 📘 2. Job-Balance
- Frühling/Sommer: mehr Outdoor-, Delivery-, Tourismus- und Event-Jobs
- Herbst/Winter: mehr Logistik-, Reparatur-, Sicherheit- und Instandhaltungsjobs

### 📘 3. Item- und Ressourcenlogik
- Winter: Heizmaterialien, Schutzkleidung, Energieverbrauch relevant
- Sommer: Getränke, Sommerartikel, Reisen, Outdoor-Items
- Herbst: Materialien, Reparaturen, Verbrauch, Lagerung
- Frühling: Pflanzen, Nahrungsmittel, Recycling, Bewässerung

### 📘 4. Fraktionslogik
- Fraktionen können saisonale Bonusziele oder temporäre Events bekommen:
  - Police: Winter-Notfälle, Straßenverhältnisse, Verkehrskontrollen
  - EMS: Kälte- und Hitzeschutz, mehr medizinische Events
  - Government: Steuer- oder Budget-Jahreszyklus
  - Businesses: saisonale Drifts, Produkt-Sets, Tourismus-Angebote

## 📘 Auswirkung auf die Economy

Die Season beeinflusst das Wirtschaftssystem systematisch, nicht nur dekorativ.

### 📘 Beispielkonzept

- Sommer: Preise für Luxuskonsumwaren +10%
- Winter: Preise für Versorgungsgüter +8%
- Herbst: Reparaturkosten +12%
- Frühling: Lebensmittelpreise leicht -5%, Lieferlogistik +8%

Diese Werte können in zentralen Saison-Konstanten definiert werden:

```pawn
new Float:g_SeasonPriceModifier[4] =
{
    1.0,    // Spring
    1.12,   // Summer
    1.08,   // Autumn
    1.15    // Winter
};
```

Diese Modifikatoren sollten über zentrale Helper-Funktionen ausgewertet werden:

```pawn
stock Float:Season_GetPriceModifier()
{
    switch (g_CurrentSeason)
    {
        case E_SEASON_SPRING: return 1.0;
        case E_SEASON_SUMMER: return 1.12;
        case E_SEASON_AUTUMN: return 1.08;
        case E_SEASON_WINTER: return 1.15;
    }
    return 1.0;
}
```

## 🏗️ Event-System

Die Saison sollte Events einbinden, die nur zu bestimmten Zeiten aktiv sind.

### 📘 Typen von Season-Events

- Festival-Event
- Stadtweihnacht / Winterfestival
- Sommerfest / Beach-Event
- Herbstmarkt / Erntedankfest
- Fraktionsturnier / PvP-Event / Gangs-War-Event
- Wirtschaftswettbewerb
- Tourismus-Wettbewerb
- Natur-/Umwelt-Event

### 🏗️ Event-Architektur

Ein Event sollte in folgenden Schritten laufen:

1. Saison definiert Event-Typ
2. Event wird für die Saison aktiviert
3. Startzeit und Dauer werden gesetzt
4. Belohnungen, Boni und Ziele werden erstellt
5. Abschluss + Verteilung der Rewards

```pawn
enum E_SEASON_EVENT
{
    E_SEASON_EVENT_NONE,
    E_SEASON_EVENT_FESTIVAL,
    E_SEASON_EVENT_MARKET,
    E_SEASON_EVENT_RACE,
    E_SEASON_EVENT_TOURNAMENT,
    E_SEASON_EVENT_FALL_CLEANUP,
    E_SEASON_EVENT_WINTER_FEST
};
```

Die konkrete Eventlogik sollte in einem separaten Modul liegen, um das Hauptsystem sauber zu halten.

## 🏗️ Integration in das bestehende Modul-System

Das Season-System passt elegant in die vorhandene modularisierte Architektur.

### 📘 Mögliche Modulaufteilung

- `core/` – globale Saison-Flags und zentrale API
- `state/` – staatlich relevante Auswirkungen und Treasury-Logik
- `jobs/` – Saison-spezifische Job- und Raubaktivitäten
- `economy/` – Preis-, Angebot- und Umsatzmodifikatoren
- `factions/` – Fraktions-Events und saisonale Ziele
- `business/` – Geschäftsangebote, Lieferketten und saisonale Waren
- `ui/` – Anzeige der aktuellen Saison im HUD/Info-Panel
- `admin/` – manuelle Saison-Übergänge und Debug-Steuerung

### 📘 Beispiel-API

```pawn
stock Season_SetCurrentSeason(seasonid)
stock Season_GetCurrentSeason()
stock Season_GetSeasonName(seasonid, name[], len)
stock Season_GetPriceModifier()
stock Season_StartSeasonEvent(eventid)
stock Season_EndSeasonEvent()
stock Season_AdvanceToNextSeason()
stock Season_IsSeasonActive(seasonid)
```

Diese API ist wichtig, damit verschiedene Module unabhängig voneinander auf den aktuellen Zustand reagieren können.

## 📘 Datenhaltung / Persistenz

Damit die Saison nicht nach Neustart „verloren“ geht, sollte sie persistent gespeichert werden.

### 📘 Beispiel für persistente Werte

- current_season
- season_started_at
- season_duration_days
- active_event_id
- reward_tier
- completed_events_count

Beispiel für SQLite-Struktur:

```sql
CREATE TABLE IF NOT EXISTS season_state (
    id INTEGER PRIMARY KEY CHECK (id = 1),
    current_season INTEGER NOT NULL,
    season_started_at INTEGER NOT NULL,
    season_duration_days INTEGER NOT NULL DEFAULT 7,
    active_event_id INTEGER DEFAULT 0,
    reward_tier INTEGER NOT NULL DEFAULT 0,
    completed_events_count INTEGER NOT NULL DEFAULT 0
);
```

Dadurch bleibt der Spielzustand über Server-Reboots konsistent und wird durch den Core-/DB-Layer sauber verwaltet.

## 🧩 UI- und Präsentationslogik

Die Saison sollte dem Spieler nicht nur indirekt spürbar sein, sondern auch sichtbar werden.

### 📘 Mögliche Anzeigen

- Widget im HUD: "Aktuelle Saison: Sommer"
- State-Info: "Sommerfest aktiv in Los Santos"
- Job-Board: "Saisonale Spezialaufträge verfügbar"
- Server-Announcement: "Frühling beginnt – neue Jobs und Preise aktiv"
- Logbuch: "Winter-Event gestartet. Sicherheitsaufträge gestiegen."

### 📘 Beispieltext

```pawn
new msg[128];
format(msg, sizeof(msg), "Aktuelle Saison: %s", Season_GetSeasonName(g_CurrentSeason));
TextDrawSetString(g_SeasonTD, msg);
```

Das UI darf aber nicht zu „copy-paste-UI“ werden. Es sollte nur die wichtigsten Informationen anzeigen und die eigentliche Logik im System selbst liegen.

## 📘 Belohnungen und Progression

Saisonale Fortschritte können zusätzlich als echten Progression-Prozess dienen.

### 📘 Beispiele

- Spieler erhalten Belohnungen für bestimmte Event-Teilnahmen
- Fraktionen erhalten saisonale Event-Punkte
- Businesses erhalten saisonal gesteigerte Gewinnmodifikatoren
- Spieler können saisonale Trophäen, Outfits, Accessoires oder Titel verdienen

### 📘 Reward-Stufen

```pawn
enum E_SEASON_REWARD_TIER
{
    E_SEASON_REWARD_NONE,
    E_SEASON_REWARD_BRONZE,
    E_SEASON_REWARD_SILVER,
    E_SEASON_REWARD_GOLD,
    E_SEASON_REWARD_PLATINUM
};
```

Das macht das System strategisch und nachvollziehbar, ohne eine komplette separate Meta-Progression aufbauen zu müssen.

## ✅ Balance- und Designrichtlinien

Ein gutes Season-System sollte niemals die Kernspielmechanik ersetzen oder unplanbar machen. Die Regeln dafür sind:

1. Die Saison muss für alle sichtbar und nachvollziehbar sein.
2. Die Auswirkungen müssen sauber und nicht zufällig sein.
3. Events sollten die Welt bereichern, nicht nur stören.
4. Core- und Economy-Logik müssen konsistent bleiben.
5. Die Season darf keine separate „Meta-Welt“ erzeugen, die außerhalb des Server-Systems steht.

## 📘 Beispiel-Flow

Ein typischer Lauf sieht so aus:

1. Server startet
2. `Season_Init()` lädt den aktuellen Zustand aus der DB
3. Saison wird dargestellt und aktiviert
4. globale Preis-/Job-Modifikatoren werden standardmäßig gesetzt
5. Eventmanager prüft, ob ein saisonales Event aktiv ist
6. Spieler sehen die Saison im HUD / UI
7. Wechsel nach X Tagen: `Season_AdvanceToNextSeason()`
8. neue Modifikatoren aktivieren sich
9. neue Events und Rewards werden erzeugt

## 📘 Empfehlung für die Umsetzung im Projekt

Für dieses Projekt ist die saubere Umsetzung am sinnvollsten als:

- separate Season-API im Core-/State-Layer
- saisonale Event-Logik in eigenem Module
- Economy-Modifikatoren zentralisiert im Economy-/State-System
- UI-Integrationsmodul nur für Anzeige und Notification
- persistente Speicherung über die bestehende SQLite-Schicht

Die optimale Architektur lautet daher:

- `core/` = Saisonstatus + API
- `state/` = politik-/staatliche saisonale Effekte
- `economy/` = Preismodifikatoren & Ressourcen-Strategien
- `jobs/` = saisonale Job-Balance
- `factions/` = Event- und Fraktionsziele
- `ui/` = Anzeige im HUD und Servernachrichten
- `db/` = Saison-Storage + Persistenz

## 📌 Roadmap

### 📘 Phase 1 – Core-Season
- Saison-Enum und zentrale API
- Persistenz und Ladepfad
- UI-Anzeige und Server-Announcements

### 📘 Phase 2 – Wirtschaftseffekte
- Preismodifikatoren pro Saison
- saisonale Lieferketten und Job-Balance
- Event-Start-/Ende-Logik

### 📘 Phase 3 – Lebenswelt-Events
- Fraktions-, Business- und Tourismusevents
- Bräuche und saisonale Szenarien
- erweiterte Reward- und Ranking-Logik

### 📘 Phase 4 – Long-Term-Progression
- jährliche Saison-Historie
- Persistente Weltveränderungen
- Saison-Stats, Leaderboards und Saison-Abschlüsse

## 📘 Fazit

Das Season-System ist ein essenzieller Bestandteil für einen langfristig lebendigen GTA:SA-Reallife-Gamemode. Es schafft nicht nur „bessere Optik“, sondern echtes RP- und Progressionsgefühl. Wenn es sauber im Core-, State-, Economy- und Job-Layer integriert wird, verstärkt es die Spielwelt ohne sie zu destabilisieren. Gerade in einem modularen Gamemode ist die Season ein idealer Brückenknoten zwischen Weltgefühl, wirtschaftlicher Logik und langfristiger Motivation.

Es ist die Art von System, das den Server von einer reinen Spielumgebung zu einer echten, sich entwickelnden Welt macht.

## 📘 Umsetzung 2026-11-20 (additiv)

Die folgende Ausbaustufe wurde direkt im Quellbaum ergänzt und ist bewusst als additiver, quellbasiert umsetzbarer Schritt gehalten:

- neues Modul `gamemodes/modules/events/season_system.inc`
- zentrale saisonale API mit `SeasonSystem_Initialize()`, `SeasonSystem_AdvanceSeason()`, `SeasonSystem_GetCurrentSeason()`, `SeasonSystem_GetEconomyMultiplier()` und `SeasonSystem_GetJobModifier()`
- Integration der Saison in die Lifecycle-/Initialisierungsreihenfolge über `gamemodes/gamemode.pwn`
- Lifecycle-Contract für `events`/`season` ergänzt, ohne parallele Schatten-States oder live-Server-Abhängigkeit einzuführen
- die Saison bleibt an bestehende Core-/State-/Economy-/Job-/UI-Logik angebunden und wird als reine Source-of-Truth-Implementierung behandelt, nicht als 7780-Live-Funktion

Diese Ergänzung dokumentiert den aktuellen Stand des Moduls und hält die ältere Roadmap im Dokument unverändert; sie erweitert sie lediglich um den umgesetzten Codepfad und die konsequente Quellbaum-Umsetzung ohne bestehende Abschnitte zu überschreiben.


---
## Erweiterungsstand / Aktualisierung (2026-11-21)

Diese Ergänzung dient dazu, den bisherigen Dokumentationsstand auf den aktuellen Quellcode zu synchronisieren, ohne vorhandene Inhalte zu löschen oder zu überschreiben.

### Grundlage der Aktualisierung
- Basis: `gamemodes/gamemode.pwn` und `gamemodes/modules/**`
- Prüfkriterium: aktuelle Modulanbindung, Commands, UI-Initialisierung und TextDraw-Erzeugung
- Stand: source-level inspection, keine laufende Server-Validierung

### Aktueller Projektstatus
- Die Modulanbindung erfolgt über `OnGameModeInit()` und den Befehlshandler in `OnPlayerCommandText()`.
- Viele Systeme sind vorhanden und als aktive Kernmodule strukturell eingebunden.
- Einige Module und UI-Elemente sind angelegt, aber noch nicht vollständig in den aktiven Runtime-Flow eingehängt.
- Die maßgebliche Source-Quelle für alle Aussagen bleibt `gamemodes/`; `Compiler/samp03/gamemodes/` ist ein älterer Spiegelstand.

### Aktualisierungsprinzip
- Vorhandene Inhalte bleiben erhalten.
- Neue Informationen werden als Ergänzungen an das vorhandene Dokument angehängt.
- Statusangaben werden bei Bedarf als `aktiv`, `verdeckt`, `gebaut aber nicht angebunden` oder `vorbereitet` markiert.

### Querverweise zu den zentralen Dokus
- Commands: `COMMANDS.md`
- Module: `MODULES.md`
- TextDraws: `TEXTDRAWS.md`
- Moduldynamik: `MODULE_DEPENDENCIES.md`
- Admin-Architektur: `ADMIN_SYSTEMS.md`
- State-System: `STATE_SYSTEM.md`
- DB-Persistenz: `DATABASE_SCHEMA.md`
- UI-Flow: `UI_FLOW.md`
- Progression: `PLAYER_PROGRESSION.md`
- Fahrzeugsystem: `VEHICLE_SYSTEM.md`
- Wirtschaft: `ECONOMY_SYSTEMS.md`
- Faction/War: `FACTION_AND_WAR_SYSTEMS.md`
- Law/Emergency: `EMERGENCY_AND_LAW.md`
- Housing/Inventory: `HOUSING_AND_INVENTORY.md`
- Events/Season: `EVENTS_AND_SEASON.md`

### Kurzfazit
Das Projekt bleibt modular aufgebaut, mit einer klaren Trennung zwischen Core, State, Economy, Jobs, Fraktionen, UI und Persistenz. Die dokumentierte Architektur ist konsistent mit dem aktuellen Code-Stand, auch wenn einzelne Funktionen noch als vorbereitet oder nur teilweise angebunden erkennbar sind.


---

## Zusätzliche Erweiterung / Projekt-Lage (2026-11-21)

Diese Erweiterung dient dazu, den bisherigen Dokumentationsstand noch weiter auf den aktuellen Quellcode zu kalibrieren, ohne bestehende Informationen zu überschreiben.

### Was hier zusätzlich klarer wird
- Die Projektstruktur ist weiterhin als modularer SA:MP-Gamemode aufgebaut, mit klarer Trennung zwischen Core, DB, State, Economy, Jobs, Law, Faction, UI und World-Systems.
- Die wichtigsten Laufzeitpunkte bleiben `OnGameModeInit()` und `OnPlayerCommandText()`. Dadurch wird die aktive Funktionalität sauber von vorbereiteten, verdeckten oder nur im Code angelegten Systemen abgegrenzt.
- Die Dokumentation beschreibt bewusst den source-basierten Ist-Stand: Sie zeigt, was im Code aktiv, verknüpft, vorbereitet oder nur noch als Entwurf vorhanden ist.
- Viele Systeme sind bereits architektonisch sauber angelegt, aber nicht immer vollständig in den Spiel-Flow eingehängt. Das ist ein normaler Status bei modularen RP-Gamemodes in Aufbauphase.

### Wichtig für Weiterentwicklung
- Neue Funktionen sollten immer mit Status markiert werden: `aktiv`, `verdeckt`, `gebaut aber nicht angebunden`, `vorbereitet` oder `archiviert`.
- Befehls- und UI-Liste sollten mit der tatsächlichen Initialisierung im Haupt-Gamemode abgeglichen werden, damit keine falschen Annahmen über Live-Funktionalität entstehen.
- Module ohne aktive Einbindung sollten als „integrationsreif“ gekennzeichnet werden, aber nicht als fertig dokumentiert gelten, wenn sie in der aktuellen Laufzeit nicht erreichbar sind.

### Gesamtüberblick des aktuellen Stands
- Das Projekt ist bereits stark strukturiert und fachlich klar gegliedert.
- Es besitzt eine solide Kernarchitektur, eine wiederverwendbare Modullandschaft und eine klare Trennung zwischen Daten, Logik und UI.
- Die wichtigsten offenen Punkte liegen eher in der Finalisierung und Verknüpfung von einzelnen Systemen als in der Grundarchitektur selbst.
- Die Dokumente bleiben damit als echte Arbeitsbasis nutzbar: detailliert genug für Entwicklung, aber immer nachvollziehbar als Quellcode-basierte Bestandsaufnahme.

### Nächster sinnvoller Schritt
- Die nächste Stufe wäre keine weitere Neu-Dokumentation, sondern eine gezielte Runtime-Validierung: Compile, serverseitiges Testen und Abgleich mit den bereits dokumentierten aktiven Systemen.
- Danach kann man die Dokumentation mit einer „Live-Status-Checkliste“ ergänzen, die jeden Command, TextDraw und Modul nach „aktiv / verdeckt / nicht verbunden“ kategorisiert.

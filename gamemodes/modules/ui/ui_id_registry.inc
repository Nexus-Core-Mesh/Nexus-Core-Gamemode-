#if !defined UI_ID_REGISTRY_INC
#define UI_ID_REGISTRY_INC

// -----------------------------------------------------------------------------
// Nexus Core UI-ID-Registry
// -----------------------------------------------------------------------------
// Zweck:
//   - zentrale Zuordnung aller Dialog-/UI-IDs und zugehöriger Modulbereiche
//   - eindeutige, dokumentierte Ranges, damit keine Mehrfachbelegung entsteht
//   - kurze Randnotizen für UI-Fluss, Anbindung und Hauptfunktionen
// -----------------------------------------------------------------------------
// Regeln:
//   - neue Dialoge immer in einem freien Range anlegen
//   - nur Module mit aktivem Runtime-Pfad dürfen IDs in Gebrauch nehmen
//   - TextDraw-/PlayerTextDraw-Handles müssen sauber über INVALID_TEXT_DRAW checked werden
//   - Optionale Eingaben bleiben optional; UI-Hauptpfad darf nicht an Chat-Commands hängen
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Dialog-ID Ranges
// -----------------------------------------------------------------------------
// HUB / UI-Hauptmenü: 9000..9099
// Zuordnungen: Main-Hub, Inventar, Handy, Business, Mission-Board, Garage, Tuning,
// State-Menu, Stats, Auth-Flow und Schnellzugriffe.
#define UI_REGISTRY_RANGE_HUB        "9000..9099"
#define UI_REGISTRY_HUB_MAIN         9000  // DIALOG_UI_MAIN - Haupteinstieg, Dialog-First UI-Hub
#define UI_REGISTRY_INV_LIST         9010  // DIALOG_UI_INV_LIST - Inventarliste / Slot- und Stack-Ansicht
#define UI_REGISTRY_INV_ACTION       9011  // DIALOG_UI_INV_ACTION - Item-Aktion (Nutzen, Wegwerfen, Transfer)
#define UI_REGISTRY_INV_GIVE_TARGET  9012  // DIALOG_UI_INV_GIVE_TARGET - Zielspieler oder Name für Transfers
#define UI_REGISTRY_INV_GIVE_QTY     9013  // DIALOG_UI_INV_GIVE_QTY - Menge für Item-Transfer
#define UI_REGISTRY_PHONE_MAIN       9020  // Handy-Hauptmenü
#define UI_REGISTRY_PHONE_ADD        9021  // Kontakt hinzufügen
#define UI_REGISTRY_PHONE_SMS        9022  // SMS / Nachricht verfassen
#define UI_REGISTRY_BIZ_LIST         9030  // Business-Liste / Unternehmen
#define UI_REGISTRY_BIZ_ACTION       9031  // Business-Aktion (Kaufen, Verwalten, Payout)
#define UI_REGISTRY_AMMU_LIST        9040  // Ammu-Nation-Liste
#define UI_REGISTRY_AMMU_CONFIRM     9041  // Waffen-/Ausrüstungs-Bestätigung
#define UI_REGISTRY_MISSION_LIST     9050  // Missionsliste / Mission-Board
#define UI_REGISTRY_MISSION_CONFIRM  9051  // Mission annehmen / bestätigen
#define UI_REGISTRY_GARAGE_LIST      9060  // Garage-Liste / Fahrzeugliste
#define UI_REGISTRY_GARAGE_ACTION    9061  // Garage-Aktion (Teleport, Einlagern, Tuning)
#define UI_REGISTRY_TUNING_LIST      9070  // Tuning-Liste
#define UI_REGISTRY_TUNING_CONFIRM   9071  // Tuning-Bestätigung
#define UI_REGISTRY_STATE_PICK       9072  // Staatswahl / Spawn-/Home-State-Auswahl
#define UI_REGISTRY_VEHICLE_STATUS   9073  // Fahrzeug-Status / Fahrzeug-Info
#define UI_REGISTRY_VEHICLE_MENU     9074  // Fahrzeug-Hub / Fahrzeugsystem
#define UI_REGISTRY_JOB_MENU         9075  // Job-Menü / Job-Übersicht
#define UI_REGISTRY_STATE_MENU       9076  // Schnellzugriff: Staatsinfo / Wirtschaft / Status
#define UI_REGISTRY_STATS_MENU       9077  // Spieler-Stats / Dashboard
#define UI_REGISTRY_ADMIN_MENU       9078  // Admin-Menü / Betriebskonsole
#define UI_REGISTRY_AUTH_MAIN        9079  // Login-/Register-Hauptdialog
#define UI_REGISTRY_AUTH_PASSWORD    9080  // Passwort-Eingabe (optional, aber sicher)
#define UI_REGISTRY_AUTH_STATE       9081  // Staatswahl / Login-Startzustand
#define UI_REGISTRY_TUTORIAL         9082  // Tutorial-Weiter-Dialog

// JOBS / Job-Dialoge: 9100..9119
// Zuordnungen: Job-Zentrum, Job-Admin-Status, Job-Aktionen, Quit-Flow.
#define UI_REGISTRY_RANGE_JOBS       "9100..9119"
#define UI_REGISTRY_JOB_MAIN         9100  // Job-Zentrum / Hauptmenü
#define UI_REGISTRY_JOB_LIST         9101  // Verfügbare Jobs / Auswahl
#define UI_REGISTRY_JOB_CONFIRM      9102  // Job-Annahme-Bestätigung
#define UI_REGISTRY_JOB_STATUS       9103  // Job-Fortschritt / Statusboard
#define UI_REGISTRY_JOB_QUIT         9104  // Job kündigen / Bestätigung

// STATE / Staats- und Regierungsverwaltung: 9120..9159
// Zuordnungen: Tax, Transfer, Politik, Lohnfaktor, Budget, Kassenüberblick.
#define UI_REGISTRY_RANGE_STATE      "9120..9159"
#define UI_REGISTRY_STATE_MAIN       9120  // Staats-Leitstand, Hauptmenü
#define UI_REGISTRY_STATE_PICK       9121  // Auswahl des Staats / State-Target
#define UI_REGISTRY_STATE_ACTION     9122  // Staats-Übersicht / Kasse / Werte
#define UI_REGISTRY_STATE_TAX_INPUT  9123  // Steuersatz eingeben
#define UI_REGISTRY_STATE_TRANSFER_INPUT 9124  // Treasury-Transfer: Betrag eingeben
#define UI_REGISTRY_STATE_TRANSFER_DIR 9125  // Transferart: Einzahlen / Auszahlen
#define UI_REGISTRY_STATE_POLICY_INPUT 9126  // Politiktext / Policy-Wert
#define UI_REGISTRY_STATE_WAGE_INPUT 9127  // Job-Lohnfaktor / Gehaltsfaktor
#define UI_REGISTRY_STATE_BUDGET_INPUT 9128  // Budgetgenehmigung / Staatskassen-Budget

// WAR / Fraktions- und Territoriumskontext: 9160..9199
// Zuordnungen: Text-Wars, Territorien, Angriffe, Faction-Selection.
#define UI_REGISTRY_RANGE_WAR        "9160..9199"
#define UI_REGISTRY_WAR_MAIN         9160  // Text-Wars Hauptmenü
#define UI_REGISTRY_WAR_TERRITORY_LIST 9161  // Territorienliste
#define UI_REGISTRY_WAR_TERRITORY_ACTION 9162  // Gebietsaktion / Beanspruchen
#define UI_REGISTRY_WAR_INFO         9163  // War-Info / Status-Info
#define UI_REGISTRY_WAR_FACTION_PICK 9164  // Faktion wählen / Angriffs-Ziel

// -----------------------------------------------------------------------------
// Modulbezogene TextDraw-/HUD-Gruppen
// -----------------------------------------------------------------------------
// Die TextDraw-/PlayerTextDraw-Handles sind pro Spieler und Modul separat zu halten.
// Jedes Modul verwendet seine eigenen Handles und sollte vor Show/Hide mit
// INVALID_TEXT_DRAW geprüft werden.
#define UI_REGISTRY_TEXTDRAW_GROUP_AUTH        "login_system.inc / register-flow / tutorial"
#define UI_REGISTRY_TEXTDRAW_GROUP_ROLEPLAY   "roleplay_ui_system.inc / HUD dashboard"
#define UI_REGISTRY_TEXTDRAW_GROUP_MISSION    "mission_system.inc / mission board"
#define UI_REGISTRY_TEXTDRAW_GROUP_LAW        "law_system.inc / wanted status"
#define UI_REGISTRY_TEXTDRAW_GROUP_JAIL       "jail_system.inc / jail HUD"
#define UI_REGISTRY_TEXTDRAW_GROUP_CRIME      "crime_system.inc / crime tracker"
#define UI_REGISTRY_TEXTDRAW_GROUP_GANGWAR    "gangwar_system.inc / gang war HUD"
#define UI_REGISTRY_TEXTDRAW_GROUP_SHOP       "shop_system.inc / shop overview"
#define UI_REGISTRY_TEXTDRAW_GROUP_TRANSPORT  "transport_system.inc / route HUD"
#define UI_REGISTRY_TEXTDRAW_GROUP_CITYECON  "city_economy_system.inc / city economy" 
#define UI_REGISTRY_TEXTDRAW_GROUP_JOB        "dynamic_job_system.inc / job list"
#define UI_REGISTRY_TEXTDRAW_GROUP_ADMIN      "admin_config_system.inc / admin map + config"

// -----------------------------------------------------------------------------
// Runtime-Notizen zu den belegten Bereichen
// -----------------------------------------------------------------------------
// - Hub/State/Jobs/War sind die aktive Dialog-Logik; neue Dialoge sollen nur in
//   den frei reservierten Bereichen ergänzt werden.
// - Main-UI-Pfade sollten primär über Dialoge / klickbare TextDraws / J-Key in Reichweite laufen.
// - Player-ID, Player-Name oder Geldbetrag bleiben als optionale Eingabepfade erhalten,
//   sind aber kein Pflichtweg für Nutzung der Standardmenüs.
// - UI-Hub, State-Control, Job-Center, TextWars und Mission-Board sind die primären
//   Menüpunkte, an die die System-Module angebunden werden sollten.
// - Safety-Pattern: TextDrawDestroy/Hide nur nach INVALID_TEXT_DRAW-Check, damit alte Handles
//   oder bereits abgelaufene UI-Instanzen keine Runtime-Abstürze erzeugen.

stock bool:UiRegistry_IsValidDialogId(dialogid)
{
    return dialogid == UI_REGISTRY_HUB_MAIN ||
           dialogid == UI_REGISTRY_INV_LIST ||
           dialogid == UI_REGISTRY_INV_ACTION ||
           dialogid == UI_REGISTRY_INV_GIVE_TARGET ||
           dialogid == UI_REGISTRY_INV_GIVE_QTY ||
           dialogid == UI_REGISTRY_PHONE_MAIN ||
           dialogid == UI_REGISTRY_PHONE_ADD ||
           dialogid == UI_REGISTRY_PHONE_SMS ||
           dialogid == UI_REGISTRY_BIZ_LIST ||
           dialogid == UI_REGISTRY_BIZ_ACTION ||
           dialogid == UI_REGISTRY_AMMU_LIST ||
           dialogid == UI_REGISTRY_AMMU_CONFIRM ||
           dialogid == UI_REGISTRY_MISSION_LIST ||
           dialogid == UI_REGISTRY_MISSION_CONFIRM ||
           dialogid == UI_REGISTRY_GARAGE_LIST ||
           dialogid == UI_REGISTRY_GARAGE_ACTION ||
           dialogid == UI_REGISTRY_TUNING_LIST ||
           dialogid == UI_REGISTRY_TUNING_CONFIRM ||
           dialogid == UI_REGISTRY_STATE_PICK ||
           dialogid == UI_REGISTRY_VEHICLE_STATUS ||
           dialogid == UI_REGISTRY_VEHICLE_MENU ||
           dialogid == UI_REGISTRY_JOB_MENU ||
           dialogid == UI_REGISTRY_STATE_MENU ||
           dialogid == UI_REGISTRY_STATS_MENU ||
           dialogid == UI_REGISTRY_ADMIN_MENU ||
           dialogid == UI_REGISTRY_AUTH_MAIN ||
           dialogid == UI_REGISTRY_AUTH_PASSWORD ||
           dialogid == UI_REGISTRY_AUTH_STATE ||
           dialogid == UI_REGISTRY_JOB_MAIN ||
           dialogid == UI_REGISTRY_JOB_LIST ||
           dialogid == UI_REGISTRY_JOB_CONFIRM ||
           dialogid == UI_REGISTRY_JOB_STATUS ||
           dialogid == UI_REGISTRY_JOB_QUIT ||
           dialogid == UI_REGISTRY_STATE_MAIN ||
           dialogid == UI_REGISTRY_STATE_PICK ||
           dialogid == UI_REGISTRY_STATE_ACTION ||
           dialogid == UI_REGISTRY_STATE_TAX_INPUT ||
           dialogid == UI_REGISTRY_STATE_TRANSFER_INPUT ||
           dialogid == UI_REGISTRY_STATE_TRANSFER_DIR ||
           dialogid == UI_REGISTRY_STATE_POLICY_INPUT ||
           dialogid == UI_REGISTRY_STATE_WAGE_INPUT ||
           dialogid == UI_REGISTRY_STATE_BUDGET_INPUT ||
           dialogid == UI_REGISTRY_WAR_MAIN ||
           dialogid == UI_REGISTRY_WAR_TERRITORY_LIST ||
           dialogid == UI_REGISTRY_WAR_TERRITORY_ACTION ||
           dialogid == UI_REGISTRY_WAR_INFO ||
           dialogid == UI_REGISTRY_WAR_FACTION_PICK;
}

stock bool:UiRegistry_IsValidTextDrawHandle(Text:td)
{
    return td != Text:INVALID_TEXT_DRAW;
}

#endif

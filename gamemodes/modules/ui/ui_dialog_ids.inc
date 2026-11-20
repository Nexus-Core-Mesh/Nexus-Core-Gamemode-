#if !defined UI_DIALOG_IDS_INC
#define UI_DIALOG_IDS_INC

// Zentrale Dialog-ID-Vergabe. Jede UI-Funktion hat einen reservierten Bereich,
// damit OnDialogResponse deterministisch an genau ein Modul geroutet wird.
#define DIALOG_UI_NONE 0

#define DIALOG_UI_RANGE_HUB_FIRST 9000
#define DIALOG_UI_RANGE_HUB_LAST 9099
#define DIALOG_UI_MAIN 9000
#define DIALOG_UI_INV_LIST 9010
#define DIALOG_UI_INV_ACTION 9011
#define DIALOG_UI_INV_GIVE_TARGET 9012
#define DIALOG_UI_INV_GIVE_QTY 9013
#define DIALOG_UI_PHONE_MAIN 9020
#define DIALOG_UI_PHONE_ADD 9021
#define DIALOG_UI_PHONE_SMS 9022
#define DIALOG_UI_BIZ_LIST 9030
#define DIALOG_UI_BIZ_ACTION 9031
#define DIALOG_UI_AMMU_LIST 9040
#define DIALOG_UI_AMMU_CONFIRM 9041
#define DIALOG_UI_MISSION_LIST 9050
#define DIALOG_UI_MISSION_CONFIRM 9051
#define DIALOG_UI_GARAGE_LIST 9060
#define DIALOG_UI_GARAGE_ACTION 9061
#define DIALOG_UI_TUNING_LIST 9070
#define DIALOG_UI_TUNING_CONFIRM 9071
#define DIALOG_UI_STATE_PICK 9072
#define DIALOG_UI_VEHICLE_STATUS 9073
#define DIALOG_UI_VEHICLE_MENU 9074
#define DIALOG_UI_JOB_MENU 9075
#define DIALOG_UI_STATE_MENU 9076
#define DIALOG_UI_STATS_MENU 9077
#define DIALOG_UI_ADMIN_MENU 9078
#define DIALOG_UI_AUTH_MAIN 9079
#define DIALOG_UI_AUTH_PASSWORD 9080
#define DIALOG_UI_AUTH_STATE 9081

#define DIALOG_UI_RANGE_JOBS_FIRST 9100
#define DIALOG_UI_RANGE_JOBS_LAST 9119
#define DIALOG_JOB_MAIN 9100
#define DIALOG_JOB_LIST 9101
#define DIALOG_JOB_CONFIRM 9102
#define DIALOG_JOB_STATUS 9103
#define DIALOG_JOB_QUIT 9104

#define DIALOG_UI_RANGE_STATE_FIRST 9120
#define DIALOG_UI_RANGE_STATE_LAST 9159
#define DIALOG_STATE_MAIN 9120
#define DIALOG_STATE_PICK 9121
#define DIALOG_STATE_ACTION 9122
#define DIALOG_STATE_TAX_INPUT 9123
#define DIALOG_STATE_TRANSFER_INPUT 9124
#define DIALOG_STATE_TRANSFER_DIR 9125
#define DIALOG_STATE_POLICY_INPUT 9126
#define DIALOG_STATE_WAGE_INPUT 9127
#define DIALOG_STATE_BUDGET_INPUT 9128

#define DIALOG_UI_RANGE_WAR_FIRST 9160
#define DIALOG_UI_RANGE_WAR_LAST 9199
#define DIALOG_WAR_MAIN 9160
#define DIALOG_WAR_TERRITORY_LIST 9161
#define DIALOG_WAR_TERRITORY_ACTION 9162
#define DIALOG_WAR_INFO 9163
#define DIALOG_WAR_FACTION_PICK 9164

#define UI_DIALOG_BUTTON_OK "Auswählen"
#define UI_DIALOG_BUTTON_BACK "Zurück"
#define UI_DIALOG_BUTTON_CLOSE "Schließen"

// Eingaben landen in SQL-/format-Strings, daher Sonderzeichen konsequent abweisen.
stock UiDialog_IsSafeText(const text[])
{
    for (new i = 0; text[i] != 0; i++)
    {
        if (text[i] == '%' || text[i] == '\'' || text[i] == '"' || text[i] == '\\' || text[i] == '|' || text[i] == '`' || text[i] == ';')
        {
            return false;
        }
    }
    return true;
}

stock UiDialog_Show(playerid, dialogid, style, const caption[], const info[], const button1[], const button2[])
{
    new tmpCaption[128];
    new tmpInfo[1024];
    new tmpButton1[32];
    new tmpButton2[32];
    format(tmpCaption, sizeof(tmpCaption), "%s", caption);
    format(tmpInfo, sizeof(tmpInfo), "%s", info);
    format(tmpButton1, sizeof(tmpButton1), "%s", button1);
    format(tmpButton2, sizeof(tmpButton2), "%s", button2);
    ShowPlayerDialog(playerid, dialogid, style, tmpCaption, tmpInfo, tmpButton1, tmpButton2);
}

stock Text:UiDialog_TextDrawCreate(Float:x, Float:y, const text[])
{
    new tmp[128];
    format(tmp, sizeof(tmp), "%s", text);
    return TextDrawCreate(x, y, tmp);
}

stock PlayerText:UiDialog_CreatePlayerTextDraw(playerid, Float:x, Float:y, const text[])
{
    new tmp[128];
    format(tmp, sizeof(tmp), "%s", text);
    return CreatePlayerTextDraw(playerid, x, y, tmp);
}

#endif

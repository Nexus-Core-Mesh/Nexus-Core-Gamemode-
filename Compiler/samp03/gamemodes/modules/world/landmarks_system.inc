#if !defined LANDMARKS_SYSTEM_INC
#define LANDMARKS_SYSTEM_INC

#include <a_samp>

#define MAX_LANDMARKS 32

new gLandmarkState[MAX_LANDMARKS];
new Float:gLandmarkX[MAX_LANDMARKS];
new Float:gLandmarkY[MAX_LANDMARKS];
new Float:gLandmarkZ[MAX_LANDMARKS];
new gLandmarkType[MAX_LANDMARKS][24];
new gLandmarkName[MAX_LANDMARKS][48];

stock LandmarksSystem_Initialize()
{
    for (new i = 0; i < MAX_LANDMARKS; i++)
    {
        gLandmarkState[i] = -1;
        gLandmarkX[i] = 0.0;
        gLandmarkY[i] = 0.0;
        gLandmarkZ[i] = 0.0;
        format(gLandmarkType[i], 24, "unknown");
        format(gLandmarkName[i], 48, "Landmark");
    }

    LandmarksSystem_RegLandmark(STATE_LOS_SANTOS, "Alhambra", "landmark", 1194.0, 1024.0, 10.0);
    LandmarksSystem_RegLandmark(STATE_LOS_SANTOS, "Vinewood Sign", "landmark", 1339.0, 1030.0, 12.0);
    LandmarksSystem_RegLandmark(STATE_SAN_FIERRO, "San Fierro Airport", "airport", -1318.0, -123.0, 14.0);
    LandmarksSystem_RegLandmark(STATE_SAN_FIERRO, "Bayside Marina", "harbor", -1903.0, 157.0, 12.0);
    LandmarksSystem_RegLandmark(STATE_LAS_VENTURAS, "Las Venturas Strip", "casino", 2034.0, 1118.0, 10.0);
    LandmarksSystem_RegLandmark(STATE_LAS_VENTURAS, "Alhambra", "landmark", 1968.0, 1207.0, 11.0);

    print("[LANDMARKS] Stadtmarken und Landmarken initialisiert");
}

stock LandmarksSystem_RegLandmark(stateid, const name[], const type[], Float:x, Float:y, Float:z)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    for (new i = 0; i < MAX_LANDMARKS; i++)
    {
        if (gLandmarkState[i] == -1)
        {
            gLandmarkState[i] = stateid;
            format(gLandmarkName[i], 48, "%s", name);
            format(gLandmarkType[i], 24, "%s", type);
            gLandmarkX[i] = x;
            gLandmarkY[i] = y;
            gLandmarkZ[i] = z;
            return true;
        }
    }
    return false;
}

stock LandmarksSystem_GetNearest(playerid, stateid, landmarkName[], landmarkNameLen, Float:radius)
{
    if (!Core_IsValidState(stateid) || playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    new closestIndex = -1;
    new Float:closestDistance = radius + 1.0;

    for (new i = 0; i < MAX_LANDMARKS; i++)
    {
        if (gLandmarkState[i] != stateid)
        {
            continue;
        }

        new Float:dist = GetDistanceBetweenPoints3D(px, py, pz, gLandmarkX[i], gLandmarkY[i], gLandmarkZ[i]);
        if (dist < closestDistance)
        {
            closestIndex = i;
            closestDistance = dist;
        }
    }

    if (closestIndex == -1)
    {
        return false;
    }

    format(landmarkName, landmarkNameLen, "%s", gLandmarkName[closestIndex]);
    return true;
}

stock LandmarksSystem_NotifyNear(playerid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        return;
    }

    new landmarkName[48];
    if (LandmarksSystem_GetNearest(playerid, stateid, landmarkName, sizeof(landmarkName), 30.0))
    {
        SendClientMessage(playerid, 0xD6D6D6AA, "[LANDMARK] In deiner Nähe: %s", landmarkName);
    }
}

#endif

#if defined _INC_dini
    #endinput
#endif
#define _INC_dini

#if !defined INVALID_FILE_HANDLE
    #define INVALID_FILE_HANDLE (File:0)
#endif

stock dini_Create(const filename[])
{
    new file[128];
    format(file, sizeof(file), "scriptfiles/%s", filename);
    if (strfind(filename, ".ini", true) == -1)
    {
        format(file, sizeof(file), "scriptfiles/%s.ini", filename);
    }

    new File:hFile = fopen(file, io_write);
    if (hFile != INVALID_FILE_HANDLE)
    {
        fclose(hFile);
        return 1;
    }
    return 0;
}

stock dini_Exists(const filename[])
{
    new file[128];
    format(file, sizeof(file), "scriptfiles/%s", filename);
    if (strfind(filename, ".ini", true) == -1)
    {
        format(file, sizeof(file), "scriptfiles/%s.ini", filename);
    }
    return fexist(file);
}

stock dini_Set(const filename[], const key[], const value[])
{
    new file[128];
    format(file, sizeof(file), "scriptfiles/%s", filename);
    if (strfind(filename, ".ini", true) == -1)
    {
        format(file, sizeof(file), "scriptfiles/%s.ini", filename);
    }

    if (!dini_Exists(filename))
    {
        dini_Create(filename);
    }

    new output[512];
    new line[128];
    new tmp[256];
    new found = 0;

    new File:hFile = fopen(file, io_read);
    if (hFile != INVALID_FILE_HANDLE)
    {
        while (fread(hFile, tmp))
        {
            if (tmp[0] == '\0')
            {
                continue;
            }

            new eqPos = strfind(tmp, "=", true);
            if (eqPos != -1)
            {
                new currentKey[64];
                strmid(currentKey, tmp, 0, eqPos);
                trim(currentKey);
                if (strcmp(currentKey, key, true) == 0)
                {
                    format(line, sizeof(line), "%s=%s\n", key, value);
                    format(output, sizeof(output), "%s%s", output, line);
                    found = 1;
                    continue;
                }
            }

            format(output, sizeof(output), "%s%s", output, tmp);
            if (tmp[strlen(tmp) - 1] != '\n' && tmp[strlen(tmp) - 1] != '\r')
            {
                format(output, sizeof(output), "%s\n", output);
            }
        }
        fclose(hFile);
    }

    if (!found)
    {
        format(output, sizeof(output), "%s%s=%s\n", output, key, value);
    }

    new tempFile[128];
    format(tempFile, sizeof(tempFile), "%s.tmp", file);
    new File:tempHandle = fopen(tempFile, io_write);
    if (tempHandle == INVALID_FILE_HANDLE)
    {
        return 0;
    }

    fwrite(tempHandle, output);
    fclose(tempHandle);

    new validation[256];
    new bool:hasContent = false;
    new File:validateHandle = fopen(tempFile, io_read);
    if (validateHandle == INVALID_FILE_HANDLE)
    {
        fremove(tempFile);
        return 0;
    }

    while (fread(validateHandle, tmp))
    {
        if (tmp[0] != '\0')
        {
            hasContent = true;
            break;
        }
    }
    fclose(validateHandle);

    if (!hasContent && output[0] == '\0')
    {
        fremove(tempFile);
        return 0;
    }

    if (fexist(file))
    {
        fremove(file);
    }

    new File:commitHandle = fopen(file, io_write);
    if (commitHandle == INVALID_FILE_HANDLE)
    {
        if (fexist(tempFile))
        {
            fremove(tempFile);
        }
        return 0;
    }

    validateHandle = fopen(tempFile, io_read);
    if (validateHandle != INVALID_FILE_HANDLE)
    {
        while (fread(validateHandle, tmp))
        {
            fwrite(commitHandle, tmp);
        }
        fclose(validateHandle);
    }
    fclose(commitHandle);
    fremove(tempFile);
    return 1;
}

stock dini_Get(const filename[], const key[], value[], size = sizeof(value))
{
    new file[128];
    format(file, sizeof(file), "scriptfiles/%s", filename);
    if (strfind(filename, ".ini", true) == -1)
    {
        format(file, sizeof(file), "scriptfiles/%s.ini", filename);
    }
    if (!dini_Exists(filename))
    {
        value[0] = '\0';
        return 0;
    }

    new line[128], tmp[256], found = 0;
    new File:hFile = fopen(file, io_read);
    if (hFile == INVALID_FILE_HANDLE)
    {
        value[0] = '\0';
        return 0;
    }

    while (fread(hFile, tmp))
    {
        if (tmp[0] != '\0')
        {
            new eqPos = strfind(tmp, "=", true);
            if (eqPos != -1)
            {
                new currentKey[64];
                strmid(currentKey, tmp, 0, eqPos);
                trim(currentKey);
                if (strcmp(currentKey, key, true) == 0)
                {
                    strmid(line, tmp, eqPos + 1, strlen(tmp));
                    trim(line);
                    strmid(value, line, 0, strlen(line));
                    found = 1;
                    break;
                }
            }
        }
    }
    fclose(hFile);
    if (!found)
    {
        value[0] = '\0';
        return 0;
    }
    return 1;
}

stock dini_IntSet(const filename[], const key[], value)
{
    new buf[32];
    format(buf, sizeof(buf), "%d", value);
    return dini_Set(filename, key, buf);
}

stock dini_Int(const filename[], const key[])
{
    new value[32];
    if (dini_Get(filename, key, value, sizeof(value)))
    {
        return strval(value);
    }
    return 0;
}

stock Float:dini_FloatSet(const filename[], const key[], Float:value)
{
    new buf[32];
    format(buf, sizeof(buf), "%f", value);
    dini_Set(filename, key, buf);
    return value;
}

stock Float:dini_Float(const filename[], const key[])
{
    new value[32];
    if (dini_Get(filename, key, value, sizeof(value)))
    {
        return floatstr(value);
    }
    return 0.0;
}

stock dini_RemoveKey(const filename[], const key[])
{
    new file[128], output[512], line[128], tmp[256], valid = 0;
    format(file, sizeof(file), "scriptfiles/%s", filename);
    if (strfind(filename, ".ini", true) == -1)
    {
        format(file, sizeof(file), "scriptfiles/%s.ini", filename);
    }
    if (!dini_Exists(filename))
    {
        return 0;
    }

    new File:hFile = fopen(file, io_read);
    if (hFile == INVALID_FILE_HANDLE)
    {
        return 0;
    }

    while (fread(hFile, tmp))
    {
        if (tmp[0] != '\0')
        {
            new eqPos = strfind(tmp, "=", true);
            if (eqPos == -1)
            {
                format(output, sizeof(output), "%s%s", output, tmp);
                continue;
            }
            new currentKey[64];
            strmid(currentKey, tmp, 0, eqPos);
            trim(currentKey);
            if (strcmp(currentKey, key, true) != 0)
            {
                format(output, sizeof(output), "%s%s", output, tmp);
                valid = 1;
            }
        }
    }
    fclose(hFile);

    hFile = fopen(file, io_write);
    if (hFile == INVALID_FILE_HANDLE)
    {
        return 0;
    }
    if (valid)
    {
        fwrite(hFile, output);
    }
    fclose(hFile);
    return 1;
}

stock dini_RemoveFile(const filename[])
{
    new file[128];
    format(file, sizeof(file), "scriptfiles/%s", filename);
    if (strfind(filename, ".ini", true) == -1)
    {
        format(file, sizeof(file), "scriptfiles/%s.ini", filename);
    }
    return fremove(file);
}

#if !defined MODERN_HELPERS_INC
#define MODERN_HELPERS_INC

#define MODERN_HELPERS_VERSION "1.0.0"
#define INVALID_STATE_ID -1
#define STATE_COUNT 3
#define IS_EMPTY_STRING(%1) ((%1)[0] == '\0')
#define ARRAY_SIZE(%1) (sizeof(%1))

stock min_val(value, min_value, max_value)
{
    if (value < min_value) return min_value;
    if (value > max_value) return max_value;
    return value;
}

stock SafeStringCopy(dest[], const source[], length)
{
    if (length <= 0) return;
    format(dest, length, "%s", source);
}

stock bool:IsValidPlayerId(player_id)
{
    return player_id >= 0 && player_id < MAX_PLAYERS;
}

#endif

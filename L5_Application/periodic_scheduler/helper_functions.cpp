
float decode_long(char *bytecode)
{
    float longitude = 0;
    while(bytecode && *bytecode)
    {
        longitude *= 10;
        longitude += *bytecode - '0';
    }
    return (longitude);
}

float decode_lat(char *bytecode)
{
    int     sign = 1;
    if (!bytecode)
        return 0;
    if (*bytecode == '-'){
        sign = -1;
        bytecode++;
    }
    return (sign * decode_long(bytecode));
}


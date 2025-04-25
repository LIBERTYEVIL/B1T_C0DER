
/*
    ==============================================================
                   COPYRIGHT (C) 2025, LIBERTYEVIL
    ==============================================================
*/

#include <string.h>
#include "public_funcs.h"

char ch_to_lower(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return (ch + ('a' - 'A'));
    }
    else
    {
        return ch;
    }  
}

void str_to_lowercase(char *str) 
{
    while (*str)
    {
        *str = ch_to_lower(*str);
        str++;
    }
}

char if_str_in_array(char *str, const char *arr[], int arrlen)
{
    for(int i=0; i<arrlen; i++)
    {
        if (strcmp(str, *(arr + i)) == 0)
        return 1;
    }
    return 0;
}
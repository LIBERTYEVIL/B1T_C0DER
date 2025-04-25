/*
    ==============================================================
                   COPYRIGHT (C) 2025, LIBERTYEVIL
    ==============================================================
*/

#ifndef BASE64_H
#define BASE64_H

static const char BASE64T[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const char *input);
/*
    Usage:
    char *base64_str = base64_encode("...");
    ...;
    free(base64_str);
*/

char *base64_decode(const char *input);
/*
    Usage:
    char *decode_str = base64_decode("...");
    ...;
    free(decode_str);
*/

char is_str_base64(const char *input);
/* 
    Usage:
    is_str_base64(str)
    return 0 -> false
    return 1 -> true
 */

 char howMany_b64str_hidenBits(const char *b64str);
/*
    Usage:
    char numOfHiddenBits = howMany_b64str_hidenBits("...");
    return 0/2/4
*/

char findout_base64_hidenBits(const char *b64str, char bits_num_2or4);
/*
    usage:
    char b64HiddenBits = findout_base64_hidenBits("...", 2/4);
    then do bitewise operation to b64HiddenBits to get the hidden info.
*/

#endif // BASE64_H
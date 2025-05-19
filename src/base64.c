/*
    ==============================================================
                   COPYRIGHT (C) 2025, LIBERTYEVIL
    ==============================================================
*/

#include "base64.h"
#include <stdlib.h>
#include <stdio.h>////////////////////
#include <string.h>
#include <stddef.h> // for using size_t

char *base64_encode (const char *input)
{
    size_t input_len  = strlen(input);
    size_t strLp = input_len;           // == strlen(pointer)
    size_t oinput = 0;
    size_t output_len = ((input_len + 2) / 3) * 4 + 1;

    const char * pointer = input;       // 常量指针，读input中的字符
    char * output = malloc(output_len); // 为输出结果分配内存
    if (output == NULL) return NULL;    // 分配失败返回空指针
    
    while (strLp > 0)
    {
        if(strLp>=3)
        {
            /* 1. Full 3 chars to 4 base64 code, 3Bytes = 24Bits = 4Base64code */
            char char_3gp[3] = {0, 0, 0};    // 3 chars for a char-group
            char b64i_4gp[4] = {0,0,0,0};    // 4 base64 index for a group
            for (int i = 0; i < 3; i++)
            {
                char_3gp[i] = *(pointer + i);
            }

            b64i_4gp[0] = char_3gp[0] >> 2;
            b64i_4gp[1] =((char_3gp[0] & 0x03) << 4) + (char_3gp[1] >> 4);
            b64i_4gp[2] =((char_3gp[1] & 0x0f) << 2) +((char_3gp[2] & 0xc0) >> 6);
            b64i_4gp[3] =  char_3gp[2] & 0x3f;

            output[oinput++] = BASE64T[b64i_4gp[0]];
            output[oinput++] = BASE64T[b64i_4gp[1]];
            output[oinput++] = BASE64T[b64i_4gp[2]];
            output[oinput++] = BASE64T[b64i_4gp[3]];

            pointer += 3;
            strLp   -= 3;
        }
        else if(strLp==2)
        {
            /* 2. Two chars to 3 base64 code, the last base64-code's low 2 bits are ZERO. */
            char char_2gp[2] = {0 , 0};    // 2 chars for a char-group
            char b64i_3gp[3] = {0,0,0};    // 3 base64 index for a group
            for (int i = 0; i < 2; i++)
            {
                char_2gp[i] = *(pointer + i);
            }

            b64i_3gp[0] = ((char_2gp[0] & 0xfc) >> 2);
            b64i_3gp[1] = ((char_2gp[0] & 0x03) << 4) + ((char_2gp[1] & 0xf0) >> 4);
            b64i_3gp[2] = ((char_2gp[1] & 0x0f) << 2) + 0;    // Two bits 0 may hide something. Fill in 1 "=".

            output[oinput++] = BASE64T[b64i_3gp[0]];
            output[oinput++] = BASE64T[b64i_3gp[1]];
            output[oinput++] = BASE64T[b64i_3gp[2]];

            output[oinput++] = '=';
            pointer += 2;
            strLp   -= 2;
        }
        else if (strLp==1)
        {
            /* 3. A char to 2 base64 code, the last base64-code's low 4 bits are ZERO. */
            char char_single = *pointer;                    // A single char
            char b64i_2gp[2] = {0, 0};                      // 2 base64 index for a group
            b64i_2gp[0] = ((char_single & 0xfc) >> 2);
            b64i_2gp[1] = ((char_single & 0x03) << 4) + 0;  // Four bits 0 may hide something. Fill in 2 "="s.

            output[oinput++] = BASE64T[b64i_2gp[0]];
            output[oinput++] = BASE64T[b64i_2gp[1]];

            output[oinput++] = '=';
            output[oinput++] = '=';
            pointer += 1;
            strLp   -= 1;
        }
    }
    output[oinput++] = '\0';
    return output;
}

char *base64_decode (const char *input) 
{
    /* The 8Bits ASCII code in BASE64TABLE to 0-63 BASE64 decoded integer (only use low 6bits). */
    const char *pointer = input;           // Movable pointer for reading chars in the input.
                                            // 256个元素为了使BASE64T[]中的ASCII字符都能作为解码表的索引，
    int DECODE_BYTES_6Bits[256] = {-1};      // 使用int类型以便在解码表中存储额外的信息，如输入非BASE64字符作为索引返回的值
                                            // 64 == 0100 0000 B == 0x40
    for (int i = 0; i < 64; i++) { DECODE_BYTES_6Bits[(unsigned char)BASE64T[i]] = i; }

    size_t input_len  = strlen(input);
    size_t inputLp = input_len;       // == strlen(pointer)
    size_t oinput = 0;
    size_t output_len = ( input[input_len-2]=='=' ? input_len/4 - 2 : (input[input_len-1]=='=' ? input_len/4 - 1 : input_len/4 ));
    
    char *output = malloc(output_len); // 为输出结果分配内存
    if (output == NULL) {return NULL;}    // 分配失败返回空指针

    /* BASE64 encoded string must be multiple of 4 */
    while (inputLp > 0)
    {
        char b64ch_char4gp[4] = {0};    // The BASE64 code read from the input.

        /* 1. There must be 4 chars per group. */
        if (inputLp > 4)
        {
            char decdd_char3gp[3] = {0};    // The characters decoded from BASE64 code.
            for(int i=0; i<4; i++) 
            {
                /* 遇到非BASE64编码表中的字符直接返回空指针退出函数 */
                if (DECODE_BYTES_6Bits[*(pointer + i)] == -1  && (*(pointer + i) != '='))
                {
                    return NULL;    
                }
                else
                {
                    b64ch_char4gp[i] = *(pointer + i); 
                }
            }
            inputLp -= 4;
            pointer += 4;
            decdd_char3gp[0] = ((DECODE_BYTES_6Bits[b64ch_char4gp[0]] & 0x3f) << 2) + ((DECODE_BYTES_6Bits[b64ch_char4gp[1]] & 0x30) >> 4);
            decdd_char3gp[1] = ((DECODE_BYTES_6Bits[b64ch_char4gp[1]] & 0x0f) << 4) + ((DECODE_BYTES_6Bits[b64ch_char4gp[2]] & 0x3c) >> 2);
            decdd_char3gp[2] = ((DECODE_BYTES_6Bits[b64ch_char4gp[2]] & 0x03) << 6) + ( DECODE_BYTES_6Bits[b64ch_char4gp[3]] & 0x3f);
            output[oinput++] = decdd_char3gp[0];
            output[oinput++] = decdd_char3gp[1];
            output[oinput++] = decdd_char3gp[2];
        }
        /* 2. Or the last 4 chars. */
        else if (inputLp == 4)
        {
            for(int i=0; i<4; i++) 
            {
                if ((DECODE_BYTES_6Bits[*(pointer + i)] == -1) && (*(pointer + i) != '=')) return NULL; else b64ch_char4gp[i] = *(pointer + i); 
            }
            inputLp -= 4;
            pointer += 4;
            /* 1> "==" */
            if (b64ch_char4gp[2] == '=' && b64ch_char4gp[3] == '=')
            {
                char decdd_char = ((DECODE_BYTES_6Bits[b64ch_char4gp[0]] & 0x3f) << 2) + ((DECODE_BYTES_6Bits[b64ch_char4gp[1]] & 0x30) >> 4);
                output[oinput++] = decdd_char;
            }
            /* 2> '=' */
            else if(b64ch_char4gp[3] == '=')
            {
                char decdd_char2gp[2] = {0};
                decdd_char2gp[0] = ((DECODE_BYTES_6Bits[b64ch_char4gp[0]] & 0x3f) << 2) + ((DECODE_BYTES_6Bits[b64ch_char4gp[1]] & 0x30) >> 4);
                decdd_char2gp[1] = ((DECODE_BYTES_6Bits[b64ch_char4gp[1]] & 0x0f) << 4) + ((DECODE_BYTES_6Bits[b64ch_char4gp[2]] & 0x3c) >> 2);
                output[oinput++] = decdd_char2gp[0];
                output[oinput++] = decdd_char2gp[1];
            }
            /* 3> 4 Chars are both not '=' */
            else
            {
                char decdd_char3gp[3] = {0};
                decdd_char3gp[0] = ((DECODE_BYTES_6Bits[b64ch_char4gp[0]] & 0x3f) << 2) + ((DECODE_BYTES_6Bits[b64ch_char4gp[1]] & 0x30) >> 4);
                decdd_char3gp[1] = ((DECODE_BYTES_6Bits[b64ch_char4gp[1]] & 0x0f) << 4) + ((DECODE_BYTES_6Bits[b64ch_char4gp[2]] & 0x3c) >> 2);
                decdd_char3gp[2] = ((DECODE_BYTES_6Bits[b64ch_char4gp[2]] & 0x03) << 6) + ( DECODE_BYTES_6Bits[b64ch_char4gp[3]] & 0x3f);
                output[oinput++] = decdd_char3gp[0];
                output[oinput++] = decdd_char3gp[1];
                output[oinput++] = decdd_char3gp[2];
            }
        }
        else return NULL;
    }
    output[oinput] = '\0';
    return output;
}

char is_str_base64 (const char *input) 
{
    const char *pointer = input;
    int DECODE_BYTES_6Bits[256] = {-1};
    size_t input_len = strlen(input);

    if (input_len % 4 != 0) 
    {
        return 0;
    }
    for (int i = 0; i < 64; i++) { DECODE_BYTES_6Bits[(unsigned char)BASE64T[i]] = i; }
    
    for(int i=0; i<input_len; i++) 
    {
        if ((DECODE_BYTES_6Bits[*(pointer + i)] == -1) && (*(pointer + i) != '='))
        {
            return 0;    
        }
    }
    return 1;
}

char howMany_b64str_hidenBits (const char *b64str)
{
    if(is_str_base64(b64str) == 0) return -1;
    const char *pointer = b64str + strlen(b64str);
    if ((*(pointer - 2) == '=') && (*(pointer - 1) == '='))
    {
        return 4;
    }
    else if(*(pointer - 1) == '=')
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

char findout_base64_hidenBits (const char *b64str, char bits_num_2or4)
{
    if(is_str_base64(b64str) == 0) return -1;
    const char *pointer = b64str + strlen(b64str);
    if (bits_num_2or4 == 2)
    {
        return (*(pointer -3 )) & 0x03;     // 2 bits
    }
    else if(bits_num_2or4 == 4)
    {
        return (*(pointer -2 )) & 0x0f;     // 4 bits
    }
    else
    {
        return -1;
    }
}

char* merge_bits_to_str (hd_b64_bits* hd_arr, size_t num, int* ret_str_len)
{
    size_t total_bit_num  = 0;  // 从hd_arr拿到的需要还原的数据的总位数
    size_t total_char_num = 0;  // 需要还原的数据还原出来总共有多少字节
    size_t remain_bit_num = 0;  // 万一隐藏的信息有未凑成完整字节的情况
    int temp_bit_num      = 0;  // 临时变量，到达8时将八位数据拼成1字节
    
    size_t now_char_index = 0;
    char a_char     = 0;    // 存储拼凑完成的一个字节
    char temp2bits  = 0;    // 如果一个四位的数据被截成两半存在不同字节中，临时存储其中一半

    /* 创建用于存放最终结果的内存空间 */
    for (size_t n = 0; n < num; n++) total_bit_num += hd_arr[n].num;
    total_char_num = total_bit_num / 8;
    remain_bit_num = total_bit_num % 8;
    if (remain_bit_num > 0) total_char_num ++;
    char* ret_str = malloc(sizeof(char) * total_char_num);
    if (ret_str == NULL) { printf("ERROR: 内存分配失败\n"); return NULL; }

    /* 遍历每一个数据，从它是2位，还是4位判断拼凑一个字节还需多少位，依次处理 */
    for (size_t i=0; i<num; i++)
    {
        if (hd_arr[i].num == 0)
        {
            continue;
        }
        else if(temp2bits != 0)
        {
            a_char = (temp2bits & 0x03) << 6;
            temp_bit_num = 2;
            temp2bits = 0;
        }
        else
        {
            switch (temp_bit_num)
            {
                case 0:
                    if(hd_arr[i].num == 2)
                    {
                        a_char = (hd_arr[i].bits & 0x03) << 6;
                        temp_bit_num = 2;
                    }
                    else if(hd_arr[i].num == 4)
                    {
                        a_char = (hd_arr[i].bits & 0x0f) << 4;
                        temp_bit_num = 4;
                    }
                    break;
                case 2:
                    if(hd_arr[i].num == 2)
                    {
                        a_char += (hd_arr[i].bits & 0x03) << 4;
                        temp_bit_num = 4;
                    }
                    else if(hd_arr[i].num == 4)
                    {
                        a_char += (hd_arr[i].bits & 0x0f) << 2;
                        temp_bit_num = 6;
                    }
                    break;
                case 4:
                    if(hd_arr[i].num == 2)
                    {
                        a_char += (hd_arr[i].bits & 0x03) << 2;
                        temp_bit_num = 6;
                    }
                    else if(hd_arr[i].num == 4)
                    {
                        a_char += hd_arr[i].bits & 0x0f;
                        ret_str[now_char_index++] = a_char;
                        a_char = 0;
                        temp_bit_num = 0;
                    }
                    break;
                case 6:
                    if(hd_arr[i].num == 2)
                    {
                        a_char += hd_arr[i].bits & 0x03;
                        ret_str[now_char_index++] = a_char;
                        a_char = 0;
                        temp_bit_num = 0;
                    }
                    else if(hd_arr[i].num == 4)
                    {
                        temp2bits = hd_arr[i].bits & 0x03;  // 0011
                        a_char   += hd_arr[i].bits & 0x0c;  // 1100
                        ret_str[now_char_index++] = a_char;
                        a_char = 0;
                        temp_bit_num = 0;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    ret_str[now_char_index++] = '\0';
    /* 循环结束后得到结果，总计now_char_index-1个有效字符 */
    return ret_str;
}
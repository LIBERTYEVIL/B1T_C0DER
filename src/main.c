/*
    ==============================================================
                   COPYRIGHT (C) 2025, LIBERTYEVIL
    ==============================================================
*/

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"
#include "public_funcs.h"
#include "main.h"

void print_LOGO() 
{
    printf("\n\n");
    for (int i = 0; i<7; i++)
    {
        printf("    %s", LOGOSTR[i]);
    }
    printf("\n");
}

void print_usage() 
{
    printf("·使用方法:\n");
    printf("  b1tc -t CODETYPE -c COMMAND [-i input_text / -if input_file] [-o output_file]\n");
    printf("·程序参数:\n");
    printf("  -t    编码类型, 支持以下编码类型:\n");
    printf("     · base64\n");
    printf("  -c    执行的操作, 编码、解码或找出隐藏信息(需文件输入):\n");
    printf("     · encode(e)\n");
    printf("     · decode(d)\n");
    printf("     · find_hidden(fh)\n");
    printf("  -i    直接输入的文本。\n");
    printf("  -if   输入的文本文件。\n");
    printf("  -o    输出到文本文件。\n");
    printf("  -dbg  输出调试信息。\n");
    printf("·示例:\n");
    printf("  b1tc -t base64 -c encode -i \"Hello, World!\"\n");
    printf("  b1tc -t base64 -c decode -if input.txt -o output.txt\n");
    printf("!!! BETA_VERSION, 文件输入模式尚未完成开发 !!!\n");   // ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
}

oprt_info *initialize_infos_struct (oprt_info * infos)
{
    infos -> codetype   = -1        ;
    infos -> command    = -1        ;
    infos -> Iput_mode  = -1        ;
    infos -> Oput_mode  = TEXTMODE  ;
    infos -> i_text     = NULL      ;
    infos -> i_file     = NULL      ;
    infos -> o_file     = NULL      ;
    return infos;
}

void print_infos(oprt_info * infos)
{
    putchar('\n');
    printf("DEBUG INFOS ARE FOLLOWING:\n");
    printf("CODETYPE: %d\n", infos -> codetype);
    printf("COMMAND : %d\n", infos -> command);
    printf("I_MODE  : %d\n", infos -> Iput_mode);
    printf("O_MODE  : %d\n", infos -> Oput_mode);
    printf("I_TEXT  : %s\n", infos -> i_text);
    printf("I_FILE  : %s\n", infos -> i_file);
    printf("O_FILE  : %s\n", infos -> o_file);
    putchar('\n');
}

void execute_oprt(oprt_info * infos)
{
    if(infos->Iput_mode == -1 || infos->codetype == -1 || infos->command == -1)
    {
        printf("未正确输入必要的参数，请修正参数后重新尝试。\n");
        return;
    }
    else if (infos->Iput_mode == TEXTMODE && infos->i_text == NULL)
    {
        printf("输入了空的字符串，请修正参数后重新尝试。\n");
        return;
    }
    else if (infos->Iput_mode == TEXTMODE)  // TEXTMODE ------------------------------
    {
        if (infos -> command == CMD_DECODE)
        {
            char *output = NULL;
            if (infos -> codetype == TYPE_BASE64)
            {
                if(is_str_base64(infos->i_text) == 0)
                {
                    printf("输入的字符串与指定编码类型不符，请修正参数后重新尝试。\n");
                    return;
                }
                else
                {
                    output = base64_decode(infos->i_text);
                    if(output==NULL)
                    {
                        printf("内存分配失败或程序出现问题。\n");
                    }
                    printf("RESULT: %s", output);
                    free(output);
                    output = NULL;
                    return;
                }
            }
        }
        else if (infos -> command == CMD_ENCODE)
        {
            char *output = NULL;
            if (infos -> codetype == TYPE_BASE64)
            {
                output = base64_encode(infos->i_text);
                printf("RESULT: %s", output);
                free(output);
                output = NULL;
                return;
            }
        }
        else if (infos -> command == CMD_FIND_HIDDEN)
        {
            if(is_str_base64(infos->i_text) == 0)
            {
                printf("输入的字符串与指定编码类型不符，请修正参数后重新尝试。\n");
                return;
            }
            else 
            {
                int res_int = 0;
                switch(howMany_b64str_hidenBits(infos->i_text))
                {
                    case 0:
                        printf("RESULT: No hidden bits in this string.\n");
                        break;
                    case 2:
                        res_int = findout_base64_hidenBits(infos->i_text, 2);
                        printf("RESULT: %d, 2 bits.", res_int);
                        break;
                    case 4:
                        res_int = findout_base64_hidenBits(infos->i_text, 4);
                        printf("RESULT: %d, 4 bits.", res_int);
                        break;
                    default:
                        printf("ERROR: howMany_b64str_hidenBits(infos->i_text) returned -1.\n");
                        break;
                } 
            }
        }
    }
    else if (infos->Iput_mode == FILEMODE)  // FILEMODE ------------------------------
    {
        printf("The file mode input is developing......\n");
    }
}

int main(int argc, char *argv[])
{
    oprt_info *infos;
    char debug_flag = 0;
    /* For Windows */
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Output UTF-8
    SetConsoleCP(65001);       // Input UTF-8
    #endif

    infos = malloc(sizeof(oprt_info));
    if (infos == NULL) { printf("\nError: 内存分配失败\n"); return 0; }
    infos = initialize_infos_struct(infos);

    /* 参数备份到缓存区再统一为小写 */
    // twice malloc, first for pointers, second for strings
    char **args = malloc(sizeof(char*) * argc);
    if (args == NULL) { printf("\nError: 内存分配失败\n"); return 0; }
    for (int c=0; c<argc; c++)
    {
        *(args+c) = malloc (strlen(argv[c]) + 1);
        if(*(args+c) == NULL) { printf("\nError: 内存分配失败\n"); return 0; }
        strcpy (*(args + c), argv[c]);
        // -i 和 -if的参数字符串不转小写，在录入info结构体时输入原始参数
        str_to_lowercase (*(args + c));
    }
    
    /* 参数解析 */
    for (int arg_i=1; arg_i<argc; arg_i++)
    {
        if (strcmp(args[arg_i], "-t") == 0)
        {
            if (strcmp(args[arg_i + 1], "base64") == 0)
            {
                infos -> codetype = TYPE_BASE64;
            }
        }
        else if (strcmp(args[arg_i], "-c") == 0)
        {
            if (if_str_in_array(args[arg_i + 1], cmd_enc, C_EN_LEN) == 1)
            {
                infos -> command = CMD_ENCODE;
            }
            else if (if_str_in_array(args[arg_i + 1], cmd_dec, C_DE_LEN) == 1)
            {
                infos -> command = CMD_DECODE;
            }
            else if (if_str_in_array(args[arg_i + 1], cmd_fdh, C_FH_LEN) == 1)
            {
                infos -> command = CMD_FIND_HIDDEN;
            }
        }
        // 对于-i、-if、-o参数后面的字符串，需要新开辟内存空间来存储它们
        else if (strcmp(args[arg_i], "-i") == 0)
        {
            if (infos->Iput_mode != -1)
            {
                printf("\nError: 不能同时使用-i和-if参数。\n");
                return 0;
            }
            infos -> Iput_mode = TEXTMODE;
            infos -> i_text = malloc (strlen(argv[arg_i+1]) + 1);
            if (infos->i_text == NULL) { printf("\nError: 内存分配失败\n"); return 0; }
            if(if_str_in_array(args[arg_i + 1], arg_list, ARGL_LEN) != 0)
            {
                infos -> i_text = NULL; // 如果未输入-i参数的值则i_text设为空字符串
            }
            else 
            {
                strcpy (infos->i_text, argv[arg_i + 1]);
            }
        }
        else if (strcmp(args[arg_i], "-if") == 0)
        {
            if (infos->Iput_mode != -1)
            {
                printf("\nError: 不能同时使用-i和-if参数。\n");
                return 0;
            }
            infos -> Iput_mode = FILEMODE;
            infos -> i_file = malloc (strlen(argv[arg_i+1]) + 1);
            if (infos -> i_file == NULL) { printf("\nError: 内存分配失败\n"); return 0; }
            strcpy (infos -> i_file, argv[arg_i + 1]);
        }
        else if (strcmp(args[arg_i], "-o") == 0)
        {
            infos -> Oput_mode = FILEMODE;
            infos -> o_file = malloc (strlen(argv[arg_i+1]) + 1);
            if (infos->o_file == NULL) { printf("\nError: 内存分配失败\n"); return 0; }
            strcpy (infos->i_file, argv[arg_i + 1]);
        }
        else if (strcmp(args[arg_i], "-dbg") == 0)
        {
            debug_flag = 1;
        }
    }

    /* Release the memory of args and things in args */
    /* Then make the pointers point at NULL */
    for (int c=0; c<argc; c++)
    {
        free(*(args+c));
        *(args+c) = NULL;
    }
    free(args);
    args = NULL;

    /* Now the program got operation infos, knowing what to do */
    if(argc == 1)
    {
        print_LOGO();
        print_usage();
    }
    else 
    {
        if (debug_flag !=0 ) { print_infos(infos); }
        execute_oprt(infos);
    }
    return 0;
}


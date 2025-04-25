
/*
    ==============================================================
                   COPYRIGHT (C) 2025, LIBERTYEVIL
    ==============================================================
*/

#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>

/* Defined Types */
enum CODETYPES {
    TYPE_BASE64 = 0
};
enum COMMANDS {
    CMD_ENCODE = 0,
    CMD_DECODE, 
    CMD_FIND_HIDDEN
};
enum IO_MODE{
    TEXTMODE = 0,
    FILEMODE
};
typedef struct oprt
{
    int codetype    ;
    int command     ;
    int Iput_mode   ;
    int Oput_mode   ;
    char *i_text    ;
    char *i_file    ;
    char *o_file    ;
} oprt_info;

/* Static Values */
static const char *cmd_enc[] ={"encode", "e", "en", "enc"};
static const char *cmd_dec[] ={"decode", "d", "de", "dec"};
static const char *cmd_fdh[] ={"find_hidden", "fh", "fd", "fdh"};
static const char *arg_list[] ={"-t", "-c", "-i", "-if", "-o", "-dbg"};

/* Macros */
#define C_EN_LEN 4
#define C_DE_LEN 4
#define C_FH_LEN 4
#define ARGL_LEN 6

/* Functions */
void print_LOGO();
void print_usage();
oprt_info *initialize_infos_struct (oprt_info * infos);
void print_infos(oprt_info * infos);


/* Logo */
static const char *(LOGOSTR []) = {
    "██████╗  ██╗████████╗ ██████╗ ██████╗ ██████╗ ███████╗██████╗ \n",
    "██╔══██╗███║╚══██╔══╝██╔════╝██╔═████╗██╔══██╗██╔════╝██╔══██╗\n",
    "██████╔╝╚██║   ██║   ██║     ██║██╔██║██║  ██║█████╗  ██████╔╝\n",
    "██╔══██╗ ██║   ██║   ██║     ████╔╝██║██║  ██║██╔══╝  ██╔══██╗\n",
    "██████╔╝ ██║   ██║   ╚██████╗╚██████╔╝██████╔╝███████╗██║  ██║\n",
    "╚═════╝  ╚═╝   ╚═╝    ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝\n",
    "-------------- COPYRIGHT (C) 2025, LIBERTYEVIL ---------------\n"
};

#endif
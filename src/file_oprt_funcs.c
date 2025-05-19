
/*
    ==============================================================
                   COPYRIGHT (C) 2025, LIBERTYEVIL
    ==============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "file_oprt_funcs.h"

/* Get the number of characters per line */
size_t read_char_num_inline (FILE* file)
{
    int ch_now = 0;
    size_t ch_num = 0;
    
    do {
        ch_now = fgetc(file);
        ch_num ++;
    } while (ch_now!='\n'&&ch_now!=EOF);
    return ch_num-1;
}

/* Get number of lines */
size_t read_line_num_infile (FILE* file)
{
    size_t line_num = 0;
    int ch_now = 0;
    do {
        do {
            ch_now = fgetc(file);
        } while (ch_now != (int)'\n' && ch_now != EOF);
        line_num ++;
    } while (ch_now != EOF);
    return line_num;
}

/* Get two numbers into a structure and return as a pointer */
// This function will use malloc() to create a file_info structure
file_info* get_file_info (FILE* file)
{
    file_info *f_info = (file_info*)malloc(sizeof(file_info));
    size_t file_line_num = read_line_num_infile(file);
    rewind(file);
    f_info->line_num = file_line_num;
    f_info->char_num_list = (size_t*)malloc(sizeof(size_t)*file_line_num);
    if (f_info==NULL || f_info->char_num_list==NULL) 
    {
        free(f_info->char_num_list);
        free(f_info);
        return NULL;
    }
    for (size_t i = 0; i < file_line_num; i++)
    {
        *(f_info->char_num_list + i) = read_char_num_inline(file);
    }
    rewind(file);
    return f_info;
}

/* Create a line-buffer and read a line into it */
// Needs to be released
// The current_num starts from 0
// If there is an empty line, you need add 2 to file_pos before read next line
line_struct line_buffer (FILE* file, file_info* finfo, size_t current_num)
{
    line_struct line_ret;
    long file_pos = ftell(file);
    char* lbuffer = malloc(finfo->char_num_list[current_num]+2);
    fgets(lbuffer, finfo->char_num_list[current_num]+2, file);
    if (lbuffer[finfo->char_num_list[current_num]] == '\n') 
    {
        lbuffer[finfo->char_num_list[current_num]] = '\0';
    }
    file_pos = ftell(file);
    line_ret.f_pos = file_pos;
    line_ret.buf_add = lbuffer;
    return line_ret;
}
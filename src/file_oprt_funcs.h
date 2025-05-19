
/*
    ==============================================================
                   COPYRIGHT (C) 2025, LIBERTYEVIL
    ==============================================================
*/

#include <stddef.h> // For using size_t

/* TYPE-DECLARATION */
typedef struct file_info
{
    size_t line_num;
    size_t *char_num_list;
} file_info;

typedef struct line_struct
{
    long f_pos;
    char* buf_add;
} line_struct;


/* FUNCTION-DECLARATION */
size_t read_char_num_inline (FILE* file);   // No rewind
size_t read_line_num_infile (FILE* file);   // No rewind
file_info* get_file_info    (FILE* file);   // rewind, but create a file_info
line_struct line_buffer (FILE* file, file_info* finfo, size_t current_num);


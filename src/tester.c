#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_oprt_funcs.h"

int main()
{
    FILE* txtfile = fopen("readtest.txt","r");
    if (txtfile == NULL)
    {
        printf("ERROR_FILE_OPEN\n");
        return 0;
    }
    file_info* finfo = get_file_info(txtfile);
    printf("line_num: %d\n", finfo->line_num);
    for (size_t i = 0; i < finfo->line_num; i++)
    {
        printf("char_num_%d: %d\n", i, finfo->char_num_list[i]);
    }
    line_struct linfo = {0, NULL};
    for (size_t i = 0; i < finfo->line_num; i++)
    {
        if (finfo->char_num_list[i]==0)
        {
            printf("Empty line %d.\n", i);
            linfo.f_pos += 2;
            fseek(txtfile, linfo.f_pos, SEEK_SET);
            continue;
        }
        printf("%d_line:", i);
        linfo = line_buffer(txtfile, finfo, i);
        printf("%s\n", linfo.buf_add);
        free(linfo.buf_add);
    }
}
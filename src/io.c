#include "../include/io.h"
#include "../include/util.h"



char* io_read_text (const char* file_path)
{
    int line1, line2;

    char* buffer = 0;
    unsigned long length;
    FILE* f = fopen (file_path, "rb"); 
    line1 = __LINE__ - 1;

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);

        buffer = malloc (length);
        line2 = __LINE__ - 1;
        if (buffer)
        {
            fread (buffer, 1, length, f);
        }
        else
        {
            ut_errmsg (
                "Can't alloc memory for the buffer.", 
                __FILE__, line2, 1
            );
        }

        fclose (f);
    }
    else 
    {
        char msgbuf[1024];
        sprintf (msgbuf, "Can't open file %s.", file_path);
        ut_errmsg (
            msgbuf, 
            __FILE__, line1, 1
        );
    }

    return buffer;
}



char** io_cut_lines (char* text, int* num_lines)
{
    int i, j, mark, line;
    int currline = 0, count = 0;
    size_t textsize = strlen(text);

    // First count the # of '\n' characters
    for (i = 0; i < textsize; i++)
    {
        if (text[i] == '\n' || i == textsize-1)
            count += 1;
    }

    // Now allocate memory for the pointers
    char** lines = 0;
    lines = (char**) malloc (count * sizeof(char*));
    line  = __LINE__ - 1;

    if (!lines)
    {
        ut_errmsg (
            "Can't allocate memory for the lines pointers.",
            __FILE__, line, 1
        );
    }

    // Finally, cut the lines out of the main string,
    // by marking the initial position and findind an '\n' or end of string
    mark = 0;
    for (i = 0; i < textsize; i++) // O(2n) => O(n) complexity
    {
        // Building a new line
        if (text[i] == '\n' || i == textsize-1)
        {
            // Allocate memory for the new string
            int len = i-mark;
            lines[currline] = 0;
            lines[currline] = (char*) malloc (len+1); // Last cell for '\0'
            line = __LINE__ - 1;

            if (!lines[currline])
            {
                char msgbuf[128];
                sprintf(msgbuf, "Can't allocate memory for line %d.", currline);
                ut_errmsg (
                    msgbuf,
                    __FILE__, line, 1
                );
            }

            // Build the new string
            for (j = 0; j < len; j++)
                lines[currline][j] = text[mark+j];
            lines[currline][len] = '\0';

            mark = i + 1;
            currline += 1;
        }
    }

    // Return the # of lines
    *num_lines = count;

    // Return the lines
    return lines;
}



char** io_get_lines (const char* file_path, int* num_lines)
{
    char*  text  = io_read_text(file_path);
    char** lines = io_cut_lines(text, num_lines);
    free(text);
    
    return  lines;
}
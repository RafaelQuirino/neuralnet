#ifdef __cplusplus
extern "C" {
#endif
#ifndef _TEXT_H_
#define _TEXT_H_



typedef char*          text_t;
typedef unsigned char* utext_t;

utext_t* txt_get_ulines   (const char* fname, int* num_lines);
void     txt_print_ulines (utext_t* ulines, int num_lines);

void txt_trim                  (char* str);
void txt_uppercase             (char* str);
void txt_lowercase             (char* str);
void txt_remove_accents        (char* str);

void txt_remove_multiple_chars (char* str, char c);
void txt_replace_chars         (char* str, char src, char dst);



#endif // _TEXT_H_
#ifdef __cplusplus
}
#endif
/* String functions */
/*//////////////////*/

#ifndef BL_STRING_H
#define BL_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

void bl_str_swap(char **s1, char **s2);
void bl_str_rstrip(char *s);
char *bl_str_lstrip(char *s);
size_t bl_str_chrnum(char *str, char *chars);
void bl_str_split_lazy(char *str, char split_char);
char **bl_str_split_2pass(char *str, char split_char);
char **bl_str_split_dynamic(char *str, char split_char);

#endif

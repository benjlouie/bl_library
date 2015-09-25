/* BL_STRING Functions */
/*/////////////////////*/

#include "bl_string.h"


void bl_str_swap(char **s1, char **s2)
{
	char *tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

void bl_str_rstrip(char *s)
{
	int len = strlen(s) - 1;
	while(s[len] == 0x20 ||
			s[len] == '\t' ||
			s[len] == '\n' ||
			s[len] == '\f' ||
			s[len] == '\r' ||
			s[len] == '\v') {
				s[len--] = '\0';
			}
}

char *bl_str_lstrip(char *s)
{
	int len = strlen(s) - 1;
	int i = 0;
	while(i < len && isspace(s[i]) != 0) {
		i++;
	}
	return s + i;
}

/* make more efficient with array that holds the chars to count (0-255) turned on and then checked */
size_t bl_str_chrnum(char *str, char *chars)
{
    int size = strlen(chars);
    size_t count = 0;
    int i = 0;
    while(*str != '\0') {
        for(i = 0; i < size; i++) {
            if(*str == *(chars + i)) {
                count++;
                break;
            }
        }
        str++;
    }
    return count;
}

void bl_str_split_lazy(char *str, char split_char)
{
    while(*str != '\0') {
        if(*str == split_char) {
            *str = '\0';
        }
        str++;
    }
}

char **bl_str_split_2pass(char *str, char split_char)
{
    char **ret = NULL;
    char *tmp = str;
    int count = 0;
    int i = 0;
    
    if(*tmp != split_char) {
        count++;
        tmp++;
    }
    while(*tmp != '\0') {
        if(*tmp == split_char) {
            while(*(tmp + 1) == split_char)
                tmp++;
            if(*(tmp + 1) != '\0')
                count++;
        }
        tmp++;
    }
    
    ret = malloc(sizeof(char *) * (count + 1)); /* additional for null ptr */
    tmp = str;
    if(*tmp != split_char) {
        ret[i] = tmp;
        i++;
        tmp++;
    }
    while(*tmp != '\0') {
        if(*tmp == split_char ) {
            *tmp = '\0';
            while(*(tmp + 1) == split_char)
                tmp++;
            if(*(tmp + 1) != '\0') {
                ret[i] = tmp + 1;
                i++;
            }
        }
        tmp++;
    }
    
    ret[i] = NULL;
    return ret;
}

char **bl_str_split_dynamic(char *str, char split_char)
{
    char **ret = NULL;
    char *tmp = str;
    int i = 0, ret_size = 0;
    
    ret = malloc(sizeof(char *) * 2);
    ret_size = 2;
    tmp = str;
    if(*tmp != split_char) {
        ret[i] = tmp;
        i++;
        tmp++;
    }
    while(*tmp != '\0') {
        if(*tmp == split_char && *(tmp + 1) != '\0') {
            *tmp = '\0';
            while(*(tmp + 1) == split_char)
                tmp++;
            if(*(tmp + 1) != '\0') {
                ret[i] = tmp + 1;
                i++;
            }
            if(i >= ret_size) {
                ret = realloc(ret, sizeof(char *) * (ret_size *= 2));
            }
        }
        tmp++;
    }

    if(ret_size != (i + 1))
        ret = realloc(ret, sizeof(char *) * (i + 1));
    ret[i] = NULL;
    return ret;
}


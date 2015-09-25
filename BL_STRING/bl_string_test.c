#include <stdio.h>
#include "bl_string.h"

int main(void)
{
	char str[100] = "well this is it and all the ones";
	char **test = NULL;
	char **tmp = NULL;
	
	test = bl_str_split_2pass(str, ' ');
	tmp = test;
	while(*tmp != NULL) {
	    printf("%s\n", *tmp);
	    tmp++;
	}
	free(test);
	
	return 0;
}

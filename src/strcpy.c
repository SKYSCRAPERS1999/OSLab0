#include <am.h>
char* strcpy(char* s1, const char* s2){
	char *s = s1;
	while ((*s1++ = *s2++) != '\0');
	return s;
}

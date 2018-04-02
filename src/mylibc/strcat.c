#include <am.h>
char* strcat(char* s1, const char* s2){
	char* s = s1;
	while (*s++);
	--s;
	while ((*s++ = *s2++) != '\0');
	return s1;
}

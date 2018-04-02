#include <am.h>
void* memset(void* s, char c, size_t n){
	char* p = (char*)s;
	while (n){
		*p++ = c;
		n--;
	}
	return s;
}

#include <am.h>
void* memset(void* s, int c, size_t n){
	char* p = (char*)s;
	while (n--){
		*p++ = c;
	}
	return s;
}

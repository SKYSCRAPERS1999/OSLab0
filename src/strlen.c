#include <am.h>
size_t strlen(const char *s){
	char *p;
	for (p = s; *p; p++);
	return p - s;
}

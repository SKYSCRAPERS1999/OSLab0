#include <am.h>
size_t strlen(const char *s){
	const char *p;
	for (p = s; *p; p++);
	return p - s;
}

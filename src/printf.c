#include <am.h>
#include <stdarg.h>

static char mp[] = "0123456789ABCDEF";
static int printf_int(unsigned int d, int base, int length, int type){
	int ret = 0, d2 = d;
	if (base == 10){
		d2 = (int)d;
		if (d2 < 0){
			d2 = -d2;
			_putc('-'), ret++; 
		}
	}
	char *ptr; char dig[64];
	ptr = &dig[63]; *ptr = '\0';
	for ( ; d2 != 0; *--ptr = mp[d2 % base], d2 /= base, ret++);
	while (length > ret && type >= 0){
		char c; 
		switch (type){
			case 0: c = '0'; break;
			case 1: c = ' '; break;
			default: c = ' ';
		}
		*--ptr = c; 
		ret++; 
	}
	/*if d = 0*/ if (*ptr == '\0') *--ptr = '0', ret++;
	while (*ptr) _putc(*ptr++);
	
	return ret;
}
int printf(const char *fmt, ...) {
    /*
    for (; *fmt; fmt++) {
      _putc(*fmt);
    }
    */
    int ret = 0;
	va_list ap; 
	const char* iter;
    unsigned int d; char c; char* s; 
    va_start(ap, fmt);
    for (iter = fmt; *iter != '\0'; ++iter){
		if (*iter != '%'){
			_putc(*iter);
		}else{
			++iter;
			char op[20]; int opn = 0;
			while (*iter!='x'&&*iter!='d'&&*iter!='c'&&*iter!='s'){
				op[opn++] = *iter;
				++iter;
			}
			int len = 0, type = -1;
			if (opn > 0){
				if (op[0] == '0') {
					type = 0;
					for (int i = opn - 1; i >= 1; i--) len = 10*len + (op[i] - '0');
				}else{
					type = 1;
					for (int i = opn - 1; i >= 0; i--) len = 10*len + (op[i] - '0');
				}
			}
			
			switch (*iter){
				case 's':
					s = va_arg(ap, char*);
					for (int i = 0; s[i] != '\0'; i++) _putc(s[i]), ret++;
					break; 
				
				case 'd':
					d = va_arg(ap, int);
					ret += printf_int(d, 10, len, type);
					break;

				case 'x':
					d = va_arg(ap, unsigned int);
					ret += printf_int(d, 16, len, type);
					break; 	
			  
				case 'c':
					c = (char)va_arg(ap, int);
					_putc(c), ret++;
					break;
			}
		}
    }
	va_end(ap);
    return 0;
}

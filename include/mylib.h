#include <stdint.h>

typedef uint8_t bool;
#define false 0
#define true 1

#define KEYDOWN_MASK 0x8000

int printf(const char*, ...);

size_t strlen(const char*i);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
char* strcmp(const char*, const char*);

void* memset(void*, int, size_t);
void srand(int);
unsigned int rand();

uint32_t uptime(); // 系统启动的时间(ms)
int read_key(); // 读取一个按键
void draw_rect(uint32_t *pixels, int x, int y, int w, int h); // 在屏幕上绘制
void draw_sync(); // 同步屏幕
int screen_width(); // 获得屏幕的宽度
int screen_height(); // 获得屏幕的高度

void start_game();

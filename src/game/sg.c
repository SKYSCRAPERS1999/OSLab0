#include <am.h>
#include <amdev.h>
#include <mylib.h>

#define bd 25

static int next_frame = 0;
static int key = _KEY_NONE;
static int w, h;
static int FPS = 150;
enum{UP, DOWN, LEFT, RIGHT};
static struct Ball{
	int x, y, dir;
}ball;

static void kbd_event(int);
static void game_progress();
static void screen_erase();
static void screen_update();
static void screen_clear();

void start_game(){
	next_frame = 0;
	w = screen_width(), h = screen_height();
	ball.x = w / 2, ball.y = h / 2, ball.dir = -1;
	while (1) {
		while (uptime() < next_frame) ; // 等待一帧的到来
		while ((key = read_key()) != _KEY_NONE) {
			kbd_event(key);         // 处理键盘事件
		}
		screen_erase();
		game_progress();          // 处理一帧游戏逻辑，更新物体的位置等
		screen_update();          // 重新绘制屏幕
		next_frame += 1000 / FPS; // 计算下一帧的时间
	}
}

static void kbd_event(int key){
	int keycode = key & 0xfff;
	bool keydown = (key & KEYDOWN_MASK) != 0;
	if (keydown == true){
		switch (keycode){
			case _KEY_UP:
				ball.dir = UP;	
				break;
			case _KEY_DOWN:
				ball.dir = DOWN;	
				break;
			case _KEY_LEFT:
				ball.dir = LEFT;	
				break;
			case _KEY_RIGHT:
				ball.dir = RIGHT;	
				break;
			default:
				break;
		}
	}
}

void game_progress(){
	switch (ball.dir){
		case UP:
			if (ball.y <= bd){
				ball.dir = DOWN; ball.y = bd + 1;
			}else{
				ball.y--;
			}
			break;
		case DOWN:
			if (ball.y >= h - bd - 1){
				ball.dir = UP; ball.y = h - bd - 2;
			}else{
				ball.y++;
			}
			break;
		case LEFT:
			if (ball.x <= bd){
				ball.dir = RIGHT; ball.x = bd + 1;
			}else{
				ball.x--;
			}
			break;
		case RIGHT:
			if (ball.x >= w - bd - 1){
				ball.dir = LEFT; ball.x = w - bd - 2;
			}else{
				ball.x++;
			}
			break;
		default:
			break;
	}
}

void screen_clear(){
	uint32_t a[1] = {0xffffffff};
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			draw_rect(a, i, j, 1, 1);	
		}
	}
}

void screen_erase(){
	uint32_t a[bd * bd + 11];
	memset(a, 0, sizeof(a));
	draw_rect(a, ball.x, ball.y, bd, bd);	
}

void screen_update(){
	uint32_t a[bd * bd + 11];
	memset(a, 0x77, sizeof(a));
	draw_rect(a, ball.x, ball.y, bd, bd);	
}

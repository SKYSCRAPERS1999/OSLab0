#include <am.h>
#include <amdev.h>
#include <stdarg.h>

#define KEYDOWN_MASK 0x8000

static _Device* get_dev(int name){
	for (int n = 1; ; n++){
		_Device *dev = _device(n);
		if (!dev) break;
		else if (dev->id == name){
			return dev;
		}
	}
	return NULL;
}
uint32_t uptime(){
	_UptimeReg uptime;
	_Device* dev = get_dev(_DEV_TIMER);
	dev->read(_DEVREG_TIMER_UPTIME, &uptime, sizeof(uptime));
	return uptime.lo;
}

int read_key(){
	_KbdReg key;
	_Device* dev = get_dev(_DEV_INPUT);
	dev->read(_DEVREG_INPUT_KBD, &key, sizeof(key));
	if (key.keycode == _KEY_NONE) return _KEY_NONE;
	else return (key.keycode | (key.keydown ? KEYDOWN_MASK : 0));
}

int screen_width(){
	_VideoInfoReg vdo;
	_Device* dev = get_dev(_DEV_VIDEO);
	dev->read(_DEVREG_VIDEO_INFO, &vdo, sizeof(vdo));
	return vdo.width;
}

int screen_height(){
	_VideoInfoReg vdo;
	_Device* dev = get_dev(_DEV_VIDEO);
	dev->read(_DEVREG_VIDEO_INFO, &vdo, sizeof(vdo));
	return vdo.height;
}

void draw_rect(uint32_t *pixels, int x, int y, int w, int h){	
	_FBCtlReg vdo;
	_Device* dev = get_dev(_DEV_VIDEO);
	vdo.pixels = pixels; vdo.x = x; vdo.y = y; vdo.w = w; vdo.h = h; vdo.sync = 0;
	dev->write(_DEVREG_VIDEO_FBCTL, &vdo, sizeof(vdo));
}

void draw_sync(){

}

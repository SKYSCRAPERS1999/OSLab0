#include <am.h>
#include <amdev.h>
#include <stdarg.h>

#define KEYDOWN_MASK 0x8000
uint32_t uptime(){
	_UptimeReg uptime;
	for (int n = 1; ; n++){
		_Device *dev = _device(n);
		if (!dev) break;
		else if (dev->id == _DEV_TIMER){
			dev->read(_DEVREG_TIMER_UPTIME, &uptime, sizeof(uptime));
			return uptime.lo;
		}
	}
	return 0;
}

int read_key(){
	_KbdReg key;
	for (int n = 1; ; n++){
		_Device *dev = _device(n);
		if (!dev) break;
		else if (dev->id == _DEV_INPUT){
			dev->read(_DEVREG_INPUT_KBD, &key, sizeof(key));
			if (key.keycode == _KEY_NONE) return _KEY_NONE;
			else return (key.keycode | (key.keydown ? KEYDOWN_MASK : 0));
		}
	}
	return 0;
}

#include <am.h>
#include <amdev.h>
#include <stdarg.h>

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

#include <am.h>
#include <amdev.h>

uint32_t uptime(){
	_UptimeReg uptime;
	_Device *dev = _device(_DEV_TIMER);
	dev->read(_DEVREG_TIMER_UPTIME, &uptime, sizeof(uptime));
	return uptime.lo;
}

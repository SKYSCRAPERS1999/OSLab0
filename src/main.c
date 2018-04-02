#include <am.h>
#include <amdev.h>
#include <stdarg.h>
// TODO: implement necessary libraries
void printf_int(unsigned int d, int base){
	char mp[20] = "0123456789ABCDEF";
	if (d < 0){
		d = -d;
		_putc('-'); 
	}
	char *ptr; char dig[64];
	ptr = &dig[63]; *ptr = '\0';
	for ( ; d != 0; *--ptr = mp[d % base], d /= base);
	while (*ptr++) _putc(*ptr);
}
int printf(const char *fmt, ...) {
    /*
    for (; *fmt; fmt++) {
      _putc(*fmt);
    }
    */
    
	va_list ap; 
	const char* iter;
    unsigned int d; char c; char* s; 
    va_start(ap, fmt);
    for (iter = fmt; *iter != '\0'; ++iter){
		if (*iter != '%'){
			_putc(*iter);
		}else{
			++iter;
			switch (*iter){
				case 's':
					s = va_arg(ap, char*);
					for (int i = 0; s[i] != '\0'; i++) _putc(s[i]);
					break; 
				
				case 'd':
					d = va_arg(ap, int);
					printf_int(d, 10);
					break;

				case 'x':
					d = va_arg(ap, unsigned int);
					printf_int(d, 16);
					break; 	
			  
				case 'c':
					c = (char)va_arg(ap, int);
					_putc(c);
					break;
			}
		}
    }
	va_end(ap);
    return 0;
}

static void input_test(_Device *dev);
static void timer_test(_Device *dev);
static void video_test(_Device *dev);
static void pciconf_test(_Device *dev);
static void ata_test(_Device *dev);

int main() {
  if (_ioe_init() != 0) _halt(1);
  printf("_heap = [%08x, %08x)\n", _heap.start, _heap.end);
  for (int n = 1; ; n++) {
    _Device *dev = _device(n);
    if (!dev) break;
    printf("* Device: %s\n", dev->name);
    switch (dev->id) {
      case _DEV_INPUT: input_test(dev); break;
      case _DEV_TIMER: timer_test(dev); break;
      case _DEV_VIDEO: video_test(dev); break;
      case _DEV_PCICONF: pciconf_test(dev); break;
      case _DEV_ATA0: ata_test(dev); break;
    }
    printf("\n");
  }
  return 0;
}

static void input_test(_Device *dev) {
  printf("Input device test skipped.\n");
}

static void timer_test(_Device *dev) {
  _UptimeReg uptime;
  uint32_t t0, t1;
  
  dev->read(_DEVREG_TIMER_UPTIME, &uptime, sizeof(uptime));
  t0 = uptime.lo;

  for (int volatile i = 0; i < 10000000; i ++) ;

  dev->read(_DEVREG_TIMER_UPTIME, &uptime, sizeof(uptime));
  t1 = uptime.lo;

  printf("Loop 10^7 time elapse: %d ms\n", t1 - t0);
}

static void video_test(_Device *dev) {
  _VideoInfoReg info;
  dev->read(_DEVREG_VIDEO_INFO, &info, sizeof(info));
  printf("Screen size: %d x %d\n", info.width, info.height);
  for (int x = 0; x < 100; x++)
    for (int y = 0; y < 100; y++) {
      _FBCtlReg ctl;
      uint32_t pixel = 0x006a005f;
      ctl.x = info.width / 2 - 50 + x;
      ctl.y = info.height / 2 - 50 + y;
      ctl.w = ctl.h = 1;
      ctl.sync = 1;
      ctl.pixels = &pixel;
      dev->write(_DEVREG_VIDEO_FBCTL, &ctl, sizeof(ctl));
    }
  printf("You should see a purple square on the screen.\n");
}

static uint32_t pci_conf_read(_Device *dev, uint8_t bus, uint8_t slot,
                              uint8_t func, uint8_t offset) {
  uint32_t res;
  dev->read(_DEVREG_PCICONF(bus, slot, func, offset), &res, 4);
  return res;
}

static void pciconf_test(_Device *dev) {
  for (int bus = 0; bus < 256; bus ++)
    for (int slot = 0; slot < 32; slot ++) {
      uint32_t info = pci_conf_read(dev, bus, slot, 0, 0);
      uint16_t id = info >> 16, vendor = info & 0xffff;
      if (vendor != 0xffff) {
        printf("Get device %d:%d, id %x vendor %x", bus, slot, id, vendor);
        if (id == 0x100e && vendor == 0x8086) {
          printf(" <-- This is an Intel e1000 NIC card!");
        }
        printf("\n");
      }
    }
}

static uint8_t readb(_Device *dev, uint32_t reg) {
  uint8_t res;
  dev->read(reg, &res, 1);
  return res;
}

static uint32_t readl(_Device *dev, uint32_t reg) {
  uint32_t res;
  dev->read(reg, &res, 4);
  return res;
}

static void writeb(_Device *dev, uint32_t reg, uint8_t res) {
  dev->write(reg, &res, 1);
}

#define SECTSZ 512

static void ata_test(_Device *dev) {
  int offset = 0;
  while ((readb(dev, _DEVREG_ATA_STATUS) & 0xc0) != 0x40);
  writeb(dev, _DEVREG_ATA_NSECT,  1);
  writeb(dev, _DEVREG_ATA_SECT,   offset);
  writeb(dev, _DEVREG_ATA_CYLOW,  offset >> 8);
  writeb(dev, _DEVREG_ATA_CYHIGH, offset >> 16);
  writeb(dev, _DEVREG_ATA_DRIVE,  (offset >> 24) | 0xe0);
  writeb(dev, _DEVREG_ATA_STATUS, 0x20);
  while ((readb(dev, _DEVREG_ATA_STATUS) & 0xc0) != 0x40);
  uint32_t buf[SECTSZ / sizeof(uint32_t)];
  for (int i = 0; i < SECTSZ / sizeof(uint32_t); i++) {
    buf[i] = readl(dev, _DEVREG_ATA_DATA);
  }
  printf("Reading out the MBR:\n");
  for (int i = 0; i < SECTSZ / 16 / sizeof(uint16_t); i ++) {
    for (int j = 0; j < 16; j++) {
      printf("%04x ", ((uint16_t *)buf)[i * 16 + j] & 0xffff);
    }
    printf("\n");
  }
}

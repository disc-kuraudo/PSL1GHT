#ifndef __SPU_PRINTF_H__
#define __SPU_PRINTF_H__

#include <sys/spu_event.h>


extern int _spu_call_event_va_arg(uint32_t _spup, const char *fmt, ...);

#define spu_printf(fmt, args...) \
	_spu_call_event_va_arg(EVENT_PRINTF_PORT<<EVENT_PORT_SHIFT, fmt, ##args)


#endif

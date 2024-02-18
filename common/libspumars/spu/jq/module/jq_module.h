#ifndef __MARS_JQ_MODULE_H__
#define __MARS_JQ_MODULE_H__

#include <stdint.h>
#include <unistd.h>

#include "mars/callback_types.h"
#include "mars/mutex_types.h"
#include "mars/jq_types.h"

#include "jq_internal_types.h"

struct mars_jq_module_syscalls
{
    uint32_t (*get_ticks)(void);
    struct mars_jq_context* (*get_jobQ)(void);
};


extern const struct mars_jq_module_syscalls *mars_jq_module_syscalls;


#endif /* __MARS_JQ_MODULE_H__ */

#ifndef __MARS_JQ_TYPES_H__
#define __MARS_JQ_TYPES_H__

#include <stdint.h>

#define MARS_JQ_NAME_LEN_MAX				21


struct mars_jq_id {
    uint64_t mars_context_ea;
    uint16_t workload_id;
    uint8_t name[MARS_JQ_NAME_LEN_MAX + 1];
};


#endif /* __MARS_JQ_TYPES_H__ */
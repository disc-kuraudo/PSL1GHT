#ifndef __SPU_DMA_H__
#define __SPU_DMA_H__

#include <stdint.h>
#include <spu_mfcio.h>
#include <spu_intrinsics.h>

#define MAX_DMA_BLOCK_SIZE	16384

#define spu_dma_ea2ls(ea, ls)	(void*)((uintptr_t)(ls)+((uint32_t)(ea)&15))

#ifdef NO_SPU_DMA_ASSERT
#define spu_dma_assert(cond, ...)
#else
#ifndef SPU_DMA_ASSERT_VERBOSE
#define spu_dma_assert(cond, ...)	spu_hcmpeq((cond), 0)
#else
#include <spu_printf.h>
#define spu_dma_assert(cond, fmt, ...) \
do { \
	if(!(cond)) { \
		spu_printf("%s:%u %s" fmt, __FILE__,__LINE__,"[spu_dma_assert]",##__VA_ARGS__); \
		__asm__ volatile("stopd $0,$0,$0\n"); \
	} \
} while(0)
#endif
#endif

//---------------------------------------------------------------------------------------------------------------------------------------------
// DMA assertion
//---------------------------------------------------------------------------------------------------------------------------------------------
#define spu_dma_normal_assert(ls,ea,size,tag) \
	spu_dma_assert((((uintptr_t)(ls) & 0xf) == 0)	& \
				   (((uintptr_t)(ea) & 0xf) == 0)	& \
				   (((size) & 0xf) == 0)			& \
				   ((size) <= (16<<10))				& \
				   ((tag) < 32), "ls=%#x,ea=%#llx,size=%#x,tag=%u\n", ls,ea,size,tag)

#define spu_dma_small_assert(ls,ea,size,tag) \
	spu_dma_assert((((uintptr_t)(ls) & 0xf) == ((uintptr_t)(ea) & 0xf))	& \
				   (((uintptr_t)(ls) & (size - 1)) == 0)				& \
				   (((size)==1)||((size)==2)||((size)==4)||((size)==8))	& \
				   ((tag) < 32), "ls=%#x,ea=%#llx,size=%#x,tag=%u\n", ls,ea,size,tag)

#define spu_dma_list_assert(ls,ea,la,lsize,tag) \
	spu_dma_assert((((uintptr_t)(ls) & 0xf) == 0)	& \
				   (((uintptr_t)(ea) & 0xf) == 0)	& \
				   (((uintptr_t)(la) &   7) == 0)	& \
				   (((lsize) & 7) == 0)				& \
				   ((lsize) <= (16<<10))			& \
				   ((tag) < 32), "ls=%#x,ea=%#llx,la=%#x,lsize=%#x,tag=%u\n", ls,ea,(uintptr_t)(la),lsize,tag)

#define spu_dma_atomic_assert(ls,ea) \
	spu_dma_assert((((uintptr_t)(ls) & 0x7f) == 0)	& \
				   (((uintptr_t)(ea) & 0x7f) == 0), "ls=%#x,ea=%#llx\n", ls,ea)

#define spu_dma_putqlluc_assert(ls,ea,tag) \
	spu_dma_assert((((uintptr_t)(ls) & 0x7f) == 0)	& \
				   (((uintptr_t)(ea) & 0x7f) == 0)	& \
				   ((tag) < 32), "ls=%#x,ea=%#llx,tag=%u\n", ls,ea,tag)

#define spu_dma_large_assert(ls,ea,tag) \
	spu_dma_assert((((uintptr_t)(ls) & 0xf) == 0)	& \
				   (((uintptr_t)(ea) & 0xf) == 0)	& \
				   ((tag) < 32), "ls=%#x,ea=%#llx,tag=%u\n", ls,ea,tag)



typedef mfc_list_element_t spu_dma_list_element;

//---------------------------------------------------------------------------------------------------------------------------------------------
// DMA which transfer size is a multiple of 16Bytes
//---------------------------------------------------------------------------------------------------------------------------------------------
__attribute__ ((__always_inline__))
static inline void spu_dma_put(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_normal_assert(ls, ea, size, tag);
	mfc_put((volatile void*)(uintptr_t)ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_putb(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_normal_assert(ls, ea, size, tag);
	mfc_putb((volatile void*)(uintptr_t)ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_putf(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_normal_assert(ls, ea, size, tag);
	mfc_putf((volatile void*)(uintptr_t)ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_get(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_normal_assert(ls, ea, size, tag);
	mfc_get(ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_getb(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_normal_assert(ls, ea, size, tag);
	mfc_getb(ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_getf(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_normal_assert(ls, ea, size, tag);
	mfc_getf(ls, ea, size, tag, tid, rid);
}


//---------------------------------------------------------------------------------------------------------------------------------------------
// DMA which transfer size is within 16Bytes
//---------------------------------------------------------------------------------------------------------------------------------------------
__attribute__ ((__always_inline__))
static inline void spu_dma_small_put(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_small_assert(ls, ea, size, tag);
	mfc_put((volatile void*)(uintptr_t)ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_small_putb(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_small_assert(ls, ea, size, tag);
	mfc_putb((volatile void*)(uintptr_t)ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_small_putf(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_small_assert(ls, ea, size, tag);
	mfc_putf((volatile void*)(uintptr_t)ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_small_get(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_small_assert(ls, ea, size, tag);
	mfc_get(ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_small_getb(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_small_assert(ls, ea, size, tag);
	mfc_getb(ls, ea, size, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_small_getf(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_small_assert(ls, ea, size, tag);
	mfc_getf(ls, ea, size, tag, tid, rid);
}


//---------------------------------------------------------------------------------------------------------------------------------------------
// List DMA
//---------------------------------------------------------------------------------------------------------------------------------------------
__attribute__ ((__always_inline__))
static inline void spu_dma_list_put(const void *ls, uint64_t ea, const spu_dma_list_element *list, uint32_t lsize, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_list_assert(ls, ea, list, lsize, tag);
	mfc_putl((volatile void*)(uintptr_t)ls, ea, list, lsize, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_list_putb(const void *ls, uint64_t ea, const spu_dma_list_element *list, uint32_t lsize, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_list_assert(ls, ea, list, lsize, tag);
	mfc_putlb((volatile void*)(uintptr_t)ls, ea, list, lsize, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_list_putf(const void *ls, uint64_t ea, const spu_dma_list_element *list, uint32_t lsize, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_list_assert(ls, ea, list, lsize, tag);
	mfc_putlf((volatile void*)(uintptr_t)ls, ea, list, lsize, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_list_get(void *ls, uint64_t ea, const spu_dma_list_element *list, uint32_t lsize, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_list_assert(ls, ea, list, lsize, tag);
	mfc_getl(ls, ea, list, lsize, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_list_getb(void *ls, uint64_t ea, const spu_dma_list_element *list, uint32_t lsize, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_list_assert(ls, ea, list, lsize, tag);
	mfc_getlb(ls, ea, list, lsize, tag, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_list_getf(void *ls, uint64_t ea, const spu_dma_list_element *list, uint32_t lsize, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_list_assert(ls, ea, list, lsize, tag);
	mfc_getlf(ls, ea, list, lsize, tag, tid, rid);
}


//---------------------------------------------------------------------------------------------------------------------------------------------
// Atomic DMA
//---------------------------------------------------------------------------------------------------------------------------------------------
__attribute__ ((__always_inline__))
static inline void spu_dma_getllar(void *ls, uint64_t ea, uint32_t tid, uint32_t rid)
{
	spu_dma_atomic_assert(ls, ea);
	mfc_getllar(ls, ea, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_putllc(const void *ls, uint64_t ea, uint32_t tid, uint32_t rid)
{
	spu_dma_atomic_assert(ls, ea);
	mfc_putllc((volatile void*)(uintptr_t)ls, ea, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_putlluc(const void *ls, uint64_t ea, uint32_t tid, uint32_t rid)
{
	spu_dma_atomic_assert(ls, ea);
	mfc_putlluc((volatile void*)(uintptr_t)ls, ea, tid, rid);
}

__attribute__ ((__always_inline__))
static inline void spu_dma_putqlluc(const void *ls, uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_putqlluc_assert(ls, ea, tag);
	mfc_putqlluc((volatile void*)(uintptr_t)ls, ea, tag, tid, rid);
}


//---------------------------------------------------------------------------------------------------------------------------------------------
// DMA utilities - data typed DMA
//---------------------------------------------------------------------------------------------------------------------------------------------
void spu_dma_and_wait(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t cmd);

static inline void spu_dma_put_uint8(uint8_t value, uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));
static inline void spu_dma_put_uint16(uint16_t value, uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));
static inline void spu_dma_put_uint32(uint32_t value, uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));
static inline void spu_dma_put_uint64(uint64_t value, uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));

static inline uint8_t spu_dma_get_uint8(uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));
static inline uint16_t spu_dma_get_uint16(uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));
static inline uint32_t spu_dma_get_uint32(uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));
static inline uint64_t spu_dma_get_uint64(uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid) __attribute__ ((__always_inline__));

#define spu_dma_put_uint_template(SIZE)																				\
__attribute__ ((__always_inline__))																					\
static inline void																									\
spu_dma_put_uint##SIZE(uint##SIZE##_t value, uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid)					\
{																													\
	qword buf = (qword)spu_splats(value);																			\
	spu_dma_small_assert(ea, ea, sizeof(uint##SIZE##_t), tag);														\
	spu_dma_and_wait(spu_dma_ea2ls(ea,&buf), ea, sizeof(uint##SIZE##_t), tag, MFC_CMD_WORD(tid,rid,MFC_PUT_CMD));	\
}
spu_dma_put_uint_template(8)
spu_dma_put_uint_template(16)
spu_dma_put_uint_template(32)
spu_dma_put_uint_template(64)

#define spu_dma_get_uint_template(SIZE)																				\
__attribute__ ((__always_inline__))																					\
static inline uint##SIZE##_t																						\
spu_dma_get_uint##SIZE(uint64_t ea, uint32_t tag, uint32_t tid, uint32_t rid)										\
{																													\
	qword buf;																										\
	spu_dma_small_assert(ea, ea, sizeof(uint##SIZE##_t), tag);														\
	spu_dma_and_wait(spu_dma_ea2ls(ea,&buf), ea, sizeof(uint##SIZE##_t), tag, MFC_CMD_WORD(tid,rid,MFC_GET_CMD));		\
	return *(uint##SIZE##_t*)((uintptr_t)&buf + ((uintptr_t)ea&15));												\
}
spu_dma_get_uint_template(8)
spu_dma_get_uint_template(16)
spu_dma_get_uint_template(32)
spu_dma_get_uint_template(64)


//---------------------------------------------------------------------------------------------------------------------------------------------
// DMA utilities - any size DMA
//---------------------------------------------------------------------------------------------------------------------------------------------
void spu_dma_large_cmd(uintptr_t ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t cmd);

__attribute__ ((__always_inline__))
static inline void spu_dma_large_put(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_large_assert(ls, ea, tag);
	spu_dma_large_cmd((uintptr_t)ls,ea,size,tag,MFC_CMD_WORD(tid,rid,MFC_PUT_CMD));
}

__attribute__ ((__always_inline__))
static inline void spu_dma_large_putb(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_large_assert(ls, ea, tag);
	spu_dma_large_cmd((uintptr_t)ls,ea,size,tag,MFC_CMD_WORD(tid,rid,MFC_PUTB_CMD));
}

__attribute__ ((__always_inline__))
static inline void spu_dma_large_putf(const void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_large_assert(ls, ea, tag);
	spu_dma_large_cmd((uintptr_t)ls,ea,size,tag,MFC_CMD_WORD(tid,rid,MFC_PUTF_CMD));
}

__attribute__ ((__always_inline__))
static inline void spu_dma_large_get(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_large_assert(ls, ea, tag);
	spu_dma_large_cmd((uintptr_t)ls,ea,size,tag,MFC_CMD_WORD(tid,rid,MFC_GET_CMD));
}

__attribute__ ((__always_inline__))
static inline void spu_dma_large_getb(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_large_assert(ls, ea, tag);
	spu_dma_large_cmd((uintptr_t)ls,ea,size,tag,MFC_CMD_WORD(tid,rid,MFC_GETB_CMD));
}

__attribute__ ((__always_inline__))
static inline void spu_dma_large_getf(void *ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t tid, uint32_t rid)
{
	spu_dma_large_assert(ls, ea, tag);
	spu_dma_large_cmd((uintptr_t)ls,ea,size,tag,MFC_CMD_WORD(tid,rid,MFC_GETF_CMD));
}


//---------------------------------------------------------------------------------------------------------------------------------------------
// DMA utilities - tag wait
//---------------------------------------------------------------------------------------------------------------------------------------------
__attribute__ ((__always_inline__))
static inline void spu_dma_cancel_tag_status_update()
{
	mfc_write_tag_update_immediate();
	do {} while(__builtin_expect(mfc_stat_tag_update() == 0,0));
	mfc_read_tag_status();
}

__attribute__ ((__always_inline__))
static inline uint32_t spu_dma_cancel_and_wait_tag_status_any(uint32_t tagmask)
{
	spu_dma_cancel_tag_status_update();
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_any();
}

__attribute__ ((__always_inline__))
static inline uint32_t spu_dma_cancel_and_wait_tag_status_all(uint32_t tagmask)
{
	spu_dma_cancel_tag_status_update();
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_all();
}

__attribute__ ((__always_inline__))
static inline uint32_t spu_dma_wait_tag_status_immediate(uint32_t tagmask)
{
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_immediate();
}

__attribute__ ((__always_inline__))
static inline uint32_t spu_cma_wait_tag_status_any(uint32_t tagmask)
{
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_any();
}

__attribute__ ((__always_inline__))
static inline uint32_t spu_dma_wait_tag_status_all(uint32_t tagmask)
{
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_all();
}

#define spu_dma_wait_atomic_status() mfc_read_atomic_status()


#endif

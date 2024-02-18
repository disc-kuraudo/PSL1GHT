#ifndef __SPU_ATOMIC_H__
#define __SPU_ATOMIC_H__

#include <stdint.h>
#include <spu_mfcio.h>
#include <spu_intrinsics.h>


uint32_t spu_atomic_nop32(uint32_t *ls, uint64_t ea);
uint64_t spu_atomic_nop64(uint64_t *ls, uint64_t ea);
uint32_t spu_atomic_incr32(uint32_t *ls, uint64_t ea);
uint64_t spu_atomic_incr64(uint64_t *ls, uint64_t ea);
uint32_t spu_atomic_decr32(uint32_t *ls, uint64_t ea);
uint64_t spu_atomic_decr64(uint64_t *ls, uint64_t ea);
uint32_t spu_atomic_test_and_decr32(uint32_t *ls, uint64_t ea);
uint64_t spu_atomic_test_and_decr64(uint64_t *ls, uint64_t ea);
uint32_t spu_atomic_or32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t spu_atomic_or64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t spu_atomic_add32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t spu_atomic_add64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t spu_atomic_and32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t spu_atomic_and64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t spu_atomic_sub32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t spu_atomic_sub64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t spu_atomic_store32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t spu_atomic_store64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t spu_atomic_compare_and_swap32(uint32_t *ls, uint64_t ea, uint32_t compare, uint32_t value);
uint64_t spu_atomic_compare_and_swap64(uint64_t *ls, uint64_t ea, uint64_t compare, uint64_t value);

__attribute__ ((__always_inline__))
static inline uint32_t spu_atomic_lock_line32(uint32_t *ls, uint64_t ea)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 2;

	ea &= ~0x7f;
	mfc_getllar(ls, ea, 0, 0);
	mfc_read_atomic_status();
	spu_dsync();
	return ls[i];
}

__attribute__ ((__always_inline__))
static inline uint64_t spu_atomic_lock_line64(uint64_t *ls, uint64_t ea)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 3;

	ea &= ~0x7f;
	mfc_getllar(ls, ea, 0, 0);
	mfc_read_atomic_status();
	spu_dsync();
	return ls[i];
}

__attribute__ ((__always_inline__))
static inline int spu_atomic_store_conditional32(uint32_t *ls, uint64_t ea, uint32_t value)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 2;

	ls[i] = value;
	ea &= ~0x7f;
	spu_dsync();
	mfc_putllc(ls, ea, 0, 0);
	return mfc_read_atomic_status();
}

__attribute__ ((__always_inline__))
static inline int spu_atomic_store_conditional64(uint64_t *ls, uint64_t ea, uint64_t value)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 3;

	ls[i] = value;
	ea &= ~0x7f;
	spu_dsync();
	mfc_putllc(ls, ea, 0, 0);
	return mfc_read_atomic_status();
}


#endif

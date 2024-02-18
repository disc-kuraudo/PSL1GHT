#ifndef __MARS_TASK_MODULE_H__
#define __MARS_TASK_MODULE_H__

#include <stdint.h>
#include <unistd.h>

#include "mars/callback_types.h"
#include "mars/mutex_types.h"
#include "mars/task_types.h"

#include "task_internal_types.h"

#define MARS_TASK_MODULE_DMA_TAG	31
#define MARS_TASK_MODULE_DMA_TAG_MAX	31

/* mars task module syscalls */
struct mars_task_module_syscalls {
	uint32_t			(*get_ticks)(void);
	uint16_t			(*get_kernel_id)(void);
	struct mars_task_context *	(*get_task)(void);
	struct mars_task_context *	(*get_task_by_id)
					   (const struct mars_task_id *task_id);

	void	(*exit)(void);
	void	(*yield)(void *heap);
	int	(*schedule)(uint16_t workload_id,
			    const struct mars_task_args *args,
			    uint8_t priority);
	int	(*unschedule)(uint16_t workload_id, int32_t exit_code);
	int	(*wait)(uint16_t workload_id, void *heap);
	int	(*try_wait)(uint16_t workload_id);
	void	(*signal_host)(uint64_t watch_point_ea);
	int	(*signal_send)(uint16_t workload_id);
	int	(*signal_wait)(void *heap);
	int	(*signal_try_wait)(void);
	int	(*call_host)(uint64_t callback_ea,
			     const struct mars_callback_args *in,
			     struct mars_callback_args *out, void *heap);
	
	int	(*mutex_lock_get)(uint64_t mutex_ea,
				  struct mars_mutex *mutex);
	int	(*mutex_unlock_put)(uint64_t mutex_ea,
				    struct mars_mutex *mutex);

	int	(*dma_get)(void *ls, uint64_t ea, uint32_t size, uint32_t tag);
	int	(*dma_put)(const void *ls, uint64_t ea, uint32_t size,
			   uint32_t tag);
	int	(*dma_wait)(uint32_t tag);
};


extern const struct mars_task_module_syscalls *mars_task_module_syscalls;

static inline void *mars_task_module_get_heap(void)
{
	return sbrk(0);
}

static inline uint32_t mars_task_module_get_ticks(void)
{
	return (*mars_task_module_syscalls->get_ticks)();
}

static inline uint16_t mars_task_module_get_kernel_id(void)
{
	return (*mars_task_module_syscalls->get_kernel_id)();
}

static inline struct mars_task_context *mars_task_module_get_task(void)
{
	return (*mars_task_module_syscalls->get_task)();
}

static inline struct mars_task_context *mars_task_module_get_task_by_id
					(const struct mars_task_id *task_id)
{
	return (*mars_task_module_syscalls->get_task_by_id)(task_id);
}

static inline void mars_task_module_exit(void)
{
	(*mars_task_module_syscalls->exit)();
}

static inline void mars_task_module_yield(void *task_heap)
{
	(*mars_task_module_syscalls->yield)(task_heap);
}

static inline int mars_task_module_schedule(uint16_t workload_id,
					    const struct mars_task_args *args,
					    uint8_t priority)
{
	return (*mars_task_module_syscalls->schedule)(workload_id, args,
						      priority);
}

static inline int mars_task_module_unschedule(uint16_t workload_id,
					      int32_t exit_code)
{
	return (*mars_task_module_syscalls->unschedule)(workload_id, exit_code);
}

static inline int mars_task_module_wait(uint16_t workload_id, void *task_heap)
{
	return (*mars_task_module_syscalls->wait)(workload_id, task_heap);
}

static inline int mars_task_module_try_wait(uint16_t workload_id)
{
	return (*mars_task_module_syscalls->try_wait)(workload_id);
}

static inline void mars_task_module_signal_host(uint64_t watch_point_ea)
{
	(*mars_task_module_syscalls->signal_host)(watch_point_ea);
}

static inline int mars_task_module_signal_send(uint16_t workload_id)
{
	return (*mars_task_module_syscalls->signal_send)(workload_id);
}

static inline int mars_task_module_signal_wait(void *task_heap)
{
	return (*mars_task_module_syscalls->signal_wait)(task_heap);
}

static inline int mars_task_module_signal_try_wait(void)
{
	return (*mars_task_module_syscalls->signal_try_wait)();
}

static inline int mars_task_module_call_host(uint64_t callback_ea,
					const struct mars_callback_args *in,
					struct mars_callback_args *out,
					void *task_heap)
{
	return (*mars_task_module_syscalls->call_host)(callback_ea, in, out,
						       task_heap);
}

static inline int mars_mutex_lock_get(uint64_t mutex_ea,
				      struct mars_mutex *mutex)
{
	return (*mars_task_module_syscalls->mutex_lock_get)(mutex_ea, mutex);
}

static inline int mars_mutex_unlock_put(uint64_t mutex_ea,
					struct mars_mutex *mutex)
{
	return (*mars_task_module_syscalls->mutex_unlock_put)(mutex_ea, mutex);
}

static inline int mars_dma_get(void *ls, uint64_t ea, uint32_t size,
			       uint32_t tag)
{
	return (*mars_task_module_syscalls->dma_get)(ls, ea, size, tag);
}

static inline int mars_dma_put(const void *ls, uint64_t ea, uint32_t size,
			       uint32_t tag)
{
	return (*mars_task_module_syscalls->dma_put)(ls, ea, size, tag);
}

static inline int mars_dma_wait(uint32_t tag)
{
	return (*mars_task_module_syscalls->dma_wait)(tag);
}


#endif

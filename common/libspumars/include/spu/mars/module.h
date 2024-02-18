#ifndef __MARS_MODULE_H__
#define __MARS_MODULE_H__

#include <stdint.h>

#include <mars/callback_types.h>
#include <mars/mutex_types.h>
#include <mars/workload_types.h>


/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Entry point for workload module.
 *
 * This function is the main entry point for the workload module. All workload
 * modules will need to have a definition of this function. This function is
 * called from the MARS kernel when a workload context that specifies this
 * workload module is scheduled for execution.
 *
 * \note Returning from this function is equivalent to calling
 * \ref mars_module_workload_finish.
 */
void mars_module_main(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Gets tick counter value.
 *
 * \note Counter's frequency depends on runtime environment.
 *
 * \return
 *	uint32_t		- 32-bit tick counter value
 */
uint32_t mars_module_get_ticks(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Gets ea of MARS context.
 *
 * \return
 *	uint64_t		- ea of MARS context
 */
uint64_t mars_module_get_mars_context_ea(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Gets id of kernel that the module is being executed on.
 *
 * \return
 *	uint16_t		- id of MARS kernel
 */
uint16_t mars_module_get_kernel_id(void);


/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Gets id of current workload context.
 *
 * \return
 *	uint16_t		- id of workload
 */
uint16_t mars_module_get_workload_id(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Gets pointer to current workload context.
 *
 * \return
 *	struct mars_workload_context *	- pointer to current workload context
 */
struct mars_workload_context *mars_module_get_workload(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Gets pointer to workload context specified by id.
 *
 * \param[in] id		- id of workload
 * \return
 *	struct mars_workload_context *	- pointer to specified workload context
 */
struct mars_workload_context *mars_module_get_workload_by_id(uint16_t id);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Returns whether or not specified query is satisfied.
 *
 * \param[in] id		- id of workload
 * \param[in] query		- query type
 * \return
 *	int			- non-zero if query satisfied

 */
int mars_module_workload_query(uint16_t id, int query);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Sets calling workload to wait for completion of specified workload.
 *
 * \note This function only sets the id of workload to wait for completion.
 * The caller should also \ref mars_module_workload_wait immediately after this
 * call so the calling workload yields execution and enters the waiting state.
 *
 * \param[in] id		- id of workload
 * \return
 *	MARS_SUCCESS		- id of workload to wait for set
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 */
int mars_module_workload_wait_set(uint16_t id);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Sets calling workload to not wait for completion of any workloads.
 *
 * \return
 *	MARS_SUCCESS		- id of workload to wait for reset
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 */
int mars_module_workload_wait_reset(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Sets signal for specified workload.
 *
 * \param[in] id		- id of workload
 * \return
 *	MARS_SUCCESS		- signal set
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 */
int mars_module_workload_signal_set(uint16_t id);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Resets signal for specified workload.
 *
 * \return
 *	MARS_SUCCESS		- signal reset
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 */
int mars_module_workload_signal_reset(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Begins scheduling of specified workload.
 *
 * This function will begin scheduling the workload specified.
 * This only initiates the scheduling of the workload.
 * This function must be completed with a matching call to
 * \ref mars_module_workload_schedule_end to guarantee the completion of the
 * scheduling.
 *
 * The workload scheduling process is not complete until the matching call to
 * \ref mars_module_workload_schedule_end is made.
 * The user should make any necessary updates to the returned workload context
 * in between this begin call and the end call.
 *
 * \param[in] id		- id of workload
 * \param[in] priority		- scheduling priority of workload
 * \param[out] workload		- address of pointer to workload context
 * \return
 *	MARS_SUCCESS		- workload scheduling started
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 * \n	MARS_ERROR_STATE	- specified workload not added or finished
 */
int mars_module_workload_schedule_begin(uint16_t id, uint8_t priority,
				struct mars_workload_context **workload);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Ends scheduling of specified workload.
 *
 * This function will complete a schedule operation previously initiated with
 * \ref mars_module_workload_schedule_begin.
 * This function must be called in pair for each call to
 * \ref mars_module_workload_schedule_begin to guarantee the completion of the
 * initiated schedule operation.
 *
 * \param[in] id		- id of workload
 * \param[in] cancel		- cancels the schedule operation
 * \return
 *	MARS_SUCCESS		- workload scheduling complete
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 * \n	MARS_ERROR_STATE	- workload scheduling not started
 */
int mars_module_workload_schedule_end(uint16_t id, int cancel);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Begins unscheduling of specified workload.
 *
 * This function will begin unscheduling the workload specified.
 * This only initiates the unscheduling of the workload.
 * This function must be completed with a matching call to
 * \ref mars_module_workload_unschedule_end to guarantee the completion of the
 * unscheduling.
 *
 * The workload unscheduling process is not complete until the matching call to
 * \ref mars_module_workload_unschedule_end is made.
 * The user should make any necessary updates to the returned workload context
 * in between this begin call and the end call.
 *
 * When a workload is unscheduled, it will be put into a finished state and any
 * entities waiting on the workload to finish will be resumed.
 *
 * If a scheduled workload is unscheduled before execution, the workload will
 * not be executed until a subsequent scheduling request is made.
 *
 * If the workload is currently in a waiting state, calling unschedule will
 * finish the workload and will not be resumed from the waiting state.
 *
 * If the workload is currently in a running state, calling unschedule will
 * immediately put the workload into a finished state. However, execution of the
 * workload will only be suspended when the workload yields, waits, or finishes.
 *
 * \note
 * Trying to unschedule a workload that has not yet been scheduled, or has
 * already finished a previously scheduled execution will return an error.
 *
 * \param[in] id		- id of workload
 * \param[out] workload		- address of pointer to workload context
 * \return
 *	MARS_SUCCESS		- workload unscheduling started
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 * \n	MARS_ERROR_STATE	- workload is not scheduled or has finished
 */
int mars_module_workload_unschedule_begin(uint16_t id,
				struct mars_workload_context **workload);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Ends unscheduling of specified workload.
 *
 * This function will complete an unschedule operation previously initiated with
 * \ref mars_module_workload_unschedule_begin.
 * This function must be called in pair for each call to
 * \ref mars_module_workload_unschedule_begin to guarantee the completion of the
 * initiated unschedule operation.
 *
 * \return
 *	MARS_SUCCESS		- workload unscheduling complete
 * \n	MARS_ERROR_PARAMS	- invalid workload id specified
 * \n	MARS_ERROR_STATE	- workload unscheduling not started
 */
int mars_module_workload_unschedule_end(uint16_t id);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Returns execution to kernel with workload in wait state.
 *
 * This function will yield execution of the calling workload module and return
 * execution back to the kernel. The workload currently being processed will be
 * put into a waiting state.
 *
 * \note This function will exit the workload module and is not re-entrant.
 */
void mars_module_workload_wait(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Returns execution to kernel with workload in ready state.
 *
 * This function will yield execution of the calling workload module and return
 * execution back to the kernel. The workload currently being processed will be
 * put into a ready state.
 *
 * \note This function will exit the workload module and is not re-entrant.
 */
void mars_module_workload_yield(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Returns execution to kernel with workload in finished state.
 *
 * This function will yield execution of the calling workload module and return
 * execution back to the kernel. The workload currently being processed will be
 * put into a finished state.
 *
 * \note This function will exit the workload module and is not re-entrant.
 */
void mars_module_workload_finish(void);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Notify host a particular 32-bit area is modified.
 *
 * \param[in] watch_point_ea	- ea of modified area
 *
 */
void mars_module_host_signal_send(uint64_t watch_point_ea);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Request host to call registered callback.
 *
 * \param[in] callback_ea	- ea of function of type \ref mars_callback
 * \param[in] in		- pointer to input args in MPU storage
 *
 * \return
 *	MARS_SUCCESS		- callback requested to host
 */
int mars_module_host_callback_set(uint64_t callback_ea,
				  const struct mars_callback_args *in);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Resets a host callback request and requests result.
 *
 * \param[out] out		- pointer to output args to store result
 *
 * \return
 *	MARS_SUCCESS		- callback request reset
 */
int mars_module_host_callback_reset(struct mars_callback_args *out);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Locks a mutex.
 *
 * This function locks a mutex and blocks other requests to lock it.
 * It also loads the mutex instance from the effective address specified
 * into the local mutex instance.
 *
 * \param[in] mutex_ea		- ea of mutex instance to lock
 * \param[in] mutex		- pointer to local mutex instance
 * \return
 *	MARS_SUCCESS		- successfully locked mutex
 * \n	MARS_ERROR_NULL		- ea is 0 or mutex is NULL
 * \n	MARS_ERROR_ALIGN	- ea or mutex not aligned properly
 */
int mars_module_mutex_lock_get(uint64_t mutex_ea, struct mars_mutex *mutex);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Unlocks a mutex.
 *
 * This function unlocks a previously locked mutex to allow other lock requests.
 * It also stores the local mutex instance into the effective address specified.
 *
 * \param[in] mutex_ea		- ea of mutex instance to unlock
 * \param[in] mutex		- pointer to local mutex instance
 * \return
 *	MARS_SUCCESS		- successfully unlocked mutex
 * \n	MARS_ERROR_NULL		- ea is 0 or mutex is NULL
 * \n	MARS_ERROR_ALIGN	- ea or mutex not aligned properly
 * \n	MARS_ERROR_STATE	- instance not in locked state
 */
int mars_module_mutex_unlock_put(uint64_t mutex_ea, struct mars_mutex *mutex);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> DMA transfer from host storage to MPU storage.
 *
 * This function begins a DMA transfer request from host storage to MPU storage.
 * Transfer completion is not guaranteed until calling \ref mars_module_dma_wait
 * with the corresponding tag used to request the transfer.
 *
 * \param[in] ls		- address of MPU storage to transfer to
 * \param[in] ea		- ea of host storage to transfer from
 * \param[in] size		- size of dma transfer
 * \param[in] tag		- tag of dma transfer
 * \return
 *	MARS_SUCCESS		- successfully tranferred data
 * \n	MARS_ERROR_PARAMS	- invalid tag specified
 * \n	MARS_ERROR_ALIGN	- ls or ea not aligned properly
 */
int mars_module_dma_get(void *ls, uint64_t ea, uint32_t size, uint32_t tag);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> DMA transfer from MPU storage to host storage.
 *
 * This function begins a DMA transfer request from MPU storage to host storage.
 * Transfer completion is not guaranteed until calling \ref mars_module_dma_wait
 * with the corresponding tag used to request the transfer.
 *
 * \param[in] ls		- address of MPU storage to transfer to
 * \param[in] ea		- ea of host storage to transfer from
 * \param[in] size		- size of dma transfer
 * \param[in] tag		- tag of dma transfer
 * \return
 *	MARS_SUCCESS		- successfully tranferred data
 * \n	MARS_ERROR_PARAMS	- invalid tag specified
 * \n	MARS_ERROR_ALIGN	- ls or ea not aligned properly
 */
int mars_module_dma_put(const void *ls, uint64_t ea, uint32_t size,
			uint32_t tag);

/**
 * \ingroup group_mars_workload_module
 * \brief <b>[MPU]</b> Waits for completion of requested DMA transfer.
 *
 * This function waits until completion of all previously started DMA transfer
 * requests with the same tag.
 *
 * \param[in] tag		- tag of dma transfer
 * \return
 *	MARS_SUCCESS		- successfully waited for transfer completion
 * \n	MARS_ERROR_PARAMS	- invalid tag specified
 */
int mars_module_dma_wait(uint32_t tag);


#endif

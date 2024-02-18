#ifndef __MARS_TASK_H__
#define __MARS_TASK_H__

#include <stdint.h>

#include <mars/error.h>
#include <mars/task_barrier.h>
#include <mars/task_event_flag.h>
#include <mars/task_queue.h>
#include <mars/task_semaphore.h>
#include <mars/task_signal.h>
#include <mars/task_types.h>


/**
 * \ingroup group_mars_task
 * \brief <b>[MPU]</b> Entry point for task.
 *
 * This function is the main entry point for the task program. All task
 * programs will need to have a definition of this function. The arguments
 * passed into this function are specified during task scheduling through
 * the call to \ref mars_task_schedule.
 *
 * \note If NULL was specified for \e args when calling \ref mars_task_schedule,
 * the contents of args are undefined.
 *
 * \param[in] args		- pointer to task args structure in MPU storage
 * \return
 *	user specified
 */
int mars_task_main(const struct mars_task_args *args);

/**
 * \ingroup group_mars_task
 * \brief <b>[MPU]</b> Exits and terminates task.
 *
 * This function causes the task to exit and terminate execution. Calling this
 * function will cause the task to enter the finished state, and will no
 * longer be scheduled to run. This function does not need to be called when
 * returning from \ref mars_task_main since it is called automatically.
 *
 * \note This function is a scheduling call and will put the caller task into a
 * finished state.
 *
 * <b>Key Parameters</b>:
 * \n \n
 * \e exit_code
 * - The value passed into here can be obtained when calling \ref mars_task_wait
 * or \ref mars_task_try_wait.
 *
 * \param[out] exit_code	- value to be returned to the task wait call
 */
void mars_task_exit(int32_t exit_code);

/**
 * \ingroup group_mars_task
 * \brief <b>[MPU]</b> Yields caller task so other workloads can run.
 * <b>(Task Switch Call)</b>
 *
 * \note The <b>[MPU]</b> call may result in a task switch and put this
 * task into the waiting state. Understand all the limitations before calling
 * a <b>Task Switch Call</b> (<b>See</b> \ref sec_7_5).
 *
 * This function causes the task to yield and allow other workloads to be
 * scheduled to run if available. The task's context state is saved and the
 * kernel will reschedule the next available workload. If there are no other
 * workloads to be scheduled, the task that called to yield will be rescheduled
 * for resumed execution.
 *
 * \note This function is a scheduling call and may cause a task switch and put
 * the caller task into a ready state.
 *
 * \return
 *	MARS_SUCCESS		- successfully yielded task
 * \n	MARS_ERROR_FORMAT	- no context save area specified
 */
int mars_task_yield(void);

/**
 * \ingroup group_mars_task
 * \brief <b>[host/MPU]</b> Schedules a MARS task for execution.
 *
 * This function schedules the task specified for execution.
 * The actual time of execution is determined by the scheduler.
 * Once the task is scheduled for execution by this function, it may not be
 * scheduled for execution until previous execution has finished.
 * You can wait for task completion by calling \ref mars_task_wait or
 * \ref mars_task_try_wait.
 *
 * You can call this function with a valid task id returned by
 * \ref mars_task_create as many times as you want after each scheduled
 * execution has completed. The task id is valid until the task is destroyed by
 * \ref mars_task_destroy.
 *
 * <b>Key Parameters</b>:
 * \n \n
 * \e args
 * - The task args (\ref mars_task_args) are optional, and only
 * need to be passed in if the task executable expects it.
 * - If NULL is specified the task args passed into the task executable's main
 * function will not be initialized.
 * - The args does need to be kept allocated after this function returns.
 *
 * \e priority
 * - This is the priority of the task between 0 to 255, 0 being the lowest
 * priority and 255 being the highest priority.
 * - Tasks with higher priority will be prioritized during scheduling if both
 * tasks are ready to run at the time of scheduling.
 *
 * \param[in] id		- pointer to task id to schedule
 * \param[in] args		- pointer to task args to pass into task main
 * \param[in] priority		- priority of scheduling for the task
 * \return
 *	MARS_SUCCESS		- successfully scheduled MARS task for execution
 * \n	MARS_ERROR_NULL		- null pointer specified
 * \n	MARS_ERROR_PARAMS	- bad task id specified
 * \n	MARS_ERROR_STATE	- task is in an invalid state
 */
int mars_task_schedule(const struct mars_task_id *id,
		       const struct mars_task_args *args,
		       uint8_t priority);

/**
 * \ingroup group_mars_task
 * \brief <b>[host/MPU]</b> Unschedules a MARS task from being executed.
 *
 * This function unschedules a previously scheduled task.
 *
 * When a task is unscheduled, it will behave as if the task has completed
 * execution. The user is responsible for specifying a unique exit code when
 * unscheduling tasks to handle the condition accordingly.
 *
 * If a scheduled task is unscheduled before execution, the workload will
 * not be executed until a subsequent scheduling request is made.
 *
 * If the task is currently in a waiting state, calling unschedule will
 * finish the workload and will not be resumed from the waiting state.
 *
 * If the task is currently in a running state, calling unschedule will
 * immediately put the task into a finished state. However, execution of the
 * task will only be suspended when the task yields, waits, or exits.
 *
 * \note
 * Trying to unschedule a task that has not yet been scheduled, or has
 * already finished a previously scheduled execution will return an error.
 *
 * <b>Key Parameters</b>:
 * \n \n
 * \e exit_code
 * - The value passed into here can be obtained when calling \ref mars_task_wait
 * or \ref mars_task_try_wait.
 *
 * \param[in] id		- pointer to task id to abort
 * \param[out] exit_code	- value to be returned to the task wait call
 * \return
 *	MARS_SUCCESS		- successfully scheduled MARS task for execution
 * \n	MARS_ERROR_NULL		- null pointer specified
 * \n	MARS_ERROR_PARAMS	- bad task id specified
 * \n	MARS_ERROR_STATE	- task is in an invalid state
 */
int mars_task_unschedule(const struct mars_task_id *id, int32_t exit_code);

/**
 * \ingroup group_mars_task
 * \brief <b>[host/MPU]</b> Waits for task completion.
 * <b>(Task Switch Call)</b>
 *
 * \note The <b>[MPU]</b> call may result in a task switch and put this
 * task into the waiting state. Understand all the limitations before calling
 * a <b>Task Switch Call</b> (<b>See</b> \ref sec_7_5).
 *
 * This function will block until the scheduled task specified is finished.
 *
 * Any number of host threads or tasks can wait for a specific task to complete
 * execution as long as it holds the task's id. However, the task being waited
 * on should not be re-scheduled until all wait calls for the task have
 * returned. Otherwise it is not guaranteed that all wait calls will return
 * after the completion of the initial schedule call.
 *
 * <b>Key Parameters</b>:
 * \n \n
 * \e exit_code
 * - Pass in a pointer to store the task exit code.
 * - If NULL is specified, no exit code can be obtained when the task
 * completes and returns.
 *
 * \note This function is a scheduling call and may cause a task switch and put
 * the caller task into a waiting state.
 *
 * \param[in] id		- pointer to task id to wait for
 * \param[out] exit_code	- pointer to variable to store task exit code
 * \return
 *	MARS_SUCCESS		- task execution finished
 * \n	MARS_ERROR_NULL		- null pointer specified
 * \n	MARS_ERROR_PARAMS	- bad task id specified
 * \n	MARS_ERROR_STATE	- task is in an invalid state
 * \n	MARS_ERROR_FORMAT	- no context save area specified
 */
int mars_task_wait(const struct mars_task_id *id, int32_t *exit_code);

/**
 * \ingroup group_mars_task
 * \brief <b>[host/MPU]</b> Waits for a task completion.
 *
 * This function will check whether the scheduled task specified is finished
 * or not and return immediately without blocking.
 *
 * <b>Key Parameters</b>:
 * \n \n
 * \e exit_code
 * - Pass in a pointer to store the task exit code.
 * - If NULL is specified, no exit code can be obtained when the task
 * completes and returns.
 *
 * \param[in] id		- pointer to task id to wait for
 * \param[out] exit_code	- pointer to variable to store task exit code
 * \return
 *	MARS_SUCCESS		- task execution finished
 * \n	MARS_ERROR_NULL		- null pointer specified
 * \n	MARS_ERROR_PARAMS	- bad task id specified
 * \n	MARS_ERROR_STATE	- task is in an invalid state
 * \n	MARS_ERROR_BUSY		- task has not yet finished execution
 */
int mars_task_try_wait(const struct mars_task_id *id, int32_t *exit_code);

/**
 * \ingroup group_mars_task
 * \brief <b>[MPU]</b> Calls the specified host callback.
 * <b>(Task Switch Call)</b>
 *
 * \note The <b>[MPU]</b> call may result in a task switch and put this
 * task into the waiting state. Understand all the limitations before calling
 * a <b>Task Switch Call</b> (<b>See</b> \ref sec_7_5).
 *
 * This function will block until the requested host callback function returns.
 *
 * <b>Key Parameters</b>:
 * \n \n
 * \e callback_ea
 * - Pass in the EA of the host callback function you want called. This function
 * should be of type \ref mars_callback.
 * - If an invalid EA is specified, the resulting behavior is undetermined.
 *
 * \e in
 * - Pass in a pointer to an initialized callback argument structure instance.
 * - If NULL is specified, a pointer to an uninitialized callback argument
 * structure will be passed into the host callback function.
 *
 * \e out
 * - Pass in a pointer to a callback argument structure instance, which can be
 * initialized by the host callback function.
 * - If NULL is specified, the output argument structure initialized by the host
 * callback function will not be returned to the caller.
 *
 * \param[in] callback_ea	- ea of host callback of type \ref mars_callback
 * \param[in] in		- pointer to args, passed into to host callback
 * \param[out] out		- pointer to args, initialized by host callback
 * \return
 *	MARS_SUCCESS		- host callback successful
 * \n	MARS_ERROR_LIMIT	- host callback queue is full
 * \n	MARS_ERROR_FORMAT	- no context save area specified
 */
int mars_task_call_host(uint64_t callback_ea,
			const struct mars_callback_args *in,
			struct mars_callback_args *out);

/**
 * \ingroup group_mars_task
 * \brief <b>[host/MPU]</b> Gets tick counter value.
 *
 * \note Counter's frequency depends on runtime environment.
 *
 * \return
 *	uint32_t			- 32-bit tick counter value
 */
uint32_t mars_task_get_ticks(void);

/**
 * \ingroup group_mars_task
 * \brief <b>[MPU]</b> Gets id of kernel that the task is being executed on.
 *
 * \note The kernel id refers to an index of the MPU it is being executed on and
 * will range from 0 to (# of MPUs initiialized for MARS context) - 1.
 *
 * \return
 *	uint16_t			- id of MARS kernel
 */
uint16_t mars_task_get_kernel_id(void);

/**
 * \ingroup group_mars_task
 * \brief <b>[MPU]</b> Gets id of caller task.
 *
 * \return
 *	const struct mars_task_id *	- pointer to task id in MPU storage
 */
const struct mars_task_id *mars_task_get_id(void);

/**
 * \ingroup group_mars_task
 * \brief <b>[MPU]</b> Gets name of caller task.
 *
 * \return
 *	const char *			- pointer to task name in MPU storage
 */
const char *mars_task_get_name(void);


#endif

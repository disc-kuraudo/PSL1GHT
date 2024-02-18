#ifndef __MARS_TASK_SIGNAL_H__
#define __MARS_TASK_SIGNAL_H__

#include <mars/task_types.h>


/**
 * \ingroup group_mars_task_signal
 * \brief <b>[host/MPU]</b> Sends a signal to the specified task.
 *
 * This function sends a signal to the task specified. If the task had
 * previously called \ref mars_task_signal_wait and was in the waiting state,
 * this function will cause that task to switch to a ready state and scheduled
 * to run accordingly. If the task has not yet called
 * \ref mars_task_signal_wait, the tasks's signal buffer will be set and the
 * same order of events will occur as previously explained once
 * \ref mars_task_signal_wait is called. The task signal buffer depth is 1.
 * Therefore if the signal buffer is already set when a another signal is
 * received, it has no effect.
 *
 * \param[in] id		- pointer to task id of task to signal
 * \return
 *	MARS_SUCCESS		- successfully yielded task
 * \n	MARS_ERROR_NULL		- null pointer specified
 * \n	MARS_ERROR_PARAMS	- bad task id specified
 * \n	MARS_ERROR_STATE	- task is in an invalid state
 */
int mars_task_signal_send(struct mars_task_id *id);

/**
 * \ingroup group_mars_task_signal
 * \brief <b>[MPU]</b> Waits and yields caller task until receiving signal.
 * <b>(Task Switch Call)</b>
 *
 * \note The <b>[MPU]</b> call may result in a task switch and put this
 * task into the waiting state. Understand all the limitations before calling
 * a <b>Task Switch Call</b> (<b>See</b> \ref sec_7_5).
 *
 * This function will cause the task to enter a waiting state until some other
 * entity calls \ref mars_task_signal_send to this task to return it to the
 * ready state. The task will not be scheduled to run until it receives the
 * signal and returned to the ready state. Once the task receives the signal and
 * this function returns MARS_SUCCESS, the signal buffer is cleared.
 *
 * \return
 *	MARS_SUCCESS		- successfully yielded task
 * \n	MARS_ERROR_FORMAT	- no context save area specified
 */
int mars_task_signal_wait(void);

/**
 * \ingroup group_mars_task_signal
 * \brief <b>[MPU]</b> Waits for task until receiving signal.
 *
 * This function will check the state of a task to see if some other entity
 * has called \ref mars_task_signal_send to this task and returns immediately
 * with the result. Once the task receives the signal and this function returns
 * MARS_SUCCESS, the signal buffer is cleared.
 *
 * \return
 *	MARS_SUCCESS		- successfully yielded task
 * \n	MARS_ERROR_BUSY		- signal not yet received
 */
int mars_task_signal_try_wait(void);


#endif

#ifndef __MARS_TASK_SEMAPHORE_H__
#define __MARS_TASK_SEMAPHORE_H__

#include <stdint.h>

#include <mars/task_semaphore_types.h>


/**
 * \ingroup group_mars_task_semaphore
 * \brief <b>[MPU]</b> Acquires a task semaphore.
 * <b>(Task Switch Call)</b>
 *
 * \note The <b>[MPU]</b> call may result in a task switch and put this
 * task into the waiting state. Understand all the limitations before calling
 * a <b>Task Switch Call</b> (<b>See</b> \ref sec_7_5).
 *
 * This function will attempt to acquire the semaphore.
 * If the total number of current accesses of the semaphore is greater than or
 * equal to the total allowed specified at semaphore creation, the caller task
 * will enter a waiting state until some other tasks release the semaphore and
 * becomes available for acquiring.
 *
 * \param[in] semaphore_ea	- ea of initialized semaphore instance
 * \return
 *	MARS_SUCCESS		- successfully acquired semaphore
 * \n	MARS_ERROR_NULL		- ea is 0
 * \n	MARS_ERROR_ALIGN	- ea not aligned properly
 * \n	MARS_ERROR_LIMIT	- maximum number of tasks already waiting
 * \n	MARS_ERROR_FORMAT	- no context save area specified
 */
int mars_task_semaphore_acquire(uint64_t semaphore_ea);

/**
 * \ingroup group_mars_task_semaphore
 * \brief <b>[MPU]</b> Releases a task semaphore.
 *
 * This function will release a previously acquired semaphore.
 * If their are other tasks currently waiting to acquire a semaphore, calling
 * this function will resume a waiting task to allow it to acquire the
 * semaphore.
 *
 * \param[in] semaphore_ea	- ea of initialized semaphore instance
 * \return
 *	MARS_SUCCESS		- successfully released semaphore
 * \n	MARS_ERROR_NULL		- ea is 0
 * \n	MARS_ERROR_ALIGN	- ea not aligned properly
 */
int mars_task_semaphore_release(uint64_t semaphore_ea);


#endif

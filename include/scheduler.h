#ifndef __ILRD_SCHEDULER_H__
#define __ILRD_SCHEDULER_H__

#include "../include/uid.h" /* task_uid_t */


typedef enum
{
	INTERNAL_ERROR = -1,
	STOPPED_DEFAULT,
	STOPPED_BY_USER,
	ALREADY_RUNNING

}status_t;
typedef enum
{
	STOPPED = 0,
	RUNNING

}state_t;
typedef struct sched sched_t;

/*
	Function to be called when time_to_run is reached.
	return:
		1 - for Reschedule
		0 - destroy task
*/
typedef int (*sched_action_t) (void*);

/*
	Function to be called when task destroyed.
	Note:
		Invoked when destroy task
*/
typedef void (*sched_clean_t) (void*);


/*
	The function creates Scheduler
	Input:
		None
	Output:
		sched_t*	- pointer to sched_t (scheduler object)
--------
	Complexity:
		O(1)
	Limitations:
		malloc can fail
*/
sched_t* SchedCreate(void);


/*
	The function destroy a Scheduler
	Input:
		sched_t*	- pointer to sched_t (scheduler object)
	Output:
		None
--------
	Complexity:
		O(1)
	Limitations:
		None
*/
void SchedDestroy(sched_t* sc);


/*
	The function adds task to the scheduler tasks list.
	Input:
		sched_t*	- pointer to sched_t (scheduler object)
		time_t		- time to execute
		sched_action_t	- action to execute
		void*		- param for action operation
		sched_clean_t	- action to run at destruction
		void*		- param for clean operation
		unsigned long	- time between actions. [Secondes]
		
	Output:
		task_uid_t		- the Unique ID of the created task. or invalid_uid for Error.
--------
	Complexity:
		O(n)
	Limitations:
		Action & Clean operations should be non-NULL.
*/
task_uid_t SchedAddTask(sched_t* sc, time_t date, sched_action_t sched_action, void* action_param, \
					    sched_clean_t sched_clean, void* clean_param, unsigned long interval);


/*
	The function removes task from the Scheduler
	Input:
		sched_t*	- pointer to sched_t (scheduler object)
		task_uid_t		- task id
	Output:
		None
--------
	Complexity:
		O(n)
	Limitations:
		None		
		
*/
void SchedRemoveTask(sched_t* sc, task_uid_t task_id);

/*
	The function checks if the tasks list is empty
	Input:
		const sched_t*	- pointer to sched_t (scheduler object)
	Output:
		int		- 1 for Empty.
--------
	Complexity:
		O(1)
	Limitations:
		None		
		
*/
int SchedIsEmpty(const sched_t* sc);

/*
	The function returns the tasks size
	Input:
		const sched_t*	- pointer to sched_t (scheduler object)
	Output:
		size_t		- number of tasks.
--------
	Complexity:
		O(n)
	Limitations:
		None		
		
*/
size_t SchedSize(const sched_t* sc);

/*
	The function starts the scheduler 
	Input:
		const sched_t*	- pointer to sched_t (scheduler object)
	Output:
		status_t	- Exit Code status.
--------
	Complexity:
		O(t)
	Limitations:
		None		
		
*/
status_t SchedRun(sched_t* sc);

/*
	The function stops the scheduler 
	being called from task.
	Input:
		sched_t*	- pointer to sched_t (scheduler object)
	Output:
		None
--------
	Complexity:
		O(1)
	Limitations:
		None		
		
*/
void SchedStop(sched_t* sc);

/*
	The function clears the scheduler tasks list
	Input:
		sched_t*	- pointer to sched_t (scheduler object)
	Output:
		None
--------
	Complexity:
		O(n)
	Limitations:
		None		
		
*/
void SchedClear(sched_t* sc);



#endif /* end of define guard */

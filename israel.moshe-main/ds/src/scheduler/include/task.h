#ifndef __ILRD_TASK_H__
#define __ILRD_TASK_H__

#include <time.h> /* time_t */

#include "../include/uid.h" /* task_uid_t */

typedef int (*task_action_t) (void*);
typedef void (*task_clean_t) (void*);


typedef struct task task_t;


/*
	The function creates new task.
	Input:
		time_t		- time to execute
		sched_action_t	- action to execute
		void*		- param for action operation
		sched_clean_t	- action to run at destruction
		void*		- param for clean operation
		unsigned long	- time between actions. [Secondes]
		
	Output:
		task_t*		- the created task object.
--------
	Complexity:
		O(1)
	Limitations:
		Action & Clean operations should be non-NULL.
		time_t should be valid.
*/
task_t* TaskCreate(time_t date, task_action_t task_action, task_clean_t task_clean, \
			void* action_param, void* clean_param, unsigned long interval);

/*
	The function destroy task.
	Input:
		task_t*		- pointer to the task object.
	Output:
		None
--------
	Complexity:
		O(1)
	Limitations:
		None
*/		
void TaskDestroy(task_t* t);

/*
	The function returns the ID
	Input:
		const task_t*		- pointer to the task object.
	Output:
		task_uid_t		- task uid object
--------
	Complexity:
		O(1)
	Limitations:
		None
*/
task_uid_t TaskGetID(const task_t* t);

/*
	The function returns the TimeToRun
	Input:
		const task_t*		- pointer to the task object.
	Output:
		time_t			- time to run
--------
	Complexity:
		O(1)
	Limitations:
		invalid on error
*/
time_t TaskGetTime(task_t* t);

/*
	The function executes the task action
	Input:
		task_t*			- pointer to the task object.
	Output:
		int			- 1 for ReSchedule	
--------
	Complexity:
		O(action)
	Limitations:
		None
*/
int TaskExec(task_t* t);

/*
	The function updates the task date to the next interval date
	Input:
		task_t*			- pointer to the task object.
	Output:
		int			- 0 for success	
--------
	Complexity:
		O(1)
	Limitations:
		time can fail
*/
int TaskUpdateTimeToRun(task_t* t);

#endif /* end of define guard */

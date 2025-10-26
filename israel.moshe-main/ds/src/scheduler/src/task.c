/*
|	Written By Israel Moshe
	Date 29.04.2025
|	Reviewed by Yuval S. Almog
|	(ILRD)
*/
#include <time.h> /* time_t, time(NULL)*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "../include/uid.h" /* task_uid_t, UIDCreate */

#define CURRENT_TIME (time(NULL))

typedef int (*task_action_t) (void*);
typedef void (*task_clean_t) (void*);

typedef struct
{
	task_uid_t _id;
	time_t _time_to_run;
	unsigned long _intervals_of_delay;
	void* _action_param;
	void* _clean_param;
	task_action_t _task_action;
	task_clean_t _task_clean;	
}task_t;

task_t* TaskCreate(time_t date, task_action_t task_action, task_clean_t task_clean, \
			void* action_param, void* clean_param, unsigned long interval)
	
{
	task_t* instance = NULL;
	
	assert(NULL != task_action);
	assert(NULL != task_clean);
	assert((time_t)-1 != date);
	
	instance = (task_t*)malloc(sizeof(task_t));
	
	if (NULL != instance)
	{
		if (0 == UIDIsSame(invalid_uid,(instance->_id = UIDCreate())))
		{
			instance->_time_to_run = date;
			instance->_action_param = action_param;
			instance->_clean_param = clean_param;		
			instance->_task_clean = task_clean;
			instance->_task_action = task_action;
			instance->_intervals_of_delay = interval;
		}
		else
		{
			free(instance);
			instance = NULL;
		}
	}
	
	return (instance);
}
void TaskDestroy(task_t* t)
{
	assert(NULL != t);
	
	t->_task_clean(t->_clean_param);
	
	free(t);
	t = NULL;
}

task_uid_t TaskGetID(const task_t* t)
{
	assert(NULL != t);

	return (t->_id);
}
time_t TaskGetTime(task_t* t)
{
	assert(NULL != t);

	return (t->_time_to_run);
}

int TaskExec(task_t* t)
{
	assert(NULL != t);
	assert(NULL != t->_task_action);
	
	return (t->_task_action(t->_action_param));
}

int TaskUpdateTimeToRun(task_t* t)
{	
	time_t temp = {0};
	
	assert(NULL != t);

	if ((time_t)-1 != (temp = CURRENT_TIME))
	{
		t->_time_to_run = t->_intervals_of_delay + temp;
	}
	
	assert(((time_t)-1) != temp);
	
	return (((time_t)-1) == temp);
}

/*
|	Written By Israel Moshe
	Date 29.04.2025
|	Reviewed by Yuval S. Almog
|	(ILRD)
*/

#ifndef NDEBUG
#include <stdio.h>  /* printf */
#endif /* one line above's if*/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <time.h> /* time() */

#include "../include/pq.h" /* pq_t ...*/

#include "../include/task.h" /* task_t ...*/

#define CURRENT_TIME (time(NULL))
#define RESCHEDULE (1)	
#define SUCCESS (0)

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

struct scheduler
{
	state_t _state;
	pq_t* _tasks;
	task_t* _curr_task;
	/*int action_was_self_remove;*/
};

typedef struct scheduler sched_t;

/* used in SchedDestroy */
void SchedClear(sched_t* sc);


typedef int (*sched_action_t) (void*);
typedef void (*sched_clean_t) (void*);

/* Check if 2 tasks are the same | Used in PQ as comparer */
static int MatchDates(const void* data1, const void* data2, const void* param)
{
	assert(NULL != data1);
	assert(NULL != data2);
	
	param = param;
	
	return (TaskGetTime((task_t*)data1) - TaskGetTime((task_t*)data2));
}
/* Compare function for UIDs | Used in Erase as match function */
static int IsMatchTaskAndID(const void* task, const void* id)
{
	long status = 0;
	
	assert(NULL != ((task_t*)(task)));
	
	status = UIDIsSame(TaskGetID(task), *(task_uid_t*)id);
	
	return (status);
}

sched_t* SchedCreate(void)
{
	sched_t* instance = NULL;
	
	instance = (sched_t*)malloc(sizeof(sched_t));
	
	if (NULL != instance)
	{
		if (NULL != (instance->_tasks = PQCreate(MatchDates, (void*)0)))
		{
			instance->_state = STOPPED;
			instance->_curr_task = NULL;
		}
		else
		{
			instance->_curr_task = NULL;
			instance->_tasks = NULL;
			free(instance);
			instance = NULL;
		}
	}
	
	return (instance);
}
void SchedDestroy(sched_t* sc)
{
	assert(NULL != sc);
	assert(NULL != sc->_tasks);
	assert(RUNNING != sc->_state);
	
	SchedClear(sc);
	
	PQDestroy(sc->_tasks);
	sc->_curr_task = NULL;
	sc->_tasks = NULL;
	
	free(sc);
	sc = NULL;
}

task_uid_t SchedAddTask(sched_t* sc, time_t date, sched_action_t sched_action, void* action_param, \
					    sched_clean_t sched_clean, void* clean_param, unsigned long interval)
{
	task_uid_t result_id = {0};
	
	task_t* instance = NULL;
	
	assert(NULL != sc);
	assert(NULL != sched_clean);
	assert(NULL != sched_action);
		
	result_id = invalid_uid;
	instance = TaskCreate(date, sched_action, sched_clean, action_param, clean_param, interval);
	

	if (NULL != instance)
	{
		PQEnqueue(sc->_tasks, instance);
		result_id = TaskGetID(instance);
	}
	
	return (result_id);

}

void SchedRemoveTask(sched_t* sc, task_uid_t task_id) /* should be good as action func  */
{
	task_t* data = NULL;
	
	assert(NULL != sc);
	assert(NULL != sc->_tasks);	
	assert(UIDIsSame(invalid_uid, task_id) == 0);

	if ((NULL != sc->_curr_task) && ((TaskGetID(sc->_curr_task))._id == task_id._id))
	{
		TaskDestroy(sc->_curr_task);		
		sc->_curr_task = NULL;
	}
	else
	{
		data = (task_t*)PQErase(sc->_tasks, (pq_is_match_t)IsMatchTaskAndID, (void*)&task_id);
		
		if (NULL != data && UIDIsSame(TaskGetID(data), task_id) == 1)
		{
			
			TaskDestroy(data);
		}	
		
	}
}

int SchedIsEmpty(const sched_t* sc)
{
	assert(NULL != sc);

	return (PQIsEmpty(sc->_tasks));		
}
size_t SchedSize(const sched_t* sc)
{
	assert(NULL != sc);
	assert(NULL != sc->_tasks);
	
	return (PQSize(sc->_tasks));
}
static int TaskReInsert(sched_t* sc)
{
	int exit_code = 0;
	
	assert(NULL != sc);
	
	if (SUCCESS == (exit_code = TaskUpdateTimeToRun(sc->_curr_task)))
	{		
		PQEnqueue(sc->_tasks, sc->_curr_task);
		sc->_curr_task = NULL;		
	}
	else
	{
		assert(0);
	}
	
	return(exit_code);
}
static int TimeToRun(sched_t* sc, time_t s_time)
{
	status_t status = STOPPED_DEFAULT;
	
	
	assert(NULL != sc);
	assert(NULL != sc->_curr_task);
	
	#ifndef NDEBUG
	printf("[Task: %lu | %lu]\n", (TaskGetID(sc->_curr_task)._id), \
				    TaskGetTime(sc->_curr_task)-s_time);
	#else
	s_time = s_time;		  
	#endif /* one print above's if*/
	
	PQDequeue(sc->_tasks);
	
	if(TaskExec(sc->_curr_task) == RESCHEDULE)
	{
		if (SUCCESS != TaskReInsert(sc))
		{
			status = INTERNAL_ERROR;
		}
	}
	else if(NULL != sc->_curr_task)
	{
	
		TaskDestroy(sc->_curr_task);
		sc->_curr_task = NULL;
	}
	
	return (status);
}
status_t SchedRun(sched_t* sc)
{
	time_t delay  = {0};
	time_t current_time = {0};

	status_t status = STOPPED_DEFAULT;
	
	assert(NULL != sc);
	
	if (sc->_state != RUNNING)
	{
		
		sc->_state = RUNNING;
		current_time = CURRENT_TIME;

		#ifndef NDEBUG
		printf("CurrentTime: %lu\n", current_time);
		#endif /* one line above's if*/
		
		if ((time_t)-1 == current_time)
		{
			sc->_state = STOPPED;
			status = INTERNAL_ERROR;
		}
		
		while (0 == SchedIsEmpty(sc) && RUNNING == sc->_state)
		{
			sc->_curr_task = PQPeek(sc->_tasks);
			assert(NULL != sc->_curr_task);
			
			delay = TaskGetTime(sc->_curr_task);
			delay = (delay > current_time) ? delay - current_time : 0;
			
			sleep(delay);

			if ((time_t)-1 == (current_time = CURRENT_TIME))
			{
				sc->_state = STOPPED;
				status = INTERNAL_ERROR;
			}
			else if (TaskGetTime(sc->_curr_task) <= current_time)
			{
				 /* Execute Task */
				status = TimeToRun(sc, current_time);				
			}
			
		}
		
		sc->_curr_task = NULL;
		
		status =  (STOPPED == sc->_state) ? STOPPED_BY_USER : status;
		sc->_state = STOPPED;
	}
	else
	{
		status = ALREADY_RUNNING;
	}
		
	return (status);	
}
void SchedStop(sched_t* sc)
{
	assert(NULL != sc);
	
	sc->_state = STOPPED;
}

void SchedClear(sched_t* sc)
{
	assert(NULL != sc);
	
	while (!PQIsEmpty(sc->_tasks))
	{
		TaskDestroy(PQPeek(sc->_tasks));
		PQDequeue(sc->_tasks);
	}
}

#include <assert.h> /* assert */
#include <stdio.h> /* printf */

#include "../include/scheduler.h" /* sched_t & funcs*/
#include "../include/task.h" /* task_t & funcs */
#include "../include/uid.h" /* uid funcs */
#include "../../../../utils/test/test_utils.h" /* TEST_N_EQUAL, TEST_EQUAL, LOG */

#define BUFFER_SIZE (52) /* to hold time on string-format */

/* shared var */
typedef struct global_param
{
	sched_t* _sched;
	task_uid_t _id;
	task_t* _task;

}gparam;

/* Inner functions */
int ExecuteAction(void* param);
int DestroyAction(void* param);
int EmptyAction(void* param);
int IntAction(void* param);
void EmptyClean(void* param);
int StopperAction(void* param);
int DestroyTAction(void* param);

/* Task */
int Test_TaskGetID(void);
int Test_TaskCreate(void);
int Test_TaskGetTime(void);
int Test_TaskExec(void);
int Test_TaskUpdateTime(void);

/* Scheduler */
int Test_SchedCreate(void);
int Test_SchedIsEmpty(void);
int Test_SchedSize(void);
int Test_SchedAddTask(void);
int Test_SchedClear(void);
int Test_SchedRemove(void);
int Test_SchedRun(void);

int main(void)
{
	int exit_code = 0;

	exit_code += Test_TaskCreate();	
	exit_code += Test_TaskGetID();
	exit_code += Test_TaskGetTime();
	exit_code += Test_TaskExec();
	exit_code += Test_TaskUpdateTime();
	
	exit_code += Test_SchedCreate();
	exit_code += Test_SchedIsEmpty();
	exit_code += Test_SchedSize();
	exit_code += Test_SchedAddTask();
	exit_code += Test_SchedClear();
	exit_code += Test_SchedRemove();
	exit_code += Test_SchedRun();
	
	return (0);
}
int Test_SchedRun(void)
{
	int exit_code = 0;
	sched_t* instance = NULL;
	task_uid_t id = {0};	
/*--------------------------------------*/
	
	LOG("**SchedulerRun: BreakingPoint**");
	{
		instance = SchedCreate();
		
		if (NULL != instance)
		{
			SchedAddTask(instance, time(NULL)+2, EmptyAction, (void*)0, EmptyClean, (void*)0, 2);
			SchedAddTask(instance, time(NULL)+10, StopperAction, (void*)instance, 
						EmptyClean, (void*)0,5);			          
					  
			SchedRun(instance);
			SchedDestroy(instance);
		}	          
	}	
	
	LOG("**SchedulerRun: Remove**");
	{
		gparam storage = {0};
		instance = SchedCreate();
		
		if (NULL != instance)
		{
			storage._sched = instance;
			id = SchedAddTask(instance, \
				   	  time(NULL)+10, \
					  EmptyAction, \
					  (void*)0, \
					  EmptyClean, \
					  (void*)0, \
					  2);
					  
			storage._id = id;			         
			SchedAddTask(instance, \
				   	  time(NULL)+3, \
					  DestroyAction, \
					  (void*)&storage, \
					  EmptyClean, \
					  (void*)0, \
					  5);			          
					  
			SchedRun(instance);	          
			SchedDestroy(instance);	
		}		          
	}	
	LOG("**SchedulerRun: Self Remove**");
	{
		gparam storage = {0};
		instance = SchedCreate();
		
		if (NULL != instance)
		{
			storage._sched = instance;
			id = SchedAddTask(instance, \
				   	  time(NULL)+5, \
					  DestroyAction, \
					  (void*)&storage, \
					  EmptyClean, \
					  (void*)0, \
					  2);
					  
			storage._id = id;			         
			          
					  
			SchedRun(instance);	          
			SchedDestroy(instance);	
		}		          
	}	
	return (exit_code);
}
int Test_SchedRemove(void)
{

	int exit_code = 0;
	sched_t* instance = NULL;
	task_uid_t id = {0};	
/*--------------------------------------*/
	
	LOG("**SchedulerRemove**");
	{
		instance = SchedCreate();
		
		SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        id = SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);
  		SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);
			          
		TEST_EQUAL(SchedSize(instance), 8, "%d", sizeof(int), &exit_code, "SchedAddTask");		
		TEST_N_EQUAL(UIDIsSame(id, invalid_uid), 1, "%d", sizeof(int), &exit_code, "id check");				
		SchedRemoveTask(instance, id);
		TEST_EQUAL(SchedSize(instance), 7, "%d", sizeof(int), &exit_code, "SchedSize");		
		TEST_EQUAL(SchedIsEmpty(instance), 0, "%d", sizeof(int), &exit_code, "SchedIsEmpty");		
						
		SchedDestroy(instance);
	}
	
	return (exit_code);
}
int Test_SchedClear(void)
{

	int exit_code = 0;
	sched_t* instance = NULL;	
/*--------------------------------------*/
	
	LOG("**SchedulerClear**");
	{
		instance = SchedCreate();
		
		SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

	        SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);

		TEST_EQUAL(SchedSize(instance), 7, "%d", sizeof(int), &exit_code, "SchedAddTask");		
		
		SchedClear(instance);
		TEST_EQUAL(SchedSize(instance), 0, "%d", sizeof(int), &exit_code, "SchedSize");		
		TEST_EQUAL(SchedIsEmpty(instance), 1, "%d", sizeof(int), &exit_code, "SchedIsEmpty");		
						
		SchedDestroy(instance);
	}
	
	return (exit_code);
}
int Test_SchedAddTask(void)
{
	int exit_code = 0;
	sched_t* instance = NULL;
	task_uid_t id = {0};
/*--------------------------------------*/
	
	LOG("**SchedAddTask**");
	{
		instance = SchedCreate();
		
		id = SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          30);
	        printf("%ld\n", id._id);
		TEST_EQUAL(UIDIsSame(id, invalid_uid), 0, "%d", sizeof(int), &exit_code, "SchedAddTask");			

		id = SchedAddTask(instance, \
			   	  time(NULL), \
			          EmptyAction, \
			          (void*)0, \
			          EmptyClean, \
			          (void*)0, \
			          50);
	        printf("%ld\n", id._id);
		TEST_EQUAL(UIDIsSame(id, invalid_uid), 0, "%d", sizeof(int), &exit_code, "SchedAddTask");			

		
		/********************************/
		
		TEST_EQUAL(SchedSize(instance), 2, "%d", sizeof(int), &exit_code, "SchedSize");	
		TEST_EQUAL(SchedIsEmpty(instance), 0, "%d", sizeof(int), &exit_code, "SchedIsEmpty");	
				
		SchedDestroy(instance);	
	}
	
	return (exit_code);
}
int Test_SchedSize(void)
{
	int exit_code = 0;
	sched_t* instance = NULL;	
/*--------------------------------------*/
	
	LOG("**SchedulerSize**");
	{
		instance = SchedCreate();
		
		TEST_EQUAL(SchedSize(instance), 0, "%d", sizeof(int), &exit_code, "SchedSize");		
		
		SchedDestroy(instance);
	}
	
	return (exit_code);
}
int Test_SchedIsEmpty(void)
{
	int exit_code = 0;
	sched_t* instance = NULL;	
/*--------------------------------------*/
	
	LOG("**SchedulerIsEmpty**");
	{
		instance = SchedCreate();
		
		TEST_EQUAL(SchedIsEmpty(instance), 1, "%d", sizeof(int), &exit_code, "SchedIsEmpty");		
		
		SchedDestroy(instance);
	}
	
	return (exit_code);
}
int Test_SchedCreate(void)
{
	int exit_code = 0;
	sched_t* instance = NULL;	
/*--------------------------------------*/
		
	

	
	LOG("**SchedulerCreate**");
	{
		instance = SchedCreate();
	
		TEST_N_EQUAL(instance, NULL, "%p", sizeof(void*), &exit_code, "SchedCreate");
	
		SchedDestroy(instance);
		instance = NULL;
	}
	
	return (exit_code);
}
int Test_TaskUpdateTime(void)
{

	int exit_code = 0;
	task_t* instance = NULL;
	struct tm* local_time = NULL;
	char buffer[BUFFER_SIZE] ={0};
	time_t curr_time = {0}, old_time = {0};
	
/*--------------------------------------*/
	
	LOG("**TaskUpdateTime**");
	{
		instance = TaskCreate(time(NULL), \
				      EmptyAction, \
				      EmptyClean, \
      				      (void*)0, \
				      (void*)0, \
				      30);
				      
		old_time = TaskGetTime(instance);		
		TaskUpdateTimeToRun(instance); 		      
		curr_time = TaskGetTime(instance);
		local_time = localtime(&curr_time);
		
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
		
		printf("{%s}\n", buffer);
		TEST_EQUAL(TaskGetTime(instance) , (void*)(old_time + 30), "%d", sizeof(int), &exit_code, "Task Update Time");			
				      
		TaskDestroy(instance);
	}

	return (exit_code);
}
int Test_TaskExec(void)
{
	int exit_code = 0;
	task_t* instance1 = NULL;
	task_t* instance2 = NULL;
	task_uid_t idt = {0};
	
	gparam saver = {0};
	
/*--------------------------------------*/
	
	LOG("**TaskExec**");
	{
		instance1 = TaskCreate(time(NULL), \
				      ExecuteAction, \
				      EmptyClean, \
      				      (void*)&saver, \
				      (void*)0, \
				      30);
		instance2 = TaskCreate(time(NULL), \
				      IntAction, \
				      EmptyClean, \
      				      (void*)13, \
				      (void*)0, \
				      30);	      
		
		saver._task = instance2;
		TEST_EQUAL(TaskExec(instance1), 13, "%d", sizeof(int), &exit_code, "TaskExec");		
		TaskDestroy(instance1);
		TaskDestroy(instance2);
		instance1 = instance2 = NULL;
	}
	LOG("**TaskExec: Remove**");	
	{
		sched_t* instance = SchedCreate();
		
		if (NULL != instance)
		{
			saver._sched = instance;
			
			instance1 = TaskCreate(time(NULL), \
					      DestroyAction, \
					      EmptyClean, \
	      				      (void*)&saver, \
					      (void*)0, \
					      30);
			
			idt = SchedAddTask(instance,time(NULL), IntAction, (void*)0, EmptyClean, \
							(void*)13, 30);	      
			
			saver._id = idt;
			TaskExec(instance1);
			
			/* Valgrind Check */
			/*TaskDestroy(instance2);*/
			
			TaskDestroy(instance1);
			SchedDestroy(instance);
			saver._sched = NULL;
			instance1  = NULL;
		}

	}
	LOG("**TaskExec: Self remove**");
	{

			
		instance1 = TaskCreate(time(NULL), \
				      DestroyTAction, \
				      EmptyClean, \
      				      (void*)&saver, \
				      (void*)0, \
				      30);
      

		saver._task = instance1;
		TaskExec(instance1);
		/* Valgrind Check */
		
		/*TaskDestroy(instance1);*/

		instance1  = NULL;
	}
	
	return (exit_code);
}
int Test_TaskGetTime(void)
{
	int exit_code = 0;
	task_t* instance = NULL;
	struct tm* local_time = NULL;
	char buffer[BUFFER_SIZE] ={0};
	time_t curr_time = {0};
	
/*--------------------------------------*/
	
	LOG("**TaskGetTime**");
	{
		instance = TaskCreate(time(NULL), \
				      EmptyAction, \
				      EmptyClean, \
      				      (void*)0, \
				      (void*)0, \
				      30);
				      
		curr_time = TaskGetTime(instance);
		local_time = localtime(&curr_time);
		
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
		
		printf("{%s}\n", buffer);
		
				      
		TaskDestroy(instance);
	}

	return (exit_code);
}
int Test_TaskGetID(void)
{	
	int exit_code = 0;
	task_uid_t id1 = {0};
	task_uid_t id2 = {0};	
	task_t* instance1 = NULL;
	task_t* instance2 = NULL;	
/*--------------------------------------*/


	LOG("**TaskGetID**");
	{
		instance1 = TaskCreate(time(NULL), \
				      EmptyAction, \
				      EmptyClean, \
      				      (void*)0, \
				      (void*)0, \
				      30);
		/*sleep(10);*/
		instance2 = TaskCreate(time(NULL), \
				      EmptyAction, \
				      EmptyClean, \
      				      (void*)0, \
				      (void*)0, \
				      30);
		
		assert(NULL != instance1);
		assert(NULL != instance2);
		
		
		      				      
		id1 = TaskGetID(instance1);
		
		TEST_EQUAL(id1._id, 2, "%d", sizeof(int), &exit_code, "TaskGetID");
		
		id2 = TaskGetID(instance2);
		
		TEST_EQUAL(id2._id, 3, "%d", sizeof(int), &exit_code, "TaskGetID");		

		TEST_EQUAL(UIDIsSame(id1, invalid_uid), 0, "%d", sizeof(int), &exit_code, "UIDIsSame");
			
		TEST_N_EQUAL(UIDIsSame(id1, id2), 1, "%d", sizeof(int), &exit_code, "UIDIsSame");

		id1._id = id2._id;
		TEST_EQUAL(UIDIsSame(id1, id2), 1, "%d", sizeof(int), &exit_code, "UIDIsSame");
				
		TaskDestroy(instance1);
		instance1 = NULL;		
		TaskDestroy(instance2);
		instance2 = NULL;
	}	
	
	return (exit_code);
}
int Test_TaskCreate(void)
{
	int exit_code = 0;
	task_t* instance = NULL;
/*--------------------------------------*/
		
	
	LOG("**TaskCreate**");
	{
		instance = TaskCreate(time(NULL) + 32, \
				      EmptyAction, \
				      EmptyClean, \
      				      (void*)0, \
				      (void*)0, \
				      30);
	
		TEST_N_EQUAL(instance, NULL, "%p", sizeof(void*), &exit_code, "TaskCreate");
		
		TaskDestroy(instance);
		instance = NULL;
	}
	
	return (exit_code);
}

int ExecuteAction(void* param)
{
	gparam* saver = (gparam*)(param);
	TaskExec(saver->_task);
	
	return 0;
}
int IntAction(void* param)
{
	
	return ((int)param);
}

int EmptyAction(void* param)
{
	printf("Time:[%u]\n", time(NULL));
	param = param;	
	
	return (1);
}
int DestroyAction(void* param)
{
	gparam* saver = *(gparam**)(&param);

	printf("Destroy\n");
	SchedRemoveTask(saver->_sched, saver->_id);
	
	return (0);
}
int DestroyTAction(void* param)
{
	gparam* saver = *(gparam**)(&param);

	printf("Destroy\n");
	TaskDestroy(saver->_task);
	
	return (0);
}
int StopperAction(void* param)
{
	assert(NULL != param);
	
	SchedClear(((sched_t*)param));
	SchedStop((sched_t*)param);
	printf("STOP THE SCHEDULER\n");
	
	return (0);
}
void EmptyClean(void* param)
{
	printf("[EXIT/%ld]", param);

}


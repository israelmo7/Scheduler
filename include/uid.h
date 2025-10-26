#ifndef __ILRD_UID_H__
#define __ILRD_UID_H__

#include <stdint.h> /* uint32_t */
#include <time.h> /* time_t, time() */
#include <unistd.h> /* pid */
#include <sys/types.h> /* pid_t */

typedef uint32_t ip_t;

typedef struct
{
	ip_t _ip;
	pid_t _pid;
	time_t _time;
	unsigned long _id;
}task_uid_t;

extern const task_uid_t invalid_uid;

/*
	The function creates UID
	Input:
		None
	Output:
		task_uid_t		- an uid object.
---------
	Limitations:
		None
	Complexity:
		O(1)
*/
task_uid_t UIDCreate(void);

/*
	The function checks if 2 UID objects are the same.
	Input:
		const task_uid_t	- uid object number One
		const task_uid_t	- uid object number Two
	Output:
		int		- 1 for "Same". otherwise 0
---------
	Limitations:
		None
	Complexity:
		O(1)
*/
int UIDIsSame(const task_uid_t a, const task_uid_t b);

#endif /* end of define guard */

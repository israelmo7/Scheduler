/*
|	Written By Israel Moshe
	Date 29.04.2025
|	Reviewed by Yuval S. Almog
|	(ILRD)
*/
#include <time.h> /* time_t, time() */
#include <unistd.h> /* pid */
#include <sys/types.h> /* pid_t */
#include <netinet/in.h> /* ip */
/*----     ---- */
/*Includes for GetIP()*/
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
/*---- END ---- */
#include <string.h> /* strcmp */

#define CURRENT_TIME (time(NULL))
#define BUFF_SIZE 64

static unsigned long counter = 1;

typedef uint32_t ip_t;

typedef struct
{
	ip_t _ip;
	pid_t _pid;
	time_t _time;
	unsigned long _id;
}task_uid_t;

const task_uid_t invalid_uid = {0};


static uint32_t Ifa2Ip(struct ifaddrs *ifa)
{
	uint32_t result = 0;	
	void* tmpAddrPtr = NULL;
	char ip[INET6_ADDRSTRLEN] = {0};
		
	tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
	inet_ntop(AF_INET, tmpAddrPtr, ip, INET_ADDRSTRLEN);

	
	if (strcmp(ifa->ifa_name, "lo") != 0)
	{
		result = ntohl(((struct in_addr *)tmpAddrPtr)->s_addr);
	}
	
	return (result);
}
static uint32_t GetIP()
{
	uint32_t result = 0;
	int keep_going = 1;

	struct ifaddrs *ifaddr = NULL;
	struct ifaddrs *ifa    = NULL;



	if (getifaddrs(&ifaddr) != -1) 
	{

		for (ifa = ifaddr; ifa != NULL && keep_going; ifa = ifa->ifa_next) 
		{

			if (ifa->ifa_addr && AF_INET == ifa->ifa_addr->sa_family) 
			{
				if (0 != (result = Ifa2Ip(ifa)))
				{
					keep_going = 0;
				}	
			}
	
		}
		freeifaddrs(ifaddr);
	}


	return (result);
}

task_uid_t UIDCreate(void)
{
	task_uid_t instance = {0};
	
	if (((time_t)-1) != (instance._time = CURRENT_TIME))
	{	
		instance._ip = GetIP();
		instance._pid = getpid();

		instance._id = counter;
		counter = counter + 1;
	}
	else
	{
		instance = invalid_uid;
	}
	
	return (instance);
}
int UIDIsSame(const task_uid_t a, const task_uid_t b)
{
	int status = 0;
	
	status = (a._id == b._id) && (a._time == b._time);
	status = (status) && (a._ip == b._ip) && (a._pid == b._pid);

	return (status);
}

#ifndef __CQPG_TIMER__
#define __CQPG_TIMER__

#include <time.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct __timer
{
	struct timespec start;
	struct timespec stop;
} timer;


extern "C" {

/*length of fmt must > 20. if ok, return fmt*/
char *TimerGetLocalTime(char *fmt)
{
	time_t tp;
	struct tm *t;

	if (NULL == fmt)
	{
		return NULL;
	}
	
	time(&tp);
	t = localtime(&tp);
	sprintf(fmt, "%04d-%02d-%02d %02d:%02d:%02d", \
		1900+t->tm_year, 1+t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	return fmt;
}

/*if ok , return 0; otherwise return -1*/
int TimerInit(timer *tmr)
{
	if (NULL == tmr)
	{
		return -1;
	}

	tmr->start.tv_sec = 0;
	tmr->start.tv_nsec = 0;
	tmr->stop.tv_sec = 0;
	tmr->stop.tv_nsec = 0;
	return 0;
}

/*if ok , return 0; otherwise return -1*/
int TimerStart(timer *tmr)
{
	if (NULL == tmr)
	{
		return -1;
	}

#ifdef CLOCK_MONOTONIC
	return clock_gettime(CLOCK_MONOTONIC, &(tmr->start));
#else
	return clock_gettime(CLOCK_REALTIME, &(tmr->start));
#endif

}

/*if ok , return 0; otherwise return -1*/
int TimerStop(timer *tmr)
{
	if (NULL == tmr)
	{
		return -1;
	}

#ifdef CLOCK_MONOTONIC
	return clock_gettime(CLOCK_MONOTONIC, &(tmr->stop));
#else
	return clock_gettime(CLOCK_REALTIME, &(tmr->stop));
#endif

}

/*if ok , return stop-start; otherwise return -1.0. runtime is in seconds*/
double TimerGetRuntime(timer *tmr)
{
	if (NULL == tmr)
	{
		return -1.0;
	}

	return (tmr->stop.tv_sec-tmr->start.tv_sec) + \
		(tmr->stop.tv_nsec-tmr->start.tv_nsec)/1.e+9;

}

}


#endif

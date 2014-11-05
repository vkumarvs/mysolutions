#include "Poco/Util/TimerTask.h"
#include "Poco/Util/Timer.h"
#include "Poco/Thread.h"
#include <stdio.h>

/*Hi this is the second message i am adding from different user to test geerit
 * code review*/
/*vipin:Hi this is my first message to see how gerrit works*/
/*After review added second line */

using namespace std;
using Poco::Util::Timer;
using Poco::Util::TimerTask;
class MinuteKPITask : public Poco::Util::TimerTask
{
public:
	void run();
};

class HourlyKPITask : public Poco::Util::TimerTask
{
public:
	void run();
};

class KPIRunnable : public Poco::Runnable
{
public:
	void run();
};

void MinuteKPITask::run()
{
    printf("I am inside Minute KPI task\n");
}

void HourlyKPITask::run()
{
    printf("I am inside Hourly KPI task\n");
}
#include <sys/prctl.h>
void KPIRunnable::run()
{
    char threadName[128];
    snprintf(threadName, sizeof(threadName), "%s", "kpi_vipin");
    prctl(PR_SET_NAME, threadName, NULL, NULL, NULL);


#if 0
	const long minuteInterval = 3000L; // 3 second 
	const long hourlyInterval = 3000L; // 3 second
	Poco::Util::Timer kpiTimer;
	MinuteKPITask minuteTask;
	HourlyKPITask hourlyTask;

	struct timespec currentTime;
	clock_gettime(CLOCK_REALTIME, &currentTime);
	struct tm *calenderTime = gmtime(&(currentTime.tv_sec));

	time_t toNextFiveMinute = (1- calenderTime->tm_min % 1) * 60
			- calenderTime->tm_sec;
	time_t toNextHour = 3600 - calenderTime->tm_min * 60 - calenderTime->tm_sec
			+ 1;

	kpiTimer.scheduleAtFixedRate(&minuteTask, (long)(toNextFiveMinute * 1000), minuteInterval);
	//kpiTimer.scheduleAtFixedRate(&hourlyTask, (long)(toNextHour * 1000), hourlyInterval);
#endif
	while (1)
		sleep(10);
}

int main()
{
	KPIRunnable kpiRunnable;
	Poco::Thread thread;
    //thread.setName("vipin_kpi");
	thread.start(kpiRunnable);
    printf("i am waiting to join %s you\n", thread.getName().c_str());
#if 1
	const long minuteInterval = 3000L; // 3 second 
	const long hourlyInterval = 3000L; // 3 second
	Poco::Util::Timer kpiTimer;
	MinuteKPITask minuteTask;
	HourlyKPITask hourlyTask;

	struct timespec currentTime;
	clock_gettime(CLOCK_REALTIME, &currentTime);
	struct tm *calenderTime = gmtime(&(currentTime.tv_sec));

	time_t toNextFiveMinute = (1- calenderTime->tm_min % 1) * 60
			- calenderTime->tm_sec;
	time_t toNextHour = 3600 - calenderTime->tm_min * 60 - calenderTime->tm_sec
			+ 1;

	kpiTimer.scheduleAtFixedRate(&minuteTask, (long)(toNextFiveMinute * 1000), minuteInterval);
#endif
	thread.join();
	return 0;
}

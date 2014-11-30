#include "scheduler.h"

static TaskType *Tasks;
uint8_t taskCounter = 0;

void CreateTask(uint32_t interval, void (*f)(void))
{
	if(taskCounter == 0)
	{
		Tasks = malloc(sizeof(TaskType));
	}
	else
	{
		realloc(Tasks,(taskCounter + 1) * sizeof(TaskType));
	}

	Tasks[taskCounter].Func = f;
	Tasks[taskCounter].interval = interval;
	Tasks[taskCounter].lastTick = 0;
	taskCounter++;
}
 
 void RunScheduler(void)
 {
	 while(1)
	 {
		 for(uint8_t i = 0; i<taskCounter; i++)
		 {
			 if( ( Micros() - Tasks[i].lastTick ) >= Tasks[i].interval )
			 {
				 (*Tasks[i].Func) ();
				 Tasks[i].lastTick = Micros();
			 }
		 }
	 }
 }

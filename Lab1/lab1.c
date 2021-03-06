
/*****************************************************************************\
* Laboratory Exercises COMP 3510                                              *
* Author: Saad Biaz                                                           *
* Date  : January 18, 2014                                                    *
\*****************************************************************************/

/*****************************************************************************\
*                             Global system headers                           *
\*****************************************************************************/


#include "common.h"
#include "math.h"

/*****************************************************************************\
*                             Global data types                               *
\*****************************************************************************/



/*****************************************************************************\
*                             Global definitions                              *
\*****************************************************************************/
#define  OUT_OF_BOUNDS 2147483648




/*****************************************************************************\
*                            Global data structures                           *
\*****************************************************************************/




/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/
int avgResponseTimes;
int avgTurnTimes;


int totalMissedEvents;
int totalMissedEventsPerDevice[31];
int totalEventsPerDevice[31];
float missedPercentage[31];
int nextEventNumber[31];
float missedAvg;

/*****************************************************************************\
*                               Function prototypes                           *
\*****************************************************************************/

void Control(void);



/*****************************************************************************\
* function: main()                                                            *
* usage:    Create an artificial environment for embedded systems. The parent *
*           process is the "control" process while children process will gene-*
*           generate events on devices                                        *
*******************************************************************************
* Inputs: ANSI flat C command line parameters                                 *
* Output: None                                                                *
*                                                                             *
* INITIALIZE PROGRAM ENVIRONMENT                                              *
* WHILE JOBS STILL RUNNING                                                    *
*    CREATE PROCESSES                                                         *
*    RUN LONG TERM SCHEDULER                                                  *
*    RUN SHORT TERM SCHEDULER                                                 *
*    DISPATCH NEXT PROCESS                                                    *
\*****************************************************************************/

int main (int argc, char **argv) {

   if (Initialization(argc,argv)){
     Control();
   }
} /* end of main function */

/***********************************************************************\
 * Input : none                                                          *
 * Output: None                                                          *
 * Function: Monitor Devices and process events                          *
 \***********************************************************************/
void Control(void) {
  int j = 0;// Denotes location in BufferLastEvent[]
  int temp = 0; // Holds Value of flags so Flags can be operated on
  Event e; //Added

	float startTime;
	float responseTime;
	float turnaroundTime;
	int totalEvents = 0; // used to calculate averages
	float averageResponseTime;
	float averageTurnaroundTime;
        int missedEvents;
	int totalProcessedEvents;



  while (1) {
    if (Flags != 0){
      	temp = Flags; //Hold current flag values
	Flags = 0; //Clear to hold next flag values after loop below
        j = 0;
        missedAvg = 0;
        while (temp != 0) {
            if (temp & 1) {
               e = BufferLastEvent[j];
               startTime = e.When;

               if (nextEventNumber[j] != e.EventID) {
		  totalMissedEvents += e.EventID - nextEventNumber[j];
                  printf("\nTOTAL MISSED %d", totalMissedEvents);
                  totalMissedEventsPerDevice[j] += e.EventID - nextEventNumber[j];
               }

                startTime = e.When;
               DisplayEvent('a', &e); //char arg arbitrary, used for debugging purposes per Biaz
                responseTime = Now() - startTime;
               Server(&e);
	        turnaroundTime = Now() - startTime;
		averageResponseTime += responseTime;
		averageTurnaroundTime += turnaroundTime;
		totalProcessedEvents++;
		printf("\nTOTALPROCESSEDEVENTS %d", totalProcessedEvents);
                totalEventsPerDevice[j] += 1;
		nextEventNumber[j] = e.EventID++;
   		totalEvents = totalProcessedEvents + totalMissedEvents;
                printf("\nTOTAL EVENTS %d", totalEvents);
              printf("ART: %10.3f ATT: %10.3f", (averageResponseTime/totalEvents),
                   (averageTurnaroundTime/totalEvents));

               missedAvg = (totalMissedEvents/totalEvents);
               missedPercentage[j] = (totalMissedEventsPerDevice[j]/totalEventsPerDevice[j]);
            }
          temp = temp >> 1;
	  j++;
      }
    }
  }
}


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This must print out the number of Events buffered not yet   *
*           not yet processed (Server() function not yet called)        *
\***********************************************************************/
void BookKeeping(void){
  printf("MISSED AVG %d", missedAvg);
  printf("\n >>>>>> Done\n");
}

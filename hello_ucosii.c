/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"
#include "system.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];
/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3
/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{
  INT8U prio=TASK1_PRIORITY;
  OS_TCB *ptcb= OSTCBPrioTbl[prio];
  ptcb->REMAINING_TIME    = ptcb->compute;
  while (1)
  { 
    ptcb->TASK_ACTUAL_START_TIME = OSTimeGet();  
    ptcb->RESPONSE_TIME   = 0;
    //printf("1 start: %d %d %d\n", ptcb->TASK_ACTUAL_START_TIME,ptcb->compute,ptcb->period);
   // ptcb->TASK_CURRENT_TICK = ptcb->TASK_ACTUAL_START_TIME % ptcb->period;
    while( 0 < ptcb->REMAINING_TIME){
        
   }
   
    ptcb->RESPONSE_TIME = OSTimeGet() - ptcb->TASK_SHOULD_START_TIME;
    int todelay = ptcb->period - ptcb->RESPONSE_TIME;
    ptcb->TASK_SHOULD_START_TIME = ptcb->period + ptcb->TASK_SHOULD_START_TIME; 
    ptcb->REMAINING_TIME    = ptcb->compute;
    OSTimeDly(todelay);
  }
}
/* Prints "Hello World" and sleeps for three seconds */
void task2(void* pdata)
{
  
  INT8U prio=TASK2_PRIORITY;
  OS_TCB *ptcb= OSTCBPrioTbl[prio];
  ptcb->REMAINING_TIME    = ptcb->compute;
  while (1)
  {
    
    ptcb->TASK_ACTUAL_START_TIME = OSTimeGet();  
    ptcb->RESPONSE_TIME   = 0;
    //printf("1 start: %d %d %d\n", ptcb->TASK_ACTUAL_START_TIME,ptcb->compute,ptcb->period);
   // ptcb->TASK_CURRENT_TICK = ptcb->TASK_ACTUAL_START_TIME % ptcb->period;
    while( 0 < ptcb->REMAINING_TIME){ 
        
    }
    ptcb->RESPONSE_TIME = OSTimeGet() - ptcb->TASK_SHOULD_START_TIME;
    int todelay = ptcb->period - ptcb->RESPONSE_TIME;
    ptcb->TASK_SHOULD_START_TIME = ptcb->period + ptcb->TASK_SHOULD_START_TIME; 
    ptcb->REMAINING_TIME    = ptcb->compute;
    OSTimeDly(todelay);
    
  }
}

void task3(void* pdata)
{
  INT8U prio=TASK3_PRIORITY;
  OS_TCB *ptcb= OSTCBPrioTbl[prio];
  ptcb->REMAINING_TIME    = ptcb->compute;
  while (1)
  {
    
    ptcb->TASK_ACTUAL_START_TIME = OSTimeGet();  
    ptcb->RESPONSE_TIME   = 0;
    //printf("1 start: %d %d %d\n", ptcb->TASK_ACTUAL_START_TIME,ptcb->compute,ptcb->period);
   // ptcb->TASK_CURRENT_TICK = ptcb->TASK_ACTUAL_START_TIME % ptcb->period;
    while( 0 < ptcb->REMAINING_TIME){ 
        
    }
    ptcb->RESPONSE_TIME = OSTimeGet() - ptcb->TASK_SHOULD_START_TIME;
    int todelay = ptcb->period - ptcb->RESPONSE_TIME;
    ptcb->TASK_SHOULD_START_TIME = ptcb->period + ptcb->TASK_SHOULD_START_TIME; 
    ptcb->REMAINING_TIME    = ptcb->compute;
    OSTimeDly(todelay);
    
  }
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{

  OSTaskCreate(task1,NULL,(void *)&task1_stk[TASK_STACKSIZE-1],TASK1_PRIORITY);
  OSTaskCreate(task2,NULL,(void *)&task2_stk[TASK_STACKSIZE-1],TASK2_PRIORITY);
  //OSTaskCreate(task3,NULL,(void *)&task3_stk[TASK_STACKSIZE-1],TASK3_PRIORITY);
  printf("Current Time\t Event\t\tFrom Task ID    To Task ID     Response Time\tRemain Time\n");
   OSTCBPrioTbl[TASK1_PRIORITY]->compute=1;
   OSTCBPrioTbl[TASK1_PRIORITY]->period=5;
   OSTCBPrioTbl[TASK2_PRIORITY]->compute=4;
   OSTCBPrioTbl[TASK2_PRIORITY]->period=7;
   //OSTCBPrioTbl[TASK3_PRIORITY]->compute=6;
   //OSTCBPrioTbl[TASK3_PRIORITY]->period=24;
  OSTimeSet(0);
  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/

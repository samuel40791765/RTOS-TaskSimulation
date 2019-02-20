#Implementing a task Simulation on the Nios II 12sp2
Code only includes the basic OS Files of the RTOS System and the files I modified.

1.	Implementation Description:
The main objective of this project is to simulate tasks that run for both within and for certain period (c, p).

(1)	Information
First, we set up the information we need for the Task simulation. We need to record the remaining time, response time, and actual start time, and the supposed starting time. We set this up in the structure OS_TCB.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic1.png)

(1)	Task_id is the ID number of the task
(2)	Task_times is the number of time the task has computed
(3)	Period is the period time of the task
(4)	Compute is the computing time of the task
(5)	Computing is a flag to determine if the task if currently computing or not
(6)	TASK_SHOULD_START_TIME is the start of each period of the task
(7)	TASK_ACTUAL_START_TIME is the time the task actually starts computing
(8)	REMAINING_TIME is the remaining computation time the task has to compute
(9)	RESPONSE_TIME is the difference between the time the task finishes computing and TASK_ACTUAL_START_TIME
(10)	DEADLINE is the task’s deadline time

After setting up, the task information should be initialized in OSTCBInit.
 
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic2.png)

(2)	Task Simulation
The main part where I simulated a task period was within the application layer.
First, we assign the correct TCB used for this task from the OSTCBPrioTbl. We assign the remaining time of our task before truly starting.
“While (0 < ptcb->REMAINING_TIME)” is the time spent on computing. After computing, we retrieve the current time and calculate the response time of the task.
We also reassign the information needed for the next period of this task. The task is then delayed until the start of the next period.
 
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic3.png)

Meanwhile, the task counter is updated after each period is done with its computation (within OSSched()). If the task counter is updated too early, the information of our task will be updated too fast for our output and will cause errors.

![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic4.png)

(3)	Task Computation and Deadline Misses
(a)	Task Computation
OSTimeTick is a task within the operating system that runs once per second. Here, we do the actual computing of the current task (OSTCBCur). If a task is currently running, the remaining time of that task lowers by one.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic5.png)
 

(b)	Deadline Misses
We also constantly check for Deadline Misses here. We calculate the supposed ending point of this period and if the current time exceeds that, it means the deadline has been missed. If the deadline is missed, we output the results and suspend the task for the rest of the program.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic6.png)

(4)	Task Preemption
We check for preemption in the ISR Context Switcher, OSIntExit. Normally if  a task is currently computing and suddenly goes through the ISR Context switcher, the task is getting ready for preemption to compute a higher priority task. We check if the task is currently running by checking if the REMAINING_TIME is larger than 0. If the task is running, it means it is being preempted and we output the preemption results into the console.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic7.png)
 




(5)	Task Completion
(a)	Our task is completed
Next, we check for the completion of a task within the Task Switcher, OS_Sched().
Here we use a simple printf, as OS_Sched() is called only when the task is completed and about to be switched.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic8.png)

(b)	Idle task is completed
However, OS_Sched() is not called when an idle task (Priority 63) is about to end and start running a task. To fix this problem, we implement a “Completed Task” circumstance for the idle task within OSIntExit(). The reason this works is because a task goes through an interrupt (ISR) first before it starts to compute.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic9.png)
 

(c)	Task completion problems and fixes
Within our task simulation, our task is considered to be finished computing after the computing loop is finished. After computing is done, new  information essential for the next period is inputted into the task TCB. However, if a higher priority task is about to run, a lower priority task is often preempted without the new information inputted. To fix this problem, we add an if to check if the new information has been successfully inputted into the task before the context switch in OSIntExit().

There are other times when the new information is successfully inputted, but the task is preempted before it can be delayed! To fix this, we implemented a check for preemption, and a check for the delay time. If the task is not preempting (previous task is completed), but the delay time is 0 (which it shouldn’t be), we do not the task perform a context switch.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic10.png)
 
2.	Simulation Result:
Task Set 1: 1(1,5) 2(4,7):
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic11.png)


Task Set 2: 1(2,8) 2(9,18) 3(6,24):
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-TaskSimulation/master/projectimages/pic12.png)
Task 2 experiences a deadline at TimeTick 24.

3.	Pros and Cons
Pros:
(1)	The overall OS is more responsive as tasks that are deemed to be more important are processed first, resulting in a faster response time.
(2)	More efficient and deadlines can be met easier if the time sets are arranged well enough.
(3)	Infinite periods cannot block the system. Cons:
(1)	If the computing time and period for different tasks are not set well enough, a deadline miss could easily be invoked.
(2)	If a high priority task is not delayed long enough or takes too long computing, low priority tasks could easily be led to starvation.
(3)	A lot harder to implement than Non-Preemptive kernels.
 
4.	When to use preemptive kernels?
In multitasking environments where the response time for certain resources is vital, preemptive kernels should be used. Infinite loops also rarely occur, and preemptive kernels force tasks to be switched after a certain time limit.
On the other hand, sometimes tasks cannot be halted in the middle of something important. Tasks results could be heavily effected if they are suddenly preempted by tasks using the same results. In these sort of cases, non-preemptive kernels should be used.

5.	Experience:
This project mainly focuses on the concept of context switching and task simulating. Even though my understanding the process of context switching was clear, there were still many other hurdles I had to overcome. Knowing where to observe where context switching happened wasn’t hard, but the process of task simulating was.
Problems like knowing how to fix a task from being preempted too early hindered my progress immensely. This was the main problem within my code the first time I did this project.
Even though this project took up way more time than I expected, I learned a lot. Preemptive kernels are often seen within modern operating systems and learning how to successfully implement them will be essential to our future within coding. Luckily, our teacher has given us a chance to make up for our grades. My understanding of the code and what was to be implemented was more clear after a few hints from the instructor and our TA. I sincerely hope my newfound understanding of UC/OS can help me do my projects faster in the future.

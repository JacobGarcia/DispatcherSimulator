Mid-term Scheduler (Dispatcher) Simulator
Copyright © 2014 Mario J. García
=========================================

=========================================
+             USEFUL NOTES 		+
=========================================
The source code, version changes, and
more information about the project can
be found in: 
github.com/JacobGarcia/DispatcherSimulator

=========================================
+             CONTRIBUTORS 		+
=========================================
Mario Jacob García Navarro   - A01363206
Luis Emmanuel Sánchez Flores - A0
Joel Jair López Villalva     - A01362464

=========================================
+        IMPLEMENTATION PROCESS         +
=========================================
Six different algorithms were implemented 
with the purpose of simulate the behaviour 
of diverse dispatchers. The average 
waiting time of the processes were
obtained in order to conclude which was 
the best algorithm in these 
terms.
Basically, for the solution procedure, it
was considered the "classical" solution
seen in the TC2008 class. Which considers
the: (The last time that process was
running - The arrival time of the 
process - How many time unities have
been executed). That basic solution is 
applied to the 6 different algorithms. 
Which will be explained in the next 
section.

-----------------------------------------
        IMPORTANT CONSIDERATIONS        
-----------------------------------------
The code is implemented using static 
structures, due to this this fact, the 
number of processes is limited to 10. If 
by any means this rule is not considered, 
the code simply will not work as 
expected.
There is no management if exists 'gaps' 
nor 'spaces' between processes in the 
main process list   

-----------------------------------------
         FIRST COME FIRST SERVED        
-----------------------------------------
This is the most basic implementation. 
Initially, a sorting procedure will be 
done considering the arrival times of the 
processes, leaving at top the process 
which arrives at first. Then, the waiting
time of the first process is set to 0, 
since this process start it's execution 
when actually arrives, so it does not 
wait anytime. Then, the first time that 
is  executed is when it arrives. Since 
that time could be different from 0, then 
this is not considered. In second place, 
the waiting time of the other processes 
are determined, considering that the next 
process that is going to execute, waits
the burst time of the previous process 
plus it's first time execution. Since in
this algorithm are no interruptions, the
"How many time unities have been
executed" is simply ignored. It's
important to note that the "CPU burst"
time is an accumulated considering the
burst time for the previous
processes.

-----------------------------------------
             NON PREEMPTIVE        
-----------------------------------------
This algorithms were solved considering
that every process can be sorted in a 
specific execution order. So, the main
solution basically consists in find that
execution order, this through taking 
account the arrival times of the 
processes, since they can not be included
in the sorting procedures until they 
arrive. With the previous information,
a several sorting procedures, depending, 
on the nature of the main list, are done,
until all the processes of the main list
are included in the sorting. After 
ordering the list as it's intended,the 
same conditions to find the average 
waiting time in the FCFS algorithm is 
applied on this cases. 
This can be done through sorting 
procedures considering the CPU burst time
as Priority of the processes.   

-----------------------------------------
               PREEMPTIVE        
-----------------------------------------
This algorithms were solved considering
mainly that every time a process is 
executed, finishing or being interrupted,
is a new process, and must be stored as 
one on a new list of processes. Every
unity of time the list is checked in
order to know if a process can interrupt
another. When a process finish it's 
execution, apart from adding it to the 
new list of processes, it's left to the
top of the original main list. The 
processes are sorted as they arrive, 
taking account their CPU burst or
Priority, depending the scheduling 
algorithm indicated by the user, 
informing which is going to be the next
process to be executed. Since the 
processes that are interrupted are stored
as new different processes, it is 
indicated through a value that they were
originally part of a single process, 
this in order to satisfy the "How many
unities have been executed". When the 
new list is fulfilled, and all the 
processes of the main list have finished
their executions, then is applied the
same procedure as FCFS, but now, since
there are interruptions, it is included
the "How many time unities have been 
executed".

-----------------------------------------
               ROUND ROBIN
-----------------------------------------
This implementation was made considering
the structure as a list, specifically as
a queue data structure. Were process
whom finish their execution period 
(quantum expiration) are send back to the
last position on the list, and whose
arrive, stays on that position. There is
used a time variable, which is used 
through the entire procedure in order to 
know how many time unities the scheduling
algorithm have last. If a process 
finish it's execution time, then it's 
"destroyed", being replaced with the 
information of the process next to it.
The periods of execution are stored in 
order to indicate "How many time unities
have been executed". Then, the same
"classic" solution is applied, as in
the other algorithms. It is important to
consider that a new list of processes is
needed, since when the processes 
finish their execution time are 
destroyed, and we need to store their 
information to be able to calculate the
average waiting time of all the 
processes.
 

=========================================
+      HOW TO BUILD THE EXECUTABLE      +
=========================================

------------------------------------------
	  gcc -o filename *.c 
------------------------------------------

When the build is executed, the processes 
file must be passed as an argument:
NOTE: Consider for the next command 
that the process file must be in the same 
folder. If that were not the case, then 
add the file address.
-----------------------------------------
	/.filename processfile.txt
-----------------------------------------

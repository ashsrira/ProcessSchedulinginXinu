/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>
unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
        //STATWORD ps;
        //disable(ps);
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

        if (currpid != 0) {
           /* reduce the number of procs if preempt = 0 and equate to unused ticks for the process */
           if ((preempt ==0) && (sched_option ==2) && ((optr= &proctab[currpid])->pstate == PRCURR)) {
              number_of_proc--;
              optr->unused_ticks = 0;
           }
           else
           optr->unused_ticks = preempt;
           if (number_of_proc <0 )
              number_of_proc =0;
        }

   if (sched_option ==0) {
	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
                //restore(ps);
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
                optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
		insert(currpid,rdyhead,optr->pprio);
	}
        if (optr->pstate == PRSLEEP)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRSUSP)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRRECV)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRWAIT)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRTRECV)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);


	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];

   }
   else if (sched_option ==1) {
        if ((optr= &proctab[currpid])->pstate == PRCURR && (currpid !=0)) {
           if (optr->pprio > 65) {
              if (len1 - head1 ==1) {
                 if ((len2 == head2) && (len2 >=0) && (head2 >=0)) {
                    if ((len3 == head3) && (len3 >= 0) && (head3 >= 0)) {
                       //printall();
                       return(OK);
                    }
                 }
              }
           }
           else if((optr->pprio > 32) && (optr->pprio <66)){
              if (len2 - head2 ==1) {
                 if ((len1 == head1) && (len1 >=0) && (head1 >=0)) {
                    if ((len3 == head3) && (len3 >= 0) && (head3 >= 0)) {
                       //printall();
                       return(OK);
                    }
                 }
              }
           }
           else if (optr->pprio <34) {
               if (len3 - head3 ==1) {
                  if ((len1 == head1) && (len1 >=0) && (head1 >=0)) {
                     if ((len2 == head2) && (len2 >= 0) && (head2 >= 0)) {
                        //printall();
                        return(OK);
                     }
                  }
               }
           }
        }

         if (optr->pstate == PRCURR) {
                optr->pstate = PRREADY;
                optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
                //kprintf("\n the process to be removed is %d",currpid);
                if (currpid ==0)
                   insert(currpid,rdyhead,optr->pprio);
                else{
                   insert(currpid,rdyhead,optr->pprio);
                   my_enq(currpid,optr->pprio);
                   //kprintf("\n pid =%d inserted in",currpid);
                }

        }
        if (optr->pstate == PRSLEEP)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRSUSP)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRRECV)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRWAIT)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRTRECV)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
               if (currpid != 0) {
                  if (optr->pprio > 65)
                  head1++;
                  if ((optr->pprio > 32) && (optr->pprio < 66))
                  head2++;
                  if (optr->pprio <33)
                  head3++;
              }
        /* first need to check if all queues are empty -- if so, we need to run prnull */
           allqsempty();
           if (empty ==1)
               nptr= &proctab[ (currpid = dequeue(NULLPROC)) ];
           else {
           /* get the random value r */
           chooser();
           if (rndm <6)
              nptr = &proctab[ (currpid = dequeue(q1[head1])) ];
           if ((rndm > 5) && (rndm < 9))
              nptr = &proctab[ (currpid = dequeue(q2[head2])) ];
           if (rndm > 8)
              nptr = &proctab[ (currpid = dequeue(q3[head3])) ];
           }


   }
   else if (sched_option ==2) {
           //kprintf("\n entered resched with currpid = %d, preempt = %d",currpid,preempt);
           if ((currpid ==0) && (number_of_proc ==0))
              return(OK);
           //kprintf("\n numberof proc = %d",number_of_proc);
           if (number_of_proc == 0) {
              /* signifies the end of an epoch */
              new_epoch();
           }
           if (optr->unused_ticks ==0) {
              /* make the goodness of that proces zero */
              optr->goodness =0;
           }
           if (optr->pstate== PRCURR) {
              optr->pstate = PRREADY;
              optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
              insert(currpid,rdyhead,optr->pprio);
           }

        if (optr->pstate == PRSLEEP)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRSUSP)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRRECV)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRWAIT)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);
        if (optr->pstate == PRTRECV)
            optr-> clk_ticks = optr-> clk_ticks + (optr-> initial - preempt);

           nptr = &proctab[ (currpid = get_max_goodness() ) ];
   }

           
        nptr->initial = QUANTUM;
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
        if( (sched_option ==1) ||(sched_option ==0))
	   preempt = QUANTUM;		/* reset preemption counter	*/
        else if (sched_option ==2)
           preempt = nptr->unused_ticks; 
#endif
	//restore(ps);
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lab1.h>

int rndm;    /* random value for selecting the queue */
int empty;
int r;

/* Function setschedclass ---- to set the type of scheduling algorithm */


void setschedclass(int choice) {
    /* there are 4 cases: 1.) xinu to random 2.) xinu to linux 3.) random to linux and 4.) random to xinu that are being considered */
    prev = sched_option;
    sched_option =choice ;
     if ((prev ==0) && (sched_option ==2)) {
      /* from xinu to linux scheduling */
      //kprintf("\n xinu to linux ");
      int i;
      for (i=1;i<NPROC;i++) {
         struct pentry *proc = &proctab[i];
         if ((proc->belongs_to_epoch ==0) && ((proc->pstate == PRREADY) || (proc->pstate == PRCURR))) {
            //kprintf("\n pid =%d set",i);
            proc->belongs_to_epoch =1;
            number_of_proc++;
         }
      }
   }
   else if ((prev ==0) && (sched_option == 1)) {
      //kprintf("\n xinu to random scheduling ");
      srand(ctr1000);
   }
   else if ((prev ==1) && (sched_option ==2)) {
      /* random to linux */
      //kprintf("\n random to linux scheduling ");
      int i;
      for (i=1;i<NPROC;i++) {
         struct pentry *proc = &proctab[i];
         if ((proc->belongs_to_epoch ==0) && ((proc->pstate == PRREADY) || (proc->pstate == PRCURR))) {
            //kprintf("\n pid =%d set",i);
            proc->belongs_to_epoch =1;
            proc->goodness = proc->unused_ticks + proc->pprio;
            proc->quant = (proc->unused_ticks/2) + proc->pprio;
            proc->unused_ticks = proc->quant;
            number_of_proc++;
         }
      }
  }
  else if ((prev ==2) && (sched_option ==1)) {
     srand(ctr1000);
     /* linux to random */
     kprintf("\n linux to random scheduling ");
     head1=0;
     len1=0;
     head2=0;
     len2=0;
     head3=0;
     len3=0;
     int i;
     for(i=0;i<NPROC;i++) {
        struct pentry *proc = & proctab[i];
         if (((proc->pstate == PRREADY) || (proc->pstate == PRCURR))  && (i != 0)) {
            my_enq(i,proc->pprio);
            proc->belongs_to_rndm_queue =1;
            //kprintf("\n inserting process %d onto the 3 queue ",i);
         }
      }
  }
 
  resched();
}

/* Function: getschedclass -- to get what scheduling algo is used */

int getschedclass() {
   return(sched_option);
}





/* ------------------------------ LINUX LIKE SCHEDULING FUNCTIONS ---------------------------- */


/* function to get the process with maximum goodness */

int get_max_goodness() {
   int i;
   int pro_id=0;
   int max_goodness=0;
  // kprintf("\n number of proc = %d",number_of_proc);
   for(i=0;i<NPROC;i++) {
      struct pentry *proc = &proctab[i];
      if ((proc->pstate == PRREADY) && (proc->belongs_to_epoch == 1)){
         if (proc-> goodness >= max_goodness) {
            max_goodness = proc->goodness;
            pro_id = i;
         }
      }
   }
//   kprintf("\n the process with the max goodness is %d",pro_id);

   return (dequeue(pro_id));
}



/* Function called when an epoch is over */
void new_epoch() {
 int i;
   for (i=1;i<NPROC;i++) {
      struct pentry *proc = &proctab[i];
      if ((currpid !=0) && (proc->belongs_to_epoch ==1) && ((proc->pstate == PRREADY) || (proc->pstate == PRCURR))) {
         /* for all processes that were already there in the previous epoch goodness is recalculated */
         proc->goodness = proc->unused_ticks + proc->pprio;
         proc->quant = (proc->unused_ticks/2) + proc->pprio;
         proc->unused_ticks = proc->quant;
         number_of_proc++;
      }
      if ((currpid !=0) && (proc->belongs_to_epoch ==0) && (proc->pstate == PRREADY)) {
         //kprintf("\n pid = %d set",i);
         proc->belongs_to_epoch =1;
         number_of_proc++;
      }

   }
   //kprintf("\nnew epoch has started  with number of proc = %d",number_of_proc);
}



/* ------------------------- RANDOM SCHEDULING FUNCTIONS -------------------------- */


int  my_enq(int pid, int prio) {
   //kprintf("\n entered my_enq with pid =%d   priority = %d",pid,prio);
   if (prio > 65) {
   /* put the element in queue 1 */
   if ((len1==0) && (head1==0))
      {
          //kprintf("\n first element of queue 1");
          q1[0]=pid;
          len1++;
      }
   else
      {
          q1[len1]= pid;
          len1++;
      }
   }
   else if ((prio > 32) && (prio < 66)) {
      if ((len2==0) && (head2==0))
      {
        //kprintf("\n first element of queue 2");
        q2[0]=pid;
        len2++;
      }
        else {
        q2[len2]= pid;
        len2++;
      }
   }
   else if (prio < 33){
      if ((len3==0) && (head3==0))
      {
        //kprintf("\n first element of queue 3");
        q3[0]=pid;
        len3++;
      }
      else {
        q3[len3]= pid;
        len3++;
      }
   }

   //kprintf("\n Leaving my_enq");
   //printall();
   return(OK);
}



/* function to check if all queues are empty */


void allqsempty() {
//kprintf("\n entered all queues are empty");
int i,j,k;
i=0;j=0;k=0;
if ((head1 == len1) && (head1 >= 0) && (len1 >= 0))
   i=1;
if ((head2 == len2) && (head2 >= 0) && (len2 >= 0))
   j=1;
if ((head3 == len3) && (head3 >= 0) && (len3 >= 0))
   k=1;
i=i+j+k;
if (i==3){
empty=1;
//kprintf("\n all queues are empty");
//printall();
}
else {
//printall();
//kprintf("\n queues are not empty");
empty =0;
}
}



/* function to choose an appropriate value of r */

void  chooser() {
  // kprintf("\n entered choose r");

   r = rand() % 10 +1;
  // printf("\n the value of r is %d",r);


   if (r < 6) {
      //see if first queue is empty
      if ((head1 == len1) && (head1 >= 0) && (len1 >= 0)) {
         chooser();
      }
      else {
         rndm =r;
      }
   }
   else if (r > 8){
      //see if the third queue is empty
      if ((head3 == len3) && (head3 >= 0) && (len3 >= 0)) {
         chooser();
      }
      else {
         rndm =r;
      }
   }
   else if ((r > 5) && (r < 9)) {
      // see if seond queue is empty
     if ((head2 == len2) && (head2 >= 0) && (len2 >= 0)) {
         chooser();
      }
     else {
        rndm =r;
     }

   }

  // printf("\n the final value of r is %d",r);


}


/* Function to print all 3 queues */
void printall() {

//                kprintf("\n Printing the queues initiallly ");
                int i1;
                kprintf("\n Queue 1\t");
                for(i1=head1;i1<len1;i1++)
                kprintf("\t%d",q1[i1]);

                kprintf("\n Queue 2\t");
                for(i1=head2;i1<len2;i1++)
                kprintf("\t%d",q2[i1]);

                kprintf("\n Queue 3\t");
                for(i1=head3;i1<len3;i1++)
                kprintf("\t%d",q3[i1]);

//kprintf("\n\n");
//kprintf("\n head1 =%d \t len1 = %d",head1,len1);
//kprintf("\n head2 =%d \t len2 = %d",head2,len2);
//kprintf("\n head3 =%d \t len3 = %d",head3,len3);

}


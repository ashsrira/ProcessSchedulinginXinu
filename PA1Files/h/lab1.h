#ifndef _LAB1_H_
#define _LAB1_H_

#define DEFAULTSCHED 0
#define RANDOMSCHED 1
#define LINUXSCHED 2

/* 3 queues : They are defined in the form of 3 arrays */

int q1[500];     /* Queue 1: it can hold 500 at maximum */
int q2[500];     /* Queue 2: same as above */
int q3[500];     /* Queue 3: same as above */

/* Header and lengths of the three queues */
int head1;
int len1;
int head2;
int len2;
int head3;
int len3;

/* variables */
int prev;
int number_of_proc;
int sched_option;


/* extern variables */
extern int rndm;
extern int empty;
extern int r;
extern unsigned long ctr1000;

/* functions */
extern int get_max_goodness();
extern void new_epoch();
extern void setschedclass();
extern void ps();
extern int my_enq(int pid, int prio);
extern void my_dq();
extern void allqsempty();
extern void chooser();
extern void printall();


#endif
 

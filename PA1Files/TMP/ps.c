#include <conf.h> /*All three header files required for proctab */
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
void ps()
{
        int pid,i,c,d,swap;
        int array_ticks[50];
        int array_pid[50];
        int cnt=0;
        for(i=0;i<50;i++){
           array_ticks[i]=-1;
           array_pid[i]=-1;
        }

        for(pid = 0; pid < NPROC; pid++)
        {
                struct pentry *proc = &proctab[pid];

                if(proc -> pstate != PRFREE)
                {
                   array_ticks[cnt]=proc->clk_ticks;
                   array_pid[cnt]=pid;
                   cnt++;
                }
        }
        for (c = 0 ; c < cnt; c++)
        {
                for (d = 0 ; d < cnt - c - 1; d++)
                {
                        if (array_ticks[d] < array_ticks[d+1])
                {

                        swap       = array_ticks[d];
                        array_ticks[d]   = array_ticks[d+1];
                        array_ticks[d+1] = swap;
                        swap       = array_pid[d];
                        array_pid[d]   = array_pid[d+1];
                        array_pid[d+1] = swap;
                }
                }
        }
 for (d=0;d<cnt;d++) {

           for (c=0;c<cnt;c++)
              {
                 if (array_ticks[c] == array_ticks[c+1])
                 {
                    if (array_pid[c] < array_pid[c+1])
                    {
                        swap       = array_ticks[c];
                        array_ticks[c]   = array_ticks[c+1];
                        array_ticks[c+1] = swap;
                        swap       = array_pid[c];
                        array_pid[c]   = array_pid[c+1];
                        array_pid[c+1] = swap;
                    }
                 }
              }
        }

        kprintf("\n\n");
        kprintf("NAME\tPROCESS ID\tPRIORITY\tTICKS\t\tSTATUS\n");
        for (pid=0;pid<cnt;pid++) {
           kprintf("\n");
           int a= array_pid[pid];
           struct pentry *proc = &proctab[a];
           kprintf("%s\t",proc-> pname);
           kprintf("%d\t\t",array_pid[pid]);
           kprintf("%d\t\t",proc->pprio);
           if (proc->clk_ticks < 0)
              proc->clk_ticks = proc->clk_ticks*-1;
           kprintf("%d\t\t",proc->clk_ticks);
           if (proc->pstate == PRCURR)
              kprintf("PRCURR");
           if (proc->pstate == PRFREE)
              kprintf("PRFREE");
           if (proc->pstate == PRREADY)
              kprintf("PRREADY");
           if (proc->pstate == PRRECV)
              kprintf("PRRECV");
           if (proc->pstate == PRSLEEP)
              kprintf("PRSLEEP");
           if (proc->pstate == PRSUSP)
              kprintf("PRSUSP");
           if (proc->pstate == PRWAIT)
              kprintf("PRWAIT");
           if (proc->pstate == PRTRECV)
              kprintf("PRTRECV");

                }
        }





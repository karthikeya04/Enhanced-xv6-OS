#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_freememstat(void){
  return get_freememstat();
}

int sys_getmeminfo(void){
  int pid;

  if(argint(0,&pid) < 0){
    cprintf("sysproc.c::sys_getmeminfo::Error in reading parameters\n");
    return -1;
  }
  if(pid < 0){
    cprintf("Invalid pid: %d\n",pid);
    return -1;
  }
  getmeminfo_(pid);
  return 0;
}

int sys_wait2(void){
  int *wtime,*runtime;
  if(argptr(0,(char**)&wtime,1) < 0 || argptr(1,(char**)&runtime,1)<0){
    cprintf("sysproc.c::sys_getmeminfo::Error in reading parameters\n");
    return -1;
  }
  return wait2_(wtime,runtime);
  
}
#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

static pte_t *
walkpgdir(pde_t *pgdir, const void *va, int alloc)
{
  pde_t *pde;
  pte_t *pgtab;

  pde = &pgdir[PDX(va)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
  } else {
    if(!alloc || (pgtab = (pte_t*)kalloc()) == 0)
      return 0;
    memset(pgtab, 0, PGSIZE);
    *pde = V2P(pgtab) | PTE_P | PTE_W | PTE_U;
  }
  return &pgtab[PTX(va)];
}

int
sys_mprotect(void *addr, int len) 
{
  struct proc *curproc = myproc();
  void *pgdir = curproc->pgdir;
  for (int i = 0; i < len; ++i) {
    pte_t *pt = walkpgdir(pgdir, addr, 0);
    int ipt = (int) pt;
    ipt |= 0x002; // set the writeble value
    ++addr; // increment address 
  }
  return 12;
}

int
sys_munprotect(void *addr, int len) 
{
  struct proc *curproc = myproc();
  void *pgdir = curproc->pgdir;
  for (int i = 0; i < len; ++i) {
    pte_t *pt = walkpgdir(pgdir, addr, 0);
    int ipt = (int) pt;
    ipt &= ~(0x002); // clear the writeble value
    ++addr; // increment address 
  }
  return 12;
}

 int
 sys_hello(void) {
  cprintf("Hello world!\n");
  return 12;
 }

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
  addr = myproc()->vlimit;
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

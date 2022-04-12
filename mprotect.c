#include "types.h"
#include "stat.h"
#include "user.h"
#define NULL (void *)0
 
int 
main(void) {
  printf(1, "Called mprotect\n");
  printf(1, "return val of system call is %d\n", mprotect(NULL, 10));
  exit();
 }
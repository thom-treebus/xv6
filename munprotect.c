#include "types.h"
#include "stat.h"
#include "user.h"
#define NULL (void *)0
 
int 
main(void) {
  printf(1, "Called munprotect\n");
  printf(1, "return val of system call is %d\n", munprotect(NULL, 10));
  exit();
 }
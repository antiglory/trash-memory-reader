// ------------------------- //
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf env;
int garbage_count = 0;

void sigsegv_handler(int signum) {
  printf("[!] signment error! signum: %d\n", signum);
  longjmp(env, 1);
}

void scan_memory() {
  signal(SIGSEGV, sigsegv_handler);

  void *start = malloc(15 * 1024 * 1024); // you can change the range of the memory which will be checked here
  void *end = (char *)start + (15 * 1024 * 1024);

  void *ptr = start;
  while (ptr < end) {
    if (setjmp(env) == 0) {
      volatile char c = *(char *)ptr;
    } else {
      printf("[+] %p      >>> %c\n", ptr, *(char *)ptr);
      garbage_count++;
      printf("[+] waiting client to close..\n");
      getchar();
    }
    ptr = (char *)ptr + 1;
  }

  free(start);

  fflush(stdout);

  if (garbage_count == 0) {
    printf("[!] cannot find\n");
    printf("waiting client to close..\n");
    getchar();
  }
}

int main() {
  scan_memory();
  return 0; // this code will search for garbage in a certain amount of memory, or a stack and prints the memory adress 
            // with his respective value (if there is a garbage forgotten by another program
}

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

void check_memory() {
  signal(SIGSEGV, sigsegv_handler);

  void *start = malloc(15 * 1024 * 1024);
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
  check_memory();
  return 0;
}
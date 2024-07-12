#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#define RANGE 32 * 1024 * 1024

jmp_buf env; // global var, we'll need this to handle errors
int garbage_count = 0;

void sigsegv_handler(int signum) {
    printf("[!] signment error! signum: %d\n", signum);
    longjmp(env, 1);
}

int main() {
    signal(SIGSEGV, sigsegv_handler);
  
    void* start = malloc(RANGE); // you can change the range of the memory which will be checked here
    void* end = (char*)start + (RANGE); // actually is signed 32MB
  
    void* ptr = start;

    while (ptr < end) {
        if (setjmp(env) == 0) {
            volatile char c = *(char*)ptr;
            printf("[+] 00%-*p        : 0x%x\n", ptr, c); // prints the address and the respective value
        } else
            garbage_count++;

        ptr = (char*)ptr + 1;
    }
  
    free(start);
    fflush(stdout);
  
    if (garbage_count == 0) {
        printf("[!] nothing yet\n");
        printf("waiting client to close..\n");

        getchar();
    }

    return 0;
}

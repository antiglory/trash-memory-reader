#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf env; // global var, we'll need this to handle errors
int garbage_count = 0;

void sigsegv_handler(int signum) {
    printf("[!] signment error! signum: %d\n", signum);
    longjmp(env, 1);
}

int main() {
    signal(SIGSEGV, sigsegv_handler);
  
    void* start = malloc(15 * 1024 * 1024); // you can change the range of the memory which will be checked here
    void* end = (char *)start + (15 * 1024 * 1024); // actually is signed 15MB
  
    void* ptr = start;
    while (ptr < end) {
        if (setjmp(env) == 0) {
            volatile char c = *(char *)ptr;
            printf("[+] 00%-*p        >>> %d\n", ptr, c); // prints the mem addr and the respective value (assigned to the mem addr)
        } else {
            garbage_count++;
        }
        ptr = (char *)ptr + 1;
    }
  
    free(start);
    fflush(stdout);
  
    if (garbage_count == 0) {
        printf("[!] nothing yet\n");
        printf("waiting client to close..\n");
        getchar();
    }
}

int main() {
    query();
    return 0;
}

#include "../../shit.h"

int main(void) {
    srand((unsigned)time(NULL) ^ _getpid() ^ clock() ^ 12345);
    int machine2 = rand() % 3 + 1;
    // printf("Process ID for Machine 2: %d\n", _getpid());
    printf("Machine 2 chooses: ");
    switch (machine2) {
        case 1: puts("scissor."); break;
        case 2: puts("stone.");   break;
        case 3: puts("paper.");   break;
    }
    return machine2; 
}
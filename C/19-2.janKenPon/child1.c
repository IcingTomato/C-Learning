#include "../shit.h"

int main(void) 
{
    srand((unsigned)time(NULL) ^ _getpid() ^ clock());
    int machine1 = rand() % 3 + 1;
    // printf("Process ID for Machine 1: %d\n", _getpid());
    printf("Machine 1 chooses: ");
    switch (machine1) {
        case 1: puts("scissor."); break;
        case 2: puts("stone.");   break;
        case 3: puts("paper.");   break;
    }
    return machine1;
}
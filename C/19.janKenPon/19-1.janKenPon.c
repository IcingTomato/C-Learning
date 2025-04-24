#include "../shit.h"

int machine1, machine2;

void* generate_machine1(void* arg) 
{
    srand((unsigned)time(NULL) ^ _getpid() ^ clock());
    machine1 = rand() % 3 + 1;
    printf("Thread ID for Machine 1: %lu\n", pthread_self());
    return NULL;
}

void* generate_machine2(void* arg) 
{
    srand((unsigned)time(NULL) ^ _getpid() ^ clock() ^ 12345); 
    machine2 = rand() % 3 + 1;
    printf("Thread ID for Machine 2: %lu\n", pthread_self());
    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2;

    do 
    {
        pthread_create(&thread1, NULL, generate_machine1, NULL);
        pthread_create(&thread2, NULL, generate_machine2, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        printf("Machine 1 chooses: ");
        switch (machine1)
        {
            case 1 : puts("scissor.");  break;
            case 2 : puts("stone.");    break;
            case 3 : puts("paper.");    break;
        }

        printf("Machine 2 chooses: ");
        switch (machine2)
        {
            case 1 : puts("scissor.");  break;
            case 2 : puts("stone.");    break;
            case 3 : puts("paper.");    break;
        }

        int result = machine1 - machine2;

        switch (result)
        {
            case  0 : puts("Tie.");         break;
            case -1 :
            case  2 : puts("Machine 2 wins!"); break;
            default : puts("Machine 1 wins!");  break;
        }

        printf("\n");
        break;

    } while (1);

    return 0;
}
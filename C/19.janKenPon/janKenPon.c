#include "../shit.h"

int main(void)
{
    char retry[100];
    int people, machine;

    do
    {
        srand((unsigned)time(NULL));
        machine = rand() % 3 + 1;

        printf("Enter your choice (1[Scissor], 2[Stone], 3[Paper]): ");
        while (scanf("%d", &people) != 1 || people < 1 || people > 3) 
        {
            while (getchar() != '\n');
            printf("Invalid input. Please re-enter your choice (1[Scissor], 2[Stone], 3[Paper]): ");
        }
        
        printf("You choose: ");
        switch (people)
        {
            case 1 : puts("scissor.");  break;
            case 2 : puts("stone.");    break;
            case 3 : puts("paper.");    break;
        }

        printf("Machine chooses: ");
        switch (machine)
        {
            case 1 : puts("scissor.");  break;
            case 2 : puts("stone.");    break;
            case 3 : puts("paper.");    break;
        }

        int result = people - machine;

        switch (result)
        {
            case  0 : puts("Tie.");         break;
            case -1 :
            case  2 : puts("Machine win!"); break;
            default : puts("People win!");  break;
        }
        
        printf("Retry? [y(es)/n(o)] :"); scanf("%100s", retry);
        
        while (strcmp(retry, "yes") != 0 && strcmp(retry, "y") != 0 && strcmp(retry, "no") != 0 && strcmp(retry, "n") != 0) 
        {
            printf("Invalid input. Please enter [y(es)/n(o)] :");
            scanf("%100s", retry);
        }
    } while (strcmp(retry, "no") != 0 && strcmp(retry, "n") != 0);
    return 0;
}
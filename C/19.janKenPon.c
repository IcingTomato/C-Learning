#include "shit.h"

int main(void)
{
    char retry[4];
    int people, machine;

    do
    {
        srand((unsigned)time(NULL));
        machine = rand() % 3 + 1;

        printf("Enter your choise (1[Scissor], 2[Stone], 3[Paper]): "); scanf("%d", &people);

        while (people < 1 || people > 3)
        {
            printf("Please re-enter your choise (1[Scissor], 2[Stone], 3[Paper]): "); scanf("%d", &people);
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
        printf("Retry? [y(es)/n(o)] :"); scanf("%3s", retry);
    } while (strcmp(retry, "no") != 0 && strcmp(retry, "n") != 0);

    return 0;
}
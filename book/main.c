#include <stdio.h>
#include <string.h>

#define NAME_LENGTH 25
#define BOOK_LENGTH 15

struct book{
    char surname[NAME_LENGTH];
    char name[NAME_LENGTH];
    char second_name[NAME_LENGTH];
    char number[NAME_LENGTH];
};

int main()
{
    struct book book[BOOK_LENGTH];
    int choice, n, I=0, life_cycle=1;
    char find[NAME_LENGTH];

    printf("\tNote book\n");

    while(life_cycle)
    {
        printf("\n1 - create\n2 - look\n3 - delete\n4 - search\n5 - exit\nChoose the action: ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice)
        {
            case 1: 
                printf("%s", "Surname: ");
                scanf("%s", book[I].surname);

                printf("%s", "name: ");
                scanf("%s", book[I].name);

                printf("%s", "second name: ");
                scanf("%s", book[I].second_name);

                printf("%s", "namber: ");
                scanf("%s", book[I].number);

                I++;
                break;
            case 2: 
                for(int i=0; i<BOOK_LENGTH; i++){
                    printf("%d. Surname: %s\n", i+1, book[i].surname);
                    printf("   name: %s\n", book[i].name);
                    printf("   second name: %s\n", book[i].second_name);
                    printf("   number: %s\n", book[i].number);
                    printf("\n");
                }
                break;
            case 3: 
                printf("index: ");
                scanf("%d", &n);
                
                memset (book[n-1].surname, 0, NAME_LENGTH); 
                memset (book[n-1].name, 0, NAME_LENGTH);
                memset (book[n-1].second_name, 0, NAME_LENGTH);
                memset (book[n-1].number, 0, NAME_LENGTH);
                break;
            case 4: 
            printf("Enter a surname: ");
            scanf("%s", &find);

            for(int i=0; i< NAME_LENGTH; i++){
                if (strcmp (find, book[i].surname)==0){
                    printf("\n%d. Surname: %s\n", i+1, book[i].surname);
                    printf("   name: %s\n", book[i].name);
                    printf("   second name: %s\n", book[i].second_name);
                    printf("   number: %s\n", book[i].number);
                    printf("\n");
                    break;
                }
            }
            break;
            case 5: life_cycle = 0; break;

            default: printf("\nError! Try again\n\n");
        }
    }

    return 0;
}

/*
    TODO

\/ 1. front-end
\/2. структуры
    \/ 2.1 добавление
    \/ 2.2 просмотр
    \/ 2.3 удаление
    \/ 2.4 поиск
\/ 3. логика
*/
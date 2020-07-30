#include <stdio.h>

float a, b;

void get_ab(){
    printf("a: ");
    scanf("%f", &a);
    printf("b: ");
    scanf("%f", &b);
}

float sum(float a, float b){ return a + b; }
float min(float a, float b){ return a - b; }
float mul(float a, float b){ return a * b; }
float div(float a, float b){ return a / b; }

int main(){
    
    int sym, cycle=1;
    float res;

    printf("\tCalculator\n");
    printf(" 1 - +\n 2 - -\n 3 - *\n 4 - /\n 0 - exit\n");

    while(cycle == 1){
        printf("\nchoose the symbol: ");
        scanf("%d", &sym);

        switch(sym){
            case 1: 
                get_ab();
                printf("result: %f\n", sum(a, b)); 
                break;
            case 2:
                get_ab();
                printf("result: %f\n", min(a, b));
                break;
            case 3:  
                get_ab();
                printf("result: %f\n", mul(a, b));
                break;
            case 4:  
                get_ab();
                if(b == 0) printf("\nDivision by zero!\n"); break;
                printf("result: %f\n", div(a, b));
                break;
            case 0: cycle = 0; break;
            default: printf("Error! Try again.\n"); break;
        }
    }
}
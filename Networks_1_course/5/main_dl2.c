#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){

    void *library_handler;
    int (*Mfunc)(int x, int y);	// переменная для хранения адреса функции
    int (*Dfunc)(int x, int y);	// переменная для хранения адреса функции

    library_handler = dlopen("./libFuncdyn.so", RTLD_LAZY);
    if (!library_handler){
        //если ошибка, то вывести ее на экран
        fprintf(stderr,"dlopen() error: %s\n", dlerror());
        exit(1); // в случае ошибки можно, например, закончить работу программы
    };

    //загружаем из библиотеки требуемую процедуру
	Mfunc = dlsym(library_handler, "multiplication");
	Dfunc = dlsym(library_handler, "division");

    int x, y, res;
    
    printf("Enter x: ");
    scanf("%d", &x);
    printf("Enter y: ");
    scanf("%d", &y);

    printf("multiplication: %d\n", (*Mfunc)(x, y));
    printf("division: %d\n", (*Dfunc)(x, y));
    
    //закрываем библиотеку
	dlclose(library_handler);
}

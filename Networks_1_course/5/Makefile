dl1:
	clear && source ./path.sh && gcc -c main_dl1.c && gcc main_dl1.o -L. -lFuncdyn -o main_dl1
dl2:
	clear && gcc main_dl2.c -o main_dl2 -ldl
static:
	clear && gcc main.c -o main -L. -lFunc
	
clean:
	rm -rf main_dl1 main_dl2 main

#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <errno.h>

DIR *d;
struct dirent *dir;
char *dir_arr[255];
char new_path[255];
char path[255]={"/"};

int scaner(char *path){
	int f=0;
	
	d = opendir(path);
	if( d == NULL ){ perror("opendir"); return 1; }

	while((dir = readdir(d))){
		if(strcmp( dir->d_name, "." ) == 0 || 
			strcmp( dir->d_name, ".." ) == 0){
			continue;
	}
		if(dir->d_type == DT_DIR){
			dir_arr[f] = dir->d_name;
			printf("%s\n", dir_arr[f]);
			f++;
		} 
	}
	closedir(d);
}

int main(){
	int c;
		
 	scaner(path);
	
	printf("\nchoice a folder: ");
	scanf("%d", &c);

	snprintf(new_path, sizeof new_path, "%s%s\0", path, dir_arr[c-1]);
	printf("new_path: %s\n\n", new_path);
	scaner(new_path);

  return 0;
}

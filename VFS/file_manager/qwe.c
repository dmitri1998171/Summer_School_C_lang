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
char *path="/";
char *new_path;
char *tree_path;

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
	
	// strcpy(&new_path, &path);
 	scaner(path);
	printf("\n");
	scanf("%d", &c);
	snprintf(&new_path, sizeof new_path, "%s%s\0", path, dir_arr[c-1]);
	// strcat(path, dir_arr[c-1]);
	// strcat();
	printf("new_path: %s\n", &new_path);
	
	// chdir(new_path);
	// getcwd(path, 100);
	scaner(new_path);

  return 0;
}

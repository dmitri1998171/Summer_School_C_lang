#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 

DIR *d;
struct dirent *dir;
char *dir_arr[255];
char *path="/";
char *new_path;
char *tree_path;

int scaner(){
	int i=0, f=0;

	// path = new_path;
	// strcpy(path, new_path);
	d = opendir(path);
	if( d == NULL ){ return 1; }

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
		i++;
	}
	printf("\nf: %d\n", f);
	closedir(d);
}

int main(){
	int c;
	
	// strcpy(tree_path, path);

 	scaner();
	printf("\n");
	scanf("%d", &c);
	// strcpy(path, dir_arr[c-1]);
	snprintf(new_path, sizeof new_path, "%s%s", path, dir_arr[c-1]);
	printf("dir_arr: %s\n", new_path);
	
	// chdir(new_path);
	// getcwd(path, 100);
	// scaner();

  return 0;
}

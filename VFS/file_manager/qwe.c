#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 

int walker( char *searching, char *result ) {
  DIR           *d;
  struct dirent *dir;
  d = opendir( "." );
  if( d == NULL ) {
    return 1;
  }
  while( ( dir = readdir( d ) ) ) {
    if( strcmp( dir->d_name, "." ) == 0 || 
        strcmp( dir->d_name, ".." ) == 0 ) {
      continue;
    }
    if( dir->d_type == DT_DIR ) {
      chdir( dir->d_name );
      walker( searching, result );
      chdir( ".." );
    } else {
      if( strcmp( dir->d_name, searching ) == 0 ) {
        int  len;
        getcwd( result, MAXPATHLEN );
        len = strlen( result );
        snprintf( result + len, MAXPATHLEN - len, "/%s", dir->d_name );
        break;
      }
    }
  }
  closedir( d );
  return *result == 0;
}

int main( ) {
  char buf[MAXPATHLEN] = { 0 };
  if( walker( "qwe.c", buf ) == 0 ) {
    printf( "Found: %s\n", buf );
  } else {
    puts( "Not found" );
  }
  return 0;
}

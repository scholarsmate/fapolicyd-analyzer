#include <stdio.h> 
#include <sqlite3.h>
	
int main(int argc, char * argv[])
{
	printf("SQLite library version: %s\n", sqlite3_libversion()); 
	return 0;
}


#include <stdio.h>
int main()
{
    int t;
    printf("\n\r Hello world using C Lib :)");
    scanf("%d",&t);
	FILE * fp=fopen("test","rb");
	fclose(fp);
	feof(fp);
	putenv( "adasd" );
    printf("\n\r Result %d", t);
    asm("int $1");
}

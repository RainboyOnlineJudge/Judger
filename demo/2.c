#include <stdio.h>
#include <string.h>


char mem[4096*1024];
int main(int argc, char *argv[]) {
    memset(mem,0,sizeof(mem));
    char input[1000];
    scanf("%s", input);
    printf("Hello %s\n", input);
    return 0;
}

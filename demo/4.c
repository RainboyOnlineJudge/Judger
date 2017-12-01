#include <stdio.h>
#include <string.h>

int add(int a,int b){
    return a+b;
}

char mem[4096*1024];
int main(int argc, char *argv[]) {
    memset(mem,0,sizeof(mem));
    char input[1000];
    scanf("%s", input);
    int i,j;
    int a=1;
    for(i=0;i<999;i++)
        for(j=0;j<99999;j++){
            a = add(a,8);
        }
    printf("Hello %s\n", input);
    return 0;
}

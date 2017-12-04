#include <stdio.h>
#include <errno.h>
int main() {
    FILE *f = fopen("/home/dc2-user/Judger/demo/fs", "w");
    if(f == NULL) {
        perror("error:");
        return 10;
    }
    int i;
    for(i = 0;i < 10000; i++) {
        if (fprintf(f, "%s", "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") <= 0) {
            return 2;
        }
    }
        printf("????\n");
    fclose(f);
        printf("????\n");
    return 3;
}

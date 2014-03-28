#include <stdio.h>
#include <stdlib.h>

int main() {

    FILE *f;
    char c;
    char* fname = "test.txt";
    if ((f=fopen(fname, "rt")) == NULL)
    {
        printf("File %s not found\n",fname);
        return 1;
    }
    else
    {
        while((c=fgetc(f))!=EOF){
            printf("%c",c);
        }
    }

    fclose(f);
    return 0;
}



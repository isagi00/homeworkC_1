
#include <stdio.h>
#include "../supporto.h"


int main(){
    char* str = "       hello     ";
    char* str2 = "       hello      world;   ";

    char* clean = eliminaSpaziDxSx_v2(str2);
    
    printf("%s\n",clean);

    return 0;
}

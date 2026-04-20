#include <stdio.h>
#include <string.h>

char* el(char* str) {
    int number = strlen(str);
    static char buffer[1024];
    static char buffer_ultim_pulito_pulito_super_pulito_overturned[1024];
    _Bool start_delete_space = 0;
    int position_for_buffer = 0;

    for(int i = 0; i < number; i++) {
        printf("%s\t%d\n",buffer,i);
        if(str[i] == ',' || str[i] == '=') {
            start_delete_space = 1;
        } else if(str[i] != ' ' && str[i] != '\t') {
            start_delete_space = 0;
        }
        printf("%d\n",start_delete_space);
        if(start_delete_space == 1 && (str[i] == ' ' || str[i] == '\t')) {
            printf("salta\n");
            continue;
        }
        buffer[position_for_buffer] = str[i];
        printf("%s\t%d\n",buffer,i);
        position_for_buffer++;
    }
    buffer[position_for_buffer] = '\0';
    position_for_buffer = 0;
    start_delete_space = 0;  // ← FIX 1: reset del flag

    int len_buffer = strlen(buffer);
    for(int i = len_buffer - 1; i >= 0; i--) {
        if(buffer[i] == ',' || buffer[i] == '=') {
            start_delete_space = 1;
        }
         else         if(buffer[i] != ' ' && buffer[i] != '\t') {
            start_delete_space = 0;
        }
        if(start_delete_space == 1 && (buffer[i] == ' ' || buffer[i] == '\t')) {
            continue;
        }

        buffer_ultim_pulito_pulito_super_pulito_overturned[position_for_buffer] = buffer[i];
        position_for_buffer++;
    }
    buffer_ultim_pulito_pulito_super_pulito_overturned[position_for_buffer] = '\0';

    int len = position_for_buffer;
    for(int i = 0; i < len / 2; i++) {
        char tmp = buffer_ultim_pulito_pulito_super_pulito_overturned[i];
        buffer_ultim_pulito_pulito_super_pulito_overturned[i] = buffer_ultim_pulito_pulito_super_pulito_overturned[len - 1 - i];
        buffer_ultim_pulito_pulito_super_pulito_overturned[len - 1 - i] = tmp;
    }   

    return buffer_ultim_pulito_pulito_super_pulito_overturned;
}
int main() {
    char l[] = "long       long       int          a = 1, b = 2,          ao=                         2";
    char *lp = el(l);
    printf("%s\n", lp);
}
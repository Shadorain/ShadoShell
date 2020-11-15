#include "stdio.h"
#include "string.h"
#include "shadosh.h"

int hist_write(char **line) {
    FILE *hist = NULL;
    char *str;
    int len = 0;

    while (line[++len] != NULL);
    str = stringify(line, len);

    hist = fopen(hist_file,"a");
    if(hist == NULL) {
        printf("Failed to write to history file: %s\n", hist_file);
        return 1;
    } else {
        fputs(str,hist);
        fclose(hist);
    }
    return 0;
}

int del_hist() {
    return remove(hist_file);
}

#include <stdio.h>
#include <sys/time.h>

// Simple stopwatch program that displays from hours to milliseconds
// 2014 Michael Caldwell

int main(){
    int hours=0, minutes=0, seconds=0, milliseconds=0;
    struct timeval start, end;

    gettimeofday(&start, NULL);

    while (1){
        usleep(1000);

        gettimeofday(&end, NULL);
        unsigned long long base=(end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);

        milliseconds = base / 1000 % 1000;
        seconds      = base / 1000000 % 60;
        minutes      = base / 1000000 / 60 % 60;
        hours        = base / 1000000 / 60 / 60 % 24;

        printf("\r%02i:%02i:%02i:%03i",hours, minutes, seconds, milliseconds);
        fflush(stdout);
    }
    return (0);
}

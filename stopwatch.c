// -*- compile-command: "gcc -o stopwatch stopwatch.c -Wall" -*-
// Simple stopwatch program that displays from hours to milliseconds
// 2014 Michael Caldwell

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>

// Returns amount of milliseconds between two times
unsigned long long calcMilliseconds(struct timeval start, struct timeval end)
{
    unsigned long long microseconds=(end.tv_sec * 1000000ULL + end.tv_usec) - (start.tv_sec * 1000000ULL + start.tv_usec);
    unsigned long long milliseconds=microseconds / 1000;
    return milliseconds;
}

int main()
{
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    struct termios termios;
    tcgetattr(STDIN_FILENO, &termios);
    termios.c_lflag = termios.c_lflag & ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &termios);

    int total_hours=0, total_minutes=0, total_seconds=0, total_milliseconds=0;
    int lap_hours=0, lap_minutes=0, lap_seconds=0, lap_milliseconds=0;
    int lapcount=1;

    struct timeval start, lap, end;

    gettimeofday(&start, NULL);
    gettimeofday(&lap, NULL);

    while (1){
        char ch=' ';
        int status = read(STDIN_FILENO, &ch, 1);

        if (status == 0){  // Bad status from read
            printf("Read Error: status=%d\n", status);
        } else if (ch == 0x4){  //Exit with C^D
            printf("\n");
            break;
        } else if (ch == '\n'){  // Advance lap with return key
            gettimeofday(&lap, NULL);
            lapcount++;
        } else {  // Overwrite user input
            printf("  ");
        }

        gettimeofday(&end, NULL);

        // Calculate Total Time
        unsigned long long total_base=calcMilliseconds(start, end);
        total_milliseconds = total_base % 1000;
        total_seconds      = total_base / 1000 % 60;
        total_minutes      = total_base / 1000 / 60 % 60;
        total_hours        = total_base / 1000 / 60 / 60 % 24;

        // Calculate Lap Time
        unsigned long long lap_base=calcMilliseconds(lap, end);
        lap_milliseconds = lap_base % 1000;
        lap_seconds      = lap_base / 1000 % 60;
        lap_minutes      = lap_base / 1000 / 60 % 60;
        lap_hours        = lap_base / 1000 / 60 / 60 % 24;

        printf("\rTotal Time: %02i:%02i:%02i:%03i   Lap %2i: %02i:%02i:%02i:%03i",
               total_hours, total_minutes, total_seconds, total_milliseconds, lapcount,
               lap_hours,   lap_minutes,   lap_seconds,   lap_milliseconds);
        fflush(stdout);

        usleep(1000);
    }
    return 0;
}

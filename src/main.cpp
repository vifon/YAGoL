// File: main.cpp
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include "yagol_model.hpp"

void wait ( unsigned short ms )
{
	timespec req = { 0, ms * 1000000 };

	nanosleep ( &req, NULL );
}


int main(int argc, char *argv[]) {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    YAGoLModel g(w.ws_col, w.ws_row, "23", "3");
    std::srand(time(NULL)+getpid());

    g.randomize(10);
    while (true) {
        g.next_generation();
        wait(25);
    }

    return 0;
}

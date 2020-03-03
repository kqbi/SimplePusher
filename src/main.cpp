#include <signal.h>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>
#include "Read/MP4Reader.h"

static bool finished = false;

static void signalHandler(int signo) {
    std::cerr << "Shutting down" << std::endl;
    finished = true;
}

int main(int argc, char *argv[]) {
    MP4Reader::Ptr pReader(new MP4Reader("1.mp4", "rtsp://127.0.0.1:554/live/11"));
    if (pReader->startReadMP4()) {
        std::cerr << "play failed" << std::endl;
    }

    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        std::cerr << "Couldn't install signal handler for SIGINT" << std::endl;
        exit(-1);
    }

    if (signal(SIGTERM, signalHandler) == SIG_ERR) {
        std::cerr << "Couldn't install signal handler for SIGTERM" << std::endl;
        exit(-1);
    }

    while (!finished) {
#ifdef WIN32
        Sleep(1000);
#else
        usleep(1000*1000);
#endif
    }
    return 0;
}

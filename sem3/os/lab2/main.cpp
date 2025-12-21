#include <iostream>
#include <iomanip>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

static void print_now(const char* tag) {
    timespec ts{};
    clock_gettime(CLOCK_REALTIME, &ts);
    std::tm tmv{};
    localtime_r(&ts.tv_sec, &tmv);
    long ms = ts.tv_nsec / 1000000L;

    std::cout << "[" << tag << "] "
              << "pid=" << getpid()
              << " ppid=" << getppid()
              << " time=" << std::setfill('0')
              << std::setw(2) << tmv.tm_hour << ":"
              << std::setw(2) << tmv.tm_min  << ":"
              << std::setw(2) << tmv.tm_sec  << ":"
              << std::setw(3) << ms
              << std::endl;
}

int main() {
    print_now("parent-start");

    pid_t c1 = fork();
    if (c1 == 0) {
        print_now("child-1");
        sleep(1);
        _exit(0);
    } else if (c1 < 0) {
        perror("fork #1");
        return 1;
    }

    pid_t c2 = fork();
    if (c2 == 0) {
        print_now("child-2");
        sleep(1);
        _exit(0);
    } else if (c2 < 0) {
        perror("fork #2");
        return 1;
    }

    usleep(150 * 1000);
    print_now("parent-before-ps");
    system("ps -x");

    int status;
    waitpid(c1, &status, 0);
    waitpid(c2, &status, 0);

    return 0;
}
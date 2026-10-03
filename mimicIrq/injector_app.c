#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "hw_sim_common.h"

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "--setup-only") == 0) {
        create_node();
        destroy_node();
        return 0;
    }

    // 1. Ensure a clean node exists for the test run
    destroy_node(); 
    create_node();

    // Fire the interrupt vector down the pipe line
    int fd = open(SIM_HW_PATH, O_WRONLY);
    if (fd < 0) {
        printf("Failed to open device:%s, return errno:(%d) - \"%s\"\n", SIM_HW_PATH, errno, strerror(errno));
        return 1; // Test Failed
    }

    unsigned int test_vector = 0xDEADBEE0;
    while (test_vector++ != 0xDEADBEEF) {
        write(fd, &test_vector, sizeof(test_vector));
        printf("[Injector] Fired interrupt vector 0x%08X\n", test_vector);
        fflush(stdout);
        usleep(1000); // yield cpu
    }
    close(fd);

    return 0;
}

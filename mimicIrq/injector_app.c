#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "hw_sim_common.h"

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "--setup-only") == 0) {
        create_node();
        destroy_node();
        return 0;
    }

    // Fire the interrupt vector down the pipe line
    int fd = open(SIM_HW_PATH, O_WRONLY);
    if (fd < 0) return 1;

    unsigned int test_vector = 0xDEADBEEF;
    write(fd, &test_vector, sizeof(test_vector));
    usleep(0); // yield cpu
    close(fd);

    printf("[Injector] Fired interrupt vector 0xDEADBEEF\n");
    fflush(stdout);
    return 0;
}

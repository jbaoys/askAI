#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "hw_sim_common.h"

int main(void) {
    // 1. Ensure a clean node exists for the test run
    destroy_node(); 
    create_node();

    printf("[Listener] Opening %s (blocking mode)...\n", SIM_HW_PATH);
    int fd = open(SIM_HW_PATH, O_RDONLY);
    if (fd < 0) return 1; // Test Failed

    unsigned int interrupt_vector = 0;
    printf("[Listener] Blocking on read()...\n");

    while (interrupt_vector != 0xDEADBEEF) {
        // Natively blocks here across both Linux and VxWorks
        ssize_t bytes_read = read(fd, &interrupt_vector, sizeof(interrupt_vector));
        if (bytes_read == sizeof(interrupt_vector)) {
            printf("[SUCCESS] Received expected vector: 0x%08X\n", interrupt_vector);
            fflush(stdout);
        }
    }

    close(fd);
    destroy_node();
    return 0; // Test Passed!
}

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#ifdef TARGET_OS_VXWORKS
#include <pipeDrv.h>
#define SIM_HW_PATH "/pipe/mock_interrupt_device"
#else
#define SIM_HW_PATH "/tmp/mock_interrupt_device"
#endif


void create_node(void) {
#ifdef TARGET_OS_VXWORKS
    pipeDevCreate(SIM_HW_PATH, 10, sizeof(unsigned int));
#else
    mkfifo(SIM_HW_PATH, 0666);
#endif
}

void destroy_node(void) {
#ifdef TARGET_OS_VXWORKS
    pipeDevDelete(SIM_HW_PATH, 1);
#else
    unlink(SIM_HW_PATH);
#endif
}

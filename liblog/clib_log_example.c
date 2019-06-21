#include <stdio.h>
#include "clib_log.h"

int main() {
    int ret = 0;
    ret = log_init(LOG_DEBUG, LOG_ROTATE_TRUE, 2 << 20,
                 NULL, NULL, NULL);
    return 0;
}

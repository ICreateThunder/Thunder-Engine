#include <core/logger.h>

int main(void) {
    KFATAL("Something went wrong: %f", 3.14f);
    KWARN("Something went wrong: %f", 3.14f);
    KINFO("Something went wrong: %f", 3.14f);
    KDEBUG("Something went wrong: %f", 3.14f);
    KTRACE("Something went wrong: %f", 3.14f);
    return 0;
}
#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    KFATAL("Something went wrong: %f", 3.14f);
    KERROR("Something went wrong: %f", 3.14f)
    KWARN("Something went wrong: %f", 3.14f);
    KINFO("Something went wrong: %f", 3.14f);
    KDEBUG("Something went wrong: %f", 3.14f);
    KTRACE("Something went wrong: %f", 3.14f);

    KASSERT_MSG(1 == 1, "Testing 1 is 1");
    KASSERT_MSG(0 == 1, "Testing 0 is 1");
    return 0;
}
#include "linear_allocator_tests.h"
#include "../test_manager.h"
#include "../expect.h"

#include <memory/linear_allocator.h>

u8 linear_allocator_should_create_and_destroy() {
    linear_allocator alloc;
    linear_allocator_create(sizeof(u64), 0, &alloc);

    expect_should_not_be(0, alloc.memory);
    expect_should_be(sizeof(u64), alloc.total_size);
    expect_should_be(0, alloc.allocated);

    linear_allocator_destroy(&alloc);
    expect_should_be(0, alloc.memory);
    expect_should_be(0, alloc.total_size);
    expect_should_be(0, alloc.allocated);

    return true;
}

u8 linear_allocator_single_allocation_all_space() {
    linear_allocator alloc;
    linear_allocator_create(sizeof(u64), 0, &alloc);

    void* block = linear_allocator_allocate(&alloc, sizeof(u64));

    expect_should_not_be(0, block);
    expect_should_be(sizeof(u64), alloc.allocated);

    linear_allocator_destroy(&alloc);

    return true;
}

u8 linear_allocator_multi_allocation_all_space() {
    u64 max_allocs = 1024;
    linear_allocator alloc;
    linear_allocator_create(sizeof(u64) * max_allocs, 0, &alloc);

    void* block;
    for (u64 i = 0; i < max_allocs; ++i) {
        block = linear_allocator_allocate(&alloc, sizeof(u64));
        expect_should_not_be(0, block);
        expect_should_be(sizeof(u64) * (i + 1), alloc.allocated);
    }

    linear_allocator_destroy(&alloc);

    return true;
}

u8 linear_allocator_multi_allocation_over_allocate() {
    u64 max_allocs = 3;
    linear_allocator alloc;
    linear_allocator_create(sizeof(u64) * max_allocs, 0, &alloc);
    
    void* block;
    for (u64 i = 0; i < max_allocs; ++i) {
        block = linear_allocator_allocate(&alloc, sizeof(u64));
        expect_should_not_be(0, block);
        expect_should_be(sizeof(u64) * (i + 1), alloc.allocated);
    }

    KDEBUG("Note: The following error is intentionally caused by this test");

    block = linear_allocator_allocate(&alloc, sizeof(u64));
    expect_should_be(0, block);
    expect_should_be(sizeof(u64) * (max_allocs), alloc.allocated);

    linear_allocator_destroy(&alloc);

    return true;
}


u8 linear_allocator_multi_allocation_all_space_then_free() {
    u64 max_allocs = 1024;
    linear_allocator alloc;
    linear_allocator_create(sizeof(u64) * max_allocs, 0, &alloc);

    void* block;
    for (u64 i = 0; i < max_allocs; ++i) {
        block = linear_allocator_allocate(&alloc, sizeof(u64));
        expect_should_not_be(0, block);
        expect_should_be(sizeof(u64) * (i + 1), alloc.allocated);
    }

    linear_allocator_free_all(&alloc);
    expect_should_be(0, alloc.allocated);

    linear_allocator_destroy(&alloc);

    return true;
}

void linear_allocator_register_tests() {
    test_manager_register_test(linear_allocator_should_create_and_destroy, "Linear allocator SHOULD create and destroy");
    test_manager_register_test(linear_allocator_single_allocation_all_space, "Linear allocator SHOULD allocate for all space in total/max size of allocator via single allocate");
    test_manager_register_test(linear_allocator_multi_allocation_all_space, "Linear allocator SHOULD allocate for each space in total/max size of allocator");
    test_manager_register_test(linear_allocator_multi_allocation_over_allocate, "Linear allocator SHOULD NOT allocate over total/max size of allocator");
    test_manager_register_test(linear_allocator_multi_allocation_all_space_then_free, "Linear allocator SHOULD reset allocated amount to allow writes from beginning");
}
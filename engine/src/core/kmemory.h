#pragma once

#include "defines.h"

typedef enum memory_tag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_LINEAR_ALLOCATOR,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    MEMORY_TAG_MAX_TAGS
} memory_tag;

KAPI void initialise_memory(u64* memory_requirement, void* state);
KAPI void shutdown_memory(void* state);

KAPI void *kallocate(u64 size, memory_tag tag);
KAPI void kfree(void *block, u64 size, memory_tag tag);
KAPI void *kzero_memory(void *block, u64 size);
KAPI void *kcopy_memory(void *dest, const void *src, u64 size);
KAPI void *kset_memory(void *dest, i32 value, u64 size);

KAPI char *get_memory_usage_str();

KAPI u64 get_memory_alloc_count();
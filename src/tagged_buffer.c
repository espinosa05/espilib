#include "tagged_buffer.h"
#include <stdlib.h> /* malloc */
#include <assert.h>

/* TODO: move this someday into a 'platform' module */
#define X86_PAGE_SIZE (1 << 12)
#define PAGE_SIZE X86_PAGE_SIZE

#define ALIGN_TO_WORD(size) ( (size) + (sizeof(void *) - (size) % sizeof(void *)) * ((size) % sizeof(void *) != 0) )
#define ALIGN_TO_NEXT_PAGE(size) ( (size) + (PAGE_SIZE - (size) % PAGE_SIZE) * ((size) % PAGE_SIZE != 0) )

/* *
 * all this stupid optimization may not even do anything useful...
 * i'll have to benchmark it some day
 * */
TB_Status tb_alloc_buffer(struct tagged_buffer **tb, struct tb_alloc_info info)
{
        assert(tb != NULL);
        assert(*tb != NULL);

        ssize_t type_memory_size = ALIGN_TO_WORD(sizeof(struct tagged_buffer));
        ssize_t alloc_size = type_memory_size + info.width * info.count;

        if (alloc_size % PAGE_SIZE >= PAGE_SIZE/2)
                alloc_size = ALIGN_TO_NEXT_PAGE(alloc_size);

        *tb = malloc(alloc_size);

        (*tb)->width = info.width;
        (*tb)->count = info.count;
        (*tb)->base = (void *)tb + type_memory_size;

        return TB_STATUS_SUCCESS;
}

TB_Status tb_free_buffer(struct tagged_buffer *tb)
{
        assert(tb);

        free(tb);

        return TB_STATUS_SUCCESS;
}

TB_Status tb_assign(struct tagged_buffer **tb, void *base,
                    struct tb_alloc_info info)
{
        assert(tb != NULL);
        assert(*tb != NULL);
        assert(base != NULL);

        (*tb)->width = info.width;
        (*tb)->count = info.count;
        (*tb)->base = base;

        return TB_STATUS_SUCCESS;
}

TB_Status tb_free_assigned(struct tagged_buffer *tb)
{
        free(tb->base);

        return TB_STATUS_SUCCESS;
}

TB_Status tb_create_sub_area_copy(struct tagged_buffer *src,
                                  struct tagged_buffer **dst)
{
        return TB_STATUS_SUCCESS;
}

TB_Status tb_copy(struct tagged_buffer *src, struct tagged_buffer **dst)
{
}

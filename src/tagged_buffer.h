/* *
 * This API offers basic runtime checks and some automation for memory allocation.
 * THIS IS NOT THE MAIN MEMORY MANAGER!!!
 * */
#ifndef __ESPI_MEMORY_TAGGED_BUFFER_H__
#define __ESPI_MEMORY_TAGGED_BUFFER_H__

#include <espi/types.h>

typedef enum {
        TB_STATUS_SUCCESS = 0,

        TB_N_STATUS,
#define TB_STATUS_ERROR_UNKNOWN TB_N_STATUS
        TB_Status_ENUM_SIZE_GUARD = 0xFFFFFFFF,
} TB_Status;

struct tagged_buffer {
        size_t width;
        size_t count;
        size_t usable;
        bool assigned;
        void *base;
};

struct tb_alloc_info {
        size_t width;
        size_t count;
};

#define TB_ITERATE(tb, i) i < (tb)->count
#define TB_GET(tb, type, i) ((type *)(tb)->base)[i]

TB_Status tb_alloc_buffer(struct tagged_buffer **tb, struct tb_alloc_info info);
TB_Status tb_free_buffer(struct tagged_buffer *tb);

TB_Status tb_assign(struct tagged_buffer **tb, void *base, struct tb_alloc_info info);
TB_Status tb_free_assigned(struct tagged_buffer *tb);

TB_Status tb_create_sub_area_copy(struct tagged_buffer *src, struct tagged_buffer **dst);
TB_Status tb_copy(struct tagged_buffer *src, struct tagged_buffer **dst);

#endif /* __ESPI_MEMORY_TAGGED_BUFFER_H__ */

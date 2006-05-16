#ifndef __PLAT_MMAP_H__
#define __PLAT_MMAP_H__

#include "plat_types.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* flags */
#define FLAG_ATTRIBUTE_READ	0x00000001
#define FLAG_ATTRIBUTE_WRITE	0x00000002

/* Set the mmap handle to be invalid */
void plat_mmap_set_invalid( plat_mmap *handle );

/* Verify if the mmap handle is valid */
int  plat_mmap_is_valid( plat_mmap *handle );

/* Map a file into system meory, return size of the mapped file */
size_t plat_mmap_create( plat_mmap *handle, char *file, int fileAttr );

/* Obtain a view of the mapped file, return the page aligned offset & size */
void *plat_mmap_set_view( plat_mmap *handle, size_t *offset, size_t *size );

/* Delete the mmap handle */
void plat_mmap_close( plat_mmap *handle );

/*  Return page size*/
unsigned int plat_mmap_get_page_size();

/*  Return allocation granularity*/
unsigned int plat_mmap_get_alloc_granularity();

/* Unmap the mmap handle */
void plat_mmap_unmap( plat_mmap *handle );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PLAT_MMAP_H__ */


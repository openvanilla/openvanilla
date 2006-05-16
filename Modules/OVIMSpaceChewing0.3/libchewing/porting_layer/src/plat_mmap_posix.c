#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#ifdef UNDER_POSIX

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "plat_mmap.h"

/* set the mmap handle an invalid value */
void plat_mmap_set_invalid( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return;

	handle->fd = -1;
	handle->address = NULL;
	handle->fAccessAttr = 0;
}

/* verify if the mmap handle is valid */
int plat_mmap_is_valid( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return 0;

	return ( handle->fd == -1 );
}

/* map a file into system memory, return size of the mapped file */
size_t plat_mmap_create( plat_mmap *handle, char *file, int fileAccessAttr )
{
	size_t sizet;

	/* check error(s) */
	if ( ! handle )
		return 0;

	handle->address = NULL;
	handle->sizet = 0;

	if ( FLAG_ATTRIBUTE_READ & fileAccessAttr )
		handle->fd = open( file, O_RDONLY );
	else
		handle->fd = open( file, O_RDWR | O_CREAT );

	if ( handle->fd == -1 )
		return 0;

	sizet = lseek( handle->fd, 0, SEEK_END );
	lseek( handle->fd, 0, SEEK_SET );

	return sizet;
}

/* obtain a view of the mapped file, return the adjusted offset & size */
void *plat_mmap_set_view( plat_mmap *handle, size_t *offset, size_t *sizet )
{
	size_t pagesize = getpagesize();
	size_t edge;

	/* check error(s) */
	if ( ! handle )
		return NULL;

	if ( handle->address )
		munmap( handle->address, handle->sizet );

	edge = (*sizet) + (*offset);
	(*offset) = ((size_t)((*offset) / pagesize)) * pagesize;
	handle->sizet = (*sizet) = edge - (*offset);
	handle->address = mmap(
			0,
			*sizet,
			PROT_READ,
			MAP_SHARED,
			handle->fd,
			*offset );

	return handle->address;
}

/* close the mmap */
void plat_mmap_close( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return;

	if (handle->address) {
		munmap( handle->address, handle->sizet );
		handle->address = NULL;
	}

	if ( handle->fd !=  -1 ) {
		close( handle->fd );
		handle->fd = -1;
	}
}

/* return page size */
unsigned int plat_mmap_get_page_size()
{
	return getpagesize();

}

unsigned int plat_mmap_get_alloc_granularity()
{
	return 16 * getpagesize();
}

void plat_mmap_unmap( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return;

	if ( handle->address ) {
		munmap( handle->address, handle->sizet );
		handle->address = NULL;
	}
}

#endif /* UNDER_POSIX */


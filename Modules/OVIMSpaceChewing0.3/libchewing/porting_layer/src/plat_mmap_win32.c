#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)

#include "plat_mmap.h"

/* set the mmap handle an invalid value */
void plat_mmap_set_invalid( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return;

	handle->fd_file = NULL;
	handle->address = NULL;
	handle->fd_map = NULL;
	handle->fAccessAttr = 0;
}

/* verify if the mmap handle is valid */
int plat_mmap_is_valid( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return 0;

	return (int) (handle->fd_map != NULL);
}

/* map a file into system meory, return the file size */
size_t plat_mmap_create( plat_mmap *handle, char *file, int fileAccessAttr )
{
	LARGE_INTEGER sizet;

	/* check error(s) */
	if ( ! handle )
		return 0;

	handle->fd_map = NULL;
	handle->address = NULL;

	if ( FLAG_ATTRIBUTE_READ & fileAccessAttr ) {
#ifdef _WIN32_WCE
		handle->fd_file = CreateFileForMapping(
				file,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL );
#else /* !_WIN32_WCE */
		handle->fd_file = CreateFile(
				file,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_READONLY | FILE_FLAG_RANDOM_ACCESS,
				0 );
#endif /* _WIN32_WCE */

		if ( INVALID_HANDLE_VALUE == handle->fd_file )
			return 0;

		sizet.LowPart = GetFileSize( handle->fd_file, (LPDWORD) &sizet.HighPart );
		handle->fd_map = CreateFileMapping(
				handle->fd_file,
				NULL,
				PAGE_READONLY,
				sizet.HighPart,
				sizet.LowPart,
				0 );
	}
	else {
#ifdef _WIN32_WCE
		handle->fd_file = CreateFileForMapping(
				file,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
#else /* !_WIN32_WCE */
		handle->fd_file = CreateFile(
				file,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				0);
#endif /* _WIN32_WCE */

		if ( INVALID_HANDLE_VALUE == handle->fd_file )
			return 0;

		sizet.LowPart  = 0;
		sizet.HighPart = 1;
		handle->fd_map = CreateFileMapping(
				handle->fd_file,
				NULL,
				PAGE_READWRITE,
				0,
				sizet.LowPart,
				0 );
		sizet.LowPart  = 1024 * 1024 * 1024;
		sizet.HighPart = 0;

		while ( ! handle->fd_map ) {
			DWORD error;
			handle->fd_map = CreateFileMapping(
					handle->fd_file,
					NULL,
					PAGE_READWRITE,
					0,
					sizet.LowPart,
					0 );
			error = GetLastError();

			if ( ERROR_NOT_ENOUGH_MEMORY == error || ERROR_DISK_FULL == error )
				sizet.LowPart /= 2;
			else
				break;
		}
	}

	handle->fAccessAttr = fileAccessAttr;

	if ( FLAG_ATTRIBUTE_READ & fileAccessAttr || 16 * 1024 * 1024 <= sizet.LowPart ) {
		if ( handle->fd_map )
			return (size_t) sizet.QuadPart;
	}

	plat_mmap_close( handle );

	return 0;
}

/* obtain the a view of the mapped file */
void *plat_mmap_set_view( plat_mmap *handle, size_t *offset, size_t *sizet )
{
	LARGE_INTEGER t_offset;
	LARGE_INTEGER t_sizet;
	SYSTEM_INFO info;
	size_t pagesize;
	size_t edge;

	/* check error(s) */
	if ( ! handle )
		return NULL;

	if ( handle->address )
		UnmapViewOfFile( handle->address );

	GetSystemInfo( &info );
	pagesize = info.dwAllocationGranularity;

	edge = (*sizet) + (*offset);
	t_offset.QuadPart =
		(*offset) = (size_t)((*offset) / pagesize) * pagesize;

	t_sizet.QuadPart = (*sizet) = edge - (*offset);

	if ( FLAG_ATTRIBUTE_READ & handle->fAccessAttr ) {
		handle->address = MapViewOfFile(
				handle->fd_map,
				FILE_MAP_READ,
				t_offset.HighPart,
				t_offset.LowPart,
				t_sizet.LowPart );
	}
	else {
		handle->address = MapViewOfFile(
				handle->fd_map,
				FILE_MAP_WRITE,
				t_offset.HighPart,
				t_offset.LowPart,
				t_sizet.LowPart );
	}

	return handle->address;
}

/* close the mmap */
void plat_mmap_close( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return;

	if ( handle->address ) {
		UnmapViewOfFile( handle->address );
		handle->address = NULL;
	}

	if ( handle->fd_map ) {
		CloseHandle( handle->fd_map );
		handle->fd_map = NULL;
	}

	if ( INVALID_HANDLE_VALUE != handle->fd_file ) {
		CloseHandle( handle->fd_file );
		handle->fd_file = INVALID_HANDLE_VALUE;
	}
}

/*  return page size*/
unsigned int plat_mmap_get_page_size()
{
	SYSTEM_INFO info;

	GetSystemInfo( &info );
	return info.dwPageSize;
}

unsigned int plat_mmap_get_alloc_granularity()
{
	SYSTEM_INFO info;

	GetSystemInfo( &info );
	return info.dwAllocationGranularity;
}

void plat_mmap_unmap( plat_mmap *handle )
{
	/* check error(s) */
	if ( ! handle )
		return;

	if ( handle->address ) {
		UnmapViewOfFile( handle->address );
		handle->address = NULL;
	}
}

#endif /* defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE) */


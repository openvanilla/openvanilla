#ifndef PLAT_POSIX_H
#define PLAT_POSIX_H

#ifdef UNDER_POSIX

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>

#define PLAT_SEPARATOR "/"
#define PLAT_TMPDIR "/tmp"
#define PLAT_MKDIR(dir) \
	mkdir(dir, S_IRWXU)
#define PLAT_RENAME(oldpath, newpath) \
	rename(oldpath, newpath)
#define PLAT_UNLINK(path) \
	unlink(path)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* plat_mmap.h */
typedef struct plat_mmap {
	int fd;
	void *address;
	size_t sizet;
	int fAccessAttr;
} plat_mmap;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UNDER_POSIX */
#endif /* PLAT_POSIX_H */


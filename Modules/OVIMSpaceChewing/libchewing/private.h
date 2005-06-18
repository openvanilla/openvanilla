#ifndef CHEWING_PRIVATE_H
#define CHEWING_PRIVATE_H

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#ifdef ENABLE_DEBUG
#include <stdarg.h>
extern FILE *fp_g;
#define DEBUG_OUT( ... ) \
	do { \
		if ( fp_g ) { \
			fprintf( fp_g, __VA_ARGS__ ); \
		} \
		else { \
			fprintf( stderr, __VA_ARGS__ ); \
		} \
	} while (0)
#define DEBUG_CHECKPOINT() \
	DEBUG_OUT( "At %s %d\n", __FUNCTION__, __LINE__ )
#define DEBUG_FLUSH \
	do { \
		if ( fp_g ) { \
			fflush( fp_g ); \
		} \
	} while (0)
#define EMPHASIZE(real_string) \
	"\033[44;37m"real_string"\033[m"

#else /* ! ENABLE_DEBUG */
#define DEBUG_OUT( ... )
#define DEBUG_FLUSH
#define DEBUG_CHECKPOINT()
#endif

#define ALC(type, size) \
	(type *) calloc( size, sizeof( type ) )

typedef int (*CompFuncType)( const void *, const void * );

#define TerminateServicesNUM 5
extern int addTerminateService( void (*callback)() );
#endif

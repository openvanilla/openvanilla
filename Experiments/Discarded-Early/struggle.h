inline void murmur(FILE *f, const char* format,...) {
#ifdef OVDEBUG
    va_list args;
    va_start (args, format);
    vfprintf (f,format,args);
    va_end (args);
    fprintf (f,"\n");
#endif
}

#ifdef OVDEBUG
    #define OVDEBUGCLASS(x) \
        virtual void struggle(const char *format, ...) \
        {   \
            fprintf (stderr, "%s: ", x);    \
            va_list args;   \
            va_start (args, format);    \
            vfprintf (stderr, format, args);    \
            va_end (args);  \
            fprintf (stderr, "\n"); \
        }
#else
    #define OVDEBUGCLASS(x) 
    #define struggle (format, ...)
#endif

            
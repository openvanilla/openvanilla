// OVSQLite3.h

#ifndef __OVSQLite3_h
#define __OVSQLite3_h

#include <sqlite3.h>

class SQLite3Statement {
public:
    SQLite3Statement(sqlite3_stmt *st) : s(st) {}
    ~SQLite3Statement()                 { sqlite3_finalize(s); }
    int reset()                     { return sqlite3_reset(s); }
    int bind_text(int c, const char *t)  
    {
        return sqlite3_bind_text(s, c, t, -1, SQLITE_TRANSIENT);
    }
    int bind_int(int c, int i)      { return sqlite3_bind_int(s, c, i); }

    int step()                      { return sqlite3_step(s); }
    const char *column_text(int c)  
        { return (const char*)sqlite3_column_text(s, c); }
    int column_int(int c)           { return sqlite3_column_int(s, c); }
    int column_count()              { return sqlite3_column_count(s); }
protected:
    sqlite3_stmt *s;
};

class SQLite3 {
public:
    int open(const char *f)     { return sqlite3_open(f, &handle); }
    int close()                 { return sqlite3_close(handle); }
    int errcode()               { return sqlite3_errcode(handle); }
    SQLite3Statement *prepare(const char *sqlcmd)   
    {
        sqlite3_stmt *s;
        const char *rms;
        if (sqlite3_prepare(handle, sqlcmd, -1, &s, &rms)) return NULL;
        return new SQLite3Statement(s);
    }
protected:
    sqlite3 *handle;
};

#endif

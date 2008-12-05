// OVSQLite3.h: A minimal SQLite3 wrapper
//
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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
    double column_double(int c)     { return sqlite3_column_double(s, c); }
    int column_count()              { return sqlite3_column_count(s); }
protected:
    sqlite3_stmt *s;
};

class SQLite3 {
public:
    int open(const char *f)     { return sqlite3_open(f, &handle); }
    int close()                 { return sqlite3_close(handle); }
    int errcode()               { return sqlite3_errcode(handle); }
    const char *errmsg()        { return sqlite3_errmsg(handle); }

    int execute(const char *sqlcmd, ...) {
        va_list l;
        va_start(l, sqlcmd);
        char *cmd=sqlite3_vmprintf(sqlcmd, l);
        va_end(l);
        int r=sqlite3_exec(handle, cmd, NULL, NULL, NULL);
        sqlite3_free(cmd);
        return r;
    }
    
    SQLite3Statement *prepare(const char *sqlcmd, ...) {
        va_list l;
        va_start(l, sqlcmd);
        char *cmd=sqlite3_vmprintf(sqlcmd, l);
        va_end(l);
        
        sqlite3_stmt *s;
        const char *rms;
        SQLite3Statement *r=NULL;       
        if (!sqlite3_prepare(handle, cmd, -1, &s, &rms)) r=new SQLite3Statement(s);
        sqlite3_free(cmd);
        return r;
    }
    
protected:
    sqlite3 *handle;
};

#endif

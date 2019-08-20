#ifndef _INSERT_DB_
#define _INSERT_DB_
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
sqlite3 *open_db(const char *db_name,const char *file_name);
int insert_db(sqlite3 *,const char *,const char*,const char *);
#endif


#include"db_init.h"
int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i=0;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int db_init(char *db_name,char *file_name)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char sql[256];

    /* Open database */
    rc = sqlite3_open(db_name, &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    snprintf(sql,256,"CREATE TABLE IF NOT EXISTS %s("
            "ID INTEGER  PRIMARY KEY AUTOINCREMENT ,"
            "JSON_DATA   CHAR(256)   NOT NULL);",file_name);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
    return 0;
}


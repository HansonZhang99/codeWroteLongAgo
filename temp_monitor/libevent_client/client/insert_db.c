#include"insert_db.h"

int call_back(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i=0; i<argc; i++)
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;

}

sqlite3 *open_db(const char *db_name,const char *file_name)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(db_name, &db);             
    if( rc )                                     
    {                                
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;                    
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    return db;
}

int insert_db(sqlite3* db,const char *db_name,const char *file_name,const char *json_data)
{
   char *zErrMsg = 0;
   char sql[256];
   int rc;
   /* Create SQL statement */
   snprintf(sql,256,"INSERT INTO %s(ID,JSON_DATA)VALUES (NULL,'%s'); ",file_name,json_data);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, call_back, 0, &zErrMsg);
   if( rc != SQLITE_OK )
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      return -1;
   }
   else
   {
      fprintf(stdout, "Records created successfully\n");
   }
   return 0;
}

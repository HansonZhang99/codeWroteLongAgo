#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
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
int insert_db(char *db_name,char *file_name,char *id,char *date,char *temper)
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char sql[256];

   /* Open database */
   rc = sqlite3_open(db_name, &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return -1;
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   snprintf(sql,256,"INSERT INTO %s(I,ID,DATE,TEMPERATURE)VALUES (NULL,'%s','%s','%s'); ",file_name,id,date,temper);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, call_back, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      return -1;
   }else{
      fprintf(stdout, "Records created successfully\n");
   }
   sqlite3_close(db);
   return 0;
}

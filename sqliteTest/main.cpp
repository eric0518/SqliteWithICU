#include "sqlite3.h"
#include <stdio.h>
#include   <stdlib.h> 
#include "wcicu_utils.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";


	// testign for open
	rc = sqlite3_open("test.db", &db);
	if (rc) 
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return 0;
	}
	else 
	{
		fprintf(stderr, "Opened database successfully\n");
	}

	// register
	/*
	rc = sqlite3_register_mm_utils(db);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "sqlite3_register_mm_utils error: \n");
	}
	else 
	{
		fprintf(stdout, "sqlite3_register_mm_utils successfully\n");
	}
	*/

	/* Create SQL statement */
	sql = "CREATE VIRTUAL TABLE  ttt USING FTS5(content, tokenize = mmicu);";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) 
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
	}

	// insert row
	sql = "insert into ttt values('中华人民共和国万岁');";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Insert successfully\n");
	}


	// select
	//sql = "SELECT * from ttt where content MATCH \'人民共和国\'";
	sql = "SELECT * from ttt where content like \'%人%\'";
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}


	sqlite3_close(db);

	

	system("PAUSE ");
	return 0;

}
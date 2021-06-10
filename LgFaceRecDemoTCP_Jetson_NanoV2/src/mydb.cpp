#include "common.h"
#include "mydb.h"
#include <string>
#include <stdio.h>
#include <iostream>


#define SQL_CHECK_QUIT(INFO, RET) \
    do {if (rc != SQLITE_OK) { fprintf(stderr, "'%s' error\n", INFO); return RET; }  }while(0)
#define SQL_CHECK(INFO) \
    do {if (rc != SQLITE_OK)  { fprintf(stderr, "'%s' error\n", INFO); }  }while(0)
#define SQL_ERROR(INFO) \
    do { fprintf(stderr, "%s : '%s'\n", INFO, sqlite3_errmsg(db)); } while(0)


CMydb::CMydb() {
    printf("sql v: %s\n", sqlite3_libversion()); 
        
}

CMydb::~CMydb() {
}
 


gboolean
CMydb::initialize_database() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("tartan.db", &db);
    
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);        
        return false;
    }
    const char *sql = "DROP TABLE IF EXISTS user;" 
                "CREATE TABLE user (id INT, account TEXT, passwd TEXT, privilege INT);" 
                "INSERT INTO user VALUES(1, 'admin', 'e9b6ebe030d910d3b0c253b9bd05dfc365f1e17f61f2b64385898a8247b5b792' ,0);" 
                "INSERT INTO user VALUES(2, 'lg', '078156fd9debb7d481347e68ab19bb1f2d3028bcd61bc25994562f8a0d62e8e1' ,2);" ;

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);        
    } 
    sqlite3_close(db);

    return true;
}

gboolean
CMydb::list_alluser() {
 

    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("tartan.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", 
                sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return false;
    } else {
        sqlite3_stmt* stmt; 
        rc = sqlite3_exec(db, "BEGIN", 0, 0, 0); 
        SQL_CHECK("Trassaction begin");
        char *sql = (char*)"SELECT * from user";
        if (sqlite3_prepare(db, sql, -1, &stmt, nullptr) == SQLITE_OK) { 
            while (sqlite3_step(stmt) == SQLITE_ROW) { 
                printf("id:%d  account:%s, passwd:%s, privilege:%d \n", 
                        sqlite3_column_int(stmt, 0), 
                        (char*)sqlite3_column_text(stmt, 1), 
                        (char*)sqlite3_column_text(stmt, 2),
                        sqlite3_column_int(stmt, 3) ); 
            } 
        } else { 
            SQL_CHECK("Error");
        } 
        rc = sqlite3_exec(db, "END", 0, 0, 0); 
        SQL_CHECK("Transaction end");

        sqlite3_finalize(stmt); 
        sqlite3_close(db);
    }
    return true;
}

int 
CMydb::find_user(string id, string passwd) {
    int ret=-1;
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("tartan.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", 
                sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    } else {
        sqlite3_stmt* stmt; 
        rc = sqlite3_exec(db, "BEGIN", 0, 0, 0); 
        SQL_CHECK("Trassaction begin");
        std::ostringstream stringStream;
        stringStream << "SELECT * from user where account='";
        stringStream << id;
        stringStream << "' and passwd='";
        stringStream << passwd;
        stringStream << "';";
        string strstr = stringStream.str();
        char *sql=(char*)strstr.c_str();
        printf("SQL=%s\n",sql);
        if (sqlite3_prepare(db, sql, -1, &stmt, nullptr) == SQLITE_OK) { 
            if (sqlite3_step(stmt) == SQLITE_ROW) { 
                printf("id:%d  account:%s, passwd:%s, privilege:%d \n", 
                        sqlite3_column_int(stmt, 0), 
                        (char*)sqlite3_column_text(stmt, 1), 
                        (char*)sqlite3_column_text(stmt, 2),
                        sqlite3_column_int(stmt, 3) ); 
                ret=sqlite3_column_int(stmt, 3);
            } 
        } else { 
            SQL_CHECK("Error");
        } 
        rc = sqlite3_exec(db, "END", 0, 0, 0); 
        SQL_CHECK("Transaction end");

        sqlite3_finalize(stmt); 
        sqlite3_close(db);
    }
    return ret;
}

 
gboolean
CMydb::start() {

    sqlite3_stmt* res;
    
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return false;
    }
    
    rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);    
    
    if (rc != SQLITE_OK) 
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return false;
    }
    
    rc = sqlite3_step(res);
    
    if (rc == SQLITE_ROW)
    {
        printf("%s\n", sqlite3_column_text(res, 0));
    }
    sqlite3_finalize(res);

    return true;
}

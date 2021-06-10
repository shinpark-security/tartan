#ifndef MYDB_H_
#define MYDB_H_
#include <glib.h>
#include <string>
#include <sqlite3.h>

using namespace std;

class CMydb {
public:
    CMydb();
    ~CMydb();
    gboolean start();
    gboolean initialize_database();
    gboolean list_alluser() ;

    int find_user(string id, string passwd);

private:
    sqlite3* db;
};

#endif /*MYDB_H_*/
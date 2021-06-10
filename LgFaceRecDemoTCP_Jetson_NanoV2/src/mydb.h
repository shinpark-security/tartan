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
    gboolean initialize_database_account();
    gboolean initialize_database_faces();
    gboolean list_alluser() ;
    gboolean list_faces() ;
    gboolean add_new_face(string name, const char *buffer, ssize_t size, sqlite3 *db/*=nullptr*/);
    int find_user(string id, string passwd);
    int add_or_update_name(sqlite3 *db,string name);
    string remove_trail_number(string fname) ;
private:
    sqlite3* db;
};

#endif /*MYDB_H_*/
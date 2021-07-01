#ifndef MYDB_H_
#define MYDB_H_
#include <glib.h>
#include <string>
#include <sqlite3.h>
#include <vector>
#include "cyper.h"
using namespace std;

typedef struct {
    string name;
    unsigned char *data;
    int length;
} tFaceEntity;

class CMydb {
public:
    CMydb();
    ~CMydb();
    CCyper cyp;
    gboolean start();
    gboolean initialize_database_account();
    gboolean initialize_database_faces();
    gboolean list_alluser() ;
    gboolean list_faces(vector<tFaceEntity> *facelist=nullptr) ;
    gboolean add_new_face(string name, const char *buffer, ssize_t size, sqlite3 *db=nullptr);
    int find_user(string id, string passwd);
    int add_or_update_name(sqlite3 *db,string name);
    string remove_trail_number(string fname) ;
    string user_db_filename;
    string faces_db_filename;
private:
    sqlite3* db;
};

#endif /*MYDB_H_*/
#include "common.h"
#include "mydb.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "cyper.h"

#define SQL_CHECK_QUIT(INFO, RET)                  \
    do                                             \
    {                                              \
        if (rc != SQLITE_OK)                       \
        {                                          \
            fprintf(stderr, "'%s' error\n", INFO); \
            return RET;                            \
        }                                          \
    } while (0)
#define SQL_CHECK(INFO)                            \
    do                                             \
    {                                              \
        if (rc != SQLITE_OK)                       \
        {                                          \
            fprintf(stderr, "'%s' error\n", INFO); \
        }                                          \
    } while (0)
#define SQL_ERROR(INFO)                                           \
    do                                                            \
    {                                                             \
        fprintf(stderr, "%s : '%s'\n", INFO, sqlite3_errmsg(db)); \
    } while (0)

typedef struct {
    size_t size;
    size_t encrypted_size;
    unsigned char payload[1];
} sDBImg;

CMydb::CMydb()
{
    // printf("sql v: %s\n", sqlite3_libversion());
    user_db_filename="../tartan_user.db";
    faces_db_filename="../tartan_faces.db";
}

CMydb::~CMydb()
{
}

gboolean
CMydb::initialize_database_account()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(user_db_filename.c_str(), &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }
    const char *sql = "DROP TABLE IF EXISTS user;"
                      "CREATE TABLE user (id INTEGER PRIMARY KEY AUTOINCREMENT , account TEXT, passwd TEXT, privilege INT);"
                      "INSERT INTO user VALUES(1, 'admin', 'e9b6ebe030d910d3b0c253b9bd05dfc365f1e17f61f2b64385898a8247b5b792' ,0);"
                      "INSERT INTO user VALUES(2, 'lg', '078156fd9debb7d481347e68ab19bb1f2d3028bcd61bc25994562f8a0d62e8e1' ,2);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    sqlite3_close(db);

    return true;
}

int CMydb::add_or_update_name(sqlite3 *db, string name)
{
    char *err_msg = 0;
    int id = -1;
    CCyper cyp;
    sqlite3_stmt *stmt;
    int rc = sqlite3_exec(db, "BEGIN", 0, 0, 0);
    SQL_CHECK("Trassaction begin");
    string sql = "SELECT id FROM names WHERE name='";
    sql += name;
    sql += "';";
    // printf("sql:%s\n", sql.c_str());
    if (sqlite3_prepare(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0);
            printf("USER:%s ID:%d\n", name.c_str(), id);
        }
    }
    else
    {
        SQL_CHECK("Error");
    }
    rc = sqlite3_exec(db, "END", 0, 0, 0);
    SQL_CHECK("Transaction end");
    sqlite3_finalize(stmt);

    if (id == -1)
    {
        int ret;
        string sql = "INSERT INTO names(name) VALUES('";
        sql += cyp.encrypt_aes(name);
        sql += "');";
        // printf("sql:%s\n", sql.c_str());
        rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
        else
        {
            id = sqlite3_last_insert_rowid(db);
            // printf("NEW USER:%s ID=%d\n", name.c_str(), id);
        }
    }

    return id;
}

gboolean
CMydb::add_new_face(string name, const char *buffer, ssize_t size, sqlite3 *db /*= nullptr*/)
{
    char *err_msg = 0;
    gboolean need_close = false;

    if (db == nullptr)
    {
        int rc = sqlite3_open(faces_db_filename.c_str(), &db);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return false;
        }
        need_close = true;
    }

    int id = add_or_update_name(db, name);
    if (id < 0)
    {
        printf("Unknown error .. Can't append new user...\n");
        sqlite3_close(db);
        return false;
    }

    sqlite3_stmt *stmt = NULL;
    string sql = "INSERT INTO faces (names_id, face) VALUES('";
    sql += to_string(id);
    sql += "', ?)";
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        cerr << "prepare failed: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        size_t payload_size=cyp.aes_encryption_buffer_size(size);
        size_t total_size = payload_size + sizeof(sDBImg);
        sDBImg *newbuf=(sDBImg *)new unsigned char[total_size];
        if (newbuf) {
            newbuf->size=size;
            newbuf->encrypted_size=payload_size;
            cyp.encrypt_aes((unsigned char*)buffer, size, &newbuf->payload[0] );   
            // memcpy( &newbuf->payload[0],(unsigned char*)buffer, size )    ;     
            rc = sqlite3_bind_blob(stmt, 1, (unsigned char*)newbuf, total_size, SQLITE_STATIC);
            if (rc != SQLITE_OK)
            {
                cerr << "bind failed: " << sqlite3_errmsg(db) << endl;
                sqlite3_free(err_msg);
            }
            else
            {
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_DONE)
                    cerr << "execution failed: " << sqlite3_errmsg(db) << endl;
                sqlite3_free(err_msg);
            }
            delete newbuf;
        }

    }
    sqlite3_finalize(stmt);
    if (need_close)
        sqlite3_close(db);

    return true;
}

string
CMydb::remove_trail_number(string fname)
{
    string ret;
    // char *org=fname.c_str();
    for (auto &c : fname)
        c = tolower(c);
    int len = fname.length();
    if (len > 4 && fname.substr(len - 4) == ".jpg")
    {
        fname = fname.substr(0, len - 4);
        len = fname.length();
        // printf("Removing .jpg:%s\n",fname.c_str());
    }
    if (len > 4 && fname.substr(len - 4) == ".png")
    {
        fname = fname.substr(0, len - 4);
        len = fname.length();
        // printf("Removing .png:%s\n",fname.c_str());
    }
    ret = fname;
    for (int i = len - 1; i >= 0; i--)
    {
        if (fname[i] > '9' || fname[i] < '0')
        {
            ret = fname.substr(0, i + 1);
            break;
        }
    }
    return ret;
}

gboolean
CMydb::initialize_database_faces()
{
    sqlite3 *db;
    char *err_msg = 0;
    int ret;

    int videoFrameWidth = 640;
    int videoFrameHeight = 480;
    cv::Mat image;

    int rc = sqlite3_open(faces_db_filename.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    const char *sql = "DROP TABLE IF EXISTS names;"
                      "CREATE TABLE names (id INTEGER PRIMARY KEY AUTOINCREMENT , name TEXT );";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    sql = "DROP TABLE IF EXISTS faces;"
          "CREATE TABLE faces (id INTEGER PRIMARY KEY AUTOINCREMENT , names_id INT, face BLOB );";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    std::vector<struct Paths> paths;
    getFilePaths("../imgs", paths);
    for (int i = 0; i < paths.size(); i++)
    {
        loadInputImage(paths[i].absPath, image, videoFrameWidth, videoFrameHeight);
        ssize_t leng = image.step[0] * image.rows;
        string name = remove_trail_number(paths[i].fileName);
        // printf("%s  -- trimmed :%s\n", paths[i].fileName.c_str(), name.c_str());
        printf("%s : size=%zu  type=%d\n", name.c_str(), leng, image.type());
        add_new_face(name, (char *)image.data, leng, db);
        // cv::imshow("VideoSource", image);
        // ret = cv::waitKey(1);
    }

    sqlite3_close(db);
    return true;
}

gboolean
CMydb::list_faces(vector<tFaceEntity> *facelist/*=nullptr*/)
{
    sqlite3 *db;
    char *err_msg = 0;
    int videoFrameWidth = 640;
    int videoFrameHeight = 480;

    int rc = sqlite3_open(faces_db_filename.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return false;
    }
    else
    {
        sqlite3_stmt *stmt;
        int ret;
        CCyper cyp;
        rc = sqlite3_exec(db, "BEGIN", 0, 0, 0);
        SQL_CHECK("Trassaction begin");
        char *sql = (char *)"SELECT * from faces INNER JOIN names on names.id==faces.names_id";
        if (sqlite3_prepare(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                ssize_t leng = sqlite3_column_bytes(stmt, 2);
                sDBImg *dbimg=(sDBImg *)sqlite3_column_blob(stmt, 2);
                // printf("id:%d  face_id:%d  name=%s leng=%zu\n",
                //        sqlite3_column_int(stmt, 0),
                //        sqlite3_column_int(stmt, 1),
                //        sqlite3_column_text(stmt, 4),
                //        leng);
                unsigned char *jpg=new unsigned char[dbimg->encrypted_size];
                if (jpg) 
                {
                    string usr_name=cyp.decrypt_aes((const char*)sqlite3_column_text(stmt, 4));
                    cyp.decrypt_aes(&dbimg->payload[0], dbimg->encrypted_size, jpg );
                    // printf("JPG: name=%s  size=%zu  encrypted_size=%zu\n", usr_name.c_str(), dbimg->size, dbimg->encrypted_size);
                    if (facelist != nullptr) {
                        tFaceEntity face;
                        face.data=jpg;
                        face.length=dbimg->size;
                        face.name=usr_name;
                        facelist->push_back(face);
                    } 
                    else {
                        // cv::Mat image = cv::Mat(videoFrameHeight, videoFrameWidth, 16);
                        // image.data = jpg;
                        // cv::imshow("VideoSource", image);
                        // ret = cv::waitKey(100);
                        // printf("ret=%d\n", ret);
                        delete jpg;
                    }
                }
            }
        }
        else
        {
            SQL_CHECK("Error");
        }
        rc = sqlite3_exec(db, "END", 0, 0, 0);
        SQL_CHECK("Transaction end");

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    return true;
}

gboolean
CMydb::list_alluser()
{
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(user_db_filename.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return false;
    }
    else
    {
        sqlite3_stmt *stmt;
        rc = sqlite3_exec(db, "BEGIN", 0, 0, 0);
        SQL_CHECK("Trassaction begin");
        char *sql = (char *)"SELECT * from user";
        if (sqlite3_prepare(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                printf("id:%d  account:%s, passwd:%s, privilege:%d \n",
                       sqlite3_column_int(stmt, 0),
                       (char *)sqlite3_column_text(stmt, 1),
                       (char *)sqlite3_column_text(stmt, 2),
                       sqlite3_column_int(stmt, 3));
            }
        }
        else
        {
            SQL_CHECK("Error");
        }
        rc = sqlite3_exec(db, "END", 0, 0, 0);
        SQL_CHECK("Transaction end");

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    return true;
}

int CMydb::find_user(string id, string passwd)
{
    int ret = -1;
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(user_db_filename.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }
    else
    {
        sqlite3_stmt *stmt;
        rc = sqlite3_exec(db, "BEGIN", 0, 0, 0);
        SQL_CHECK("Trassaction begin");
        std::ostringstream stringStream;
        stringStream << "SELECT * from user where account='";
        stringStream << id;
        stringStream << "' and passwd='";
        stringStream << passwd;
        stringStream << "';";
        string strstr = stringStream.str();
        char *sql = (char *)strstr.c_str();
        // printf("SQL=%s\n", sql);
        if (sqlite3_prepare(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                printf("id:%d  account:%s, passwd:%s, privilege:%d \n",
                       sqlite3_column_int(stmt, 0),
                       (char *)sqlite3_column_text(stmt, 1),
                       (char *)sqlite3_column_text(stmt, 2),
                       sqlite3_column_int(stmt, 3));
                ret = sqlite3_column_int(stmt, 3);
            }
        }
        else
        {
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
CMydb::start()
{
    if (access(user_db_filename.c_str(), F_OK) ==0 ) {
        printf("user database ok\n");
    } else {
        printf("initialize user database\n");
        initialize_database_account();
        list_alluser();
    }
    if (access(faces_db_filename.c_str(), F_OK) ==0) {
        printf("face database ok\n");
    } else {
        printf("initialize face database\n");
        initialize_database_faces();
        list_faces();
    }
    return true;
}

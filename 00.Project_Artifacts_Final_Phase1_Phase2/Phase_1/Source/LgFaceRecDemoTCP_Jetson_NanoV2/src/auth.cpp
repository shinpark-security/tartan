#include "auth.h"
#include <stdio.h>
#include <string>
#include "mydb.h"
#include "cyper.h"

CAuth::CAuth()
{
}

CAuth::~CAuth()
{
}

int CAuth::login(string id, string passwd)
{
    CMydb db;
    CCyper cyp;
    return db.find_user(id, cyp.get_passwd_enc(passwd));
}

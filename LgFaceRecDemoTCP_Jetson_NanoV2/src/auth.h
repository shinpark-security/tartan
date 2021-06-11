
#ifndef AUTH_H_
#define AUTH_H_

#include <stdio.h>
#include <string>
using namespace std;

class CAuth
{
public:
    CAuth();
    ~CAuth();
    int login(string id, string passwd);
};

#endif /*AUTH_H_*/
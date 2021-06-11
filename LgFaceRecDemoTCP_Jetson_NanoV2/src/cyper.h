
#ifndef CYPER_H_
#define CYPER_H_

#include <glib.h>
#include <stdio.h>
#include <string>
using namespace std;

#define IV_SIZE 16
#define KEY_BIT 128

class CCyper
{
public:
    CCyper();
    ~CCyper();
    string get_passwd_enc(string pass);
    gboolean encrypt_aes(const unsigned char *inbuff, ssize_t len,  unsigned char *outbuff);
    gboolean decrypt_aes(const unsigned char *inbuff, ssize_t len, unsigned char *outbuff);

};

#endif /*CYPER_H_*/
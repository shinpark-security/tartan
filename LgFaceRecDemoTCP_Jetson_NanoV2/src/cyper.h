
#ifndef CYPER_H_
#define CYPER_H_

#include <glib.h>
#include <stdio.h>
#include <string>
#define SECRET_KEY_FILE "/var/shinpark/secret.key"
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
    string decrypt_aes(const string instr);
    string encrypt_aes(const string instr);
    string hexStr(unsigned char *data, int len);
    gboolean hex2bytes(string input, unsigned char *output) ;
    string bytes2hex(const unsigned char *input, int len);
    size_t aes_encryption_buffer_size(size_t src_size);
private:
    unsigned char secret_key[IV_SIZE];
};

#endif /*CYPER_H_*/
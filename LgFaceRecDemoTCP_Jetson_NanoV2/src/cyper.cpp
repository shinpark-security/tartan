#include "cyper.h"
#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <string>
#include "mydb.h"


AES_KEY aes_ks3;
unsigned char iv[IV_SIZE];
static const unsigned char key32[32] =
    {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
     0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12,
     0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34,
     0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34, 0x56};


CCyper::CCyper()
{
}

CCyper::~CCyper()
{
}


gboolean 
CCyper::encrypt_aes(const unsigned char *inbuff, ssize_t len,  unsigned char *outbuff)
{
    memset(iv, 0, sizeof(iv)); // init iv
    AES_set_encrypt_key(key32, KEY_BIT, &aes_ks3);
    AES_cbc_encrypt(inbuff, outbuff, len, &aes_ks3, iv, AES_ENCRYPT);

    return true;
}

gboolean
CCyper::decrypt_aes(const unsigned char *inbuff, ssize_t len, unsigned char *outbuff)
{
    memset(iv, 0, sizeof(iv)); // the same iv
    AES_set_decrypt_key(key32, KEY_BIT, &aes_ks3);
    AES_cbc_encrypt(inbuff, outbuff, len, &aes_ks3, iv, AES_DECRYPT);

    return true;
}

string
CCyper::get_passwd_enc(string pass)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, pass.c_str(), pass.length());
    SHA256_Final(digest, &ctx);

    char mdString[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
    string str = mdString;
    // printf("SHA256 digest: %s\n", mdString);
    return str;
}

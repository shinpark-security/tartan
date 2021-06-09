#include "auth.h"
#include <stdio.h>
#include <openssl/sha.h>
#include <string>


CAuth::CAuth() {

}

CAuth::~CAuth(){
    
}

string 
CAuth::get_passwd_enc(string pass)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, pass.c_str(), pass.length());
    SHA256_Final(digest, &ctx);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    string str=mdString;
    // printf("SHA256 digest: %s\n", mdString);
    return str;
}
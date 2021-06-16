#include "cyper.h"
#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <string>
#include "mydb.h"

#include "unistd.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>

static AES_KEY aes_ks3;
static unsigned char iv[IV_SIZE];

CCyper::CCyper()
{
    if (access(SECRET_KEY_FILE, F_OK) ==0 ) {
        ifstream fi;
        fi.open( SECRET_KEY_FILE, std::ios_base::in | std::ios_base::binary);
        fi.read((char*)secret_key,IV_SIZE);
        fi.close();
        // for (int i=0;i<IV_SIZE;i++)  {
        //     printf("%x ",secret_key[i]);
        // }
    	// printf("\n");
    }
    else {
        // g_assert(true);
        printf("Secret key is not exist...\n");
        exit(1);
    }
}

CCyper::~CCyper()
{
}


int 
CCyper::encrypt_aes(const unsigned char *inbuff, ssize_t len,  unsigned char *outbuff)
{
    memset(iv, 0, sizeof(iv)); // init iv
    int ret=AES_set_encrypt_key(secret_key, KEY_BIT, &aes_ks3);
    g_assert(ret==0);
    if (ret<0) return ret;
    AES_cbc_encrypt(inbuff, outbuff, len, &aes_ks3, iv, AES_ENCRYPT);
    return aes_encryption_buffer_size(len);
}

string 
CCyper::encrypt_aes(const string instr)
{
    string outstr;
    memset(iv, 0, sizeof(iv)); // init iv
    int ret=AES_set_encrypt_key(secret_key, KEY_BIT, &aes_ks3);
    g_assert(ret==0);
    if (ret<0) return "";
    size_t len=instr.length()+1;
    size_t enc_len=aes_encryption_buffer_size(len);
    unsigned char *outbuf=new unsigned char[enc_len];
    if (outbuf) {
        AES_cbc_encrypt((unsigned char*)instr.c_str(), outbuf, len, &aes_ks3, iv, AES_ENCRYPT);
        outstr=bytes2hex(outbuf,enc_len);
        delete outbuf;
    }
    return  outstr;
}

size_t
CCyper::aes_encryption_buffer_size(size_t src_size) 
{
    return src_size/16 * 16 + (src_size % 16 ? 16 :0 );
}

int
CCyper::decrypt_aes(const unsigned char *inbuff, ssize_t len, unsigned char *outbuff)
{
    memset(iv, 0, sizeof(iv)); // the same iv
    int ret = AES_set_decrypt_key(secret_key, KEY_BIT, &aes_ks3);
    g_assert(ret==0);
    AES_cbc_encrypt(inbuff, outbuff, len, &aes_ks3, iv, AES_DECRYPT);
    return ret;
}

string
CCyper::decrypt_aes(const string instr)
{
    string ss;
    memset(iv, 0, sizeof(iv)); // the same iv
    int ret = AES_set_decrypt_key(secret_key, KEY_BIT, &aes_ks3);
    if (ret<0) return "";
    g_assert(ret==0);
    size_t len=instr.length();
    char *midbuf=new char [len];
    if (midbuf) {
        hex2bytes(instr,(unsigned char*)midbuf);
        char *outbuf=new char [len];
        if (outbuf) {
            AES_cbc_encrypt((unsigned char*)midbuf, (unsigned char*)outbuf, len, &aes_ks3, iv, AES_DECRYPT);
            ss=outbuf;
            delete outbuf;
        }
        delete midbuf;
    }
    return ss;
}


string 
CCyper::bytes2hex(const unsigned char *input, int len) 
{
	std::stringstream ss;
	ss << std::hex;
	for (int i = 0; i < len; ++i) {
		ss << std::setfill('0') << std::setw(2) << (int)input[i];
	}
	return ss.str();
}

gboolean 
CCyper::hex2bytes(string input, unsigned char *output) 
{
	size_t len = input.length();
	if (len % 2 != 0) {
		return false;
	}
	for (size_t i = 0; i < len; i += 2) {
		sscanf(input.c_str() + i, "%2hhx", output);
		++output;
	}
	
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

    string str=bytes2hex(digest,SHA256_DIGEST_LENGTH );
    // printf("SHA256 digest: %s\n", str.c_str());
    return str;
}

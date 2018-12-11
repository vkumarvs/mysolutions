#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
using namespace std;

extern "C"
{
#include <openssl/sha.h>
#include <sys/types.h>
#include <openssl/evp.h>
#include <openssl/err.h>
}

int GetAESCFB1Cipher()
{
    const unsigned char* session_key =  (const unsigned char*)"abcdefghijklmnopqrstuvwxyz123456";
    const unsigned char *session_iv =  (const unsigned char*)"1234567812345678";
    //const unsigned char *in = (const unsigned char*)"My name is vipin123456789123456789bla bla";
    const unsigned char *in = (const unsigned char*)"My name is vipin---User-Agent: curl/7.19.7 (x86_64-redhat-linux-gnu) libcurl/7.19.7 NSS/3.16.2.3 Basic ECC zlib/1.2.3 libidn/1.18 libssh2/1.4.2";
    string inStr = (const char*)in;
    int inLength = inStr.length();

    int                      ret;
    int                      len;
    size_t                   block_size;
    const EVP_CIPHER        *cipher;

    EVP_CIPHER_CTX           ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CIPHER_CTX_set_padding(&ctx, 1);

    cipher = EVP_aes_256_cfb1();
    block_size = EVP_CIPHER_block_size(cipher);

    unsigned char *memStr = new unsigned char[inLength];
    unsigned char* outStr = memStr;

    unsigned char *dmemStr = new unsigned char[inLength];
    unsigned char* doutStr = dmemStr;

    ret = EVP_EncryptInit_ex(&ctx, cipher, NULL, session_key,
                             session_iv);
    if (!ret)
    {
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }

    ret = EVP_EncryptUpdate(&ctx, outStr, &len, in, inLength);
    if (!ret)
    {
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }


    printf("length is %d --- block size is %Zu \n", len, block_size);
    outStr+=len;
    ret = EVP_EncryptFinal_ex(&ctx, outStr, &len);
    if (!ret)
    {
      printf("EVP_EncryptFinal failed\n");
      return 0;
    }

    printf("length is %d \n", len);
    std::string final((const char*)memStr);

    /* we should still explicitly release the ctx
     * or we'll leak memory here */
   // EVP_CIPHER_CTX_cleanup(&ctx);
    printf("out string %s  \n", memStr);


    len = 0;
    ret = EVP_DecryptInit(&ctx, EVP_aes_256_cfb1(), session_key, session_iv);
    if (!ret)
    {
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }

    ret = EVP_DecryptUpdate(&ctx, doutStr, &len, memStr,
                            (inLength));
    if (!ret)
    {
      printf("EVP_DecryptUpdate failed\n");
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }
    doutStr+=len;
    ret = EVP_DecryptFinal(&ctx, doutStr, &len);

    EVP_CIPHER_CTX_cleanup(&ctx);

    char err[128];
    unsigned long e;
    if (!ret)
    {       
        e = ERR_get_error();
        ERR_error_string(e, err);
        printf("EVP_DecryptFinal failed with err %d and is %s\n", e, err);
        return 0;
    }
    doutStr+=len;

    printf("out string %s  \n", dmemStr);

    return 0;
}

int CPPGetAESCFB1Cipher(std::string &inData)
{
    std::string session_key1 =  "abcdefghijklmnopqrstuvwxyz123456";
    std::string session_iv2 =  "1234567812345678";
    const unsigned char* session_key =  (const unsigned char*)session_key1.c_str();
    const unsigned char *session_iv = (const unsigned char*)session_iv2.c_str() ;

    const unsigned char *in = (const unsigned char*)inData.c_str();
    string inStr = (const char*)in;
    int inLength = inStr.length();

    int                      ret;
    int                      len;
    size_t                   block_size;
    const EVP_CIPHER        *cipher;

    EVP_CIPHER_CTX           ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CIPHER_CTX_set_padding(&ctx, 1);

    cipher = EVP_aes_256_cfb1();
    block_size = EVP_CIPHER_block_size(cipher);

    unsigned char *memStr = new unsigned char[inLength];
    unsigned char* outStr = memStr;

    unsigned char *dmemStr = new unsigned char[inLength];
    unsigned char* doutStr = dmemStr;

    ret = EVP_EncryptInit_ex(&ctx, cipher, NULL, session_key,
                             session_iv);
    if (!ret)
    {
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }

    ret = EVP_EncryptUpdate(&ctx, outStr, &len, in, inLength);
    if (!ret)
    {
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }


    printf("length is %d --- block size is %Zu \n", len, block_size);
    outStr+=len;
    ret = EVP_EncryptFinal_ex(&ctx, outStr, &len);
    if (!ret)
    {
      printf("EVP_EncryptFinal failed\n");
      return 0;
    }

    printf("length is %d \n", len);
    std::string final((const char*)memStr);

    /* we should still explicitly release the ctx
     * or we'll leak memory here */
   // EVP_CIPHER_CTX_cleanup(&ctx);
    printf("out string %s  \n", memStr);


    len = 0;
    ret = EVP_DecryptInit(&ctx, EVP_aes_256_cfb1(), session_key, session_iv);
    if (!ret)
    {
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }

    ret = EVP_DecryptUpdate(&ctx, doutStr, &len, memStr,
                            (inLength));
    if (!ret)
    {
      printf("EVP_DecryptUpdate failed\n");
      EVP_CIPHER_CTX_cleanup(&ctx);
      return 0;
    }
    doutStr+=len;
    ret = EVP_DecryptFinal(&ctx, doutStr, &len);

    EVP_CIPHER_CTX_cleanup(&ctx);

    char err[128];
    unsigned long e;
    if (!ret)
    {       
        e = ERR_get_error();
        ERR_error_string(e, err);
        printf("EVP_DecryptFinal failed with err %d and is %s\n", e, err);
        return 0;
    }
    doutStr+=len;
    std::string outfinal((const char*)dmemStr);
    printf("out string %s  \n", outfinal.c_str());

    return 0;
}

int main(int argc, char** argv)
{
    std::string inData= "My name is vipin---User-Agent: curl/7.19.7 (x86_64-redhat-linux-gnu) libcurl/7.19.7 NSS/3.16.2.3 Basic ECC zlib/1.2.3 libidn/1.18 libssh2/1.4.2";
 //   GetAESCFB1Cipher();

    CPPGetAESCFB1Cipher(inData);
    return 0;
}

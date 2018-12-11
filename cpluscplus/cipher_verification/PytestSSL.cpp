#include <openssl/conf.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <openssl/evp.h>
#include "Poco/Exception.h"
#include "Poco/Crypto/CipherImpl.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Exception.h"
#include <openssl/err.h>
#if 1
#include <Poco/Crypto/CipherKeyImpl.h>
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include <assert.h>
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#endif
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/SharedPtr.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SecureStreamSocket.h"
#include <memory>
#include <iostream>

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/FileStream.h"
#include "Poco/Crypto/CryptoStream.h"
#include <fstream>
#include <sstream>
#include "Poco/MemoryStream.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/DigestEngine.h"

#include <Poco/Runnable.h>

#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Ascii.h"
#include "Poco/String.h"
#include <string>
#include <string.h>
using namespace Poco::Net;
using Poco::NumberFormatter;
using Poco::Crypto::DigestEngine;
using Poco::Net::SSLManager;
using Poco::Net::Context;
using Poco::SyntaxException;
using Poco::SharedPtr;
using Poco::Exception;
using Poco::Net::HTTPStreamFactory;
using Poco::Net::HTTPSStreamFactory;
using namespace std;
using Poco::URI;
using Poco::StreamCopier;

#define INTERNAL_RSP_READ_BUFFER 4096
typedef intptr_t        ngx_int_t;
typedef uintptr_t       ngx_uint_t;
typedef intptr_t        ngx_flag_t;

typedef struct {
    size_t      len;
    unsigned char     *data;
} ngx_str_t;

void HandleOpenSslErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

static void
TlsEncodeBase64Url(std::string &result, std::string &src, const unsigned char *basis, ngx_uint_t padding)
{
    unsigned char         *d, *s;
    size_t          len;
    size_t          dstlen;

    unsigned char *data =  new unsigned char[src.length()*2];
    len = src.length();
    s = (unsigned char *)src.c_str();
    d = data;

    while (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];
        s += 3;
        len -= 3;
    }

    if (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        if (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            if (padding) {
                *d++ = '=';
            }

        } else {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        if (padding) {
            *d++ = '=';
        }
    }

    dstlen = d - data;
    std::string dgeststring((char *)data, dstlen);
    result = dgeststring;
    delete data;
}

void
TlsBase64Url(std::string &result, std::string &src, bool isBody)
{
    uintptr_t   padding=0 ;
    static unsigned char   *basis;
    if(isBody)
    {
      basis = (unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      padding=1 ;
    }
    else
    {
      basis = (unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
      padding=0 ;
    }

    //   TlsEncodeBase64Url(result, src, basis64, 0);
    unsigned char         *d, *s;
    size_t          len;
    size_t          dstlen;

    unsigned char *data =  new unsigned char[src.length()*2];
    len = src.length();
    s = (unsigned char *)src.c_str();
    d = data;

    while (len > 2) {
      *d++ = basis[(s[0] >> 2) & 0x3f];
      *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
      *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
      *d++ = basis[s[2] & 0x3f];
      s += 3;
      len -= 3;
    }

    if (len) {
      *d++ = basis[(s[0] >> 2) & 0x3f];

      if (len == 1) {
        *d++ = basis[(s[0] & 3) << 4];
        if (padding) {
          *d++ = '=';
        }

      } else {
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[(s[1] & 0x0f) << 2];
      }

      if (padding) {
        *d++ = '=';
      }
    }

    dstlen = d - data;
    std::string dgeststring((char *)data, dstlen);
    result = dgeststring;
    delete data;
}


void GetAESPartialDecryptedData(std::string &rs, std::string &outString, bool isHdr)
{
    Poco::Crypto::CipherFactory &factory = Poco::Crypto::CipherFactory::defaultFactory();
    std::string session_key =  "abcdefghijklmnopqrstuvwxyz123456";
    std::string session_iv =  "1234567812345678";
    std::vector<unsigned char> aesKeyVector(session_key.begin(),session_key.end());
    std::vector<unsigned char> keyIvVector(session_iv.begin(), session_iv.end());
    Poco::Crypto::CipherKey aesKey("aes256", aesKeyVector, keyIvVector);
    Poco::Crypto::Cipher* pCipher = factory.createCipher(aesKey);
    if(isHdr)
    {
      outString = pCipher->decryptString(rs, Poco::Crypto::Cipher::ENC_BASE64);
      //outString = pCipher->decryptString(rs);
    }
    else
    {
      outString = pCipher->decryptString(rs);
    }
}

void GetHexSHA256DigestedIdentifier(std::string &digestedMsg)
{
    std::string session_key =  "abcdefghijklmnopqrstuvwxyz123456";
    std::string keyidentifier = session_key;
    DigestEngine engine("SHA256");
    engine.update(keyidentifier);
    printf("key length %Zu \n", session_key.length());

    unsigned char     digest1[32];
    SHA256_CTX  sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, session_key.c_str(), session_key.length());
    SHA256_Final(digest1, &sha256);
#if 0

    digestedMsg = DigestEngine::digestToHex(engine.digest());

#else
	//typedef std::vector<unsigned char> Digest;
    DigestEngine::Digest digest = engine.digest();
	std::string digestedStr;
	for (DigestEngine::Digest::const_iterator it = digest.begin(); it != digest.end(); ++it)
	{
		unsigned char c = *it;
		digestedStr+= c;
	}
    TlsBase64Url(digestedMsg, digestedStr, false);

#endif
}

void GetBase64AESEncryptedHeader(std::string &encryptedHdr)
{
    std::string hdrString =  "cookie:My name is vipin dahiya";

    Poco::Crypto::CipherFactory &factory = Poco::Crypto::CipherFactory::defaultFactory();
    std::string session_key =  "abcdefghijklmnopqrstuvwxyz123456";
    std::string session_iv =  "1234567812345678";

    std::vector<unsigned char> aesKeyVector(session_key.begin(),session_key.end());
    std::vector<unsigned char> keyIvVector(session_iv.begin(), session_iv.end());
    Poco::Crypto::CipherKey aesKey("aes256", aesKeyVector, keyIvVector);
    Poco::Crypto::Cipher* pCipher = factory.createCipher(aesKey);
    encryptedHdr = pCipher->encryptString(hdrString, Poco::Crypto::Cipher::ENC_BASE64);

}

void InitializeEncrDecrpt(EVP_CIPHER_CTX* &ctx)
{
    int             ret;
    unsigned char *key = (unsigned char *)"abcdefghijklmnopqrstuvwxyz123456";
    unsigned char *iv = (unsigned char *)"1234567812345678";

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
      HandleOpenSslErrors();
    }

    ret = EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
  //  ret = EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb1(), NULL, key, iv);
    if(ret !=1)
    {
      HandleOpenSslErrors();
      EVP_CIPHER_CTX_cleanup(ctx);
    }
}
int GetHeaderAndBodyWthEncryptDecrypt(std::string &url, std::string
                                      &encryptedHdr,
                                      std::string &filepath,
                                      std::string &encryptfilepath)
{
    EVP_CIPHER_CTX *ctx;
    InitializeEncrDecrpt(ctx);
    try
    {
      printf("Complete URL (%s)\n", url.c_str());
      URI uri(url);
      HTTPClientSession session(uri.getHost(), uri.getPort());
      HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery(), "HTTP/1.1");
      request.set("user-agent", "Poco HTTPClientSession");
      request.add("X-encrypted-headers", encryptedHdr);
      session.sendRequest(request);
      HTTPResponse response;
      std::istream& rs = session.receiveResponse(response);
      //printing response headers
      printf("\n\n\n");
      cout << "RESPONSE HEADERS:" <<endl;
      cout << response.getReason() <<endl;
      cout << response.getStatus() <<endl;
      cout << response.getVersion() <<endl;

      string name;
      string value;
      NameValueCollection::ConstIterator i = response.begin();
      while(i!=response.end())
      {
        name=i->first;
        value=i->second;
        if(name.compare("X-encrypted-headers") == 0)
        {
          //Decrypt headers
          //cout << name << "=" << value<< endl << flush;
          std::string outString;
          GetAESPartialDecryptedData(value, outString, true);
          cout <<outString;

        }
        else
        {
          cout << name << "=" << value << endl << flush;
        }
        ++i;
      }
#if 1

      int ret;
      std::string output;
      remove(filepath.c_str());
      std::ofstream out(filepath, std::ofstream::binary|std::ofstream::app);

      //encrypted files
      remove(encryptfilepath.c_str());
      std::ofstream out1(encryptfilepath, std::ofstream::binary|std::ofstream::app);
      unsigned char plaintext[INTERNAL_RSP_READ_BUFFER + EVP_MAX_BLOCK_LENGTH];
      unsigned char ciphertext[INTERNAL_RSP_READ_BUFFER];
      int outLength;
      int totallength = 0;
      int encodedlength = 0;
      while(!rs.eof())
      {
        rs.read ((char *)ciphertext, INTERNAL_RSP_READ_BUFFER);
        int ciphertext_len = rs.gcount();
        out1.write ((char *)ciphertext,(size_t)ciphertext_len);
        output.append((char *)ciphertext,(size_t)ciphertext_len);
        totallength +=ciphertext_len;
        ret = EVP_DecryptUpdate(ctx, plaintext, &outLength, ciphertext, ciphertext_len) ;
        if(ret !=1)
        {
          HandleOpenSslErrors();
          EVP_CIPHER_CTX_cleanup(ctx);
        }
        encodedlength+=outLength;
        out.write ((char *)plaintext,(size_t)outLength);
      }
      ret = EVP_DecryptFinal_ex(ctx, plaintext, &outLength);
      if(ret !=1)
      {
        EVP_CIPHER_CTX_cleanup(ctx);
      }
      encodedlength+=outLength;
      if(outLength > 0)
      {
        out.write ((char *)plaintext,(size_t)outLength);
        printf("writng last byte (%d)\n", outLength);
      }
      printf("RECEIVED (%d) --  last byte (%d) - total decoded (%d) -- extra(%d)\n",totallength, outLength, encodedlength, (totallength-encodedlength));




      out.close();
      out1.close();
#endif
      EVP_CIPHER_CTX_free(ctx);
      {
        std::string duplicate = "îmÀÜÃ<8a>~£ÛmLÖY^GØ,C^W×Ö ^L>Û!^D<93><81>$^LÓö[^?P^?<94>ï¢Xk±Ï'õu<81>z\n";
        std::string pre_copy_file  = "/home/vkumar/VIEW/testresult/copy_encrypted_public_inside_hello.txt";
        std::ifstream is (encryptfilepath, std::ifstream::binary);
        std::ifstream precopy (pre_copy_file, std::ifstream::binary);

        printf("encrypted string (%s)\n", output.c_str());
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);
        char * buffer = new char [length];
        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        is.read (buffer,length);

        std::string  output1;
        output1.append(buffer, length);

        precopy.seekg (0, is.end);
        int length1 = precopy.tellg();
        precopy.seekg (0, precopy.beg);
        char * buffer1 = new char [length];
        std::cout << "Reading " << length1 << " characters... ";
        // read data as a block:
        precopy.read (buffer1,length1);

        std::string  output2;
        output2.append(buffer1, length1);

       // if(output1.compare(output2) == 0)
        if(duplicate.compare(output1) == 0)
        {
          printf("encrypted text is equal \n");
          //printf("encrypted text is equal (%s) -- (%s)\n", output.c_str(), output1.c_str());
          //printf("encrypted text is equal (%s) -- (%s)\n", duplicate.c_str(), output1.c_str());
        }
        else
        {
          printf("encrypted text is not equal \n");
          //printf("encrypted text is not equal (%s) -- (%s)\n", duplicate.c_str(), output1.c_str());
        }
      is.close();
      precopy.close();
      }
    }
    catch (Exception& ex)
    {
      std::cout <<"ERROR:----" << ex.displayText() << std::endl;
      return 1;
    }
}


int GetHeaderAndBodyNormalHttpSession(std::string &url, std::string &filepath)
{
    try
    {
      printf("Complete URL (%s)\n", url.c_str());
      URI uri(url);
      HTTPClientSession session(uri.getHost(), uri.getPort());
      HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery(), "HTTP/1.1");
      request.set("user-agent", "Poco HTTPClientSession");
      request.add("-securelink", "true");

      session.sendRequest(request);
      HTTPResponse response;
      std::istream& rs = session.receiveResponse(response);
      //printing response headers
      printf("\n\n\n");
      cout << "RESPONSE HEADERS:" <<endl;
      cout << response.getReason() <<endl;
      cout << response.getStatus() <<endl;
      cout << response.getVersion() <<endl;

      string name;
      string value;
      NameValueCollection::ConstIterator i = response.begin();
      while(i!=response.end())
      {
        name=i->first;
        value=i->second;
        cout << name << "=" << value << endl << flush;
        ++i;
      }
      int ret;
      std::string output;
      remove(filepath.c_str());
      std::ofstream out(filepath, std::ofstream::binary|std::ofstream::app);
      unsigned char ciphertext[INTERNAL_RSP_READ_BUFFER];
      int totallength = 0;
      while(!rs.eof())
      {
        rs.read ((char *)ciphertext, INTERNAL_RSP_READ_BUFFER);
        int ciphertext_len = rs.gcount();
        totallength +=ciphertext_len;
        out.write ((char *)ciphertext,(size_t)ciphertext_len);
        output.append((char *)ciphertext,(size_t)ciphertext_len);
      }
      printf("RECEIVED --------(%d) \n",totallength);
      out.close();

      //compare result
      {
        std::string matchstr = "This file is being used for SSL pytest\n";
        if(output.compare(matchstr) == 0)
        {
          printf("encrypted text is equal (%s) -- (%s) \n", output.c_str(), matchstr.c_str());
        }
        else
        {
          printf("encrypted text is not equal (%s) -- (%s) \n", output.c_str(), matchstr.c_str());
        }
      }
    }
    catch (Exception& ex)
    {
      std::cout <<"ERROR:----" << ex.displayText() << std::endl;
      return 1;
    }
}

void PublicNetworkTest()
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    std::string digestedMsg;
    std::string encryptedHdr;
    GetHexSHA256DigestedIdentifier(digestedMsg);
    GetBase64AESEncryptedHeader(encryptedHdr);
    printf("digested msg string is (%s) -- hdrString is (%s) \n",
           digestedMsg.c_str(), encryptedHdr.c_str());

    std::string url;
    std::string filepath;
    std::string encryptfilepath;
    //Public network and outside the
    filepath = "/home/vkumar/VIEW/testresult/public_outside_hello.txt";
    encryptfilepath = "/home/vkumar/VIEW/testresult/encrypted_public_outside_hello.txt";
    url = "http://10.0.2.15:8080/" + digestedMsg  + "/outside__hello.txt";
    GetHeaderAndBodyWthEncryptDecrypt(url, encryptedHdr, filepath, encryptfilepath);

    //Public network and inside the
    filepath = "/home/vkumar/VIEW/testresult/public_inside_hello.txt";
    encryptfilepath = "/home/vkumar/VIEW/testresult/encrypted_public_inside_hello.txt";
    url = "http://10.0.2.15:8080/" + digestedMsg  + "/inside__hello.txt";
    GetHeaderAndBodyWthEncryptDecrypt(url, encryptedHdr, filepath,
                                      encryptfilepath);

    /* Clean up */
    EVP_cleanup();
    ERR_free_strings();
}

void PrivateNetworkTest()
{
    std::string url;
    std::string filepath;
    //Private network and outside the
    filepath = "/home/vkumar/VIEW/testresult/private_outside_hello.txt";
    url = "http://192.168.0.107:8080/outside__hello.txt";
    GetHeaderAndBodyNormalHttpSession(url, filepath);

    //Private network and inside the
    filepath = "/home/vkumar/VIEW/testresult/private_inside_hello.txt";
    url = "http://192.168.0.107:8080/inside__hello.txt";
    GetHeaderAndBodyNormalHttpSession(url, filepath);

}

int main(int argc, char** argv)
{
    printf("\n\n ***********Public Network Test****************\n");
    PublicNetworkTest();

    printf("\n\n ***********Private Network Test****************\n");
    PrivateNetworkTest();
    return 0;
}


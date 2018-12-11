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
#include <Poco/ThreadPool.h>

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

#include <pthread.h>
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
pthread_mutex_t count_mutex;
#if 0
const string session_key   = "abcdefghijklmnopqrstuvwxyz123456";
const string session_iv    = "1234567812345678";
#endif
inline int usleep(useconds_t usecs)
{
    int ret;
    struct timespec req;
    struct timespec rem;
    req.tv_sec = usecs / 1000000;
    req.tv_nsec = (usecs % 1000000) * 1000;
    while(((ret = nanosleep(&req, &rem)) == -1) && (errno == EINTR)) {
      req = rem;
    }
    return ret;
}
int x = 0;
Poco::Crypto::Cipher* pCipher;
void VerifyEncryptionDecryption(std::string &inData)
{
    try
    {
      pthread_mutex_lock(&count_mutex);
      x++;

      std::string outEncrypted = pCipher->encryptString(inData, Poco::Crypto::Cipher::ENC_BASE64);
      printf("Encrypted data %s\n", outEncrypted.c_str());
      outEncrypted+="hello";
      printf("%d Hello 1\n", x);
#if 0
      std::string outDecrypted = pCipher->decryptString(outEncrypted,
                                                        Poco::Crypto::Cipher::ENC_BASE64);
      printf("Hello 2\n");
      if(inData == outDecrypted)
      {
        printf("Data is same after decryption %s\n", outDecrypted.c_str());
      }
      else
      {
      }
      printf("Decryption failed\n");
      // assert (inData == outDecrypted);
#endif
      pthread_mutex_unlock(&count_mutex);
    }
catch (Exception& ex)  {
      printf(" Cookie exception (%s)\n", ex.displayText().c_str());
      return;
    }
}

class TlsRunnable : public Poco::Runnable
{
public:
    TlsRunnable()
    {
    }
    void run()
    {
    //connectSSL();
      std::string inData= "User-Agent: curl/7.19.7 (x86_64-redhat-linux-gnu) libcurl/7.19.7 NSS/3.16.2.3 Basic ECC zlib/1.2.3 libidn/1.18 libssh2/1.4.2";
      VerifyEncryptionDecryption(inData);
      delete this;
    }
private:
};  // class TlsRunnable

int main(int argc, char** argv)
{
#if 0
static const string session_key;
static const string session_iv;
#endif
#if 0
   std::string inData= "User-Agent: curl/7.19.7 (x86_64-redhat-linux-gnu) libcurl/7.19.7 NSS/3.16.2.3 Basic ECC zlib/1.2.3 libidn/1.18 libssh2/1.4.2";
   for(int x = 0; x < 100; x++)
   {
     VerifyEncryptionDecryption(inData);
   }
#else
      Poco::Crypto::CipherFactory &factory = Poco::Crypto::CipherFactory::defaultFactory();
    std::string session_key =  "abcdefghijklmnopqrstuvwxyz123456";
    std::string session_iv =  "1234567812345678";
      std::vector<unsigned char> aesKeyVector(session_key.begin(),session_key.end());
      std::vector<unsigned char> keyIvVector(session_iv.begin(), session_iv.end());
      Poco::Crypto::CipherKey aesKey("aes256", aesKeyVector, keyIvVector);
     pCipher = factory.createCipher(aesKey);

    Poco::ThreadPool*                   pocoThreadPool_;
      int minCapacity = 2;
      int maxCapacity = 20;
      int idleTimeSecs = 60;
      pocoThreadPool_ = new Poco::ThreadPool("DynamicPopInternalClientThreadPool", minCapacity, maxCapacity, idleTimeSecs);
   for(int x = 0; x < 100000; x++)
   {
     TlsRunnable * myRunnable = new TlsRunnable();
      pocoThreadPool_->start(*myRunnable);
#if 0
     Poco::Thread *thread = new Poco::Thread("MsdInternalResponseThrd");
     thread->start(*myRunnable);
     thread->join();
#endif
      usleep(1000);
   }
    pocoThreadPool_->stopAll();
    delete pocoThreadPool_;
    delete pCipher;
#endif
   return 0;
}


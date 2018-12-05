
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <sys/types.h>
#include "Poco/Exception.h"
#include "Poco/Exception.h"
#if 1
#include <Poco/Crypto/CipherKeyImpl.h>
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include <assert.h>
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#endif
#include <memory>
#include <iostream>

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/FileStream.h"
#include <fstream> 
#include <sstream>
#include "Poco/MemoryStream.h"
#include "Poco/Base64Encoder.h"

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
#include <time.h>
#include <errno.h>                                                                                   
#include <Poco/ThreadPool.h>
using namespace Poco::Net;
using Poco::NumberFormatter;
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

static int64_t errors = 0;
static int64_t noLMTHeaders = 0;
static int64_t requestSent = 0;
static int64_t mo_400_response = 0;
static int64_t mo_500_response = 0;
static int64_t mo_503_response = 0;
static int64_t mo_200_response = 0;

// 192.168.0.107 -- large.pod1.cache1.par.cdx-dev.unifieddeliverynetwork.net
struct LoadMssRequests
{
  //std::vector<std::string> args;
  const char *args[11] = {
        //MSS links
        "http://192.168.0.107/msd/netflix/sample.ism/manifest",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(381989)/Fragments(audio=0)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(863000)/Fragments(video=0)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(381989)/Fragments(audio=20053333)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(863000)/Fragments(video=84000000)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(381989)/Fragments(audio=40106666)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(863000)/Fragments(video=129600000)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(381989)/Fragments(audio=60160000)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(381989)/Fragments(audio=80000000)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(381989)/Fragments(audio=100053333)",
        "http://192.168.0.107/msd/netflix/sample.ism/QualityLevels(381989)/Fragments(audio=120106666)",
      };
  LoadMssRequests(){}
};

struct LoadHlsRequests
{
  const char *args[8] = {
        //HLS links
        "http://192.168.0.107/msd/netflix/sample.ism/sample.m3u8",
        "http://192.168.0.107/msd/netflix/sample.ism/sample-audio=381989-video=863000-1.ts",
        "http://192.168.0.107/msd/netflix/sample.ism/sample-audio=381989-video=863000-2.ts",
        "http://192.168.0.107/msd/netflix/sample.ism/sample-audio=381989-video=863000-3.ts",
        "http://192.168.0.107/msd/netflix/sample.ism/sample-audio=381989.m3u8",
        "http://192.168.0.107/msd/netflix/sample.ism/sample-audio=381989-1.ts",
        "http://192.168.0.107/msd/netflix/sample.ism/sample-audio=381989-1.ts",
        "http://192.168.0.107/msd/netflix/sample.ism/sample-audio=381989-2.ts",
      };
  LoadHlsRequests() {}
};

struct LoadDashRequests
{
  const char *args[13] = {
        //DASH links
        "http://192.168.0.107/msd/netflix/sample.ism/sample.mpd",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-video=863000-0.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989-0.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-video=863000.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989-96256.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989-192512.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989-288768.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989-384000.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-video=863000-107520.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989-480256.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-audio=381989-576512.dash",
        "http://192.168.0.107/msd/netflix/sample.ism/dash/sample-video=863000-165888.dash"
      };
  LoadDashRequests() { }
};

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
int GetHeaderAndBodyNormalHttpSession(std::string &url, std::string type)
{
    try
    {
      //printf("Complete URL (%s)\n", url.c_str());
      URI uri(url);
      HTTPClientSession session(uri.getHost(), uri.getPort());
      HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery(), "HTTP/1.1");
     // request.set("user-agent", "Poco HTTPClientSession");
      //request.add("UDN-securelink", "true");

      //request.add("X-client-token", "innermatch");
      //request.add("X-seq-match", "seqtest1");
      //Set session timeout to 10 minutes
      //session.setTimeout(Poco::Timespan(1200L,0L));
      session.sendRequest(request);
      __sync_add_and_fetch(&(requestSent),1);
#if 0
      Poco::Net::StreamSocket &str = session.socket();
      str.setReceiveTimeout(Poco::Timespan(1200L,0L));
#endif
      HTTPResponse response;
      std::istream& rs = session.receiveResponse(response);
      //printing response headers
#if 0
      printf("\n\n\n");
      cout << "RESPONSE HEADERS:" <<endl;
      cout << response.getReason() <<endl;
      cout << response.getStatus() <<endl;
      cout << response.getVersion() <<endl;
#endif

      if(response.getStatus() != 200)
      {
        printf("status is not 200 -- for(%s--%d) \n", url.c_str(), response.getStatus());
        if(response.getStatus() == 400)
        {
        __sync_add_and_fetch(&(mo_400_response),1);
        }

        if(response.getStatus() == 500)
        {
        __sync_add_and_fetch(&(mo_500_response),1);
        }

        if(response.getStatus() == 503)
        {
        __sync_add_and_fetch(&(mo_503_response),1);
        }

      }
      else
      {
        __sync_add_and_fetch(&(mo_200_response),1);
      }

      string name;
      string lmtheader;
      bool isLMTHeaderPresent = false;
      string value;
      NameValueCollection::ConstIterator i = response.begin();
      while(i!=response.end())
      {
        name=i->first;
        value=i->second;
        if(name.compare("Last-Modified") == 0)
        {
          isLMTHeaderPresent = true;
          //printf("Last Modified header:%s \n", value.c_str());
        }

        //cout << name << "=" << value << endl << flush;
        ++i;
      }
      if(!isLMTHeaderPresent)
      {
       __sync_add_and_fetch(&(noLMTHeaders),1);
        printf("Last Modified header is not present (%s)\n", url.c_str());
      }
#if 0
      int ret;
      remove("/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples/vipin.mp4");
      remove("/opt/universalcache/upload/.cache/*");
      std::ofstream out("/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples/vipin.mp4", std::ofstream::binary|std::ofstream::app);
      //flush any previous ontent
      //out.flush();
      unsigned char ciphertext[INTERNAL_RSP_READ_BUFFER];
      int totallength = 0;
      while(!rs.eof())
      {
        rs.read ((char *)ciphertext, INTERNAL_RSP_READ_BUFFER);
        int ciphertext_len = rs.gcount();
        totallength +=ciphertext_len;
        out.write ((char *)ciphertext,(size_t)ciphertext_len);
      }
      //printf("RECEIVED --------(%d) \n",totallength);
      out.close();
#endif
    }
    catch (Exception& ex)
    {
       __sync_add_and_fetch(&(errors),1);
      std::cout <<"ERROR(%s-%s):----"<<type <<"--" <<url <<"---" << ex.displayText() << std::endl;
      return 1;
    }
}

class MsdMSSThread : public Poco::Runnable
{
public:
    MsdMSSThread() 
    {
    }
    void run()
    {
      LoadMssRequests mp4urls;
      int max_array_size = 11;
      std::string url;
      for (int x = 0; x < max_array_size; x++)
      {
        url = mp4urls.args[x];
        GetHeaderAndBodyNormalHttpSession(url, "MSS");
        usleep(200000);
      }
      delete this;
    }
};

class MsdHlsThread: public Poco::Runnable
{
public:
    MsdHlsThread() 
    {
    }
    void run()
    {
      LoadHlsRequests mp4urls;
      int max_array_size = 8;
      std::string url;
      for (int x = 0; x < max_array_size; x++)
      {
        url = mp4urls.args[x];
        GetHeaderAndBodyNormalHttpSession(url, "HLS");
        usleep(200000);
      }
      delete this;
    }
};

class MsdDashThread: public Poco::Runnable
{
public:
    MsdDashThread() 
    {
    }
    void run()
    {
      LoadDashRequests mp4urls;
      int max_array_size = 13;
      std::string url;
      for (int x = 0; x < max_array_size; x++)
      {
        url = mp4urls.args[x];
        GetHeaderAndBodyNormalHttpSession(url, "DASH");
        usleep(200000);
      }
      delete this;
    }
};

int main(int argc, char** argv)
{
    Poco::ThreadPool*                   pocoThreadPool_;
    int minCapacity = 2;
    int maxCapacity = 160;
    int idleTimeSecs = 120;
    try
    {
      pocoThreadPool_ = new Poco::ThreadPool("MsdThreadPool", minCapacity, maxCapacity, idleTimeSecs);

      static int y =0;
      for(int x = 0; x < 30; x++)
      {
        MsdMSSThread* myRunnable1 = new MsdMSSThread();
        pocoThreadPool_->start(*myRunnable1);

        MsdHlsThread* myRunnable2 = new MsdHlsThread();
        pocoThreadPool_->start(*myRunnable2);

        MsdDashThread* myRunnable3 = new MsdDashThread();
        pocoThreadPool_->start(*myRunnable3);
        usleep(200000);
      }
      pocoThreadPool_->joinAll();
    }
    catch (Exception& ex)
    {
      std::cout <<"ERROR(%s):----"<< ex.displayText() << std::endl;
      return 1;
    }
    printf("Stats: requestSent(%"  PRIu64 ") 200_response (%" PRIu64 ") 400_response (%" PRIu64 ") 500_response(%" PRIu64 ") 503_response (%" PRIu64 ") errors(%"  PRIu64 ")-- noLMT(%" PRIu64 ")\n",
           requestSent, mo_200_response, mo_400_response, mo_500_response, mo_503_response, errors, noLMTHeaders);
    delete pocoThreadPool_;
    return 0;
}


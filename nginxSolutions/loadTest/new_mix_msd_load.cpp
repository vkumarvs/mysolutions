/*
 * Total of 1143 Requests will be sent by this program
 * HLS: 284*2 = 568
 * HSS: 284
 * DASH:291
 *
 */

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
#include "Poco/Util/Timer.h"
#include "Poco/Util/TimerTask.h"


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
static uint64_t currentClientLoadRate = 0;
static uint64_t peakClientLoadRate = 0;
static uint64_t totalClientReqTillLastSample = 0;

// 192.168.0.107 -- large.pod1.cache1.par.cdx-dev.unifieddeliverynetwork.net
struct LoadMssRequests
{
   //std::vector<std::string> args;
    //const char *args[249] = {
    const char *args[378] = {
      //MSS links
      "http://192.168.0.107/msd/elephant/elephant.ism/manifest",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(64000)/Fragments(audio_spa=0)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=0)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(64000)/Fragments(audio_eng=0)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=26250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=52916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=79583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=106250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=132916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=159583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=186250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=212916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=239583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=266250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=292916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=319583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=346250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=372916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=399583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=426250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=452916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=479583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=506250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=532916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=559583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=586250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=612916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=639583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=666250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=692916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=719583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=746250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=772916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=799583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=826250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=852916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=879583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=906250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=932916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=959583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=986250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=1012916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=1039583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1066250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1092916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1119583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1146250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1172916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1199583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1226250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1252916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1279583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1306250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=1332916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=1359583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1386250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1412916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1439583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1466250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=1492916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1519583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1546250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1572916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=1599583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=1626250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=1652916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1679583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1706250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1732916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1759583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1786250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1812916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1839583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1866250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1892916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1919583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1946250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1972916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=1999583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2026250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2052916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2079583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2106250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2132916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2159583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2186250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2212916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2239583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2266250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2292916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2319583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2346250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2372916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2399583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2426250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2452916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2479583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2506250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2532916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=2559583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2586250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2612916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2639583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2666250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2692916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2719583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2746250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2772916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2799583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2826250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2852916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2879583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2906250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2932916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2959583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=2986250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3012916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3039583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3066250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3092916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3119583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3146250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3172916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3199583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3226250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3252916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3279583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3306250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3332916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3359583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3386250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3412916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3439583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3466250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3492916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3519583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3546250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3572916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3599583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3626250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3652916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3679583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3706250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3732916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3759583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3786250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3812916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=3839583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3866250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3892916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3919583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3946250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3972916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=3999583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4026250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4052916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4079583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4106250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4132916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4159583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4186250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4212916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4239583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4266250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4292916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4319583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4346250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4372916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4399583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4426250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=4452916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=4479583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4506250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4532916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4559583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4586250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4612916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4639583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4666250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4692916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4719583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4746250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=4772916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4799583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4826250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4852916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4879583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4906250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4932916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4959583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=4986250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=5012916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=5039583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5066250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5092916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5119583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5146250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5172916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5199583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5226250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5252916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5279583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5306250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5332916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5359583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5386250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5412916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5439583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5466250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5492916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5519583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5546250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5572916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5599583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5626250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5652916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=5679583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=5706250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=5732916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=5759583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=5786250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=5812916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=5839583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=5866250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=5892916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=5919583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=5946250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=5972916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=5999583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=6026250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6052916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6079583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6106250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6132916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6159583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6186250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6212916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6239583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6266250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6292916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6319583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6346250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6372916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6399583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(807000)/Fragments(video_eng=6426250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=6452916667)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(551000)/Fragments(video_eng=6479583333)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=6506250000)",
      "http://192.168.0.107/msd/elephant/elephant.ism/QualityLevels(354000)/Fragments(video_eng=6532916667)",
       //Earth links 129 lines
#if 1
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/manifest",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=0)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(591000)/Fragments(video_eng=209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=20053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=16209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=40106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=32209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=60160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=48209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=80000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=64209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=100053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=80209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=120106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=96209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=140160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=112209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=160000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=128209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=180053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=144209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=200106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=160209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=220160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=176209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=240000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=192209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=260053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=208209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=280106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=224209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=300160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=240209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=320000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=256209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=340053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=272209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=360106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=288209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=380160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=304209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=400000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=320209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=420053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=336209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=440106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=352209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=460160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=368209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=480000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=384209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=500053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=400209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=520106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=416209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=540160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=432209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=560000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=448209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=580053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=464209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=600106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=480209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=620160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=496209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=640000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=512209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=660053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=528209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=680106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=544209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=700160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=560209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=720000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=576209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=740053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=592209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=760106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=608209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=780160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=624209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=800000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=640209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=820053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=656209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=840106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=672209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=860160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=688209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=880000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=704209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=900053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=720209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=920106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=736209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=940160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=752209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=960000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=768209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=980053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=784209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=1000106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=800209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=1020160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=816209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=1040000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=832209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=1060053333)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=848209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=1080106666)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=864209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=1100160000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=880209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(128930)/Fragments(audio_eng=1120000000)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=896209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=912209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=928209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=944209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=960209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=976209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=992209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1008209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1024209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1040209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1056209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1072209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1088209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1104209635)",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/QualityLevels(1183000)/Fragments(video_eng=1120209635)",
#endif
    };
  LoadMssRequests(){}
};


struct LoadHlsRequests
{
    //const char *args[284] = {
    const char *args[341] = {
    //const char *args[253] = {
      //HLS links
      "http://192.168.0.107/msd/elephant/elephant.ism/high.m3u8",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=354000.m3u8",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=354000-1.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=354000-2.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000.m3u8",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000-1.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000-2.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000-3.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000.m3u8",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-3.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-4.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-5.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-6.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-7.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-8.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-9.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-10.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-11.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-12.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-13.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-14.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-15.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-16.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-17.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-18.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-19.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-20.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-21.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-22.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-23.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-24.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-25.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-26.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-27.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-28.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-29.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-30.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-31.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-32.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-33.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-34.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-35.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-36.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-37.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-38.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-39.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-40.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-41.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-42.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-43.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-44.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-45.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-46.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-47.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-48.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-49.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-50.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-51.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-52.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-53.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-54.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-55.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-56.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-57.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-58.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-59.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-60.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-61.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-62.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-63.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-64.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-65.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-66.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-67.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-68.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-69.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-70.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-71.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-72.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-73.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-74.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-75.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-76.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-77.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-78.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-79.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-80.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-81.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-82.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-83.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-84.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-85.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-86.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-87.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-88.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-89.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-90.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-91.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-92.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-93.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-94.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-95.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-96.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-97.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-98.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-99.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-100.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-101.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-102.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-103.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-104.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-105.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-106.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-107.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-108.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-109.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-110.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-111.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-112.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-113.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-114.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-115.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-116.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-117.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-118.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-119.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-120.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-121.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-122.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-123.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-124.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-125.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-126.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-127.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-128.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-129.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-130.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-131.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-132.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-133.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-134.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-135.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-136.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-137.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-138.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-139.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-140.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-141.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-142.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-143.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-144.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-145.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-146.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-147.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-148.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-149.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-150.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-151.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-152.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-153.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-154.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-155.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-156.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-157.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-158.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-159.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-160.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-161.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-162.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-163.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-164.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-165.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-166.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-167.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-168.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-169.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-170.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-171.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-172.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-173.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-174.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-175.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-176.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-177.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-178.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-179.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-180.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-181.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-182.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-183.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-184.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-185.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-186.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-187.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-188.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-189.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-190.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-191.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-192.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-193.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-194.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-195.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-196.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-197.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-198.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-199.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-200.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-201.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-202.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-203.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-204.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-205.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-206.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-207.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-208.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-209.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-210.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-211.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-212.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-213.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-214.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-215.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-216.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-217.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-218.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-219.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-220.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-221.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-222.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-223.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-224.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-225.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-226.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-227.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-228.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-229.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-230.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-231.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-232.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-233.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-234.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-235.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-236.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-237.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-238.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-239.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-240.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-241.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-242.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-243.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-244.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-245.ts",
      "http://192.168.0.107/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-246.ts",
      //Earth Links -- 88 lines
#if 1
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/sample.m3u8",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=591000.m3u8",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=591000-1.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=591000-2.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000.m3u8",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-1.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-2.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-3.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-4.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-5.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-6.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-7.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-8.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-9.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-10.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-11.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-12.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000.m3u8",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-2.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-3.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-4.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-5.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-6.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-7.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-8.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-9.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-10.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-11.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-12.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-13.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-14.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-15.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-16.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-17.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-18.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-19.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-20.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-21.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-22.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-23.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-24.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-25.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-26.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-27.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-28.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-29.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-30.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-31.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-32.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-33.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-34.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-35.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-36.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-37.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-38.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-39.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-40.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-41.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-42.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-43.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-44.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-45.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-46.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-47.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-48.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-49.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-50.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-51.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-52.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-53.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-54.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-55.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-56.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-57.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-58.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-59.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-60.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-61.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-62.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-63.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-64.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-65.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-66.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-67.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-68.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-69.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-70.ts",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-71.ts",
#endif
    };
  LoadHlsRequests() {}
};


struct LoadDashRequests
{
    //const char *args[291] = {
    //const char *args[245] = {
    const char *args[378] = {
      //DASH links
      "http://192.168.0.107/msd/elephant/elephant.ism/high.mpd",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-0.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-audio_eng=64000-0.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-0.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-audio_eng=64000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-2625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-5291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-7958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-10625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-13291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-15958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-18625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-21291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-23958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-26625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-29291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-31958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-34625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-37291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-39958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-42625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-45291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-47958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-50625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-53291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-60000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-55958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-58625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-61291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-63958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-66625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-69291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-71958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-74625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-77291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-79958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-82625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-85291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-87958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-90625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-93291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-95958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-98625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-101291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-103958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-106625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-109291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-111958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-114625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-120000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-117291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-119958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-122625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-125291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-127958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-130625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-133291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-133291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-133291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-135958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-138625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-141291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-143958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-146625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-149291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-151958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-154625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-157291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-159958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-162625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-165291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-167958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-170625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-173291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-175958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-178625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-181291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-180000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-183958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-186625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-189291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-191958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-194625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-197291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-199958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-202625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-205291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-207958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-210625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-213291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-215958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-218625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-221291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-223958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-226625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-229291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-231958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-234625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-240000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-237291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-239958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-242625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-245291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-247958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-245291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-247958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-250625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-253291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-255958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-258625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-261291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-263958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-266625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-269291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-271958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-274625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-277291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-279958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-282625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-285291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-287958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-290625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-293291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-295958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-298625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-301291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-300000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-303958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-306625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-309291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-311958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-314625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-317291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-314625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-317291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-319958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-319958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-322625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-325291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-327958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-330625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-327958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-330625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-333291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-335958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-338625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-341291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-343958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-346625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-349291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-351958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-354625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-360000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-357291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-359958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-362625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-365291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-367958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=354000-365291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-367958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-370625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-373291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-375958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-378625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-381291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-383958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-386625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-389291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-386625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-391958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-394625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-397291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-397291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-399958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-402625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-405291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-407958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-410625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-413291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-420000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-415958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-418625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-421291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-423958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-426625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-429291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-431958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-434625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-437291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-439958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-442625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-445291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-447958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-450625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-453291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-455958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-458625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-461291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-463958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-466625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-469291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-textstream_eng=1000-480000.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-471958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-474625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-477291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-479958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-482625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-485291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-487958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-490625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-493291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-495958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=551000-493291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-495958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-498625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-501291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-503958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-506625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-509291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-511958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-514625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-517291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-519958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-522625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-525291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-527958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-530625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-533291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-535958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-538625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-541291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-543958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-546625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-549291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-551958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-554625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-557291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-559958.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-562625.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-565291.dash",
      "http://192.168.0.107/msd/elephant/elephant.ism/dash/elephant-video_eng=807000-567958.dash",
      //Earth Links //133 lines
#if 1
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/sample.mpd",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000-322.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-0.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000-24898.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-96256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000-49474.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-192512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000-74050.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000-98626.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-288768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000-123202.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-384000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=591000-147778.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-480256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-172354.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-576512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-196930.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-147778.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-221506.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-672768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-246082.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-768000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-270658.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-864256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-295234.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-960512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-319810.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-344386.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1056768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-368962.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1152000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-393538.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1248256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-418114.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1344512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-442690.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-467266.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1440768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-491842.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1536000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-516418.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1632256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-540994.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1728512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-565570.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-590146.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1824768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-614722.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-1920000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-639298.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2016256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-663874.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2112512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-688450.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-713026.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2208768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-737602.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2304000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-762178.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2400256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-786754.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2496512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-811330.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-835906.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2592768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-860482.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2688000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2784256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-885058.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2880512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-909634.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-934210.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-2976768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-958786.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3072000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-983362.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3168256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1007938.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3264512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1032514.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1057090.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3360768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1081666.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3456000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1106242.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3552256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1130818.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3648512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1155394.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1179970.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3744768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1204546.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3840000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1229122.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-3936256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1253698.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1278274.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4032512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1302850.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4128768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1327426.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1352002.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4224000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4320256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1376578.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1401154.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1425730.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4416512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4512768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1450306.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4608000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1474882.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1499458.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4704256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4800512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1524034.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1548610.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4896768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1573186.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1597762.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-4992000.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1622338.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-5088256.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1646914.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-5184512.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1671490.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-5280768.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1696066.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-video_eng=7868000-1720642.dash",
      "http://192.168.0.107/msd/netflix/multibitAudioSep/source_manifest.ism/dash/source_manifest-audio_eng=128930-5376000.dash",
#endif
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
        //printf("Last Modified header is not present (%s)\n", url.c_str());
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
#else
      unsigned char ciphertext[INTERNAL_RSP_READ_BUFFER];
      int totallength = 0;
      while(!rs.eof())
      {
        rs.read ((char *)ciphertext, INTERNAL_RSP_READ_BUFFER);
        int ciphertext_len = rs.gcount();
        totallength +=ciphertext_len;
      }
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
      //int max_array_size = 249;
      int max_array_size = 378;
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
      //int max_array_size = 253;
      int max_array_size = 341;
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
      //int max_array_size = 245;
      int max_array_size = 378;
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

class KPITimer : public Poco::Util::Timer                                               
{                                                                                       
protected:
    void run();                                                                         
};
  
class MinuteKPITask : public Poco::Util::TimerTask
{
public:
    void run();
};

void MinuteKPITask::run()
{
    //Calculate the MSD Client req rate per second
    int x = requestSent;
    currentClientLoadRate =
      (x - totalClientReqTillLastSample);

    if(peakClientLoadRate < currentClientLoadRate)
    {
      peakClientLoadRate = currentClientLoadRate ;
    }

    totalClientReqTillLastSample = x;

    printf("CurrentLoadRate(%" PRIu64 ") peakLoadRate(%" PRIu64 ")\n", currentClientLoadRate, peakClientLoadRate);   

}
    
void KPITimer::run()
{
    Poco::Util::Timer::run();
}

class MyTimerThread: public Poco::Runnable
{
public:
    MyTimerThread() 
    {
    }
    void run()
    {
      const long minuteInterval = 1000L; // 1 second
      MinuteKPITask minuteTask;
      KPITimer kpiTimer; 
      kpiTimer.scheduleAtFixedRate(&minuteTask, 100, minuteInterval);
      for (int x=0; x<2; x++)
      {
        sleep(100);
      }
    }
};

int main(int argc, char** argv)
{
    Poco::ThreadPool*                   pocoThreadPool_;
    int minCapacity = 2;
    int maxCapacity = 220;
    int idleTimeSecs = 120;
    try
    {
      pocoThreadPool_ = new Poco::ThreadPool("MsdThreadPool", minCapacity, maxCapacity, idleTimeSecs);
      static int y =0;
#if 0
      MyTimerThread* myTimer = new MyTimerThread();
      pocoThreadPool_->start(*myTimer);
#endif
      for(int x = 0; x < 1; x++)
      {
        MsdMSSThread* myRunnable1 = new MsdMSSThread();
        pocoThreadPool_->start(*myRunnable1);
        MsdHlsThread* myRunnable2 = new MsdHlsThread();
        pocoThreadPool_->start(*myRunnable2);
        MsdDashThread* myRunnable3 = new MsdDashThread();
        pocoThreadPool_->start(*myRunnable3);

        //usleep(200000);
      }
      pocoThreadPool_->joinAll();
    }
    catch (Exception& ex)
    {
      std::cout <<"ERROR(%s):----"<< ex.displayText() << std::endl;
      return 1;
    }
    printf("Stats: peakLoadRate(%" PRIu64 ") requestSent(%"  PRIu64 ") 200_response (%" PRIu64 ") 400_response (%" PRIu64 ") 500_response(%" PRIu64 ") 503_response (%" PRIu64 ") errors(%"  PRIu64 ")-- noLMT(%" PRIu64 ")\n",
           peakClientLoadRate, requestSent, mo_200_response, mo_400_response, mo_500_response, mo_503_response, errors, noLMTHeaders);
    delete pocoThreadPool_;
    return 0;
}


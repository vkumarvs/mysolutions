/**
 * @file   MsdMp4Plugin.cpp
 */
/*
 * Copyright VidScale, 2014-2015
 * All rights reserved.
 *
 * History:
 * Date       Author    Description
 * ------------------------------------------------------------------------
 * 15-Dec-14  vkumar Initial version
 */



#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <streambuf>

#include <ctype.h>

#include <Poco/Util/XMLConfiguration.h>
#include "Poco/URI.h"
#include "MsdMp4Plugin.h"
#include "headers/HttpHeaderUtils.h"
#include "LocationConfigPlugin.h"

using namespace std;
using Poco::URI;
namespace
{
    InternalOriginService* MsdInternalFactory(InternalOriginFD* fd) 
    {
      return new MsdMp4InternalService(fd);
    }

    void log_error_callback(void* context, fmp4_log_level_t level, char const* str)
    {
      switch(level)
      {
        case FMP4_LOG_DEBUG:
        break;
        case FMP4_LOG_WARNING:
        case FMP4_LOG_ERROR:
        default:
        break;
      }

      LOGINFO(INFO(1), "MSD-fmp4 error: \"%s\"", str); 
    }

bool m_isManifestStatic = false;
bool m_isPassThrTrue = false;

void manipulateMpd(string &token)
{   
    string strInitReplace = "initialization=\"oceans-$RepresentationID$.dash\"";
    string strMediaReplace = "media=\"oceans-$RepresentationID$-$Time$.dash\"";
    string strinit = "initialization=\"oceans-$RepresentationID$.dash?" + token + "\"";                   
    string media = "media=\"oceans-$RepresentationID$-$Time$.dash?" + token + "\"";                       
    std::ifstream infile("/opt/universalcache/upload/msd/video/oceans/vipin.mpd");                                 
    std::ofstream out("/opt/universalcache/upload/msd/video/oceans/newfile.mpd", std::ofstream::binary);           
    for (std::string line; std::getline(infile, line, '\n'); )                                            
    {   
      size_t found = line.find(strInitReplace);
      if(found!=std::string::npos)                                                                        
      {
        line.replace(found, strinit.length(), strinit);                                                   
      }

      found = line.find(strMediaReplace);                                                                 
      if(found!=std::string::npos)                                                                        
      {                                                                                                   
        line.replace(found, media.length(), media);                                                       
      } 
      out << line <<endl;
    }   
    out.close();                                                                                        
    infile.close();

}       

}

//static context initialization
msdmp4internalservice::mycontext msdmp4internalservice::m_mycontext;

msdmp4internalservice::msdmp4internalservice(internaloriginfd* fd):internaloriginservice(fd)
{
    m_filename = "";
    m_query = "";
}

void msdmp4internalservice::handlerequestheaders(const httpheaders& requestheaders)
{
  string localpath = requestheaders.getfirstlinepath();

  uri uri(localpath);
  m_filename = uri.getpath();
  m_query = uri.getquery();

#if 0 //only for testing
  bool isrequestformsdlib = false;
  size_t found = m_filename.find(".ism");
  if(found!=std::string::npos)
  {
    //request to be served from msd lib
    isrequestformsdlib = true;
  }
#else
  bool isrequestformsdlib = true;
#endif
  loginfo(info(3), "msd-original> %s --- filename is \n", requestheaders.tostring().c_str(), m_filename.c_str());
  if(requestheaders.getfirstlinemethod().compare("post") == 0)
  {
    loginfo(info(3), "msd-request is post so not creating new thread \n");
    handlestreamingpostreq();
  }
  else
  {
    myrunnable* myrunnable = new myrunnable(this, isrequestformsdlib);
    poco::thread *thread = new poco::thread("msdinternalresponsethrd"); 
    thread->start(*myrunnable); 
  }
}

void msdmp4internalservice::handlerequestbodypart(const iobuffer& bodypart, bool islast) 
{
    loginfo(info(1),"msd-handlerequestbodypart -- length: %zu \n", bodypart.length());
    if(m_ppis == 0)
    {
      loginfo(info(1), "msd-pubpoint_input_stream is null\n");
      builderrorresp();
      return;
    }

    char result_text[256];
    ssize_t length = bodypart.length();
    byte* bytes = new byte[length];                                                                       
    bodypart.copyout(bytes, length);
    int result = pubpoint_push_input_stream(m_ppis,
                                            bytes, (bytes + length),
                                            result_text, sizeof(result_text) / sizeof(result_text[0]));

    if(result != 200)
    {
      loginfo(info(1),"pubpoint_push_input_stream returned: %i", result);
      if(result_text[0])

      {
        loginfo(info(1),"pubpoint_push_input_stream message: %s", result_text);
      }
    }

    if (islast)
    {
      loginfo(info(1),"msd-handlerequestbodypart -- last chunk  \n");
      pubpoint_input_stream_exit(m_ppis); //exit stream only once it is finished writing
    }

    delete bytes;
}

void msdmp4internalservice::getmediatype(string &mediatype, string &path)
{
    if((path.find(".html") !=std::string::npos))
    {
      mediatype = "text/html"; 
    }
    else if(path.find(".js") !=std::string::npos)
    {
      mediatype = "text/javascript"; 
    }
    else if(path.find(".swf") !=std::string::npos)
    {
      mediatype = "application/x-shockwave-flash"; 
    }
    if(path.find(".jpg") !=std::string::npos)
    {
      mediatype = "image/gif"; 
    }
    if(path.find(".png") !=std::string::npos)
    {
      mediatype = "image/png"; 
    }
    if(path.find(".css") !=std::string::npos)
    {
      mediatype = "text/css"; 
    }
}

//this function serves related files, like swf objects, java scripts to run player etc if any.
//this will not be used in case of deployment. it is just a quick way to test
//msd plugin while removing dependency to other plugins to serve any content

void msdmp4internalservice::handlenonstreamingreq()
{
    string mediatype = "";
    getmediatype(mediatype, m_filename);
    std::ifstream t(m_filename.c_str());
    if(!t.good())
    {
      loginfo(info(1), "msd-error:file is not in good condition\n");
      builderrorresp();
      return;
    }
    std::string str;

    t.seekg(0, std::ios::end);
    size_t length = t.tellg();
    str.reserve(length);
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());

    httpheaders headers;
    headers.addfirstline("http/1.1 200 ok");
    headers.appendheader(httpheadernames::content_type, mediatype);
    httpheaderutils::setcontentlengthinheaders(&headers, length);
    headers.appendheader(httpheadernames::connection, "close");
    addresponseheaders(headers);

    //apend response body
    loginfo(info(3), "msd-appending content (file - %s---- mediatype %s) body length is %zu\n",
            m_filename.c_str(), mediatype.c_str(), str.length());

    iobuffer body;
    body.appendstring(str);
    addresponsebodypart(body);
}

void msdmp4internalservice::builderrorresp()
{

    string result = "<html><head><title>404 not found</title><body><h1>404 not found</h1>";
    httpheaders headers;
    headers.addfirstline("http/1.1 400 not found");
    headers.appendheader(httpheadernames::content_type, "text/html");
    httpheaderutils::setcontentlengthinheaders(&headers, result.length());
    headers.appendheader(httpheadernames::connection, "close");
    addresponseheaders(headers);
    iobuffer body;
    byte* bytes = (byte*)(result.c_str());
    body.appendbytes(bytes, result.length());
    addresponsebodypart(body);
}

void msdmp4internalservice::handlestreamingreq()
{
    if(!m_mycontext.m_gctx)
    {
      loginfo(info(1), "msd-global context pointer is null\n");
      return ;
    }

    mp4_process_context_t context;
    mp4_process_context_init(&context, m_mycontext.m_gctx);

    context.get_server_variable_callback_ = null;
    //calling service context 
    context.get_server_variable_context_ = &m_mycontext; 

    //calling service log function for library 
    context.log_error_callback_ = log_error_callback;

    context.log_error_context_ = &m_mycontext;

    //false == handling get requests now 
    context.is_post_ = false; 

    // prefer static files over dynamically generated files (e.g. for client manifest files).
    context.prefer_static_ = m_ismanifeststatic;
    context.iss_pass_through_ = m_ispassthrtrue;

    context.filename_ = m_filename.c_str();

    mp4_split_options_t* options = context.options_;
    if(!mp4_split_options_set(options, m_query.c_str(), m_query.length()))
    {
      loginfo(info(1), "msd-error in processing options\n");
      mp4_process_context_exit(&context);
      return ;
    }
    handlestreaminggetreq(context);
    //exit process context
    mp4_process_context_exit(&context);
}


void MsdMp4InternalService::handleStreamingPostReq()
{
    LOGINFO(INFO(3), "MSD-Inside HandleStreaming Post request \n");
    if(!m_myContext.m_gctx)
    {
      LOGINFO(INFO(1), "MSD-global context pointer is null\n");
      return ;
    }

    mp4_process_context_t context;
    mp4_process_context_init(&context, m_myContext.m_gctx);

    context.get_server_variable_callback_ = NULL;
    //Calling service context 
    context.get_server_variable_context_ = &m_myContext; 

    //Calling service log function for library 
    context.log_error_callback_ = log_error_callback;

    context.log_error_context_ = &m_myContext;

    //true == handling POST request 
    context.is_post_ = true; 

    // Prefer static files over dynamically generated files (e.g. for client manifest files).
    context.prefer_static_ = m_isManifestStatic;
    context.iss_pass_through_ = m_isPassThrTrue;

    context.filename_ = m_filename.c_str();

    mp4_split_options_t * options = context.options_;
    if(!mp4_split_options_set(options, m_query.c_str(), m_query.length()))
    {
      LOGINFO(INFO(1), "MSD-error in processing options\n");
      mp4_process_context_exit(&context);
      return ;
    }

    if(!mp4_ends_with(context.filename_, ".isml"))
    {
      LOGINFO(INFO(1),"POST: file is not .isml \n");
      buildErrorResp();
      return ;
    }

    char const* file = mp4_split_options_get_file(options);

    //stats functions
    // Connect
    if(!file[0])
    {
      return;
    }

    if(!strcmp(file, "purge"))
    {
      int result = mp4_process(&context);

      if(result != 200)
      {
        LOGINFO(INFO(1),"mp4_process  (%s) ---  (%s) returned: %s %s\n",
                context.filename_,
                file,
                fmp4_result_to_string(context.result_),
                context.result_text_);

        return ;
      }

      //TODO::need to send headers only here
      HttpHeaders headers;
      headers.addFirstLine("HTTP/1.1 200 OK");
      HttpHeaderUtils::setContentLengthInHeaders(&headers, 0);
      addResponseHeaders(headers);
      //  r->headers_out.content_length_n = 0;
      // r->headers_out.status = result;
      // r->header_only = 1;
      // rc = ngx_http_send_header(r);
      return ;
    }

    LOGINFO(INFO(1),"MSD-streaming_handler_post: manifest=%s \n", (char const*)context.filename_);
    char result_text[256];
    m_ppis =
      pubpoint_input_stream_init((char const*)context.filename_, options,
                                 result_text, sizeof(result_text) / sizeof(result_text[0]));

    if(m_ppis == 0)
    {
      LOGINFO(INFO(1), "MSD-pubpoint_input_stream_init returned: %s\n", result_text);
      buildErrorResp();
      return;
    }

    LOGINFO(INFO(1),"MSD-streaming_handler_post: pubpoint-input-stream-init \n");
}

void MsdMp4InternalService::handleStreamingGetReq(mp4_process_context_t &context)
{
    bool error = true;
    mp4_split_options_t const* options = context.options_;
    char const* file = mp4_split_options_get_file(options);
    buckets_t* buckets = context.buckets_;

    int http_status;
    size_t tot_size1 = 0;

    //Main function of Mp4 Library
    http_status = mp4_process(&context);
    if(http_status != 200)
    {
      LOGINFO(INFO(1), "MSD-mp4_process  (%s) ---  (%s) returned: %s %s\n",
              context.filename_,
              file,
              fmp4_result_to_string(context.result_),
              context.result_text_);
    }
    else
    {
      tot_size1 = buckets_size(buckets, NULL);
      LOGINFO(INFO(3), "MSD-mp4_process(%s) -- (%s) returned: %s %s - content type %s, total bucket data length %Zu \n\n",
              context.filename_,
              file,
              fmp4_result_to_string(context.result_),
              context.result_text_, (unsigned char *)buckets->content_type_,
              tot_size1);
      error = false;
    }

    if(error)
    {
      LOGINFO(INFO(1), "MSD-Sending Error Header \n ");
      buildErrorResp();
      return;
    }

    LOGINFO(INFO(3), "MSD-Content length is %Zu and content is %s \n\n",
            strlen(buckets->content_type_), (unsigned char *)buckets->content_type_);

    if(buckets->cache_control_)
    {
      LOGINFO(INFO(3), "MSD-bucket cache control is not null \n");
    }

    if(buckets->expires_at_)
    {
      LOGINFO(INFO(3), "MSD-bucket expire at is not null  \n");
    }

    if(buckets->link_[0])
    {
      LOGINFO(INFO(3), "MSD-bucket link is not null\n");
    }

    if(buckets->revision_id_)
    {
      LOGINFO(INFO(3), "MSD-bucket revision id is %lu \n", buckets->revision_id_);
    }


    string media = buckets->content_type_;
    //Encode Header
    HttpHeaders headers;
    headers.addFirstLine("HTTP/1.1 200 OK");
    headers.appendHeader(HttpHeaderNames::CONTENT_TYPE, media);
    headers->appendHeader("Access-Control-Allow-Origin", "*");
    //Total length of response returned by Msd library
    HttpHeaderUtils::setContentLengthInHeaders(&headers, tot_size1);
    headers.appendHeader(HttpHeaderNames::CONNECTION, "close");

    LOGINFO(INFO(3), "MSD-Streaming media(filename) %s -- content length %Zu \n ", m_filename.c_str(), tot_size1);
    LOGINFO(INFO(3), "MSD-Streaming media(Header) %s --\n", headers.toString().c_str());
    //Add response headers
    addResponseHeaders(headers);

    string mpd(file);
    bool isMpd = false;
    size_t found = mpd.find(".mpd");
    if(found!=std::string::npos)
    {
      LOGINFO(INFO(1), "MSD-Request for an MPD %s --\n", mpd.c_str());
      isMpd = true;
    }

    //Add response body
    int bucket_count = 0;
    bucket_t* head = buckets->bucket_;
    bucket_t* bucket = head->next_;
    size_t total_size = 0;
    for(; bucket != head; bucket = bucket->next_)
    {
      size_t size = 0;
      // uint8_t const* src = 0;
      const byte *data = 0;
      bucket->type_->read(bucket, &data, &size);
      IOBuffer body;
      if(isMpd)
      {
        char cmd[256];
        int x = snprintf(cmd, sizeof(cmd), "/opt/universalcache/upload/msd/video/oceans/oceans.mpd");
        std::ofstream outfile (cmd, std::ofstream::binary);
        outfile.write ((const char *)src,size);

        //manipulateMpd(token);
        //std::ifstream infile ("/opt/universalcache/upload/msd/video/oceans/newfile.mpd",std::ifstream::binary);
        std::ifstream infile ("/opt/universalcache/upload/msd/video/oceans/vipin.mpd",std::ifstream::binary);
        infile.seekg (0,infile.end);
        long size1 = infile.tellg();
        infile.seekg (0);
        char* buffer = new char[size1];

        byte* bytes = const_cast<byte*>(buffer);
        //Add Response body 
        body.appendBytes(bytes, size);
        LOGINFO(INFO(3), "MSD-Invoking body part for query %s - length %Zu", m_query.c_str(), size);
        addResponseBodyPart(body);
        LOGINFO(INFO(1), "MSD-Will write mpd file filecmd %s -- size %Zu ---  output size %Zu\n", cmd, size, size1);
        delete[] buffer;
        infile.close();
        outfile.close();
      }
      else
      {
        byte* bytes = const_cast<byte*>(data);
        //Add Response body 
        body.appendBytes(bytes, size);
        LOGINFO(INFO(3), "MSD-Invoking body part for query %s - length %Zu", m_query.c_str(), size);
        addResponseBodyPart(body);
      }


      total_size+=size;
      bucket_count++;
    }

    LOGINFO(INFO(3),"bucket count is %u -- total size is %Zu---%Zu \n", bucket_count,
            total_size, tot_size1 );
}

//Plugin Functions---

MsdMp4Plugin::MsdMp4Plugin(Poco::XML::Element *e) 
  : MediawarpPlugin()
{
    m_serviceTitle = "internal://msdmp4internalservice";
    //Default directory 
    m_rootDir = "/opt/mediawarp/upload/";
    init(e);
    LOGINFO(INFO(1), "MSD-ROOT DIRECTORY FOR STREAMING IS %s \n", m_rootDir.c_str());
}

void MsdMp4Plugin::init(Poco::XML::Element *initElement)
{
    if (!initElement)
    {
      LOGINFO(INFO(1), "MSD-No initialization found\n");
      return;
    }

    // go through the children tags of initialize (initElement) and create match for each one.
    for (Poco::XML::Node *p = initElement->firstChild(); p; p = p->nextSibling())
    {

      if (p->nodeType() != Poco::XML::Node::ELEMENT_NODE) {
        continue;
      }

      Poco::XML::Element *pe = dynamic_cast<Poco::XML::Element *>(p);
      if (!pe)
        continue;

      if (pe->nodeName() == "content")
      {

        Poco::XML::XMLString rootDir = pe->getAttribute("dir");
        if (rootDir == "") 
        {
          LOGINFO(INFO(1), "MSD-Missing source attribute in match tag-Will Use Default one %s\n", m_rootDir.c_str());
        }
        else
        {
          m_rootDir = rootDir;
        }
        continue;
      }

      if (pe->nodeName() == "manifest")
      {
        Poco::XML::XMLString type = pe->getAttribute("type");
        if (type == "dynamic") 
        {
          LOGINFO(INFO(1), "MSD-manifest type is dynamic\n");
          m_isManifestStatic = false;
        }
        else
        {
          m_isManifestStatic = true;
          LOGINFO(INFO(1), "MSD-manifest type is static \n");
        }
        continue;
      }

      if (pe->nodeName() == "passthrough")
      {
        Poco::XML::XMLString type = pe->getAttribute("feature");
        if (type == "true") 
        {
          LOGINFO(INFO(1), "MSD-passthrough is enable \n");
          m_isPassThrTrue = true;
        }
        else
        {
          m_isPassThrTrue = false;
          LOGINFO(INFO(1), "MSD-passthrough is disable \n");
        }
        continue;
      }

      if (pe->nodeName() == "license")
      {
        Poco::XML::XMLString key = pe->getAttribute("key");
        m_license = key.c_str();
        continue;
      }
    }
}

#define MW_MOD_STREAMING_VERSION     "version=1.6.6"
bool MsdMp4Plugin::CheckForMsdLicense()
{
    char const* policy_check_result;
    char const* src = "mwarp_streaming";
    char const* version = MW_MOD_STREAMING_VERSION;

    m_gctx = libfmp4_global_init();
    policy_check_result = libfmp4_policy_check(m_gctx, src, version, m_license);
    if(policy_check_result != 0)
    {
      LOGINFO(INFO(1), "MSD-licence key result %s \n", policy_check_result);
    }
    else
    {
      LOGINFO(INFO(1), "MSD-licence key is valid -- result %s \n", policy_check_result);
    }

    return true;
}

extern "C"
{
    DLLEXPORT 
      MsdMp4Plugin* PluginFactoryXML(Poco::XML::Element * element) 
      {
        MsdMp4Plugin* plugin = new MsdMp4Plugin(element);
        plugin->setStatus(PluginStatus::LOADED, PluginError::NOERROR, NULL);
        return plugin;
      }
}

const char * MsdMp4Plugin::MyIdentity() 
{
  return "MsdMp4Plugin";
}


PluginError::Enum MsdMp4Plugin::PostLoadInit()
{
    string msdprogressive("msdprogressive");
    string msdVodOrLive("msdvodorlive");

    m_locConf = (LocationConfigPlugin *) MediawarpPlugin::FindPlugin("msd-location-config");
    if (!m_locConf) {
      setStatus(PluginStatus::LOADERROR, PluginError::LOADERROR, "Cannot find other plugins");
      return PluginError::PARSEERROR;
    }
    else
    {
      m_msdProgReqIdx = m_locConf->getVariableIndex(msdprogressive);
      m_msdVodLiveIdx = m_locConf->getVariableIndex(msdVodOrLive);
      LOGINFO(INFO(1), "MSD-Location Config index is %u --- %u\n", m_msdProgReqIdx , m_msdVodLiveIdx);
    }


    if(!CheckForMsdLicense())
    {
      LOGINFO(INFO(1), "MSD-License key is not valid\n");
      setStatus(PluginStatus::LOADERROR, PluginError::LOADERROR, NULL);
      return PluginError::LOADERROR;
    }

    InternalOriginServices::Factory* factory = MsdInternalFactory;
    bool ok = TheInternalOriginServices->addService("msdmp4internalservice", factory);

    if (ok)
    {
      MsdMp4InternalService::resetMsdGlblContext(m_gctx, m_serviceTitle.length());
      return PluginError::NOERROR;
    }
    else
    {
      setStatus(PluginStatus::LOADERROR, PluginError::LOADERROR, NULL);
      return PluginError::LOADERROR;
    }
}

bool MsdMp4Plugin::MsdURLHandler(HttpInfo* info, string &newURL)
{
    string orgURL = m_rootDir + info->getHeaders()->getFirstLinePath();
    URI uri(orgURL);

    LOGINFO(INFO(1), "MSD-MsdURLHandle getTheURL %s\n", uri.toString().c_str());

    char new_path[512];
    char new_args[512];
    unsigned int rewrite_ism = 1;
    unsigned int rewrite_f4f = 1;

    char const* path_first = (char const*)uri.getPath().c_str();
    char const* path_last = path_first + uri.getPath().length();
    char const* query_first = (char const*)uri.getQuery().c_str();
    char const* query_last = query_first + uri.getQuery().length();

    LOGINFO(INFO(3), "MSD-MsdURLHandle PATH [%s] ---- QUERY [%s]\n",
            uri.getPath().c_str(), uri.getQuery().c_str());

    if(mp4_rewrite_url(path_first, path_last, query_first, query_last,
                       new_path, sizeof(new_path), new_args, sizeof(new_args),
                       rewrite_ism, rewrite_f4f))
    {
      string path(new_path);
      string args(new_args);
      newURL = m_serviceTitle + (path+"?"+args);
      LOGINFO(INFO(3), "MSD-MsdURLHandle INSIDE- URL [%s] \n", newURL.c_str());
      return true;
    }
    else
    {
      newURL = m_serviceTitle + (uri.getPath()+"?"+uri.getQuery());
      return false;
    }
}

RequestChoice::Enum MsdMp4Plugin::ProcessRequest(HttpInfo* info) 
{
    LOGINFO(INFO(3), "MSD-ProcessRequest getTheURL %s\n", info->getHeaders()->getFirstLinePath().c_str());
    if (!info || info->clientAddress == NULL) 
    {
      LOGINFO(INFO(3), "MSD-ERROR: info or clientAddress is NULL %s\n");
      return RequestChoice::PASS_CONTROL;
    }

    //Use the location-config match rules here if present.
    const string *progData = NULL;
    const string *vodLiveData = NULL;
    bool isReqForMsd = false;
    m_locConf->getVariable(info, m_msdProgReqIdx, progData);
    m_locConf->getVariable(info, m_msdVodLiveIdx, vodLiveData);
    if (progData && !progData->empty())
    {
      LOGINFO(INFO(3),"MSD-msdprogressive= %s\n", progData->c_str());
      isReqForMsd  = true;
    }
    else
    {
      LOGINFO(INFO(1),"MSD-msdprogressive not found\n");
    }

    if (vodLiveData && !vodLiveData->empty())
    {
      LOGINFO(INFO(3),"MSD-msdvodorlive= %s\n", vodLiveData->c_str());
      isReqForMsd  = true;
    }
    else
    {
      LOGINFO(INFO(1),"MSD-msdvodorlive not found\n");
    }

    if(!isReqForMsd)
    {
      LOGINFO(INFO(3), "MSD-REQUEST NOT FOR MSD Plugin\n");
      return RequestChoice::PASS_CONTROL;
    }


    if (((info->getHeaders()->getFirstLineMethod().compare("GET")) == 0)
        || ((info->getHeaders()->getFirstLineMethod().compare("POST")) == 0))
    {
      string newURL;
      if(!MsdURLHandler(info, newURL))
      {
        LOGINFO(INFO(1), "MSD- Request is for static content  URL for Internal Service is [%s] \n",
                newURL.c_str());
        return RequestChoice::PASS_CONTROL;
      }

      LOGINFO(INFO(1), "MSD- Method [%s] URL for Internal Service is [%s]  \n",
              info->getHeaders()->getFirstLineMethod().c_str(), newURL.c_str());
      info->setTitle(newURL.c_str());
      info->setRewrittenURL(newURL.c_str());
    }
    else
    {
      LOGINFO(INFO(1), "MSD-2:Invalid METHOD %s \n", info->getHeaders()->getFirstLineMethod().c_str());
      return RequestChoice::PASS_CONTROL;
    }

    return RequestChoice::RESTART_REQUEST;
}



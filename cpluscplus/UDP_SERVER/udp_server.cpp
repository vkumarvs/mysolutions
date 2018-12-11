#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
#if 0
mp4_global_context_t* gctx;

char const *license="ZWNhZHdlbGxAdmlkc2NhbGUuY29tfDIwMTQwOTI2IDAwOjAwOjAwLDM3MHxwYWNrYWdlKGlzcyxtcDQsaGRzLGhscyxkYXNoKTtzdHJlYW0odm9kLGxpdmUpO2RybShhZXMscGxheXJlYWR5KTtpb19vcHQoKTtjYXB0dXJlKCk7Y2hlY2soZXZhbHVhdGlvbik7c3VwcG9ydCgwKXx2ZXJzaW9uKDE2NjApfGQ1NTExZGU4NjkzZjQzMTA4YjQxYmYxNDlmYmYzMTVlfDFhYjYyOTNiOGRhY2VhOGIyMGVkZWJmNGM0MzdkZjUyNjQzMjVkZjFlMWZkMGFiMTc1OTgxMGZmYTNkNjI5Zjc4Y2QzZTA4MGUzNDMyYzczMWQyYjhmMWZmMWY1MGEwYzU0ZGQ5YTJmOGFlZDIwMmQxMzAyZGIwY2ZmYTFlYTIxMDgyMDY5NWQyZDI4MDhiZGJlMTM5Njc5MTAwYWMzMzMwNDczMWE4OGYwYjFkN2M2YjY1ODQzZTc4NjA3MzIzZmM0MDIyYzFiMDFlODkxMjlhMzNiNjYwNTI5OTI0MDI5YmQ5ZDdkYzU0MTQ5ZDkyYmZmYmJkYmFmMjE1MDc2NWY=";

//check for license
#define X_MOD_SMOOTH_STREAMING_VERSION     "version=1.6.6"
#define X_MOD_H264_STREAMING_VERSION       "version=2.6.6"

struct LocalGlblCtxt
{
  string filename;
  string args;
  LocalGlblCtxt(string & _filename, string & _args)
  {
    filename = _filename; 
    args = _args;
  }
};

//Check for USP license 
bool CheckForLicense()
{
    char const* policy_check_result;
#if 0
    //#if defined(BUILDING_H264_STREAMING)
    char const* src = "nginx mod_h264_streaming";
    char const* version = X_MOD_H264_STREAMING_VERSION;
    //#elif defined(BUILDING_SMOOTH_STREAMING)
#endif
    char const* src = "nginx mod_smooth_streaming";
    char const* version = X_MOD_SMOOTH_STREAMING_VERSION;

    gctx = libfmp4_global_init();
    policy_check_result = libfmp4_policy_check(gctx, src, version, license);
    if(policy_check_result != 0)
    {
      printf("licence key result %s \n", policy_check_result);
    }
    else
    {
      printf("licence key is valid -- result %s \n", policy_check_result);
    }
    return true;
}

void handleStreamingPostReq(HTTPServerRequest& request, HTTPServerResponse&
                            response, mp4_process_context_t &context)
      //                      response)
{
    mp4_process_context_t context;
    if(!gctx)
    {
      printf("global context pointer is null\n");
      return ;
    }
      string filename = "usr/local/nginx/html/live/live.isml";
    mp4_process_context_init(&context, gctx);
    context.get_server_variable_callback_ = NULL;
    context.get_server_variable_context_ = &r; //vipin:pointers inside , basically calling function context
    context.log_error_callback_ = log_error_callback;
    context.log_error_context_ = &r;//Calling function context

    context.is_post_ = true; // for us it will be false .. handling only get
     // Prefer static files over dynamically generated files (e.g. for client
     // manifest files).
    context.prefer_static_ = false;
    context.iss_pass_through_ = true;

  if(!mp4_ends_with(context.filename_, ".isml"))
  {
    printf("POST: file is not .isml \n");
    return ;
  }

  mp4_split_options_t const* options = context.options_;
  char const* file = mp4_split_options_get_file(options);
  //stats functions
  // Connect
  if(!file[0])
  {
    return;
  }

  if(!strcmp(file, "purge"))
  {
    printf("2:I am inside handling of post request filename is %s \n", (char const*)context.filename_);
    int result = mp4_process(&context);

    if(result != 200)
    {
      printf("mp4_process  (%s) ---  (%s) returned: %s %s\n",
             context.filename_,
             file,
             fmp4_result_to_string(context.result_),
             context.result_text_);

      return ;
    }

    //need to send headers only here
  //  r->headers_out.content_length_n = 0;
   // r->headers_out.status = result;
   // r->header_only = 1;
   // rc = ngx_http_send_header(r);
    return ;
  }

  printf("ngx_streaming_handler_post: manifest=%s \n", (char const*)context.filename_);

  char result_text[256];

  pubpoint_input_stream_t* ppis =
    pubpoint_input_stream_init((char const*)context.filename_, options,
      result_text, sizeof(result_text) / sizeof(result_text[0]));

  if(ppis == 0)
  {
    printf( "pubpoint_input_stream_init returned: %s",
                  result_text);
    return;
  }

  printf("ngx_streaming_handler_post: pubpoint-input-stream-init \n");
  if(request.getChunkedTransferEncoding())
  {
    printf("request is chunked encoding\n");
  }

  if(request.expectContinue())
  {
    printf("Client expect continue message\n");
  }

  if(request.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH)
  {
    printf("Size of request is Unknown content length \n");
  }

  ostringstream out_string_stream;
  std::istream& is = request.stream();
#if 0
FILE* pFile;
    pFile = fopen("/home/vipin/VIEW/PERSONAL_CODE/CODING/POCO_LEARNING/SIMPLE_HTTP_SERVER/ff.txt", "w+");
    char buffer[1024] = {0x0};
    is.read(buffer, 1024);
    std::streamsize n = is.gcount();

    while (n > 0)
    {  
        if (is)
        {
            fwrite(buffer, sizeof(char), sizeof(buffer), pFile);
            is.read(buffer, 1024);
            n = is.gcount();
        }
        else n = 0;
    }
#endif

#if 0
      FILE* pFile;
      pFile = fopen("/home/vipin/VIEW/PERSONAL_CODE/CODING/POCO_LEARNING/SIMPLE_HTTP_SERVER/ff.txt", "a+");
  while(!is.eof())
  {
    std::string chunked_data = read_chunked(is);
     int length = chunked_data.length();
    if(length > 0)
    {
      printf("chunked length is %d - %s\n", length , chunked_data.c_str());
      const char *buffer = chunked_data.c_str();

      fwrite(buffer, sizeof(char), length, pFile);
      #if 0
      unsigned char buf[length + 1] ;
      memset(buf,0,(length + 1));
      memcpy(buf, chunked_data.c_str(), length);
      printf("buffer  length is %d - %s\n", length, buf);
      char result_text[256];
      int result = pubpoint_push_input_stream(ppis,
                                              (unsigned char*)buf, (unsigned char*)buf + length,
                                              result_text, sizeof(result_text) / sizeof(result_text[0]));

      if(result != 200)
      {
        printf("pubpoint_push_input_stream returned: %i", result);
        if(result_text[0])
        {
          printf("pubpoint_push_input_stream message: %s", result_text);
        }
      }
#endif
    }
  }
#endif

#if 1
  if(is)
  {
    printf("0: Response body is \n");
    while(!is.eof())
    {
      char buffer[30000];
      //is.read (buffer, 30000);
      size_t count = is.readsome (buffer, 30000);

#if 1
      char result_text[256];
      int result = pubpoint_push_input_stream(ppis,
                                              (unsigned char*)buffer, (unsigned char*)buffer+ is.gcount(),
                                              result_text, sizeof(result_text) / sizeof(result_text[0]));

      if(result != 200)
      {
        printf("pubpoint_push_input_stream returned: %i", result);
        if(result_text[0])
        {
          printf("pubpoint_push_input_stream message: %s", result_text);
        }
      }
#endif
    }
    printf("Read is complete \n");
    pubpoint_input_stream_exit(ppis);
  }
#endif

#if 0
  StreamCopier::copyStream( is, out_string_stream );
  printf("1: Response body is \n");
  string response_body = out_string_stream.str();
  printf("Response body is %s\n", response_body.c_str());
  ngx_http_request_body_t* rb = 
    ngx_pcalloc(r->pool, sizeof(ngx_http_request_body_t));
  if (rb == NULL) {
    return NGX_HTTP_INTERNAL_SERVER_ERROR;
  }

#if defined(NGX_HAVE_HANDLE_POST)
  rb->handle_post = ngx_http_fmp4_input_handle_post;
  rb->handle_post_context = ppis;
  r->request_body = rb;
#endif

  r->request_body_in_file_only = 1;

  rc = ngx_http_read_client_request_body(r, ngx_http_fmp4_input_post_read);

  ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, rc,
                "ngx_streaming_handler_post: exit"); 

  if(rc >= NGX_HTTP_SPECIAL_RESPONSE)
  {
    return rc;
  }

  return rc;
#endif
}
#endif

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[5120];

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(32000);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
   static int fileno = 0;
   char cmd[256];
   int x = 0 ;
        x = snprintf(cmd, sizeof(cmd), "/home/vipin/VIEW/PERSONAL_CODE/CODING/UDP_SERVER/output/complete.ismv" );
        std::ofstream outfile (cmd, std::ofstream::binary | std::ofstream::app);
   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,5120,0,(struct sockaddr *)&cliaddr,&len);
      if(n > 0)
      {
        outfile.write ((const char *)mesg, n);
        printf("have written %d bytes to file\n", n);
      }
#if 0
      sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received the following:\n");
      printf("%s",mesg);
      printf("-------------------------------------------------------\n");
#endif
   }
}


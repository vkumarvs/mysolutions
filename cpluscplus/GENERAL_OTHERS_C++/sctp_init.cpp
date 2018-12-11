void WorkerLoadThread::initializeSctp()
{
#if 1
    return;
#endif
    const char *server_ip_address = "169.254.19.2";
    //const char *server_ip_address = "192.168.121.19";
    //short int port_no = 10000;
    short int port_no = m_loadParams.m_serverPort;
    struct sockaddr_in servaddr;
    //struct sctp_status status;
    struct sctp_initmsg initmsg;
    //m_sockFd = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );
    m_sockFd = socket( PF_INET, SOCK_STREAM, IPPROTO_SCTP );

    /* Specify that a maximum of 5 streams will be available per socket */
    memset( &initmsg, 0, sizeof(initmsg) );
    initmsg.sinit_num_ostreams = 16;
    initmsg.sinit_max_instreams = 16;
    initmsg.sinit_max_attempts = 10;
    int ret = setsockopt( m_sockFd, IPPROTO_SCTP, SCTP_INITMSG,
                      &initmsg, sizeof(initmsg) );

    bzero( (void *)&servaddr, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port_no);
    if((inet_pton(AF_INET, server_ip_address, &servaddr.sin_addr))<=0){  
      LogNotice("inet_pton error for %s\n",server_ip_address);  
      LogNotice("errno %d: %s\n",errno,strerror(errno));  
      exit(1);  
    }
    ret = connect( m_sockFd, (struct sockaddr *)&servaddr, sizeof(servaddr) );
}

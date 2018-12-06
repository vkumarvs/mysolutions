#include <string.h>
#include <bits/sockaddr.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/rtnetlink.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "NetlinkMonitor.h"
#include <errno.h>
#include <linux/netlink.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#if 0
int NetlinkMonitor::subscribeToIntfNotifications()
{
   struct nlmsghdr *n;
   uint8_t req[1024];
   /* Issue a wild dump NETLINK request for interface information*/
   
   memset(&req, 0, sizeof(req));

   n = (struct nlmsghdr*) req;
   n->nlmsg_len    = NLMSG_LENGTH(sizeof(struct rtgenmsg));
   n->nlmsg_type   = RTM_GETLINK;
   n->nlmsg_flags  = NLM_F_ROOT|NLM_F_REQUEST;
   n->nlmsg_pid    = getpid();
   n->nlmsg_seq    = m_seqNumber++;
   n->nlmsg_flags |= NLM_F_ACK;

   if (send(m_netLinkIntfFd, n, n->nlmsg_len, 0) < 0) {
      LogError("Error : send(): %s", strerror(errno));
      return -1;
   }
   return 0;
}
#endif

NetlinkMonitor::NetlinkMonitor() : m_nlfd(-1)
{
   if ((m_nlfd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0) {
      printf("Error : socket(AF_NETLINK): %s", strerror(errno));
   }
   // m_nlfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    struct sockaddr_nl nladdr;
    bzero(&nladdr, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    //nladdr.nl_pid = getpid();
    nladdr.nl_pid = 0;
    nladdr.nl_groups = RTMGRP_LINK | RTMGRP_NOTIFY;
   // bind(m_nlfd, (struct sockaddr*) &nladdr, sizeof(nladdr));
   if (bind(m_nlfd, (struct sockaddr *) &nladdr, sizeof(nladdr)) < 0) {
      printf("Error : bind(): %s", strerror(errno));
   }

   const int          on = 1;
   if (setsockopt(m_nlfd, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)) < 0) {
      printf("Error : SO_PASSCRED: %s", strerror(errno));
   }
    FD_ZERO(&m_readset);
    FD_SET(m_nlfd, &m_readset);
}

NetlinkMonitor::~NetlinkMonitor()
{
  if(m_nlfd != -1) { close(m_nlfd); }
}

void NetlinkMonitor::ProcessNLIfMsg(struct nlmsghdr *nlp)
{
    struct ifinfomsg *ifinfomsg_;
    
    ifinfomsg_ = (struct ifinfomsg*) NLMSG_DATA(nlp);
    
    if (nlp->nlmsg_len < NLMSG_LENGTH(sizeof(struct ifinfomsg))) 
    {
        printf("NetlinkMonitor: ProcessNLIfMsg: Pkt too small\n");
        return;
    }

   struct rtattr* rtap = (struct rtattr*) ((char*) ifinfomsg_ + 
                               NLMSG_ALIGN(sizeof(struct ifinfomsg)));
   int rta_len = NLMSG_PAYLOAD(nlp, sizeof(struct ifinfomsg));
   char ifname[IFNAMSIZ+1]={0};

   while(RTA_OK(rtap, rta_len))
   {
        void* rta_data = RTA_DATA(rtap);

        switch(rtap->rta_type)
        {
           case IFLA_IFNAME:
             {
                 if (rta_len > IFNAMSIZ)
                     rta_len = IFNAMSIZ;
                 strncpy(ifname, (char*)rta_data, rta_len);
             }
             break;
           default:
             printf("Unknown IFLA attribute: %d\n", rtap->rta_type);
             break;
        }

        rtap=RTA_NEXT(rtap, rta_len);
    }

    switch(nlp->nlmsg_type)
    {
         // case RTM_NEWLINK:  ifMsg.m_eMsgType = IF_MSG_NEWIF; break;
         // case RTM_DELLINK:  ifMsg.m_eMsgType = IF_MSG_DELIF; break;
    }

    eventInterfaceStatusChange
       (ifname, ifinfomsg_->ifi_index, ifinfomsg_->ifi_flags);
}


void NetlinkMonitor::ProcessNLMsg(char* m_buf, unsigned int msgLen)
{
  struct nlmsghdr *nlp = (struct nlmsghdr *) m_buf;

  if(msgLen <= 0) return;

  printf("Rx: Msg Seq [%d] Len [%d]\n", nlp->nlmsg_seq, msgLen);

  if(nlp->nlmsg_type == NLMSG_DONE)
  {
     printf("Success\n");
     return;
  }else
  if(nlp->nlmsg_type == NLMSG_ERROR)
  {
     struct nlmsgerr *nerr = (struct nlmsgerr *) NLMSG_DATA(nlp);
     if(nerr->error == 0) { printf("Success\n"); return;}
     printf("Error: %d\n", nerr->error);
     return;
  }
  for(;NLMSG_OK(nlp, msgLen);nlp=NLMSG_NEXT(nlp, msgLen))
  {
     int msgType = nlp->nlmsg_type;
     switch(msgType)
     {
         case RTM_NEWLINK:
         case RTM_DELLINK:
              ProcessNLIfMsg(nlp);
              break;
         default:
              printf("Unknown message: %d\n", msgType);
              break;
     }
  }
}

void NetlinkMonitor::Monitor(bool block)
{
    if(block)
    {
      int select_val = select(FD_SETSIZE, &m_readset, 0, 0, 0);
      if(select_val <= 0) return;
    }
    char msgBuf[1024*8];
    int bufLen = sizeof(msgBuf);
    bzero(msgBuf, bufLen);
    int msgLen = recv(m_nlfd, msgBuf, bufLen, 0);
    if(msgLen <= 0) { return; }
    ProcessNLMsg(msgBuf, msgLen);
}


void NetlinkMonitor::eventInterfaceStatusChange
    (const char* ifname, int ifindex, unsigned int ifflags)
{
    printf("NetlinkMonitor::InterfaceStatusChange %s %d %08x\n", 
                 ifname, ifindex, ifflags);
   if(ifflags & IFF_RUNNING)
   {       
      printf("%s up \n", ifname);
   }else
   {
      printf("%s down \n", ifname, ifflags);
   }
}



int main()
{
    NetlinkMonitor netMonitor;
    while(1)
    {
    netMonitor.Monitor(true);
    }
return  0;
}

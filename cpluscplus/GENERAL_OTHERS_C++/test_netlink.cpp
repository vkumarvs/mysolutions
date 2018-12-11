#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include <sys/socket.h>
#include <asm/types.h>
#include <inttypes.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define LogDebug(...) printf(__VA_ARGS__)
#define LogInfo(...) printf(__VA_ARGS__)
#define LogError(...) printf(__VA_ARGS__)

#define TEMP_BUFF_SIZE (255)
#define MAX_LINE_LENGTH (10000)
#define ARPHRD_ETHER (1)

#define MAX_ADDR_LEN (24)

int
prefix_to_netmask(char * buf, int buffLen, uint8_t prefixlen)
{
uint8_t tmp[24];
int len = 24;
const uint8_t *a_start = tmp;
int p;
const uint8_t *a_end;
uint8_t index=0;

for (p = prefixlen; index < MAX_ADDR_LEN && p > 8; p -= 8) {
tmp[index++] = 0xff;
}
if(index >= MAX_ADDR_LEN) {
return 0;
}
tmp[index++] = 0xff << (8 - p);
while (index < MAX_ADDR_LEN) {
tmp[index++] = 0;
}

if(inet_ntop(AF_INET, tmp, buf, 100) == NULL ) {
   LogError("inet_ntop failed with[%d]\n",errno);
   return 1;
}
LogDebug("returning netmask[%s] for prefixlen[%d]\n",buf,prefixlen);
return 0;
}


/*
 * =====================================================================================
 *        Class:  CardRSCIntfHelper 
 *  Description:  class methods implement interface mgmt functionalities 
 * =====================================================================================
 */
class CardRSCIntfHelper {
   public:

      /*
       *--------------------------------------------------------------------------------------
       *       Class:  CardRSCIntfHelper
       *      Method:  CardRSCIntfHelper
       * Description:  constructor 
       *--------------------------------------------------------------------------------------
       */
      CardRSCIntfHelper();

      /*
       *--------------------------------------------------------------------------------------
       *       Class:  CardRSCIntfHelper
       *      Method:  init
       * Description:  intialized interface manager by initiating net-link sock
       *               and requesting for intf and ip addr from system
       *--------------------------------------------------------------------------------------
       */
      int init();

      /*
       *--------------------------------------------------------------------------------------
       *       Class:  CardRSCIntfHelper
       *      Method:  eventOnNetLinkFd
       * Description:  interface API called when event is detected
       *--------------------------------------------------------------------------------------
       */
      void eventOnNetLinkFd();

      /*
       *--------------------------------------------------------------------------------------
       *       Class:  CardRSCIntfHelper
       *      Method:  updateIntf
       * Description:  updates intf stats 
       *--------------------------------------------------------------------------------------
       */
      int subscribeToIpRouteNotifications();

      int subscribeToIntfNotifications();

      int subscribeToIpAddrNotifications();

   private:


      /*
       *--------------------------------------------------------------------------------------
       *       Class:  CardRSCIntfHelper
       *      Method:  handleNetLinkEvent
       * Description:  method parses NetLink event information
       *--------------------------------------------------------------------------------------
       */
      int handleNetLinkEvent(struct nlmsghdr *n);

      /*
       *--------------------------------------------------------------------------------------
       *       Class:  CardRSCIntfHelper
       *      Method:  openNetLinkFd
       * Description:  opens net-link fd, registers with boost based movik event handler
       *               and on success returns fd opened or else failure (-1)
       *--------------------------------------------------------------------------------------
       */
      int openNetLinkFd();

      /*
       *--------------------------------------------------------------------------------------
       *       Class:  CardRSCIntfHelper
       *      Method:  parse_rtattr
       * Description:  used to parse rt netlink attrs
       *--------------------------------------------------------------------------------------
       */
      int parseRTAttr(struct rtattr *tb[], int max, struct rtattr *rta, int len);
    
      /*-----------------------------------------------------------------------------
       *  private member variables
       *-----------------------------------------------------------------------------*/
      uint32_t              m_netLinkIntfFd;
      uint32_t              m_netLinkRouteFd;
      uint32_t              m_netLinkIpAddrFd;
      uint32_t              m_seqNumber;
};

CardRSCIntfHelper::CardRSCIntfHelper():
   m_netLinkIntfFd(0),
   m_netLinkRouteFd(0),
   m_netLinkIpAddrFd(0),
   m_seqNumber(1)
{
}
/*
 *--------------------------------------------------------------------------------------
 *       Class:  CardRSCIntfHelper
 *      Method:  init
 * Description:  initialized netlink sock used boost fd with given thread
 *               and opens net-link-sock for listening to interface and ip
 *               add specific events
 *--------------------------------------------------------------------------------------
 */
int CardRSCIntfHelper::init() {

   // start with reading interface information
   int fd = openNetLinkFd();
   m_netLinkIntfFd = fd;
   subscribeToIntfNotifications();

   //fd = openNetLinkFd();
   //m_netLinkIpAddrFd = fd;
  // subscribeToIpAddrNotifications();


   //fd = openNetLinkFd();
   //m_netLinkRouteFd = fd;
  // subscribeToIpRouteNotifications();
   return 0;
}

int CardRSCIntfHelper::subscribeToIntfNotifications()
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

int CardRSCIntfHelper::subscribeToIpAddrNotifications()
{
   struct nlmsghdr *n;
   uint8_t req[1024];
   /* Issue a wild dump NETLINK request for interface information*/
   
   memset(&req, 0, sizeof(req));

   n = (struct nlmsghdr*) req;
   n->nlmsg_len    = NLMSG_LENGTH(sizeof(struct rtgenmsg));
   n->nlmsg_type   = RTM_GETADDR;
   n->nlmsg_flags  = NLM_F_ROOT|NLM_F_REQUEST;
   n->nlmsg_pid    = getpid();
   n->nlmsg_seq    = m_seqNumber++;
   n->nlmsg_flags |= NLM_F_ACK;

   if (send(m_netLinkIpAddrFd, n, n->nlmsg_len, 0) < 0) {
      LogError("Error : send(): %s", strerror(errno));
      return -1;
   }
   return 0;
}

int CardRSCIntfHelper::subscribeToIpRouteNotifications()
{
   struct nlmsghdr *n;
   uint8_t req[1024];
   /* Issue a wild dump NETLINK request for interface information*/
   
   memset(&req, 0, sizeof(req));

   n = (struct nlmsghdr*) req;
   n->nlmsg_len    = NLMSG_LENGTH(sizeof(struct rtgenmsg));
   n->nlmsg_type   = RTM_GETROUTE;
   n->nlmsg_flags  = NLM_F_ROOT|NLM_F_REQUEST;
   n->nlmsg_pid    = getpid();
   n->nlmsg_seq    = m_seqNumber++;
   n->nlmsg_flags |= NLM_F_ACK;

   if (send(m_netLinkRouteFd, n, n->nlmsg_len, 0) < 0) {
      LogError("Error : send(): %s", strerror(errno));
      return -1;
   }
   return 0;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CardRSCIntfHelper
 *      Method:  openNetLinkFd
 * Description:  this method creates a netlink sock for interface updates and for
 *               ip updates.
 *--------------------------------------------------------------------------------------
 */
int CardRSCIntfHelper::openNetLinkFd()
{
   struct sockaddr_nl addr;
   const int          on = 1;
   int                netLinkFd;
   int                status;

   memset(&addr, 0, sizeof(addr));

   addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;
   addr.nl_family = AF_NETLINK;
   addr.nl_pid    = 0; /*  let kernel generate one id for me */


   if ((netLinkFd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0) {
      LogError("Error : socket(PF_NETLINK): %s", strerror(errno));
      return status=-1;
   }
   

   if (bind(netLinkFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
      LogError("Error : bind(): %s", strerror(errno));
      return status=-1;
   }

   if (setsockopt(netLinkFd, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)) < 0) {
      LogError("Error : SO_PASSCRED: %s", strerror(errno));
      return status=-1;
   }

   /*  make fd non-blocking */
   status = fcntl(netLinkFd, F_SETFL, O_NONBLOCK);
   if (status)
   {
      ::close(netLinkFd);
      return status=-1;
   }


   return netLinkFd;
}

void CardRSCIntfHelper::eventOnNetLinkFd()
{
   char buf[8192];
   struct sockaddr_nl nladdr;
   struct iovec iov = { buf, sizeof(buf) };
   int status;
   struct nlmsghdr *h = NULL; /**prev_h = NULL;*/
   struct msghdr msg = {
      (void*)&nladdr, sizeof(nladdr), &iov, 1, NULL, 0, 0
   };
   memset(&buf, '\0', sizeof(buf));

   status = recvmsg(m_netLinkIntfFd, &msg, 0);
   if(status < 0)
   {

     status = recvmsg(m_netLinkIpAddrFd, &msg, 0);
     if(status < 0)
     {

       status = recvmsg(m_netLinkRouteFd, &msg, 0);
       if(status <= 0)
       {
         return;
       }

     }
   }

   if(msg.msg_namelen != sizeof(nladdr))
   {
      return;
   }

   std::cout << "got an event of fd " << std::endl;
   h = (struct nlmsghdr*)buf;
   while(NLMSG_OK(h, status))
   {
      if(h->nlmsg_type == NLMSG_DONE) {
	 // once we have interface information 
	 // initialiate get request for ip information
	 //openNetLinkFd(GET_IP);
	 break;
      }

      handleNetLinkEvent(h);

      h = NLMSG_NEXT(h, status);
   }


   return;

}

int CardRSCIntfHelper::parseRTAttr(struct rtattr *tb[], int max, struct rtattr *rta, int len)
{
   while(RTA_OK(rta, len))
   {
      if(rta->rta_type <= max)
         tb[rta->rta_type] = rta;
      rta = RTA_NEXT(rta, len);
   }
   if(len)
      fprintf(stderr, "!!!Deficit %d, rta_len=%d\n", len, rta->rta_len);

   return 0;
}

int CardRSCIntfHelper::handleNetLinkEvent(struct nlmsghdr *n)
{
   struct ifinfomsg    *ifi  = (struct ifinfomsg*)NLMSG_DATA(n);
   struct rtattr    *rta_tb[IFLA_MAX + 1];


   if(n->nlmsg_len < NLMSG_LENGTH(sizeof(ifi))) {
      LogError("miss match in nlmsg len\n");
      return 0;
   }


   if((n->nlmsg_type == RTM_NEWLINK) ||
      (n->nlmsg_type == RTM_DELLINK))
   {

      /* Parse the attribute */
      memset(rta_tb, '\0', sizeof(rta_tb));
      parseRTAttr(rta_tb, IFLA_MAX, IFLA_RTA(ifi), IFLA_PAYLOAD(n));

      // handle only if valid ethernet interface
      // skip tun and loopback interfaces
      if(ifi && ifi->ifi_type == ARPHRD_ETHER) {

	 if(rta_tb[IFLA_IFNAME] != NULL) {
	    LogDebug("intf name[%s]\n",(const char*)RTA_DATA(rta_tb[IFLA_IFNAME]));
	 }
	 else {
	    LogError("Invalid intf name [%d]\n",ifi->ifi_index);
	 }

	 struct rtattr *rtAttr;
	 int rtLen;
	 rtAttr = (struct rtattr *)IFLA_RTA(ifi);
	 rtLen = IFLA_PAYLOAD(n);
	 for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen)){
	    void *pData = RTA_DATA(rtAttr);
	    switch(rtAttr->rta_type) {
#ifdef COMMENT
	       case IFLA_MAP:
	       {
		  struct rtnl_link_ifmap *map = (struct rtnl_link_ifmap*) RTA_DATA(rtAttr);
		  std::cout << "irq " << map->irq << std::endl;
		  std::cout << "port [" << map->port <<"]"<< std::endl;
		  std::cout << "base_addr [" << map->base_addr <<"]"<< std::endl;
		  std::cout << "dma [" << map->dma <<"]"<< std::endl;
	       }
	       break;
	       default:
		  LogDebug("\t\tintf-info with type[%d] not handled\n",rtAttr->rta_type);
		  break;
#endif /* COMMENT */
	    }
     
	 }

	 LogDebug("intf index[%d]\n",ifi->ifi_index);
	 LogDebug("\t\t intf status[%s]\n",ifi->ifi_flags &IFF_UP? "admin state up":"admin state down");
	 LogDebug("\t\t intf flags[%d]\n",ifi->ifi_flags);
	 LogDebug("\t\t intf lower_up [%s]\n",ifi->ifi_flags&IFF_LOWER_UP?"link up":"link down");
	 LogDebug("\t\t intf lower_up [%s]\n",ifi->ifi_flags&IFF_RUNNING?"opr state up":"opr state down");

	 /*  ifi->ifi_change not not used by current rtnetlink spec? 
	  *  left for future; i hope to see it :) 
	  */
      }
      else {
	 LogDebug("interface [%s] not handled of type:[%d]\n",(const char*)RTA_DATA(rta_tb[IFLA_IFNAME]),ifi->ifi_type);
      }
   }
   else if((n->nlmsg_type == RTM_NEWADDR) || (n->nlmsg_type == RTM_DELADDR))
   {
      LogInfo("Got %s notification\n",n->nlmsg_type == RTM_NEWADDR ? "RTM_NETADDR":"RTM_DELADDR");
      struct ifaddrmsg *ifa = (struct ifaddrmsg*)NLMSG_DATA(n);
      struct rtattr    *rta_tb[IFA_MAX + 1];
      char   mask[100]={};

      int index=0;
      if(prefix_to_netmask(mask,100,ifa->ifa_prefixlen) != 0 ) {
         return 0;
      }
      LogInfo("mask[%s]\n",mask);

      if(ifa && ifa->ifa_family == AF_INET) {

	 /* Parse the attribute */
	 memset(rta_tb, '\0', sizeof(rta_tb));
	 parseRTAttr(rta_tb, IFA_MAX, IFA_RTA(ifi), IFA_PAYLOAD(n));
	 if(rta_tb[IFA_LABEL] != NULL) {
	    LogInfo("addr with with inf-name:[%s] and intf-index[%d]\n",(const char*)RTA_DATA(rta_tb[IFLA_IFNAME]),ifa->ifa_index);
	 }
	 else {
	    LogError("IP UPDATE WITH NO IFA_LABEL FIELD\n");
	    return 0;
	 }

	 struct rtattr *rtAttr;
	 int rtLen;
	 rtAttr = (struct rtattr *)IFA_RTA(ifa);
	 rtLen = IFA_PAYLOAD(n);
	 for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen)){
	    void *pData = RTA_DATA(rtAttr);
	    switch(rtAttr->rta_type) {
	       case IFA_ADDRESS:
	       case IFA_LOCAL:
	       {
		  uint8_t        *src = (u_char*)pData;
		  uint32_t       addr = 0;

		  addr = (src[0]<<24) + (src[1] << 16) + (src[2] << 8) + src[3];
		  // story:
		  // what we have received is in network byte order.
		  // should we store it as it is? or should be convert to
		  // host byte order our m/c are little indian it 
		  // makes sense to get back host byte order, make life easier
		  //
		  // IFA_LOCAL and IFA_ADDRESS would differ when we provision a tunnel
		  // in case of tunnel IFA_LOCAL would represent local tunnel address
		  // and IFA_ADDRESS would represent remote address of tunnel
		  // do we need it?
		  // i believe no.. so lets use IFA_ADDRESS only
		  addr = ntohl(addr);
		  struct in_addr inAddr;
		  inAddr.s_addr = addr;
		  LogDebug("\t\t%s [%s]\n",
			   rtAttr->rta_type==IFA_ADDRESS?"IFA_ADDRESS":"IFA_LOCAL",inet_ntoa(inAddr));
	       }
	       break;
	       case IFA_LABEL:
		  break;
	       case IFA_BROADCAST:
	       {
		  uint8_t *src = (u_char*)pData;
		  uint32_t addr = 0;
		  addr = (src[0]<<24) + (src[1] << 16) + (src[2] << 8) + src[3];
		  addr = ntohl(addr);
		  struct in_addr inAddr;
		  inAddr.s_addr = addr;
		  LogInfo("\t\tIFA_BROADCAST[%s]\n",inet_ntoa(inAddr));
	       }
	       break;
	       default:
		  LogInfo("\t\trta_type:[%d] not handled\n",rtAttr->rta_type);
		  break;
	    }
	 }

      } /*  end-of AF_INET family */
      else {
	 LogInfo("Ip addr of different family[%d]\n",ifa->ifa_family);
      }

   } /*  end of RTM_NEWADDR and RTM_DELADDR */
   else if ( (n->nlmsg_type == RTM_NEWROUTE || n->nlmsg_type == RTM_DELROUTE))
   {
      struct rtmsg *rtm;
      rtm = (struct rtmsg*)NLMSG_DATA (n);
      int len;
      char anyaddr[16] = {0};
      int index;
      int table;
      void *dest;
      void *gate;


      if ( rtm->rtm_family == AF_INET ) {


      if(rtm->rtm_table != RT_TABLE_MAIN)
         return 0;

      /* Connected route. */
      LogInfo("[route] %s %s protocol[%d]\n",
	      n->nlmsg_type == RTM_NEWROUTE ? "RTM_NEWROUTE" : "RTM_DELROUTE",
	      rtm->rtm_type == RTN_UNICAST ? "unicast" : "multicast",
              rtm->rtm_protocol
	      );

	 struct rtattr *rtAttr;
	 int rtLen;
	 rtAttr = (struct rtattr *)RTM_RTA(rtm);
	 rtLen = RTM_PAYLOAD(n);
	 for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen)){
	    void *pData = RTA_DATA(rtAttr);
	    switch(rtAttr->rta_type) {
               case RTA_PREFSRC:
                 {
                 struct in_addr addr;
                 addr.s_addr = *((unsigned long*)pData);
                 LogDebug("PREFSRC:[%s]\n",inet_ntoa(addr));
                 }
                 break;
               case RTA_GATEWAY:
                 {
                 struct in_addr addr;
                 addr.s_addr = *((unsigned long*)pData);
                 LogDebug("GATEWAY:[%s]\n",inet_ntoa(addr));
                 }
                 break;
               case RTA_DST:
                 {
                 struct in_addr addr;
                 addr.s_addr = *((unsigned long*)pData);
                 LogDebug("DST:[%s]\n",inet_ntoa(addr));
                 }
                 break;
               case RTA_TABLE:
                 {
                   LogDebug("Route Index:[%d]\n",*(uint32_t*)pData);
                 }
                 break;
               case RTA_SRC:
                 {
                   LogDebug("RTA_SRC not used\n");
                 }
                 break;
               case RTA_PRIORITY:
                 {
                   LogDebug("RTA_PRIORITY not used\n");
                 }
                 break;
               case RTA_IIF:
                 {
                   LogDebug("RTA_IIF not used\n");
                 }
                 break;
               case RTA_METRICS:
                 {
                   LogDebug("RTA_METRICS not used\n");
                 }
                 break;
               case RTA_MULTIPATH:
                 {
                   LogDebug("RTA_MULTIPATH not used\n");
                 }
                 break;
               case RTA_FLOW:
                 {
                   LogDebug("RTA_FLOW not used\n");
                 }
                 break;
               case RTA_SESSION:
                 {
                   LogDebug("RTA_SESSION not used\n");
                 }
                 break;
               case RTA_CACHEINFO:
                 {
                   LogDebug("RTA_CACHEINFO not used\n");
                 }
                 break;
               case RTA_OIF:
                  printf("intf-index:[%d]\n",*(int*)pData);
                 break;
	       default:
		  LogInfo("\t\trta_type:[%d] not handled\n",rtAttr->rta_type);
		  break;
	    }
	 }
      }
#ifdef COMMENT
      if (rtm->rtm_type != RTN_UNICAST)
      {
	 return 0;
      }

      table = rtm->rtm_table;
      if (table != RT_TABLE_MAIN )
      {
	 return 0;
      }


      memset (rta_tb, 0, sizeof rta_tb);
      parseRTAttr(rta_tb,RTA_MAX,RTM_RTA(rtm),len);

      if (rtm->rtm_flags & RTM_F_CLONED)
	 return 0;
      if (rtm->rtm_protocol == RTPROT_REDIRECT)
	 return 0;
      if (rtm->rtm_protocol == RTPROT_KERNEL)
	 return 0;

      if (rtm->rtm_protocol == RTPROT_ZEBRA && n->nlmsg_type == RTM_NEWROUTE)
	 return 0;

      if (rtm->rtm_src_len != 0)
      {
	 LogInfo("netlink_route_change(): no src len");
	 return 0;
      }
  
      index = 0;
      dest = NULL;
      gate = NULL;

      if (rta_tb[RTA_OIF])
	 index = *(int *) RTA_DATA (rta_tb[RTA_OIF]);

      if (rta_tb[RTA_DST])
	 dest = RTA_DATA (rta_tb[RTA_DST]);
      else
	 dest = anyaddr;

      if (rta_tb[RTA_GATEWAY])
	 gate = RTA_DATA (rta_tb[RTA_GATEWAY]);

      LogInfo("[route][gateway][%s]\n",gate);
      LogInfo("[route][dest][%s]\n",dest);
      LogInfo("[route][index][%d]\n",index);

      if (rtm->rtm_family == AF_INET)
      {
	 struct prefix_ipv4 p;
	 p.family = AF_INET;
	 memcpy (&p.prefix, dest, 4);
	 p.prefixlen = rtm->rtm_dst_len;

	 if (n->nlmsg_type == RTM_NEWROUTE)
	    LogInfo("RTM_NEWROUTE %s/%d",inet_ntoa (p.prefix), p.prefixlen);
	 else
	    LogInfo("RTM_DELROUTE %s/%d", inet_ntoa (p.prefix), p.prefixlen);

	 if (n->nlmsg_type == RTM_NEWROUTE)
            LogInfo("[route][new][%s:index:table]\n",gate,index,table);
	 else
            LogInfo("[route][new][%s:index:table]\n",gate,index,table);
      }
#endif /* COMMENT */

   }

   return 0;
}

int main(int argc, char **argv)
{
   CardRSCIntfHelper intfHelper;
   intfHelper.init();
   while(1) {
      intfHelper.eventOnNetLinkFd();
   }
}

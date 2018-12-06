#ifndef _NETLINK_MONITOR_H_
#define _NETLINK_MONITOR_H_

/* This class helps in monitoring for Netlink Events from kernel */
/* Usage is very simple:

    1. Derive a class from this
    2. Override eventXXX functions as needed by your class
    3. Create an instance of your class, say nlm
    4. while(1)
       { 
          nlm.Monitor(true) // blocks forever
       }

    for non-blocking usage, take the fd via nlm.nlfd() and
    push it to your select/epoll write set, and when selected
    call nlm.Monitor(false)
*/
class NetlinkMonitor
{
    private:
   // MEMBER_WITH_GET(int, nlfd);
    int nlfd(){return m_nlfd;}
    fd_set m_readset;

    public:
    NetlinkMonitor();
    ~NetlinkMonitor();

    /* Apps can monitor using this call. To make this a non-blocking
       call, jsut pass false and call this function only after the fd
       is marked for read by kernel (via external select/epoll).

       Fd can be obtained by nlfd()

       Various callbacks (see at the end), will be automtically called,
       when events happen
    */
    void Monitor(bool block);
    private:
    int m_nlfd;
    void ProcessNLMsg(char* m_buf, unsigned int msgLen);
    void ProcessNLIfMsg(struct nlmsghdr *nlp);
    int subscribeToIntfNotifications();
  
    public:
    /* This will be automatically called whenever there is a 
        change in interface status */
    virtual  void eventInterfaceStatusChange
        (const char* ifname, int ifindex, unsigned int ifflags);
};

#endif

#include "ODSocket.h"
#include <stdio.h>
#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif

//#include "cocos2d.h"

bool ODSocket::m_bInitOK=false;
unsigned int ODSocket::m_uRefCount=0;

ODSocket::ODSocket(SOCKET sock/*=INVALID_SOCKET*/)
{
  m_sock=sock;
  ++m_uRefCount;
}

ODSocket::~ODSocket()
{
  if(Close())
    --m_uRefCount;
  if(0==m_uRefCount){
    (void)Clean();
  }
}

bool ODSocket::Init()
{
  if(m_bInitOK)
    return true;
#ifdef WIN32
/*http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx
  typedef struct WSAData { 
  WORD wVersion; //winsock version
  WORD wHighVersion; //The highest version of the Windows Sockets specification that the Ws2_32.dll can support
  char szDescription[WSADESCRIPTION_LEN+1]; 
  char szSystemStatus[WSASYSSTATUS_LEN+1]; 
  unsigned short iMaxSockets; 
  unsigned short iMaxUdpDg; 
  char FAR * lpVendorInfo; 
  }WSADATA, *LPWSADATA; 
*/
  WSADATA wsaData;
//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
  WORD version=MAKEWORD(2, 0);
  int ret=::WSAStartup(version, &wsaData);//win sock start up
  if(ret){
//     cerr<< "Initilize winsock error !" << endl;
    return false;
  }
#endif
  m_bInitOK=true;
  return true;
}

//this is just for windows
bool ODSocket::Clean()
{
#ifdef WIN32
  if(m_bInitOK&& 0!=::WSACleanup()){
    return false;
  };
#endif
  m_bInitOK=false;
  return true;
}

int ODSocket::GetError()
{
#ifdef WIN32
  return (::WSAGetLastError());
#else
  return (errno);
#endif
}

//只能在调用Init()之后被调用：
bool ODSocket::DnsParse(const char* domain, char* ip/*16个字符：255.255.255.255*/, unsigned short adpater, unsigned short* adpater_count)
{
  if(!m_bInitOK)//DnsParse()必须在Init()调用之后被调用。
    return false;
  hostent* p;
#ifdef WIN32
  char hostname[500]="\0";
  if(!domain&& 0==::gethostname(hostname, sizeof(hostname)))
    domain=hostname;
#else//linux:
  if(!domain)
    return GetLocalIP(ip);
#endif // WIN32
  if(!domain|| !(p=::gethostbyname(domain)))
    return false;
  if(adpater_count)
    *adpater_count=p->h_length;
  if(adpater>=p->h_length)
    return false;
  if(ip){
    sprintf(ip, "%s", inet_ntoa(*(in_addr*)p->h_addr_list[adpater]));
//     sprintf(ip, "%u.%u.%u.%u",
//       (unsigned char)p->h_addr_list[adpater][0], 
//       (unsigned char)p->h_addr_list[adpater][1], 
//       (unsigned char)p->h_addr_list[adpater][2], 
//       (unsigned char)p->h_addr_list[adpater][3]);
  }
  return true;
}

ODSocket& ODSocket::operator = (SOCKET s)
{
  m_sock=s;
  return (*this);
}

ODSocket::operator SOCKET ()
{
  return m_sock;
}

//create a socket object win/linux is the same
// af:
bool ODSocket::Create(int af, int type, int protocol)
{
  if(m_sock!=INVALID_SOCKET)
    return true;
  m_sock=::socket(af, type, protocol);
  if(m_sock==INVALID_SOCKET)
    return false;
  return true;
}

bool ODSocket::Close()
{
  if(m_sock==INVALID_SOCKET)
    return true;
  int iRet=
#ifdef WIN32
    ::closesocket(m_sock);
#else
    ::close(m_sock);
#endif
  if(iRet==0){
    m_sock=INVALID_SOCKET;
    return true;
  }
  return false;
}

bool ODSocket::setTimeout(const SET_TIMEOUT_TYPE& eSndOrRcv, long s, long us)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  timeval timeout={s, us};
  int optname=SO_RCVTIMEO;
  if(STOT_RCV==eSndOrRcv){
    //optname=SO_RCVTIMEO;
  }else if(STOT_SND==eSndOrRcv){
    optname=SO_SNDTIMEO;
  }else{
    return false;
  }
  if(::setsockopt(m_sock, SOL_SOCKET, optname, (char*)&timeout, sizeof(timeout)))
    return false;
  return true;
}

bool ODSocket::setPortReuse(bool bReuse)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  bool bRet=true;
  int opt=bReuse;
  if(::setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt))<0)
    bRet=false;
  return bRet;
}

//尽量使用上面的setPortReuse()
bool ODSocket::setPortExclusive(bool bExclusive)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  bool bRet=true;
  int opt=bExclusive;
  if(::setsockopt(m_sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&opt, sizeof(opt))<0)
    bRet=false;
  return bRet;
}

//bind部分UDP和TCP是一样的
bool ODSocket::Bind(unsigned short port, bool bReuse)
{
  if(!setPortReuse(bReuse))
    return false;
//   if(!setPortExclusive(!bReuse))
//     return false;
  sockaddr_in addr={0};
  addr.sin_family=AF_INET;
  char ip[22]="\0";
  (void)DnsParse(NULL, ip);
  addr.sin_addr.s_addr=inet_addr(ip);/*INADDR_ANY*/;
  addr.sin_port=htons(port);
  if(::bind(m_sock, (sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR)
    return false;
  return true;
}

//Dynamic Bind socket:
bool ODSocket::BindDynamic(unsigned short& wPort, bool bReuse)
{
  bool bRet=false;
  do{
    if(!setPortReuse(bReuse))
      break;
//    if(!setPortExclusive(!bReuse))
//      break;
    for(; wPort<(unsigned short)(-1); ++wPort){
      sockaddr_in addr={0};
      addr.sin_family=AF_INET;
      char ip[22]="\0";
      (void)DnsParse(NULL, ip);
      addr.sin_addr.s_addr=inet_addr(ip);/*INADDR_ANY*/;
      addr.sin_port=htons(wPort);//want data on this UDP port
      if(::bind(m_sock, (sockaddr*)&addr, sizeof(addr))!=SOCKET_ERROR){
        bRet=true;//绑定成功，说明端口可用。
        break;
      }
    }
  }while(false);
  return bRet;
}

//for server, listen之前需要bind
bool ODSocket::Listen(int backlog)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  int ret=::listen(m_sock, backlog);
  if(ret==SOCKET_ERROR)
    return false;
  return true;
}

//可以用select代替：
bool ODSocket::Accept(ODSocket& s, char* fromip/*22个字符：255.255.255.255:65536*/, unsigned short* fromport)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  sockaddr_in cliaddr={0};
  socklen_t addrlen=sizeof(cliaddr);
  SOCKET sock=::accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
  if(sock==SOCKET_ERROR)
    return false;
  s=sock;
  if(fromip)
    sprintf(fromip, "%s", ::inet_ntoa(cliaddr.sin_addr));
  if(fromport)
    *fromport=ntohs(cliaddr.sin_port);
  return true;
}

//connect之前可以bind来绑定一个本地端口
bool ODSocket::Connect(const char* ip, unsigned short port)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  sockaddr_in svraddr={0};
  svraddr.sin_family=AF_INET;
  svraddr.sin_addr.s_addr=::inet_addr(ip);
  svraddr.sin_port=htons(port);
  int ret=::connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
  if(ret==SOCKET_ERROR)
    return false;
  return true;
}

//send之前需要connect
int ODSocket::Send(const char* buf, int len, int flags)
{
  if(INVALID_SOCKET==m_sock)
    return 0;
  int bytes;
  int count=0;
  while(count<len){
    bytes=::send(m_sock, buf+count, len-count, flags);
    if(SOCKET_ERROR==bytes|| 0==bytes)
      return SOCKET_ERROR;
    count+=bytes;
  }
  return count;
}

//在connect或accept成功以后
int ODSocket::Recv(char* buf, int len, int flags)
{
  if(INVALID_SOCKET==m_sock)
    return 0;
  return (::recv(m_sock, buf, len, flags));
}

//可以不bind直接sendto, 成功后系统会自动bind端口
int ODSocket::SendTo(const char* ip, unsigned short port, const char* buf, int len, int flags)
{
  if(INVALID_SOCKET==m_sock|| !ip|| !buf|| len<=0)
    return 0;
  sockaddr_in addr={0};
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=::inet_addr(ip);
  addr.sin_port=htons(port);

  int bytes;
  int count=0;
  while(count<len){
    bytes=::sendto(m_sock, buf+count, len-count, flags, (sockaddr*)&addr, sizeof(addr));
    if(SOCKET_ERROR==bytes|| 0==bytes)
      return SOCKET_ERROR;
    count+=bytes;
  }
  return count;
}

//必须先显示bind之后才能recvfrom，或者通过sendto隐式bind后才行
int ODSocket::RecvFrom(char* buf, int len, char* fromip/*22个字符：255.255.255.255:65536*/, unsigned short* fromport, int flags)
{
  if(INVALID_SOCKET==m_sock)
    return 0;
  sockaddr_in addr={0};
  int nLen=sizeof(addr);
  int iRet=::recvfrom(m_sock, buf, len, flags, (sockaddr*)&addr, (socklen_t*)&nLen);
  if(iRet<=0){
    //int i=GetError();
    return iRet;
  }
  if(fromip)
    sprintf(fromip, "%s", ::inet_ntoa(addr.sin_addr));
  if(fromport)
    *fromport=ntohs(addr.sin_port);
  return iRet;
}

bool ODSocket::GetLocal(char* ip, unsigned short* port)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  bool bRet=false;
  sockaddr addr={0};
  sockaddr_in* addr_v4=NULL;
  int addr_len=sizeof(addr);
  if(0==getsockname(m_sock, &addr, (socklen_t*)&addr_len)){
    if(addr.sa_family==AF_INET){
      addr_v4=(sockaddr_in*)&addr;
      bRet=true;
      if(ip){
        bRet=DnsParse(NULL, ip);
// #ifdef WIN32
//         sprintf(ip, "%s", inet_ntoa(addr_v4->sin_addr));
// #else
//         bRet=GetLocalIP(ip);
// #endif // WIN32
      }
      if(port)
        *port=ntohs(addr_v4->sin_port);
    }
  }
  return bRet;
}

//获取远端地址和端口，一般用于TCP建立连接Connect或Accept以后，UDP最好使用RecvFrom获得。
bool ODSocket::GetRemote(char* ip, unsigned short* port)
{
  if(INVALID_SOCKET==m_sock)
    return false;
  bool bRet=false;
  struct sockaddr addr={0};
  struct sockaddr_in* addr_v4=NULL;
  int addr_len=sizeof(addr);
  if(0==getpeername(m_sock, &addr, (socklen_t*)&addr_len)){
    if(addr.sa_family==AF_INET){
      bRet=true;
      addr_v4=(sockaddr_in*)&addr;
      if(ip){
        sprintf(ip, "%s", inet_ntoa(addr_v4->sin_addr));
      }
      if(port)
        *port=ntohs(addr_v4->sin_port);
    }
  }
  return bRet;
}

#ifndef WIN32//linux:
bool ODSocket::GetLocalIP(char* ip)
{
  if(!ip)
    return false;
  bool bRet=false;
  int socket_fd;
  //struct sockaddr_in* sin;
  struct ifreq* ifr;
  struct ifconf conf;
  char buff[BUFSIZ];
  socket_fd=socket(AF_INET, SOCK_DGRAM, 0);
  if(socket_fd==INVALID_SOCKET)
    return bRet;
  do{
    conf.ifc_len=BUFSIZ;
    conf.ifc_buf=buff;
    if(0!=ioctl(socket_fd, SIOCGIFCONF, &conf))
      break;
    int num=conf.ifc_len/sizeof(struct ifreq);
    ifr=conf.ifc_req;
    printf("num=%d\n", num);
    for(int i=0; i<num; ++i){
      struct sockaddr_in* sin=(struct sockaddr_in*)(&ifr->ifr_addr);
      if(0!=ioctl(socket_fd, SIOCGIFFLAGS, ifr))
        continue;
      if(((ifr->ifr_flags& IFF_LOOPBACK)==0)&& (ifr->ifr_flags& IFF_UP)){
        sprintf(ip, "%s", inet_ntoa(sin->sin_addr));
        printf("%s(%s)\n", ifr->ifr_name, ip);
        bRet=true;
        break;
      }
      ifr++;
    }
  }while(false);
  ::close(socket_fd);
  return bRet;
}
#endif//linux.

/*
* define file about portable socket class. 
* description:this sock is suit both windows and linux
* design:odison
* e-mail:odison@126.com>
* 
*/

#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_

#ifdef WIN32
#include <winsock2.h>
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "errno.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
typedef int SOCKET;

//#pragma region define win32 const variable in linux
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#ifndef SO_REUSEADDR
#define SO_REUSEADDR (0x0004)/* allow local address reuse */
#endif
#ifndef SO_EXCLUSIVEADDRUSE
#define SO_EXCLUSIVEADDRUSE ((int)(~SO_REUSEADDR))/* disallow local address reuse */
#endif
//#pragma endregion
#endif

class ODSocket
{
public:
  typedef enum _SET_TIMEOUT_TYPE
  { STOT_INVALID=-1
  , STOT_RCV=0
  , STOT_SND=1
  }SET_TIMEOUT_TYPE, *LPSET_TIMEOUT_TYPE;

  ODSocket(SOCKET sock=INVALID_SOCKET);
  ~ODSocket();
  ODSocket& operator = (SOCKET s);
  operator SOCKET ();
  // Init winsock DLL 
  static bool Init();	
  // Clean winsock DLL
  static bool Clean();//根据引用计数内部调用清理，不要让外部调用。
  // Get errno
  static int GetError();
  // Domain parse, only be called after Init()
  static bool DnsParse(const char* domain, char* ip/*16个字符：255.255.255.255*/, unsigned short adpater=0, unsigned short* adpater_count=NULL);

  // Create socket object for snd/recv data
  bool Create(int af, int type, int protocol = 0);
  // Close socket
  bool Close();
  
  //Set socket timeout
  bool setTimeout(const SET_TIMEOUT_TYPE& eSndOrRcv, long s, long us=0);
  //Set port can be reused
  bool setPortReuse(bool bReuse);
  //Set port can be exclusived
  bool setPortExclusive(bool bExclusive);

  // Bind socket
  bool Bind(unsigned short port, bool bReuse=true);
  // Dynamic Bind socket
  bool BindDynamic(unsigned short& wPort, bool bReuse=true);
  
  //#region server
  // Listen socket
  bool Listen(int backlog = SOMAXCONN); 
  // Accept socket
  bool Accept(ODSocket& s, char* fromip=NULL/*16个字符：255.255.255.255*/, unsigned short* fromport=NULL);
  //#endregion

  //TCP Connect socket
  bool Connect(const char* ip, unsigned short port);
  //TCP Send socket
  int Send(const char* buf, int len, int flags = 0);
  //TCP Recv socket
  int Recv(char* buf, int len, int flags = 0);
  //UDP Send socket
  int SendTo(const char* ip, unsigned short port, const char* buf, int len, int flags=0);
  //UDP Recv socket
  int RecvFrom(char* buf, int len, char* fromip=NULL/*16个字符：255.255.255.255*/, unsigned short* fromport=NULL, int flags=0);
  //get local sock's ip and port
  bool GetLocal(char* ip, unsigned short* port=NULL);
  bool GetRemote(char* ip, unsigned short* port=NULL);
protected:
  SOCKET m_sock;
  static bool m_bInitOK;
  static unsigned int m_uRefCount;
#ifndef WIN32
  static bool GetLocalIP(char* ip);
#endif // WIN32
};

#endif

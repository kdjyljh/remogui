#ifndef __SYSTEM__H__
#define __SYSTEM__H__

#ifdef __cplusplus
extern "C" {
#endif

// #include    <limits.h>      /* PIPE_BUF */
// #include    <float.h>
// #include    <getopt.h>
// #include    <errno.h>
// #include    <unistd.h>
// #include    <pthread.h>
// #include    <signal.h>
// #include    <time.h>        /* timespec{} for pselect() */
// #include    <fcntl.h>       /* for nonblocking */
// #include    <netdb.h>

// #include    <arpa/inet.h>   /* inet(3) functions */
// #include    <net/if.h>
// #include    <net/if_arp.h>
// #include    <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
// #include    <netinet/ip6.h>
// #include    <netipx/ipx.h>

// #include    <sys/types.h>   /* basic system data types */
// #include    <sys/time.h>    /* timeval{} for select() */
// #include    <sys/uio.h>     /* for iovec{} and readv/writev */
// #include    <sys/wait.h>
// #include    <sys/stat.h>    /* for S_xxx file mode constants */
// #include    <sys/socket.h>  /* basic socket definitions */
// #include    <sys/un.h>      /* for Unix domain sockets */
// #include    <sys/ioctl.h>
// #include    <sys/select.h>  /* for convenience */
// #include    <sys/param.h>   /* OpenBSD prereq for sysctl.h */
// #include    <sys/sysctl.h>
// #include    <sys/sem.h>     /* System V semaphores */
// #include    <sys/shm.h>     /* System V shared memory */
// #include    <sys/ipc.h>     /* System V IPC */
// #include    <sys/msg.h>     /* System V message queues */
// #include    <sys/mman.h>    /* Posix shared memory */
// #include    <mqueue.h>      /* Posix message queues */
// #include    <semaphore.h>   /* Posix semaphores */
// #include    <rpc/rpc.h>     /* Sun RPC */
// #include    <syslog.h>
// #include    <poll.h>        /* for convenience */
// #include    <stropts.h>     /* for convenience */
// #include    <strings.h>     /* for convenience */

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include    <boost/cstdint.hpp>

#include    <cstdio>
#include    <cstdlib>
#include    <cstdarg>
#include    <cstring>
#include    <cmath>
#else
#include    <stdint.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include    <string.h>
#include    <math.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include    <inttypes.h> // inttypes.h already includes stdint.h
#ifdef __cplusplus
}
#endif

#ifdef      __cplusplus
#include    <typeinfo>
#include    <string>
#include    <sstream>
#include    <fstream>
#include    <iostream>
#include    <iterator>
#include    <set>
#include    <map>

#include    <boost/unordered_map.hpp>
#include    <boost/unordered_set.hpp>
#include    <boost/shared_ptr.hpp>
#include    <boost/make_shared.hpp>
#include    <boost/function.hpp>
#include    <boost/thread/mutex.hpp>
#include    <boost/thread/lock_guard.hpp>
#include    <boost/lexical_cast.hpp>

using std::ios;
using std::fstream;
using std::string;
using std::stringstream;
using std::pair;
using std::make_pair;
using std::iterator;
using std::vector;
using std::deque;
using std::map;
using std::set;

using boost::unordered_map;
using boost::unordered_set;
using boost::shared_ptr;
using boost::mutex;
using boost::unique_lock;
using boost::lexical_cast;

#include    <vector>
#include    <queue>
#include    <deque>
#include    <list>
#include    <algorithm>
#endif

#ifdef      __cplusplus

#define DISABLE_COPY_AND_ASSIGN(classname) \
private:\
  classname(const classname&);\
  classname& operator=(const classname&)

#endif

#include <glog/logging.h>

#endif /* __SYSTEM__H__ */

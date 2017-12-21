#ifndef COMMLOG_H
#define COMMLOG_H
#include <glog/logging.h>

#define STR_TO_LOG_FILE(str) \
do { \
    int store = FLAGS_logtostderr; \
    FLAGS_logtostderr = 0; \
    LOG(INFO) << str;\
    FLAGS_logtostderr = store; \
} while(0);

#define STR_TO_LOG_STDERROR(str) \
do { \
    int store = FLAGS_logtostderr; \
    FLAGS_logtostderr = 1; \
    LOG(INFO) << str;\
    FLAGS_logtostderr = store; \
} while(0);

#define CHAR_BUFF_TO_LOG_FILE(buff) \
do { \
    char temp[64]{0}; \
    std::string str; \
    for (auto it : buff) { \
        snprintf(temp, sizeof(temp), "0x%02hhx ", it); \
        str += temp; \
    } \
    STR_TO_LOG_FILE(str); \
} while(0);

#define CHAR_BUFF_TO_LOG_STDERROR(buff) \
do { \
    char temp[64]{0}; \
    std::string str; \
    for (auto it : buff) { \
        snprintf(temp, sizeof(temp), "0x%02hhx ", it); \
        str += temp; \
    } \
    STR_TO_LOG_STDERROR(str); \
} while(0);
#endif // COMMLOG_H

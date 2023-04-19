#ifndef __BIRA_LOG_H__
#define __BIRA_LOG_H__

#include <stdlib.h>
#include <stdio.h>

#define __BIRA_WARN__ "=== BIRA Warning ==="
#define __BIRA_ERROR__ "=== BIRA Error ==="
#define __BIRA_INFO__ "=== BIRA Info ==="

enum BIRALOG_LEVEL {
    BIRALOG_ALWAYS=0,
    BIRALOG_WARN,
    BIRALOG_INFO,
    BIRALOG_DEBUG,
    BIRALOG_ALL
};
#define LOG_LEVEL BIRALOG_LEVEL::BIRALOG_ALL
#define BIRA_LOG(level, format...) do { if(level<=LOG_LEVEL) fprintf(stderr, "[BIRALOG] " format); } while(0)

#define BIRA_INFO(format...) BIRA_LOG(BIRALOG_INFO, format)
#define BIRA_WARN(format...) BIRA_LOG(BIRALOG_WARN, format)
#define BIRA_ERROR(format...) do { \
    fprintf(stderr, __BIRA_ERROR__ format); \
    exit(-1); \
} while(0)

#define ASSERT(stat) do {\
    if(!stat) fprintf(stderr, "BIRA Assert Failed: " #stat);\
} while(0)
#define ASSERT_MSG(stat, msg) do {\
    if(!stat) fprintf(stderr, "BIRA Assert Failed: %s (%s)", msg, #stat);\
} while(0)

#ifdef DEBUG
#  define DEBUG_BIRAINFO(format...) BIRA_INFO(format)
#  define DASSERT(stat) ASSERT(stat) 
#  define DASSERT_MSG(stat, msg) ASSERT_MSG(stat, msg)
#else
#  define DEBUG_BIRAINFO(format...)
#  define DASSERT(stat)
#  define DASSERT_MSG(stat, msg)
#endif

#endif

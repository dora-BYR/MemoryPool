#ifndef __DEFINE_LONG__
#define __DEFINE_LONG__

#ifdef __cplusplus

#define NS_LONG_BEGIN                     namespace hlong {
#define NS_LONG_END                       }
#define USING_NS_LONG                     using namespace hlong
#define NS_LONG                           ::hlong

#else

#define NS_LONG_BEGIN
#define NS_LONG_END
#define USING_NS_LONG
#define NS_LONG

#endif

#ifndef NULL
#define NULL (0)
#endif

#define LLOG(format, ...) printf((std::string(format) + "\n").c_str(), ##__VA_ARGS__)

#endif // __DEFINE_LONG__
#ifdef __beet_begin__
#error Nested inclusion of beet_begin.h
#endif
#define __beet_begin__


#ifdef BEET_EXPORTS
#define BEET_API __declspec(dllexport)
#else
#define BEET_API __declspec(dllimport)
#endif
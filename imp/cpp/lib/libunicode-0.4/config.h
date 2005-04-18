// Written by Dave Woldrich so that build doesn't require configure to run
//
// config.h just so that the build finishes

#ifdef _MSC_VER
# define bcopy memmove
# define strcasecmp strcmpi
#endif

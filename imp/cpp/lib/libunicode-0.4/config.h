// Written by Dave Woldrich so that build doesn't require configure to run
//
// config.h just so that the build finishes

#if (defined(_MSC_VER) || defined(__BORLANDC__))
# define bcopy memmove
# define strcasecmp strcmpi
#endif

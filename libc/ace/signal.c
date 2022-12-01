#include <signal.h>
#include <errno.h>
typedef void (*SignalHandler) (int);
SignalHandler signal(int sig, SignalHandler func)
{
	errno = EINVAL;
	return -1;
}
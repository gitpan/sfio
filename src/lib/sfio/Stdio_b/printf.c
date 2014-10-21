#include	"sfstdio.h"

/*	Write out data to stdout using a given format.
**	Written by Kiem-Phong Vo
*/

#if __STD_C
int printf(const char* form, ...)
#else
int printf(va_alist)
va_dcl
#endif
{
	va_list		args;
	reg int		rv;
	reg Sfio_t*	sp;
#if __STD_C
	va_start(args,form);
#else
	reg char*	form;	/* print format */
	va_start(args);
	form = va_arg(args,char*);
#endif

	if(!(sp = _sfstream(stdout)))
		return -1;
	_stdclrerr(stdout,sp);

	rv = sfvprintf(sp,form,args);

	va_end(args);

	if(rv < 0)
		_stderr(stdout);
	return rv;
}

#include	"sfstdio.h"

/*	Reopening a stream
**	Written by Kiem-Phong Vo
*/


#if __STD_C
FILE* freopen(char*name, const char* mode, reg FILE* fp)
#else
FILE* freopen(name,mode,fp)
reg char*	name;
reg char*	mode;
reg FILE*	fp;
#endif
{
	reg Sfio_t*	sp;

	if(!(sp = _sfstream(fp)))
		return NIL(FILE*);

	_stdclrerr(fp,sp);
	if(!sfopen(sp, name, mode))
		return NIL(FILE*);

#if _FILE_cnt
	fp->std_cnt = 0;
#endif
#if _FILE_r
	fp->std_r = 0;
#endif
#if _FILE_w
	fp->std_w = 0;
#endif
#if _FILE_readptr
	fp->std_readptr = fp->std_readend = NIL(uchar*);
#endif
#if _FILE_writeptr
	fp->std_writeptr = fp->std_writeend = NIL(uchar*);
#endif
#if _FILE_flag
	fp->std_flag = 0;
#endif
#if _FILE_file
	fp->std_file = sffileno(sp);
#endif

	return fp;
}

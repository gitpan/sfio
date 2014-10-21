#ifndef _SFIO_H
#define _SFIO_H	1

#define SFIO_VERSION	19970101L

/*	Public header file for the sfio library
**
**	Written by Kiem-Phong Vo (kpv@research.att.com)
*/

#ifndef KPVDEL /* KPV-- to be removed on next release */
#define sfdisc	_sfdisc
#endif

#if _PACKAGE_ast
#include	<ast_std.h>
#else
#include	<ast_common.h>

/* to prevent stdio.h from being included */
#ifndef __stdio_h__
#define __stdio_h__	1
#endif
#ifndef _stdio_h_
#define _stdio_h_	1
#endif
#ifndef _stdio_h
#define _stdio_h	1
#endif
#ifndef __h_stdio__
#define __h_stdio__	1
#endif
#ifndef _h_stdio_
#define _h_stdio_	1
#endif
#ifndef _h_stdio
#define _h_stdio	1
#endif
#ifndef __STDIO_H__
#define __STDIO_H__	1
#endif
#ifndef _STDIO_H_
#define _STDIO_H_	1
#endif
#ifndef _STDIO_H
#define _STDIO_H	1
#endif
#ifndef __H_STDIO__
#define __H_STDIO__	1
#endif
#ifndef _H_STDIO_
#define _H_STDIO_	1
#endif
#ifndef _H_STDIO
#define _H_STDIO	1
#endif
#ifndef _stdio_included
#define _stdio_included	1
#endif
#ifndef _included_stdio
#define _included_stdio	1
#endif
#ifndef _INCLUDED_STDIO
#define _INCLUDED_STDIO	1
#endif
#ifndef _STDIO_INCLUDED
#define _STDIO_INCLUDED	1
#endif
#ifndef _INC_STDIO
#define _INC_STDIO	1
#endif

#ifndef FILE
#define _FILE_DEFINED	1	/* stop MS headers from defining FILE	*/
#define FILE	struct _sfio_s	/* because certain stdarg.h needs FILE	*/
#endif

#if __STD_C
#include	<stddef.h>
#include	<stdlib.h>
#include	<stdarg.h>
#else
#include	<varargs.h>
#endif /* __STD_C */

#endif /* _PACKAGE_ast */

/* Sfoff_t should be large enough for largest file address */
#if _typ_long_long
#define Sfoff_t		long long
#define Sflong_t	long long
#define Sfulong_t	unsigned long long
#else
#if _typ_int64_t && _typ_uint64_t
#define Sfoff_t		int64_t
#define Sflong_t	int64_t
#define Sfulong_t	uint64_t
#else
#define Sfoff_t		long
#define Sflong_t	long
#define Sfulong_t	unsigned long
#endif
#endif

#if _typ_long_double
#define Sfdouble_t	long double
#else
#define Sfdouble_t	double
#endif

typedef struct _sfio_s		Sfio_t;
typedef struct _sfdisc_s	Sfdisc_t;
typedef ssize_t		(*Sfread_f)_ARG_((Sfio_t*, Void_t*, size_t, Sfdisc_t*));
typedef ssize_t		(*Sfwrite_f)_ARG_((Sfio_t*, const Void_t*, size_t, Sfdisc_t*));
typedef Sfoff_t		(*Sfseek_f)_ARG_((Sfio_t*, Sfoff_t, int, Sfdisc_t*));
typedef int		(*Sfexcept_f)_ARG_((Sfio_t*, int, Void_t*, Sfdisc_t*));

/* discipline structure */
struct _sfdisc_s
{	Sfread_f	readf;		/* read function		*/
	Sfwrite_f	writef;		/* write function		*/
	Sfseek_f	seekf;		/* seek function		*/
	Sfexcept_f	exceptf;	/* to handle exceptions		*/
	Sfdisc_t*	disc;		/* the continuing discipline	*/
};

/* a file structure */
struct _sfio_s
{	unsigned char*	next;	/* next position to read/write from	*/
	unsigned char*	endw;	/* end of write buffer			*/
	unsigned char*	endr;	/* end of read buffer			*/
	unsigned char*	endb;	/* end of buffer			*/
	Sfio_t*		push;	/* the stream that was pushed on	*/
	unsigned short	flags;	/* type of stream			*/
	short		file;	/* file descriptor			*/
	unsigned char*	data;	/* base of data buffer			*/
	ssize_t		size;	/* buffer size				*/
	ssize_t		val;	/* values or string lengths		*/
#ifdef _SFIO_PRIVATE
	_SFIO_PRIVATE
#endif
};

/* formatting environment */
typedef struct _sffmt_s	Sffmt_t;
typedef int		(*Sfarg_f)_ARG_((Sfio_t*, Void_t*, Sffmt_t*));
typedef int		(*Sfext_f)_ARG_((Sfio_t*, Void_t*, int, Sffmt_t*));
struct _sffmt_s
{	char*		form;	/* format string to stack		*/
	va_list		args;	/* corresponding arg list		*/
	Sfarg_f		argf;	/* function to get/set arguments	*/
	Sfext_f		extf;	/* function to do extended patterns	*/

	char		fmt;	/* format character			*/
	char		flag;	/* one of: l, h, L			*/
	short		n_flag;	/* number of flag occurences		*/
	int		base;	/* conversion base			*/
	int		precis;	/* precision required			*/

	char*		t_str;	/* type string or extf's return value	*/
	int		n_str;	/* length of t_str or -1 for 0-termed	*/
};

/* various constants */
#ifndef NULL
#define NULL		0
#endif
#ifndef EOF
#define EOF		(-1)
#endif
#ifndef SEEK_SET
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#endif

/* bits for various types of files */
#define	SF_READ		0000001	/* open for reading			*/
#define SF_WRITE	0000002	/* open for writing			*/
#define SF_STRING	0000004	/* a string stream			*/
#define SF_APPENDWR	0000010	/* associated file is in append mode	*/
#define SF_APPEND	SF_APPENDWR	/* BSDI messed up the namespace	*/
				/* by using the SF_ prefix in stat.h.	*/
				/* If you run across this, either change*/
				/* to SF_APPENDWR or complain to them.	*/
#define SF_MALLOC	0000020	/* buffer is malloc-ed			*/
#define SF_LINE		0000040	/* line buffering			*/
#define SF_SHARE	0000100	/* stream with shared file descriptor 	*/
#define SF_EOF		0000200	/* eof was detected			*/
#define SF_ERROR	0000400	/* an error happened			*/
#define SF_STATIC	0001000	/* a stream that cannot be freed	*/
#define SF_IOCHECK	0002000	/* call exceptf before doing IO		*/
#define SF_PUBLIC	0004000	/* SF_SHARE and follow physical seek	*/
#define SF_BUFCONST	0010000	/* buffer not modifiable 		*/

#define SF_FLAGS	0015177	/* PUBLIC FLAGS PASSABLE TO SFNEW()	*/
#define SF_SETS		0017163	/* flags passable to sfset()		*/

/* exception events: SF_NEW(0), SF_READ(1), SF_WRITE(2) and the below 	*/
#define SF_SEEK		3	/* seek error				*/
#define SF_CLOSE	4	/* when stream is about to be closed	*/
#define SF_DPUSH	5	/* when discipline is being pushed	*/
#define SF_DPOP		6	/* when discipline is being popped	*/
#define SF_DPOLL	7	/* see if stream is ready for I/O	*/
#define SF_DBUFFER	8	/* buffer not empty during push or pop	*/
#define SF_SYNC		9	/* a sfsync() call was issued		*/
#define SF_PURGE	10	/* a sfpurge() call was issued		*/
#define SF_FINAL	11	/* closing is done except stream free	*/
#define SF_READY	12	/* a polled stream is ready		*/
#define SF_EVENT	100	/* start of user-defined events		*/

/* for stack and disciplines */
#define SF_POPSTACK	NIL(Sfio_t*)	/* pop the stream stack		*/
#define SF_POPDISC	NIL(Sfdisc_t*)	/* pop the discipline stack	*/

/* for the notify function and discipline exception */
#define SF_NEW		0	/* new stream				*/
#define SF_SETFD	(-1)	/* about to set the file descriptor 	*/

#define SF_BUFSIZE	8192	/* default buffer size			*/
#define SF_UNBOUND	(-1)	/* unbounded buffer size		*/

_BEGIN_EXTERNS_

extern ssize_t		_Sfi;

#if _DLL && _DLL_INDIRECT_DATA	/* Uwin environment */
#define sfstdin		((Sfio_t*)_ast_dll->_ast_stdin)
#define sfstdout	((Sfio_t*)_ast_dll->_ast_stdout)
#define sfstderr	((Sfio_t*)_ast_dll->_ast_stderr)

#else
#define	sfstdin		(&_Sfstdin)	/* standard input stream	*/
#define	sfstdout	(&_Sfstdout)	/* standard output stream	*/
#define	sfstderr	(&_Sfstderr)	/* standard error stream	*/
extern Sfio_t		_Sfstdin;
extern Sfio_t		_Sfstdout;
extern Sfio_t		_Sfstderr;
#endif

_END_EXTERNS_

_BEGIN_EXTERNS_
#if _BLD_sfio && defined(__EXPORT__)
#define extern	__EXPORT__
#endif
#if !_BLD_sfio && defined(__IMPORT__) && defined(__EXPORT__)
#define extern	__IMPORT__
#endif

extern Sfio_t*		sfnew _ARG_((Sfio_t*, Void_t*, size_t, int, int));
extern Sfio_t*		sfopen _ARG_((Sfio_t*, const char*, const char*));
extern Sfio_t*		sfpopen _ARG_((Sfio_t*, const char*, const char*));
extern Sfio_t*		sfstack _ARG_((Sfio_t*, Sfio_t*));
extern Sfio_t*		sfswap _ARG_((Sfio_t*, Sfio_t*));
extern Sfio_t*		sftmp _ARG_((size_t));
extern int		sfpurge _ARG_((Sfio_t*));
extern int		sfpoll _ARG_((Sfio_t**, int, int));
extern Void_t*		sfreserve _ARG_((Sfio_t*, ssize_t, int));
extern int		sfsync _ARG_((Sfio_t*));
extern int		sfclrlock _ARG_((Sfio_t*));
extern Void_t*		sfsetbuf _ARG_((Sfio_t*, Void_t*, size_t));
extern Sfdisc_t*	sfdisc _ARG_((Sfio_t*,Sfdisc_t*));
extern int		sfraise _ARG_((Sfio_t*, int, Void_t*));
extern int		sfnotify _ARG_((void(*)(Sfio_t*, int, int)));
extern int		sfset _ARG_((Sfio_t*, int, int));
extern int		sfsetfd _ARG_((Sfio_t*, int));
extern Sfio_t*		sfpool _ARG_((Sfio_t*, Sfio_t*, int));
extern ssize_t		sfread _ARG_((Sfio_t*, Void_t*, size_t));
extern ssize_t		sfwrite _ARG_((Sfio_t*, const Void_t*, size_t));
extern Sfoff_t		sfmove _ARG_((Sfio_t*, Sfio_t*, Sfoff_t, int));
extern int		sfclose _ARG_((Sfio_t*));
extern Sfoff_t		sftell _ARG_((Sfio_t*));
extern Sfoff_t		sfseek _ARG_((Sfio_t*, Sfoff_t, int));
extern ssize_t		sfputr _ARG_((Sfio_t*, const char*, int));
extern char*		sfgetr _ARG_((Sfio_t*, int, int));
extern ssize_t		sfnputc _ARG_((Sfio_t*, int, size_t));
extern int		sfungetc _ARG_((Sfio_t*, int));
extern int		sfprintf _ARG_((Sfio_t*, const char*, ...));
extern char*		sfprints _ARG_((const char*, ...));
extern int		sfsprintf _ARG_((char*, int, const char*, ...));
extern int		sfvsprintf _ARG_((char*, int, const char*, va_list));
extern int		sfvprintf _ARG_((Sfio_t*, const char*, va_list));
extern int		sfscanf _ARG_((Sfio_t*, const char*, ...));
extern int		sfsscanf _ARG_((const char*, const char*, ...));
extern int		sfvsscanf _ARG_((const char*, const char*, va_list));
extern int		sfvscanf _ARG_((Sfio_t*, const char*, va_list));

/* io functions with discipline continuation */
extern ssize_t		sfrd _ARG_((Sfio_t*, Void_t*, size_t, Sfdisc_t*));
extern ssize_t		sfwr _ARG_((Sfio_t*, const Void_t*, size_t, Sfdisc_t*));
extern Sfoff_t		sfsk _ARG_((Sfio_t*, Sfoff_t, int, Sfdisc_t*));
extern ssize_t		sfpkrd _ARG_((int, Void_t*, size_t, int, long, int));

/* portable handling of primitive types */
extern int		sfdlen _ARG_((Sfdouble_t));
extern int		sfllen _ARG_((Sflong_t));
extern int		sfulen _ARG_((Sfulong_t));

extern int		sfputd _ARG_((Sfio_t*, Sfdouble_t));
extern int		sfputl _ARG_((Sfio_t*, Sflong_t));
extern int		sfputu _ARG_((Sfio_t*, Sfulong_t));
extern int		sfputc _ARG_((Sfio_t*, int));

extern Sfdouble_t	sfgetd _ARG_((Sfio_t*));
extern Sflong_t		sfgetl _ARG_((Sfio_t*));
extern Sfulong_t	sfgetu _ARG_((Sfio_t*));
extern int		sfgetc _ARG_((Sfio_t*));

extern int		_sfputd _ARG_((Sfio_t*, Sfdouble_t));
extern int		_sfputl _ARG_((Sfio_t*, Sflong_t));
extern int		_sfputu _ARG_((Sfio_t*, Sfulong_t));
extern int		_sfflsbuf _ARG_((Sfio_t*, int));

extern Sflong_t		_sfgetl _ARG_((Sfio_t*));
extern Sfulong_t	_sfgetu _ARG_((Sfio_t*));
extern int		_sffilbuf _ARG_((Sfio_t*, int));

extern int		_sfdlen _ARG_((Sfdouble_t));
extern int		_sfllen _ARG_((Sflong_t));
extern int		_sfulen _ARG_((Sfulong_t));

/* miscellaneous function analogues of fast in-line functions */
extern Sfoff_t		sfsize _ARG_((Sfio_t*));
extern int		sfclrerr _ARG_((Sfio_t*));
extern int		sfeof _ARG_((Sfio_t*));
extern int		sferror _ARG_((Sfio_t*));
extern int		sffileno _ARG_((Sfio_t*));
extern int		sfstacked _ARG_((Sfio_t*));
extern ssize_t		sfvalue _ARG_((Sfio_t*));
extern ssize_t		sfslen _ARG_((void));

#undef extern
_END_EXTERNS_

/* coding long integers in a portable and compact fashion */
#define SF_SBITS	6
#define SF_UBITS	7
#define SF_SIGN		(1 << SF_SBITS)
#define SF_MORE		(1 << SF_UBITS)
#define SF_U1		SF_MORE
#define SF_U2		(SF_U1*SF_U1)
#define SF_U3		(SF_U2*SF_U1)
#define SF_U4		(SF_U3*SF_U1)

#if __cplusplus
#define _SF_(f)		(f)
#else
#define _SF_(f)		((Sfio_t*)(f))
#endif
#define __sf_putd(f,v)	(_sfputd(_SF_(f),(Sfdouble_t)(v)))
#define __sf_putl(f,v)	(_sfputl(_SF_(f),(Sflong_t)(v)))
#define __sf_putu(f,v)	(_sfputu(_SF_(f),(Sfulong_t)(v)))
#define __sf_putc(f,c)	(_SF_(f)->next >= _SF_(f)->endw ? \
			 _sfflsbuf(_SF_(f),(int)((unsigned char)(c))) : \
			 (int)(*_SF_(f)->next++ = (unsigned char)(c)) )

#define __sf_getl(f)	((_SF_(f)->val = sfgetc(_SF_(f))) < 0 ? (Sflong_t)(-1) : \
			 ((_SF_(f)->val&(SF_MORE|SF_SIGN)) ? _sfgetl(_SF_(f)) : \
			  (Sflong_t)(_SF_(f)->val) ) )
#define __sf_getu(f)	((_SF_(f)->val = sfgetc(_SF_(f))) < 0 ? (Sfulong_t)(-1) : \
			 ((_SF_(f)->val&SF_MORE) ? _sfgetu(_SF_(f)) : \
			  (Sfulong_t)(_SF_(f)->val) ) )
#define __sf_getc(f)	(_SF_(f)->next >= _SF_(f)->endr ? _sffilbuf(_SF_(f),0) : \
			 (int)(*_SF_(f)->next++) )

#define __sf_dlen(v)	(_sfdlen((Sfdouble_t)(v)) )
#define __sf_llen(v)	(_sfllen((Sflong_t)(v)) )
#define __sf_ulen(v)	((Sfulong_t)(v) < SF_U1 ? 1 : (Sfulong_t)(v) < SF_U2 ? 2 : \
			 (Sfulong_t)(v) < SF_U3 ? 3 : (Sfulong_t)(v) < SF_U4 ? 4 : 5)

#define __sf_fileno(f)	(_SF_(f)->file)
#define __sf_eof(f)	(_SF_(f)->flags&SF_EOF)
#define __sf_error(f)	(_SF_(f)->flags&SF_ERROR)
#define __sf_clrerr(f)	(_SF_(f)->flags &= ~(SF_ERROR|SF_EOF))
#define __sf_stacked(f)	(_SF_(f)->push != NIL(Sfio_t*))
#define __sf_value(f)	(_SF_(f)->val)
#define __sf_slen()	(_Sfi)

#if defined(__INLINE__) && !_BLD_sfio
__INLINE__ int sfputd(Sfio_t* f, Sfdouble_t v)	{ return __sf_putd(f,v);	}
__INLINE__ int sfputl(Sfio_t* f, Sflong_t v)	{ return __sf_putl(f,v);	}
__INLINE__ int sfputu(Sfio_t* f, Sfulong_t v)	{ return __sf_putu(f,v);	}
__INLINE__ int sfputc(Sfio_t* f, int c)		{ return __sf_putc(f,c);	}

__INLINE__ Sfulong_t sfgetu(Sfio_t* f)		{ return __sf_getu(f);		}
__INLINE__ Sflong_t sfgetl(Sfio_t* f)		{ return __sf_getl(f);		}
__INLINE__ int sfgetc(Sfio_t* f)		{ return __sf_getc(f);		}

__INLINE__ int sfdlen(Sfdouble_t v)		{ return __sf_dlen(v);		}
__INLINE__ int sfllen(Sflong_t v)		{ return __sf_llen(v);		}
__INLINE__ int sfulen(Sfulong_t v)		{ return __sf_ulen(v);		}

__INLINE__ int sffileno(Sfio_t* f)		{ return __sf_fileno(f);	}
__INLINE__ int sfeof(Sfio_t* f)			{ return __sf_eof(f);		}
__INLINE__ int sferror(Sfio_t* f)		{ return __sf_error(f);		}
__INLINE__ int sfclrerr(Sfio_t* f)		{ return __sf_clrerr(f);	}
__INLINE__ int sfstacked(Sfio_t* f)		{ return __sf_stacked(f);	}
__INLINE__ ssize_t sfvalue(Sfio_t* f)		{ return __sf_value(f);		}
#else
#define sfputd(f,v)				( __sf_putd((f),(v))		)
#define sfputl(f,v)				( __sf_putl((f),(v))		)
#define sfputu(f,v)				( __sf_putu((f),(v))		)
#define sfputc(f,c)				( __sf_putc((f),(c))		)

#define sfgetu(f)				( __sf_getu(f)			)
#define sfgetl(f)				( __sf_getl(f)			)
#define sfgetc(f)				( __sf_getc(f)			)

#define sfdlen(v)				( __sf_dlen(v)			)
#define sfllen(v)				( __sf_llen(v)			)
#define sfulen(v)				( __sf_ulen(v)			)

#define sffileno(f)				( __sf_fileno(f)		)
#define sfeof(f)				( __sf_eof(f)			)
#define sferror(f)				( __sf_error(f)			)
#define sfclrerr(f)				( __sf_clrerr(f)		)
#define sfstacked(f)				( __sf_stacked(f)		)
#define sfvalue(f)				( __sf_value(f)			)
#define sfslen()				( __sf_slen()			)
#endif /*__INLINE__*/

#endif /* _SFIO_H */

/*	libsfio.a nmake Makefile
**	Written by Kiem-Phong Vo (12/10/90)
*/

VERSION = 1.0
.SOURCE : Sfio_f Stdio_b Stdio_s
.INCLUDE : Stdio_b
ancestor = 2

CCFLAGS = -O

SFIO=	sfexit.c sfclose.c sfclrlock.c sfcvt.c sfdlen.c sfexcept.c sfextern.c \
	sffilbuf.c sfflsbuf.c sfprints.c sfgetd.c sfgetl.c sfgetr.c sfgetu.c \
	sfllen.c sfmode.c sfmove.c sfnew.c sfnputc.c sfopen.c sfpeek.c \
	sfpool.c sfpopen.c sfprintf.c sfputd.c sfputl.c sfputr.c sfputu.c \
	sfpkrd.c sfread.c sfscanf.c sfseek.c sfset.c sfsetbuf.c sfdisc.c \
	sfstack.c sfstrtod.c sfsync.c sftable.c sftell.c sftmp.c \
	sfungetc.c sfvprintf.c sfvscanf.c sfwrite.c sfnotify.c sfsetfd.c \
	sfrd.c sfwr.c sfsk.c sfsize.c sfpurge.c sfpoll.c sfreserve.c \
	sfswap.c sfraise.c

SFIO_F=	_sfclrerr.c _sfeof.c _sferror.c _sffileno.c _sfgetc.c _sfgetl.c _sfgetu.c \
	_sfputc.c _sfslen.c _sfulen.c _sfstacked.c _sfvalue.c

STDIO_S=stdgets.c stdopen.c stdprintf.c stdscanf.c stdsprintf.c stdvbuf.c

STDIO_B=fclose.c fdopen.c fflush.c fgetc.c fgets.c filbuf.c flsbuf.c \
	fopen.c fprintf.c fputc.c fputs.c fread.c freopen.c fscanf.c fseek.c \
	ftell.c fwrite.c getc.c getchar.c gets.c getw.c pclose.c popen.c printf.c \
	putc.c putchar.c puts.c putw.c rewind.c scanf.c setbuf.c setbuffer.c \
	setlinebuf.c setvbuf.c sprintf.c sscanf.c stdextern.c stdstream.c \
	tmpfile.c ungetc.c vfprintf.c vfscanf.c vprintf.c vscanf.c vsprintf.c \
	vsscanf.c doprnt.c doscan.c fileno.c feof.c ferror.c clearerr.c \
	fgetpos.c fsetpos.c cleanup.c snprintf.c vsnprintf.c

sfio  $(VERSION) :LIBRARY:	$(SFIO) $(SFIO_F) $(STDIO_S)
stdio $(VERSION) :LIBRARY:	$(STDIO_B)

sfio_p.h : features/sfio_p
	iffe - run features/sfio_p > sfio_p.h
sfstdgen : sfstdio.c
	$(CC) -I. $(*) -o sfstdgen
sfstdhdr.h :
	Stdio_b/sfstdhdr.sh $(CC)
sfstdio.h :	sfstdgen
	$(*) > $(<)
	rm sfstdgen sfstdhdr.h 

$(INCLUDEDIR):INSTALLDIR:	sfio.h

$(INCLUDEDIR)/stdio.h : .DONTCARE
$(INCLUDEDIR):INSTALLDIR:	stdio.h
	test -f Stdio_b/sfstdhdr.h && cp $(*) $(<) || true

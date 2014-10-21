#include	"sftest.h"

static Sfio_t*	okclose;

#ifdef __STC_C
exceptf(Sfio_t*f, int type, Void_t* data, Sfdisc_t* disc)
#else
exceptf(f, type, data, disc)
Sfio_t*	f;
int		type;
Void_t*		data;
Sfdisc_t*	disc;
#endif
{
	if((f->mode&SF_LOCK) )
		terror("Stream should not be locked in exceptf\n");
	if(type == SF_CLOSE && f != okclose)
		return -1;
	else	return 0;
}

#if __STD_C
ssize_t readf(Sfio_t* f, Void_t* buf, size_t n, Sfdisc_t* disc)
#else
ssize_t readf(f, buf, n, disc)
Sfio_t*		f;
Void_t*		buf;
size_t		n;
Sfdisc_t*	disc;
#endif
{
	if(!(f->mode&SF_LOCK) )
		terror("Stream should be locked in readf\n");
	return 0;
}

#if __STD_C
ssize_t writef(Sfio_t* f, const Void_t* buf, size_t n, Sfdisc_t* disc)
#else
ssize_t writef(f, buf, n, disc)
Sfio_t*		f;
Void_t*		buf;
size_t		n;
Sfdisc_t*	disc;
#endif
{
	if(!(f->mode&SF_LOCK) )
		terror("Stream should be locked in writef\n");
	return 0;
}

Sfdisc_t	Disc = { readf, writef, NIL(Sfseek_f), exceptf };

main()
{
	Sfio_t	*f1, *f2, *f3, *f;
	char	*s, *s1, *s2, *s3, *s4, str[1024], *ss;
	int	n;

	if(!(f1 = sfopen(NIL(Sfio_t*),"xxx","w+")) )
		terror("Opening xxx1\n");
	if(!(f2 = sfopen(NIL(Sfio_t*),"xxx","w+")) )
		terror("Opening xxx2\n");
	okclose = f2;
	sfdisc(f1,&Disc);
	sfdisc(f2,&Disc);
	sfstack(f1,f2);
	if((n = sfgetc(f1)) >= 0 || !sfeof(f1))
		terror("There should be no data n=%d\n",n);
	if(sfstacked(f1))
		terror("There should be no stack\n");
	okclose = f1;
	if(sfclose(f1) < 0)
		terror("Can't close f1\n");
	system("rm xxx >/dev/null 2>&1");


	s1 = "1234567890";
	s2 = "abcdefghijklmnopqrstuvwxyz";
	s3 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	s4 = "!@#$%^&*()_-+={}[]~`':;?/><,|";

	if(!(f1 = sfopen((Sfio_t*)0,s1,"s")) ||
	   !(f2 = sfopen((Sfio_t*)0,s2,"s")) ||
	   !(f3 = sfopen((Sfio_t*)0,s3,"s")))
		terror("Opening strings\n");

	sfdisc(sfstdin,&Disc);
	sfclose(sfstdin);
	if(sffileno(sfstdin) != 0)
		terror("Bad fd for stdin\n");

	if(!(f = sfopen(NIL(Sfio_t*),"xxx","w+")) )
		terror("Opening xxx\n");
	if(sfwrite(f,"0123456789",10) != 10)
		terror("Write xxx\n");
	if(sfseek(f,(Sfoff_t)0,0) != 0)
		terror("Seek xxx\n");
	
	if(sfstack(sfstdin,f) != sfstdin)
		terror("Stacking on stdin2\n");
	if(sfopen(sfstdout,"/dev/null","w") != sfstdout)
		terror("Opening sfstdout\n");
	if(sfmove(sfstdin,sfstdout,SF_UNBOUND,-1) != 10 ||
	   !sfeof(sfstdin) || sferror(sfstdout))
		terror("Bad sfmove\n");

	system("rm xxx >/dev/null 2>&1");

	if(!(f = sftmp(0)))
		terror("Opening temp file\n");
	if(sfputr(f,s4,-1) != (ssize_t)strlen(s4))
		terror("Writing s4\n");
	sfseek(f,(Sfoff_t)0,0);

	if(sfstack(f,f3) != f)
		terror("Stacking s3\n");
	if(sfstack(f,f2) != f)
		terror("Stacking s2\n");
	if(sfstack(f,f1) != f)
		terror("Stacking s1\n");

	sfsprintf(str,sizeof(str),"%s%s%s%s",s1,s2,s3,s4);
	if((ss = sfgetr(f,'\n',1)) )
		terror("There shouldn't have been any new-line\n");
	else
	{	if(!(ss = sfgetr(f,'\n',-1)) )
			terror("Reading streams\n");
		ss[sfvalue(f)] = 0;
	}

	if(strcmp(ss,str) != 0)
		terror("Expect=%s\n",str);

	if(!(f1 = sfopen((Sfio_t*)0,s1,"s")) ||
	   !(f2 = sfopen((Sfio_t*)0,s2,"s")) ||
	   !(f3 = sfopen((Sfio_t*)0,s3,"s")))
		terror("Opening strings2\n");
	sfseek(f,(Sfoff_t)0,0);

	if(sfstack(f,f3) != f || sfstack(f,f2) != f || sfstack(f,f1) != f)
		terror("Stacking streams2\n");

	if(!(s = sfreserve(f,SF_UNBOUND,0)) || s != s1)
		terror("Sfpeek1\n");

	if(!(s = sfreserve(f,SF_UNBOUND,0)) || s != s2)
		terror("Sfpeek2\n");

	if(!(s = sfreserve(f,SF_UNBOUND,0)) || s != s3)
		terror("Sfpeek3\n");

	if(!(s = sfreserve(f,SF_UNBOUND,0)) || strncmp(s,s4,strlen(s4)) != 0)
		terror("Sfpeek4\n");

	return 0;
}

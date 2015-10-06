/*
 * Copyright (C) Caldera International Inc. 2001-2002.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code and documentation must retain the above
 * copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 * This product includes software developed or owned by Caldera
 * International, Inc.
 * 4. Neither the name of Caldera International, Inc. nor the names of other
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * USE OF THE SOFTWARE PROVIDED FOR UNDER THIS LICENSE BY CALDERA
 * INTERNATIONAL, INC. AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL CALDERA INTERNATIONAL, INC. BE LIABLE FOR ANY DIRECT,
 * INDIRECT INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*-
 * Copyright (c) 1991
 * The Regents of the University of California. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef lint
static const char copyright[] =
"@(#) Copyright (c) 1991 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)n1.c	4.13 (Berkeley) 4/18/91";
static char sccsid[] = "@(#)n9.c	4.3 (Berkeley) 4/18/91";
#else
static const char rcsid[] = "$ABSD: n1.c,v 1.3 2014/05/06 16:41:12 mickey Exp $";
#endif
#endif /* not lint */

#include "pathnames.h"
#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#include "tdef.h"
#include <setjmp.h>

jmp_buf sjbuf;

/*
n1.c

consume options, initialization, main loop,
input routines, escape function calling
*/

int inchar[LNSIZE], *pinchar = inchar;	/* XXX */
int stopmesg;
filep ipl[NSO];
long offl[NSO];
long ioff;
char *ttyp;
int ms[] = {31,28,31,30,31,30,31,31,30,31,30,31};
#ifndef NROFF
int acctf;
#endif

void init1(int);
void init2(void);
int setz(void);
int setfield(int);
void setvline(void);
void setov(void);
void setbra(void);
void setline(void);
int cnum(char *);
void catch(int);
void kcatch(int);
void fpecatch(int);

int
main(int argc, char *argv[])
{
	char *p, *q;
	int i, j;

	signal(SIGHUP,catch);
	if(signal(SIGINT,catch) == SIG_IGN){
		signal(SIGHUP,SIG_IGN);
		signal(SIGINT,SIG_IGN);
		signal(SIGQUIT,SIG_IGN);
	}
	signal(SIGFPE,fpecatch);
	signal(SIGPIPE,catch);
	signal(SIGTERM,kcatch);
	init1(argv[0][0]);
options:
	while(--argc > 0 && (++argv)[0][0]=='-')
		switch(argv[0][1]){

		case 0:
			goto start;
		case 'i':
			stdi++;
			continue;
		case 'q':
			quiet++;
/* TODO
			if(gtty(0, &ttys) >= 0)
				ttysave = ttys.sg_flags;
*/
			continue;
		case 'n':
			npn = cnum(&argv[0][2]);
			continue;
		case 'p':
			xflg = 0;
			cps = cnum(&argv[0][2]);
			continue;
		case 'S':
			stopmesg++;
			continue;
		case 's':
			if(!(stop = cnum(&argv[0][2])))stop++;
			continue;
		case 'r':
			vlist[findr(argv[0][2])] = cnum(&argv[0][3]);
			continue;
		case 'm':
			p = &nextf[nfi];
			q = &argv[0][2];
			while((*p++ = *q++) != 0);
			if (access(nextf, 4) < 0) {
				char local[NS];

				snprintf(local, NS, "%s%s",
				    _PATH_LOCAL_TMAC, &argv[0][2]);
				if (access(local, 4) == 0)
					strlcpy(nextf, local, NS);
			}
			mflg++;
			continue;
		case 'o':
			getpn(&argv[0][2]);
			continue;
#ifdef NROFF
		case 'h':
			hflg++;
			continue;
		case 'z':
			no_out++;
			continue;
		case 'e':
			eqflg++;
			continue;
		case 'T':
			p = &termtab[tti];
			q = &argv[0][2];
			if(!((*q) & 0177))
				continue;
			while((*p++ = *q++) != 0);
			dotT++;
			continue;
#else
		case 'z':
			no_out++;
		case 'a':
			ascii = 1;
			nofeed++;
		case 't':
			ptid = 1;
			continue;
		case 'w':
			waitf = 1;
			continue;
		case 'f':
			nofeed++;
			continue;
		case 'x':
			xflg = 0;
			continue;
		case 'b':
			if (open(ptname, O_WRONLY) < 0)
				prstr("Busy.\n");
			else
				prstr("Available.\n");
			done3(0);
		case 'g':
			stop = ptid = gflag = 1;
			dpn = 0;
			continue;
		case 'F':
			fontfile = &argv[0][2];
			continue;
#endif
		default:
			pto = cnum(&argv[0][1]);
			continue;
		}

	if(argv[0][0] == '+'){
		pfrom = cnum(&argv[0][1]);
		print = 0;
		if(argc > 0)
			goto options;
	}

start:
	argp = argv;
	rargc = argc;
	init2();
	setjmp(sjbuf);
loop:
	copyf = lgf = nb = nflush = nlflg = 0;
	if (ip && (rbf0(ip) == 0) && ejf && (frame->pframe <= ejl)){
		nflush++;
		trap = 0;
		eject((struct s *)0);
		goto loop;
	}
	i = getch();
	if (pendt)
		goto lt;
	if (lit && (frame <= litlev)) {
		lit--;
		goto lt;
	}
	if ((j = (i & CMASK)) == XPAR){
		copyf++;
		tflg++;
		for (; (i & CMASK) != '\n'; )
			pchar(i = getch());
		tflg = 0;
		copyf--;
		goto loop;
	}
	if ((j == cc) || (j == c2)) {
		if (j == c2)
			nb++;
		copyf++;
		while (((j = ((i = getch()) & CMASK)) == ' ') || (j == '\t'))
			;
		ch = i;
		copyf--;
		control(getrq(), 1);
		flushi();
		goto loop;
	}
lt:
	ch = i;
	text();
	goto loop;

	return 0;
}

void
catch(int a)
{
/*
	prstr("Interrupt\n");
*/
	done3(01);
}

void
fpecatch(int a)
{
	prstrfl("Floating Exception.\n");
	signal(SIGFPE, fpecatch);
}

void
kcatch(int a)
{
	signal(SIGTERM,SIG_IGN);
	done3(01);
}

#ifndef NROFF
void
acctg(void)
{
	static char *acct_file = _PATH_TRACCT;
	acctf = open(acct_file,1);
	setuid(getuid());
}
#endif

void
init1(int a)
{
	static char tempname[] = "/tmp/taXXXXXX";
	int i;

#ifndef NROFF
	acctg();	/*open troff actg file while mode 4755*/
#endif
	if ((ibf = mkstemp(tempname)) < 0) {
		prstr("Error creating tempfile");
		exit(1);
	}

	for (i = 256; --i; )
		trtab[i] = i;
	trtab[UNPAD] = ' ';
	mchbits();
	unlkp = tempname;
}

void
init2(void)
{
	extern int block;
	int i,j;

	ttyod = 2;
	if ((ttyp = ttyname(j = 0)) != NULL ||
	    (ttyp = ttyname(j = 1)) != NULL ||
	    (ttyp = ttyname(j = 2)) != NULL)
		;
	else
		ttyp = "notty";
	iflg = j;

	if ((!ptid) && (!waitf)){
		if ((ptid = open(ptname, O_WRONLY)) < 0){
			warnx("%s: Typesetter busy.", ptname);
			done3(-2);
		}
	}
	ptinit();
	/*
	 * XXX
	 * this is totally evil writing global data like this!
	 */
	for (i = NEV; i--; )
		write(ibf, (char *)&block, EVS * sizeof(int));
	olinep = oline;
	ibufp = eibuf = ibuf;
	v.hp = init = 0;
	pinchar = inchar;	/* XXX */
	ioff = 0;
	v.nl = -1;
	cvtime();
	frame = stk = (struct s *)setbrk(DELTA);
	dip = &d[0];
	nxf = frame + 1;
	nx = mflg;
}

void
cvtime(void)
{
	struct tm *tmp;
	time_t t;

	t = time(NULL);
	tmp = localtime(&t);
	v.dy = tmp->tm_mday;
	v.dw = tmp->tm_wday + 1;
	v.yr = tmp->tm_year;
	v.mo = tmp->tm_mon + 1;
}

int
cnum(char *a)
{
	int i;

	/*
	 * XXX
	 * this is beyound evil!!!
	 */
	ibufp = a;
	eibuf = (char *) MAXPTR;
	i = atoi0();
	ch = 0;
	return i;
}

void
prstrfl(char *s)
{
	flusho();
	prstr(s);
}

void
prstr(char *s)
{
	char *p;
	int i;

	for (p = s, i = 0; *s; i++)
		s++;
	write(ttyod, p, i);
}

int
control(int a, int b)
{
	int i,j;

	i = a;
	if ((i == 0) || ((j = findmn(i)) == -1))
		return(0);
	if (contab[j].rq & MMASK) {
		nxf->nargs = 0;
		if (b)
			collect();
		flushi();
		return (pushi(((filep)contab[j].x.mx) << BLKBITS));
	} else {
		if (!b)
			return(0);
		return ((*contab[j].x.f)(0));
	}
}

int
getrq(void)
{
	int i,j;

	if ((i = getach()) == 0 || (j = getach()) == 0)
		return (i);
	else
		return (PAIR(i,j));
}

int
getch(void)
{
	int i, j, k;

	level++;
g0:
	if (ch) {
		if (((i = ch) & CMASK) == '\n')
			nlflg++;
		ch = 0;
		level--;
		return (i);
	}

	if (nlflg) {
		level--;
		return('\n');
	}

	if ((k = (i = getch0()) & CMASK) != ESC){
		if(i & MOT)
			goto g2;
		if (k == FLSS) {
			raw++;
			copyf++;
			i = getch0();
			if (!fi)
				flss = i;
			copyf--;
			raw--;
			goto g0;
		}
		if (k == RPT) {
			setrpt();
			goto g0;
		}
		if (!copyf) {
			if ((k == 'f') && lg && !lgf) {
				i = getlg(i);
				goto g2;
			}
			if ((k == fc) || (k == tabch) || (k == ldrch)) {
				if ((i=setfield(k)) == 0)
					goto g0;
				else
					goto g2;
			}
			if (k == 010) {
				i = makem(-width(' ' | chbits));
				goto g2;
			}
		}
		goto g2;
	}
	k = (j = getch0()) & CMASK;
	if (j & MOT) {
		i = j;
		goto g2;
	}
/*
	if (k == tdelim) {
		i = TDELIM;
		tdelim = IMP;
		goto g2;
	}
*/
	switch(k) {
	case '\n':	/*concealed newline*/
		goto g0;
	case 'n':	/*number register*/
		setn();
		goto g0;
	case '*':	/*string indicator*/
		setstr();
		goto g0;
	case '$':	/*argument indicator*/
		seta();
		goto g0;
	case '{':	/*LEFT*/
		i = LEFT;
		goto gx;
	case '}':	/*RIGHT*/
		i = RIGHT;
		goto gx;
	case '"':	/*comment*/
		while(((i=getch0()) & CMASK ) != '\n');
		goto g2;
	case ESC:	/*double backslash*/
		i = eschar;
		goto gx;
	case 'e':	/*printable version of current eschar*/
		i = PRESC;
		goto gx;
	case ' ':	/*unpaddable space*/
		i = UNPAD;
		goto gx;
	case '|':	/*narrow space*/
		i = NARSP;
		goto gx;
	case '^':	/*half of narrow space*/
		i = HNSP;
		goto gx;
	case '\'':	/*\(aa*/
		i = 0222;
		goto gx;
	case '`':	/*\(ga*/
		i = 0223;
		goto gx;
	case '_':	/*\(ul*/
		i = 0224;
		goto gx;
	case '-':	/*current font minus*/
		i = 0210;
		goto gx;
	case '&':	/*filler*/
		i = FILLER;
		goto gx;
	case 'c':	/*to be continued*/
		i = CONT;
		goto gx;
	case ':':	/*lem's char*/
		i = COLON;
		goto gx;
	case '!':	/*transparent indicator*/
		i = XPAR;
		goto gx;
	case 't':	/*tab*/
		i = '\t';
		goto g2;
	case 'a':	/*leader (SOH)*/
		i = LEADER;
		goto g2;
	case '%':	/*ohc*/
		i = OHC;
		goto g2;
	case '.':	/*.*/
		i = '.';
	gx:
		i = (j & ~CMASK) | i;
		goto g2;
	}

	if (!copyf)
		switch(k) {
		case 'p':	/*spread*/
			spread++;
			goto g0;
		case '(':	/*special char name*/
			if ((i = setch()) == 0)
				goto g0;
			break;
		case 's':	/*size indicator*/
			setps();
			goto g0;
		case 'f':	/*font indicator*/
			setfont(0);
			goto g0;
		case 'w':	/*width function*/
			setwd();
			goto g0;
		case 'v':	/*vert mot*/
			if ((i = vmot()))
				break;
			goto g0;
		case 'h':	/*horiz mot*/
			if ((i = hmot()))
				break;
			goto g0;
		case 'z':	/*zero with char*/
			i = setz();
			break;
		case 'l':	/*hor line*/
			setline();
			goto g0;
		case 'L':	/*vert line*/
			setvline();
			goto g0;
		case 'b':	/*bracket*/
			setbra();
			goto g0;
		case 'o':	/*overstrike*/
			setov();
			goto g0;
		case 'k':	/*mark hor place*/
			if ((i = findr(getsn())) == -1)
				goto g0;
			vlist[i] = v.hp = sumhp();	/* XXX */
			goto g0;
		case 'j':	/*mark output hor place*/
			if (!(i = getach()))
				goto g0;
			i = (i << BYTE) | JREG;
			break;
		case '0':	/*number space*/
			i = makem(width('0' | chbits));
			break;
		case 'x':	/*extra line space*/
			if ((i = xlss()))
				break;
			goto g0;
		case 'u':	/*half em up*/
		case 'r':	/*full em up*/
		case 'd':	/*half em down*/
			i = sethl(k);
			break;
		default:
			i = j;
		}
	else {
		ch0 = j;
		i = eschar;
	}
g2:
	if ((i & CMASK) == '\n') {
		nlflg++;
		v.hp = 0;
		pinchar = inchar;	/* XXX */
		if (ip == 0)
			v.cd++;
	}
	if (!--level) {
		/* j = width(i); */
		/* v.hp += j; */
		/* cwidth = j; */
		if (pinchar >= inchar + LNSIZE) {	/* XXX */
			inchar[0] = makem(sumhp());
			pinchar = &inchar[1];
		}
		*pinchar++ = i;	/* XXX */
	}
	return(i);
}

int
sumhp(void)	/* XXX - add up widths in inchar array */
{
	int n, *p;

	n = 0;
	for (p = inchar; p < pinchar; p++)
		n += width(*p);
	return (n);
}

const char ifilt[32] = {0,001,002,003,0,005,006,007,010,011,012};
int
getch0(void)
{
	int i, j;

	if (ch0) {
		i=ch0;
		ch0=0;
		return(i);
	}
	if (nchar) {
		nchar--;
		return(rchar);
	}

again:
	if (cp) {
		if ((i = *cp++) == 0) {
			cp = 0;
			goto again;
		}
	} else if (ap) {
		if((i = *ap++) == 0){
			ap = 0;
			goto again;
		}
	} else if (ip) {
		if (ip == -1)
			i = rdtty();
		else
			i = rbf();
	} else {
		if (donef)
			done(0);
		if (nx || ((ibufp >= eibuf) && (eibuf != (char *) MAXPTR))) {
			if (nfo)
				goto g1;
		g0:
			if (nextfile())  {
				if (ip)
					goto again;
				if (ibufp < eibuf)
					goto g2;
			}
		g1:
			nx = 0;
			if ((j=read(ifile,ibuf,IBUFSZ)) <= 0)
				goto g0;
			ibufp = ibuf;
			eibuf = ibuf + j;
			if (ip)
				goto again;
		}
	g2:
		i = *ibufp++ & 0177;
		ioff++;
		if (i >= 040)
			goto g4;
		else
			i = ifilt[i];
	}
	if (raw)
		return(i);
	if ((j = i & CMASK) == IMP)
		goto again;
	if ((i == 0) && !init)
		goto again;
g4:
	if ((copyf == 0) && ((i & ~BMASK) == 0) && ((i & CMASK) < 0370)) {
#ifndef NROFF
		if (spbits && (i>31) && ((codetab[i-32] & 0200)))
			i |= spbits;
		else
#endif
			i |= chbits;
	}
	if ((i & CMASK) == eschar)
		i = (i & ~CMASK) | ESC;
	return (i);
}

int
nextfile(void)
{
	char *p;

n0:
	if (ifile)
		close(ifile);
	if (nx) {
		p = nextf;
		if(*p != 0)
			goto n1;
	}
	if (ifi > 0) {
		if (popf())
			goto n0; /*popf error*/
		return (1);	/* popf ok */
	}
	if (rargc-- <= 0)
		goto n2;
	p = (argp++)[0];
n1:
	if ((p[0] == '-') && (p[1] == 0)) {
		ifile = 0;
	} else if ((ifile = open(p, O_RDONLY)) < 0) {
		prstr("Cannot open ");
		prstr(p);
		prstr("\n");
		nfo -= mflg;
		done(02);
	}
	nfo++;
	v.cd = 0;
	ioff = 0;
	return(0);
n2:
	if ((nfo -= mflg) && !stdi)
		done(0);
	nfo++;
	v.cd = ifile =  stdi = mflg = 0;
	ioff = 0;
	return (0);
}

int
popf(void)
{
	char *p, *q;
	int i;

	ioff = offl[--ifi];
	ip = ipl[ifi];
	if ((ifile = ifl[ifi]) == 0) {
		p = xbuf;
		q = ibuf;
		ibufp = xbufp;
		eibuf = xeibuf;
		while (q < eibuf)
			*q++ = *p++;
		return(0);
	}
	if (lseek(ifile, (ioff & ~(IBUFSZ - 1)), SEEK_SET) < 0 ||
	    (i = read(ifile, ibuf, IBUFSZ)) < 0)
		return(1);
	eibuf = ibuf + i;
	ibufp = ibuf;
	if (ttyname(ifile) == NULL)
		if ((ibufp = ibuf + (int)(ioff & (IBUFSZ-1))) >= eibuf)
			return(1);
	return(0);
}

void
flushi(void)
{
	if (nflush)
		return;
	ch = 0;
	if ((ch0 & CMASK) == '\n')
		nlflg++;
	ch0 = 0;
	copyf++;
	while(!nlflg) {
		if (donef && (frame == stk))
			break;
		getch();
	}
	copyf--;
	v.hp = 0;
	pinchar = inchar;	/* XXX */
}

int
getach(void)
{
	int i;

	lgf++;
	if (((i = getch()) & MOT) || ((i & CMASK) == ' ') ||
	    ((i & CMASK) == '\n') || (i & 0200)) {
		ch = i;
		i = 0;
	}
	lgf--;
	return (i & 0177);
}

int
casenx(int a)
{
	lgf++;
	skip();
	getname();
	nx++;
	nextfile();
	nlflg++;
	ip = 0;
	ap = 0;
	nchar = pendt = 0;
	frame = stk;
	nxf = frame + 1;
	
	return 0;
}

int
getname(void)
{
	int i, j, k;

	lgf++;
	for (k = 0; k < (NS - 1); k++) {
		if (((j = (i = getch()) & CMASK) <= ' ') || (j > 0176))
			break;
		nextf[k] = j;
	}
	nextf[k] = 0;
	ch = i;
	lgf--;
	return (nextf[0]);
}

int
caseso(int a)
{
	char *p, *q;
	int i = 0;

	lgf++;
	nextf[0] = 0;
	if (skip() || !getname() || ((i = open(nextf, O_RDONLY)) < 0) ||
	    ifi >= NSO) {
		prstr("can't open file ");
		prstr(nextf);
		prstr("\n");
		done(02);
	}
	flushi();
	ifl[ifi] = ifile;
	ifile = i;
	offl[ifi] = ioff;
	ioff = 0;
	ipl[ifi] = ip;
	ip = 0;
	nx++;
	nflush++;
	if (!ifl[ifi++]) {
		p = ibuf;
		q = xbuf;
		xbufp = ibufp;
		xeibuf = eibuf;
		while (p < eibuf)
			*q++ = *p++;
	}
	return 0;
}

int
casecf(int a)
{	/* copy file without change */
	char buf[OBUFSZ];
	int fd, i, n;

	flusho();
	lgf++;
	nextf[0] = 0;
	fd = -1;
	if (skip() || !getname() || (fd = open(nextf, 0)) < 0 || ifi >= NSO) {
		prstr("can't open file ");
		prstr(nextf);
		prstr("\n");
		done(02);
	}
	while ((n = read(fd, buf, OBUFSZ)) > 0)
		for (i = 0; i < n; i++)
			oput(buf[i]);
	flusho();
	close(fd);
	return 0;
}

void
getpn(char *a)
{
	int i, neg;

	if ((*a & 0177) == 0)
		return;
	neg = 0;
	ibufp = a;
	eibuf = (char *)MAXPTR;
	noscale++;
	while ((i = getch() & CMASK) != 0)
		switch(i) {
		case '+':
		case ',':
			continue;
		case '-':
			neg = MOT;
			goto d2;
		default:
			ch = i;
		d2:
			i = atoi1();
			if (nonumb)
				goto fini;
			else {
				*pnp++ = i | neg;
				neg = 0;
				if (pnp >= &pnlist[NPN-2]) {
					prstr("Too many page numbers\n");
					done3(-3);
				}
			}
		}
fini:
	if (neg)
		*pnp++ = -2;
	*pnp = -1;
	ch = noscale = print = 0;
	pnp = pnlist;
	if (*pnp != -1)
		chkpn();
}

void
setrpt(void)
{
	int i, j;

	raw++;
	copyf++;
	i = getch0();
	copyf--;
	raw--;
	if ((i < 0) || (((j = getch0()) & CMASK) == RPT))
		return;
	rchar = j;
	nchar = i & BMASK;
}

int
setz(void)
{
	int i;

	if (!((i = getch()) & MOT))
		i |= ZBIT;
	return (i);
}

void
setline(void)
{
	int *i, length, c, w, cnt, delim, rem, temp;

	if ((delim = getch()) & MOT)
		return;
	else
		delim &= CMASK;
	vflag = 0;
	dfact = EM;
	length = quant(atoi0(),HOR);
	dfact = 1;
	if (!length) {
		eat(delim);
		return;
	}
s0:
	if (((c = getch()) & CMASK) == delim) {
		ch = c;
		c = 0204 | chbits;
	} else if ((c & CMASK) == FILLER)
		goto s0;
	w = width(c);
	i = cbuf;
	if (length < 0) {
		*i++ = makem(length);
		length = -length;
	}
	if (!(cnt = length / w)) {
		*i++ = makem(-(temp = ((w-length)/2)));
		*i++ = c;
		*i++ = makem(-(w - length - temp));
		goto s1;
	}
	if ((rem = length%w))
		switch(c & CMASK) {
		case 0204: /*rule*/
		case 0224: /*underrule*/
		case 0276: /*root en*/
			*i++ = c | ZBIT;
		default:
			*i++ = makem(rem);
		}

	if (cnt) {
		*i++ = RPT;
		*i++ = cnt;
		*i++ = c;
	}
s1:
	*i++ = 0;
	eat(delim);
	cp = cbuf;
}

void
setov(void)
{
	int i, j, k, *p, delim, o[NOV+1], w[NOV+1];

	if ((delim = getch()) & MOT)
		return;
	else
		delim &= CMASK;
	for (k = 0; k < NOV && ((j = (i = getch()) & CMASK) != delim) &&
	    j != '\n'; k++) {
		o[k] = i;
		w[k] = width(i);
	}
	o[k] = w[k] = 0;
	if (o[0])
		for (j = 1; j; ) {
			j = 0;
			for (k=1; o[k] ; k++) {
				if (w[k-1] < w[k]){
					j++;
					i = w[k];
					w[k] = w[k-1];
					w[k-1] = i;
					i = o[k];
					o[k] = o[k-1];
					o[k-1] = i;
				}
			}
		}
	else
		return;
	p = cbuf;
	for (k = 0; o[k]; k++){
		*p++ = o[k];
		*p++ = makem(-((w[k] + w[k + 1]) / 2));
	}
	*p++ = makem(w[0]/2);
	*p = 0;
	cp = cbuf;
}

void
setbra(void)
{
	int i, *j, k, cnt, delim, dwn;

	if ((delim = getch()) & MOT)
		return;
	else
		delim &= CMASK;
	j = cbuf + 1;
	cnt = 0;
#ifdef NROFF
	dwn = (2*t.Halfline) | MOT | VMOT;
#endif
#ifndef NROFF
	dwn = EM | MOT | VMOT;
#endif
	while (((k = (i = getch()) & CMASK) != delim) && (k != '\n') &&
	    j <= (cbuf + NC - 4)){
		*j++ = i | ZBIT;
		*j++ = dwn;
		cnt++;
	}
	if (--cnt < 0)
		return;
	else if (!cnt) {
		ch = *(j-2);
		return;
	}
	*j = 0;
#ifdef NROFF
	*--j = *cbuf = (cnt*t.Halfline) | MOT | NMOT | VMOT;
#endif
#ifndef NROFF
	*--j = *cbuf = (cnt*EM)/2 | MOT | NMOT | VMOT;
#endif
	*--j &= ~ZBIT;
	cp = cbuf;
}

void
setvline(void)
{
	int i, c, *k, cnt, neg, rem, ver, delim;

	if ((delim = getch()) & MOT)
		return;
	else
		delim &= CMASK;
	dfact = lss;
	vflag++;
	i = quant(atoi0(),VERT);
	dfact = 1;
	if (!i) {
		eat(delim);
		vflag = 0;
		return;
	}
	if (((c = getch()) & CMASK) == delim) {
		c = 0337 | chbits;	/*default box rule*/
	} else
		getch();
	c |= ZBIT;
	neg = 0;
	if (i < 0) {
		i = -i;
		neg = NMOT;
	}
#ifdef NROFF
	ver = 2*t.Halfline;
#endif
#ifndef NROFF
	ver = EM;
#endif
	cnt = i/ver;
	rem = makem(i%ver) | neg;
	ver = makem(ver) | neg;
	k = cbuf;
	if (!neg)
		*k++ = ver;
	if (rem & ~MOTV) {
		*k++ = c;
		*k++ = rem;
	}
	while ((k < (cbuf+NC-3)) && cnt--) {
		*k++ = c;
		*k++ = ver;
	}
	*(k-2) &= ~ZBIT;
	if (!neg)
		k--;
	*k = 0;
	cp = cbuf;
	vflag = 0;
}

int
setfield(int x)
{
	static int fbuf[FBUFSZ];
	int i, j, *fp, length, ws, npad, temp, type;
	int **pp, *padptr[NPP];
	int savfc, savtc, savlc;

	if (x == tabch)
		rchar = tabc | chbits;
	else if (x == ldrch)
		rchar = dotc | chbits;
	temp = npad = ws = 0;
	savfc = fc; savtc = tabch; savlc = ldrch;
	tabch = ldrch = fc = IMP;
	for (j = 0; ; j++) {
		if ((tabtab[j] & TMASK) == 0) {
			if (x == savfc)
				prstr("Zero field width.\n");
			j = 0;
			goto rtn;
		}
		v.hp = sumhp();	/* XXX */
		if ((length = ((tabtab[j] & TMASK) - v.hp)) > 0 )
			break;
	}
	type = tabtab[j] & (~TMASK);
	fp = fbuf;
	pp = padptr;
	if (x == savfc) {
		while(1) {
			if (((j = (i = getch()) & CMASK)) == padc) {
				npad++;
				*pp++ = fp;
				if(pp > (padptr + NPP - 1))
					break;
				goto s1;
			} else if(j == savfc)
				break;
			else if(j == '\n') {
				temp = j;
				nlflg = 0;
				break;
			}
			ws += width(i);
		s1:
			*fp++ = i;
			if (fp > (fbuf + FBUFSZ -3))
				break;
		}
		if(!npad) {
			npad++;
			*pp++ = fp;
			*fp++ = 0;
		}
		*fp++ = temp;
		*fp++ = 0;
		temp = i = (j = length-ws)/npad;
		i = (i/HOR)*HOR;
		if((j -= i*npad)
			<0)j = -j;
		i = makem(i);
		if (temp <0)
			i |= NMOT;
		for (; npad > 0; npad--) {
			*(*--pp) = i;
			if(j) {
				j -= HOR;
				(*(*pp)) += HOR;
			}
		}
		cp = fbuf;
		j = 0;
	} else if (type == 0) {
	/*plain tab or leader*/
		if ((j = width(rchar)) == 0)
			nchar = 0;
		else {
			nchar = length /j;
			length %= j;
		}
		j = length? length | MOT : getch0();
	} else {
	/*center tab*/
	/*right tab*/
		while (((j = (i = getch()) & CMASK) != savtc) &&
		    j != '\n' && j != savlc) {
			ws += width(i);
			*fp++ = i;
			if(fp > (fbuf +FBUFSZ - 3))
				break;
		}
		*fp++ = i;
		*fp++ = 0;
		if (type == RTAB)
			length -= ws;
		else
			length -= ws/2; /*CTAB*/
		if (((j = width(rchar)) == 0) || (length <= 0))
			nchar = 0;
		else {
			nchar = length/j;
			length %= j;
		}
		length = (length/HOR)*HOR;
		j = makem(length);
		cp = fbuf;
		nlflg = 0;
	}
rtn:
	fc = savfc; tabch = savtc; ldrch = savlc;
	return(j);
}
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
#if 0
static char sccsid[] = "@(#)n5.c	4.4 (Berkeley) 5/2/91";
#else
static const char rcsid[] = "$ABSD: n5.c,v 1.1 2011/01/05 23:31:20 mickey Exp $";
#endif
#endif /* not lint */

#include "tdef.h"
#include <unistd.h>

/*
troff5.c

misc processing requests
*/

int iflist[NIF];
int ifx;

void getnm(int *, int);
int cmpstr(int);
int eatblk(int, int);

int
casead(int a)
{
	int i;

	ad = 1;
	/*leave admod alone*/
	if(skip())
		return -1;
	switch((i = getch() & CMASK)) {
	case 'r':	/*right adj, left ragged*/
		admod = 2;
		break;
	case 'l':	/*left adj, right ragged*/
		admod = ad = 0;	/*same as casena*/
		break;
	case 'c':	/*centered adj*/
		admod = 1;
		break;
	case 'b': case 'n':
		admod = 0;
		break;
	case '0': case '2': case '4':
		ad = 0;
	case '1': case '3': case '5':
		admod = (i - '0')/2;
	}
}

int
casena(int a)
{
	ad = 0;
}

int
casefi(int a)
{
	tbreak(0);
	fi++;
	pendnf = 0;
	lnsize = LNSIZE;
}

int
casenf(int a)
{
	tbreak(0);
	fi = 0;
/* can't do while oline is only LNSIZE
	lnsize = LNSIZE + WDSIZE;
*/
}

int
casers(int a)
{
	dip->nls = 0;
}

int
casens(int a)
{
	dip->nls++;
}

int
chget(int c)
{
	int i = 0;

	if (skip() || ((i = getch()) & MOT) || (i & CMASK) == ' ' ||
	    (i & CMASK) == '\n') {
		ch = i;
		return(c);
	} else
		return(i & BMASK);
}

int
casecc(int a)
{
	cc = chget('.');
}

int
casec2(int a)
{
	c2 = chget('\'');
}

int
casehc(int a)
{
	ohc = chget(OHC);
}

int
casetc(int a)
{
	tabc = chget(0);
}

int
caselc(int a)
{
	dotc = chget(0);
}

int
casehy(int a)
{
	int i;

	hyf = 1;
	if (skip())
		return -1;
	noscale++;
	i = atoi0();
	noscale = 0;
	if (nonumb)
		return -1;
	hyf = max(i,0);
}

int
casenh(int a)
{
	hyf = 0;
}

int
max(int aa, int bb)
{
	if (aa > bb)
		return(aa);
	else
		return (bb);
}

int
casece(int a)
{
	int i;

	noscale++;
	skip();
	i = max(atoi0(),0);
	if (nonumb)
		i = 1;
	tbreak(0);
	ce = i;
	noscale = 0;
}

int
casein(int a)
{
	int i;

	i = skip()? in1 : max(hnumb(&in), 0);
	tbreak(0);
	in1 = in;
	in = i;
	if (!nc) {
		un = in;
		setnel();
	}
}

int
casell(int a)
{
	int i;

	i = skip()? ll1 : max(hnumb(&ll), INCH / 10);
	ll1 = ll;
	ll = i;
	setnel();
}

int
caselt(int a)
{
	int i;

	i = skip()? lt1 : max(hnumb(&lt),0);
	lt1 = lt;
	lt = i;
}

int
caseti(int a)
{
	int i;

	if (skip())
		return -1;
	i = max(hnumb(&in),0);
	tbreak(0);
	un1 = i;
	setnel();
}

int
casels(int a)
{
	int i;

	noscale++;
	i = skip()? ls1 : max(inumb(&ls),1);
	ls1 = ls;
	ls = i;
	noscale = 0;
}

int
casepo(int a)
{
	int i;

	i = skip()? po1 : max(hnumb(&po),0);
	po1 = po;
	po = i;
#ifndef NROFF
	if(!ascii)esc += po - po1;
#endif
}

int
casepl(int a)
{
	int i;

	skip();
	if ((i = vnumb(&pl)) == 0)
		pl = 11 * INCH; /*11in*/
	else
		pl = i;
	if (v.nl > pl)
		v.nl = pl;
}

int
casewh(int a)
{
	int i, j, k;

	lgf++;
	skip();
	i = vnumb((int *)0);
	if (nonumb)
		return -1;
	skip();
	j = getrq();
	if ((k = findn(i)) != NTRAP){
		mlist[k] = j;
		return -1;
	}
	for (k = 0; k < NTRAP; k++)
		if(mlist[k] == 0)
			break;
	if (k == NTRAP){
		prstrfl("Cannot plant trap.\n");
		return -1;
	}
	mlist[k] = j;
	nlistx[k] = i;
}

int
casech(int a)
{
	int i, j, k;

	lgf++;
	skip();
	if (!(j = getrq()))
		return -1;
	else
		for (k = 0; k < NTRAP; k++)
			if (mlist[k] == j)
				break;
	if (k == NTRAP)
		return -1;
	skip();
	i = vnumb((int *)0);
	if (nonumb)
		mlist[k] = 0;
	nlistx[k] = i;
}

int
findn(int i)
{
	int k;

	for (k = 0; k < NTRAP; k++)
		if ((nlistx[k] == i) && (mlist[k] != 0))
			break;
	return(k);
}

int
casepn(int a)
{
	int i;

	skip();
	noscale++;
	i = max(inumb(&v.pn),0);
	noscale = 0;
	if(!nonumb) {
		npn = i;
		npnflg++;
	}
}

int
casebp(int a)
{
	struct s *savframe;
	int i;

	if (dip != d)
		return -1;
	savframe = frame;
	skip();
	if ((i = inumb(&v.pn)) < 0)
		i = 0;
	tbreak(0);
	if (!nonumb) {
		npn = i;
		npnflg++;
	} else if (dip->nls)
		return -1;
	eject(savframe);
}

int
casetm(int x)
{
	char tmbuf[NTM];
	int i;

	lgf++;
	copyf++;
	if (skip() && x)
		prstrfl("User Abort.");
	for (i = 0; i < NTM - 2;)
		if ((tmbuf[i++] = getch()) == '\n')
			break;
	if (i == NTM - 2)
		tmbuf[i++] = '\n';
	tmbuf[i] = 0;
	prstrfl(tmbuf);
	copyf--;
}

int
casesp(int a)
{
	int i, j, savlss;

	tbreak(0);
	if (dip->nls || trap)
		return -1;
	i = findt1();
	if (!a) {
		skip();
		j = vnumb((int *)0);
		if (nonumb)
			j = lss;
	} else
		j = a;
	if (j == 0)
		return -1;
	if (i < j)
		j = i;
	savlss = lss;
	if (dip != d)
		i = dip->dnl;
	else
		i = v.nl;
	if ((i + j) < 0)
		j = -i;
	lss = j;
	newline(0);
	lss = savlss;
	return 0;
}

int
casert(int arr)
{
	int a, *p;

	skip();
	if (dip != d)
		p = &dip->dnl;
	else
		p = &v.nl;
	a = vnumb(p);
	if (nonumb)
		a = dip->mkline;
	if ((a < 0) || (a >= *p))
		return -1;
	nb++;
	casesp(a - *p);
}

int
caseem(int a)
{
	lgf++;
	skip();
	em = getrq();
}

int
casefl(int a)
{
	tbreak(0);
	flusho();
}

int
caseev(int a)
{
	int nxev;

	if (skip()) {
e0:
		if (evi == 0)
			return -1;
		nxev =  evlist[--evi];
		goto e1;
	}
	noscale++;
	nxev = atoi0();
	noscale = 0;
	if (nonumb)
		goto e0;
	flushi();
	if ((nxev >= NEV) || (nxev < 0) || (evi >= EVLSZ)) {
		prstrfl("Cannot do ev.\n");
		if (error)
			done2(040);
		else
			edone(040);
		return -1;
	}

	evlist[evi++] = ev;
e1:
	if (ev == nxev)
		return -1;
	lseek(ibf, (long)(ev*EVS*sizeof(int)), 0);
	write(ibf,(char *)&block, EVS*sizeof(int));
	lseek(ibf, (long)(nxev*EVS*sizeof(int)), 0);
	read(ibf,(char *)&block, EVS*sizeof(int));
	ev = nxev;
}

int
caseel(int a)
{
	if (--ifx < 0) {
		ifx = 0;
		iflist[0] = 0;
	}
	caseif(2);
}

int
caseie(int a)
{
	if (ifx >= NIF) {
		prstr("if-else overflow.\n");
		ifx = 0;
		edone(040);
	}
	caseif(1);
	ifx++;
}

int
caseif(int x)
{
	int i, notflag, true;

	if (x == 2) {
		notflag = 0;
		true = iflist[ifx];
		goto i1;
	}
	true = 0;
	skip();
	if(((i = getch()) & CMASK) == '!') {
		notflag = 1;
	} else {
		notflag = 0;
		ch = i;
	}
	i = atoi0();
	if (!nonumb) {
		if(i > 0)
			true++;
		goto i1;
	}
	switch((i = getch()) & CMASK) {
	case 'e':
		if(!(v.pn & 01))true++;
		break;
	case 'o':
		if(v.pn & 01)true++;
		break;
#ifdef NROFF
	case 'n':
		true++;
	case 't':
#endif
#ifndef NROFF
	case 't':
		true++;
	case 'n':
#endif
	case ' ':
		break;
	default:
		true = cmpstr(i);
	}
i1:
	true ^= notflag;
	if(x == 1)
		iflist[ifx] = !true;
	if (true) {
	i2:
		do {
			v.hp = 0;
			pinchar = inchar;	/* XXX */
		}
		while (((i = getch()) & CMASK) == ' ');
		if ((i & CMASK) == LEFT)
			goto i2;
		ch = i;
		nflush++;
	} else {
		raw2++;
		copyf++;
		if (eat(LEFT) == LEFT)
			while (eatblk(RIGHT,LEFT) != RIGHT)
				nlflg = 0;
		copyf--;
		raw2--;
	}
}

int
eatblk(int right, int left)
{
	int i;

e0:
	while (((i = getch() & CMASK) != right) && (i != left) && (i != '\n'))
		;
	if(i == left) {
		while((i=eatblk(right,left)) != right)nlflg = 0;
		goto e0;
	}
	return(i);
}

int
eat(int c)
{
	int i;

	while (((i = getch() & CMASK) != c) && (i != '\n'))
		;
	return(i);
}

int
cmpstr(int delim)
{
	filep p, begin;
	int savapts, savapts1, savfont, savfont1, savpts, savpts1;
	int cnt, i, j, k;

	if(delim & MOT)
		return(0);
	delim &= CMASK;
	if(dip != d)
		wbfl();
	if((offset = begin = alloc()) == (filep)0)
		return(0);
	cnt = 0;
	v.hp = 0;
	pinchar = inchar;	/* XXX */
	savapts = apts;
	savapts1 = apts1;
	savfont = font;
	savfont1 = font1;
	savpts = pts;
	savpts1 = pts1;
	while (((j = (i=getch()) & CMASK) != delim) && (j != '\n')) {
		wbf(i);
		cnt++;
	}
	wbt(0);
	k = !cnt;
	if (nlflg)
		goto rtn;
	p = begin;
	apts = savapts;
	apts1 = savapts1;
	font = savfont;
	font1 = savfont1;
	pts = savpts;
	pts1 = savpts1;
	mchbits();
	v.hp = 0;
	pinchar = inchar;	/* XXX */
	while (((j = (i=getch()) & CMASK) != delim) && (j != '\n')) {
		if (rbf0(p) != i) {
			eat(delim);
			k = 0;
			break;
		}
		p = incoff(p);
		k = !(--cnt);
	}
rtn:
	apts = savapts;
	apts1 = savapts1;
	font = savfont;
	font1 = savfont1;
	pts = savpts;
	pts1 = savpts1;
	mchbits();
	offset = dip->op;
	ffree(begin);
	return(k);
}

int
caserd(int a)
{
	lgf++;
	skip();
	getname();
	if(!iflg){
		if(quiet) {
/* TODO
			ttys.sg_flags &= ~ECHO;
			stty(0, &ttys);
*/
			prstrfl(""); /*bell*/
		} else {
			if(nextf[0]){
				prstr(nextf);
				prstr(":");
			} else
				prstr(""); /*bell*/
		}
	}
	collect();
	tty++;
	pushi((filep)-1);
}

int
rdtty(void)
{
	char onechar;

	onechar = 0;
	if (read(0, &onechar, 1) == 1) {
		if(onechar == '\n')
			tty++;
		else
			tty = 1;
		if(tty != 3)
			return(onechar);
	}
	popi();
	tty = 0;
	if (quiet) {
/* TODO
		ttys.sg_flags |= ECHO;
		stty(0, &ttys);
*/
	}
	return(0);
}

int
caseec(int a)
{
	eschar = chget('\\');
}

int
caseeo(int a)
{
	eschar = 0;
}

int
caseli(int a)
{

	skip();
	lit = max(inumb((int *)0),1);
	litlev = frame;
	if((dip == d) && (v.nl == -1))
	newline(1);
}

int
caseta(int a)
{
	int i;

	tabtab[0] = nonumb = 0;
	for(i=0; ((i < (NTAB-1)) && !nonumb); i++){
		if(skip())break;
		tabtab[i] = tabtab[max(i-1,0)] & TMASK;
		tabtab[i] = max(hnumb(&tabtab[i]),0) & TMASK;
		if (!nonumb)
			switch(ch & CMASK){
			case 'C':
				tabtab[i] |= CTAB;
				break;
			case 'R':
				tabtab[i] |= RTAB;
				break;
			default: /*includes L*/
				break;
			}
		nonumb = ch = 0;
	}
	tabtab[i] = 0;
}

int
casene(int a)
{
	int i, j;

	skip();
	i = vnumb((int *)0);
	if(nonumb)
		i = lss;
	if (i > (j = findt1())) {
		i = lss;
		lss = j;
		dip->nls = 0;
		newline(0);
		lss = i;
	}
}

int
casetr(int a)
{
	int i, j;

	lgf++;
	skip();
	while((i = getch() & CMASK) != '\n'){
		if((i & MOT) || ((j = getch()) & MOT))
			return 1;
		if((j &= CMASK) == '\n')
			j = ' ';
		trtab[i] = j;
	}
}

int
casecu(int a)
{
	cu++;
	caseul(0);
}

int
caseul(int a)
{
	int i;

	noscale++;
	if (skip())
		i = 1;
	else
		i = atoi0();
	if (ul && (i == 0)){
		font = sfont;
		ul = cu = 0;
	}
	if(i){
		if(!ul){
			sfont = font;
			font = ulfont;
		}
		ul = i;
	}
	noscale = 0;
	mchbits();
}

int
caseuf(int a)
{
	int i, j;

	if (skip() || !(i = getrq()) || i == 'S' ||
	    (j = find(i, fontlab))  == -1)
		ulfont = 1; /*default position 2*/
	else
		ulfont = j;
#ifdef NROFF
	if (ulfont == 0)
		ulfont = 1;
#endif
	ulbit = ulfont<<9;
}

int
caseit(int a)
{
	int i;

	lgf++;
	it = itmac = 0;
	noscale++;
	skip();
	i = atoi0();
	skip();
	if (!nonumb && (itmac = getrq()))
		it = i;
	noscale = 0;
}

int
casemc(int a)
{
	int i;

	if (icf > 1)
		ic = 0;
	icf = 0;
	if (skip())
		return -1;
	ic = getch();
	icf = 1;
	skip();
	i = max(hnumb((int *)0),0);
	if (!nonumb)
		ics = i;
}

int
casemk(int a)
{
	int i, j;

	if(dip != d)
		j = dip->dnl;
	else
		j = v.nl;

	if(skip()){
		dip->mkline = j;
		return -1;
	}
	if ((i = getrq()) == 0)
		return -1;
	vlist[findr(i)] = j;
}

int
casesv(int a)
{
	int i;

	skip();
	if ((i = vnumb((int *)0)) < 0)
		return -1;
	if(nonumb)i = 1;
	sv += i;
	caseos(0);
}

int
caseos(int a)
{
	int savlss;

	if(sv <= findt1()){
		savlss = lss;
		lss = sv;
		newline(0);
		lss = savlss;
		sv = 0;
	}
}

int
casenm(int a)
{
	int i;

	lnmod = nn = 0;
	if (skip())
		return -1;
	lnmod++;
	noscale++;
	i = inumb(&v.ln);
	if(!nonumb)
		v.ln = max(i,0);
	getnm(&ndf,1);
	getnm(&nms,0);
	getnm(&ni,0);
	noscale = 0;
	nmbits = chbits;
}

void
getnm(int *p, int min)
{
	int i;

	eat(' ');
	if(skip())
		return;
	i = atoi0();
	if(nonumb)
		return;
	*p = max(i,min);
}

int
casenn(int a)
{
	noscale++;
	skip();
	nn = max(atoi0(),1);
	noscale = 0;
}

int
caseab(int a)
{
	casetm(1);
	done2(0);
}

int
casefc(int a)
{
	int i;

	fc = IMP;
	padc = ' ';
	if (skip() || ((i = getch()) & MOT) || ((i &= CMASK) == '\n'))
		return -1;
	fc = i;
	if (skip() || (ch & MOT) || ((ch &= CMASK) == fc))
		return -1;
	padc = ch;
}

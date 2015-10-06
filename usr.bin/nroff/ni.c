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
static char sccsid[] = "@(#)ni.c	4.5 (Berkeley) 4/18/91";
#else
static const char rcsid[] = "$ABSD: ni.c,v 1.1 2011/01/05 23:31:20 mickey Exp $";
#endif
#endif /* not lint */

#include "tdef.h"
#include "pathnames.h"

char obuf[OBUFSZ];
char *obufp = obuf;
int r[NN] = {
	PAIR('%',0),
	PAIR('n','l'),
	PAIR('y','r'),
	PAIR('h','p'),
	PAIR('c','t'),
	PAIR('d','n'),
	PAIR('m','o'),
	PAIR('d','y'),
	PAIR('d','w'),
	PAIR('l','n'),
	PAIR('d','l'),
	PAIR('s','t'),
	PAIR('s','b'),
	PAIR('c','.')
};
int pto = 10000;
int pfrom = 1;
int print = 1;
char nextf[NS] = _PATH_TMAC;
int nfi = sizeof(_PATH_TMAC) - 1;
#ifdef NROFF
char termtab[NS] = _PATH_TERM;
int tti = sizeof(_PATH_TERM) - 1;
#endif
#ifndef NROFF
int oldbits = -1;
#endif
int init = 1;
int fc = IMP;
int eschar = '\\';
int pl = 11*INCH;
int po = PO;
int dfact = 1;
int dfactd = 1;
int res = 1;
int smnt = 4;
int ascii = ASCII;
int ptid = PTID;
char ptname[] = "/dev/stdout";
int lg = LG;
int pnlist[NPN] = {-1};
int *pnp = pnlist;
int npn = 1;
int npnflg = 1;
int xflg = 1;
int dpn = -1;
int totout = 1;
int ulfont = 1;
int ulbit = 1<<9;
int tabch = TAB;
int ldrch = LEADER;
int xxx;
struct contab contab[NM]= {
	{ PAIR('d','s'), {caseds} },
	{ PAIR('a','s'), {caseas} },
	{ PAIR('s','p'), {casesp} },
	{ PAIR('f','t'), {caseft} },
	{ PAIR('p','s'), {caseps} },
	{ PAIR('v','s'), {casevs} },
	{ PAIR('n','r'), {casenr} },
	{ PAIR('i','f'), {caseif} },
	{ PAIR('i','e'), {caseie} },
	{ PAIR('e','l'), {caseel} },
	{ PAIR('p','o'), {casepo} },
	{ PAIR('t','l'), {casetl} },
	{ PAIR('t','m'), {casetm} },
	{ PAIR('b','p'), {casebp} },
	{ PAIR('c','h'), {casech} },
	{ PAIR('p','n'), {casepn} },
	{ PAIR('b','r'), {tbreak} },
	{ PAIR('t','i'), {caseti} },
	{ PAIR('n','e'), {casene} },
	{ PAIR('n','f'), {casenf} },
	{ PAIR('c','e'), {casece} },
	{ PAIR('f','i'), {casefi} },
	{ PAIR('i','n'), {casein} },
	{ PAIR('l','i'), {caseli} },
	{ PAIR('l','l'), {casell} },
	{ PAIR('n','s'), {casens} },
	{ PAIR('m','k'), {casemk} },
	{ PAIR('r','t'), {casert} },
	{ PAIR('a','m'), {caseam} },
	{ PAIR('d','e'), {casede} },
	{ PAIR('d','i'), {casedi} },
	{ PAIR('d','a'), {caseda} },
	{ PAIR('w','h'), {casewh} },
	{ PAIR('d','t'), {casedt} },
	{ PAIR('i','t'), {caseit} },
	{ PAIR('r','m'), {caserm} },
	{ PAIR('r','r'), {caserr} },
	{ PAIR('r','n'), {casern} },
	{ PAIR('a','d'), {casead} },
	{ PAIR('r','s'), {casers} },
	{ PAIR('n','a'), {casena} },
	{ PAIR('p','l'), {casepl} },
	{ PAIR('t','a'), {caseta} },
	{ PAIR('t','r'), {casetr} },
	{ PAIR('u','l'), {caseul} },
	{ PAIR('c','u'), {casecu} },
	{ PAIR('l','t'), {caselt} },
	{ PAIR('n','x'), {casenx} },
	{ PAIR('s','o'), {caseso} },
	{ PAIR('i','g'), {caseig} },
	{ PAIR('t','c'), {casetc} },
	{ PAIR('f','c'), {casefc} },
	{ PAIR('e','c'), {caseec} },
	{ PAIR('e','o'), {caseeo} },
	{ PAIR('l','c'), {caselc} },
	{ PAIR('e','v'), {caseev} },
	{ PAIR('r','d'), {caserd} },
	{ PAIR('a','b'), {caseab} },
	{ PAIR('f','l'), {casefl} },
	{ PAIR('e','x'), {done} },
	{ PAIR('s','s'), {casess} },
	{ PAIR('f','p'), {casefp} },
	{ PAIR('c','s'), {casecs} },
	{ PAIR('b','d'), {casebd} },
	{ PAIR('l','g'), {caselg} },
	{ PAIR('h','c'), {casehc} },
	{ PAIR('h','y'), {casehy} },
	{ PAIR('n','h'), {casenh} },
	{ PAIR('n','m'), {casenm} },
	{ PAIR('n','n'), {casenn} },
	{ PAIR('s','v'), {casesv} },
	{ PAIR('o','s'), {caseos} },
	{ PAIR('l','s'), {casels} },
	{ PAIR('c','c'), {casecc} },
	{ PAIR('c','2'), {casec2} },
	{ PAIR('e','m'), {caseem} },
	{ PAIR('a','f'), {caseaf} },
	{ PAIR('h','w'), {casehw} },
	{ PAIR('m','c'), {casemc} },
	{ PAIR('p','m'), {casepm} },
#ifdef NROFF
	{ PAIR('p','i'), {casepi} },
#endif
	{ PAIR('u','f'), {caseuf} },
	{ PAIR('p','c'), {casepc} },
	{ PAIR('h','t'), {caseht} },
#ifndef NROFF
	{ PAIR('f','z'), {casefz} },
#endif
	{ PAIR('c','f'), {casecf} },
};

/*
troff environment block
*/

int block = 0;
int ics = ICS;
int ic = 0;
int icf = 0;
int chbits = 0;
int spbits = 0;
int nmbits = 0;
int apts = PS;
int apts1 = PS;
int pts = PS;
int pts1 = PS;
int font = FT;
int font1 = FT;
int sps = SPS;
int spacesz = SS;
int lss = VS;
int lss1 = VS;
int ls = 1;
int ls1 = 1;
int ll = LL;
int ll1 = LL;
int lt = LL;
int lt1 = LL;
int ad = 1;
int nms = 1;
int ndf = 1;
int fi = 1;
int cc = '.';
int c2 = '\'';
int ohc = OHC;
int tdelim = IMP;
int hyf = 1;
int hyoff = 0;
int un1 = -1;
int tabc = 0;
int dotc = '.';
int adsp = 0;
int adrem = 0;
int lastl = 0;
int nel = 0;
int admod = 0;
int *wordp = 0;
int spflg = 0;
int *linep = 0;
int *wdend = 0;
int *wdstart = 0;
int wne = 0;
int ne = 0;
int nc = 0;
int nb = 0;
int lnmod = 0;
int nwd = 0;
int nn = 0;
int ni = 0;
int ul = 0;
int cu = 0;
int ce = 0;
int in = 0;
int in1 = 0;
int un = 0;
int wch = 0;
int pendt = 0;
int *pendw = 0;
int pendnf = 0;
int spread = 0;
int it = 0;
int itmac = 0;
int lnsize = LNSIZE;
int *hyptr[NHYP] = {0};
int tabtab[NTAB] = {DTAB,DTAB*2,DTAB*3,DTAB*4,DTAB*5,DTAB*6,DTAB*7,DTAB*8,
	DTAB*9,DTAB*10,DTAB*11,DTAB*12,DTAB*13,DTAB*14,DTAB*15,0};
int line[LNSIZE] = {0};
int word[WDSIZE] = {0};
int blockxxx[EVS-68-NHYP-NTAB-WDSIZE-LNSIZE] = {0};
/*spare 5 words*/
int oline[LNSIZE+1] = {0};
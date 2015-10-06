$gate: patch-Makefile.tmux,v 1.1 2010/12/14 02:43:05 dukzcry Exp $
--- Makefile.orig	Thu Dec  9 18:40:23 2010
+++ Makefile	Sat Dec 11 02:14:17 2010
@@ -38,13 +38,17 @@ SRCS=	attributes.c cfg.c client.c clock.c \
 	tmux.c tty-acs.c tty-keys.c tty-term.c tty.c utf8.c \
 	window-choose.c window-clock.c window-copy.c window.c \
 	xterm-keys.c xmalloc.c
+	
+BINDIR=	/usr/bin
 
 CDIAGFLAGS+= -Wno-long-long -Wall -W -Wnested-externs -Wformat=2
 CDIAGFLAGS+= -Wmissing-prototypes -Wstrict-prototypes -Wmissing-declarations
 CDIAGFLAGS+= -Wwrite-strings -Wshadow -Wpointer-arith -Wsign-compare
 CDIAGFLAGS+= -Wundef -Wbad-function-cast -Winline -Wcast-align
 
-LDADD=  -lutil -lcurses -levent
-DPADD=  ${LIBUTIL} ${LIBCURSES} ${LIBEVENT}
+LDADD=	-Wl,-Bdynamic -lutil -levent \
+	-Wl,-Bstatic -L../../lib/libutil.tmux -lfakeutil \
+	-Wl,-Bdynamic -lcurses
+DPADD=	${LIBCURSES}
 
 .include <bsd.prog.mk>

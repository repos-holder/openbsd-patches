$gate: patch-tmux.h,v 1.1 2010/12/14 02:43:05 dukzcry Exp $
--- tmux.h.orig	Wed Dec  8 18:23:11 2010
+++ tmux.h	Wed Dec  8 18:36:37 2010
@@ -30,7 +30,7 @@
 #include <bitstring.h>
 #include <event.h>
 #include <getopt.h>
-#include <imsg.h>
+#include "../../lib/libutil.tmux/imsg.h"
 #include <limits.h>
 #include <signal.h>
 #include <stdarg.h>

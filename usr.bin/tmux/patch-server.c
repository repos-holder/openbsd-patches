$gate: patch-server.c,v 1.1 2010/12/14 02:43:05 dukzcry Exp $
--- server.c.orig	Wed Dec  8 18:50:57 2010
+++ server.c	Wed Dec  8 18:51:44 2010
@@ -24,7 +24,7 @@
 #include <sys/wait.h>
 
 #include <errno.h>
-#include <event.h>
+#include "event.h"
 #include <fcntl.h>
 #include <paths.h>
 #include <signal.h>

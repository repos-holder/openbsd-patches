--- /usr/src/sys/sys/timeout.h	Fri Feb 26 21:15:22 2010
+++ /usr/src/sys/sys/timeout.h	Fri Feb 26 21:16:25 2010
@@ -86,6 +86,7 @@ void timeout_add_tv(struct timeout *, struct timeval *
 void timeout_add_ts(struct timeout *, struct timespec *);
 void timeout_add_bt(struct timeout *, struct bintime *);
 void timeout_add_sec(struct timeout *, int);
+void timeout_add_msec(struct timeout *, int);
 void timeout_add_usec(struct timeout *, int);
 void timeout_add_nsec(struct timeout *, int);
 void timeout_del(struct timeout *);

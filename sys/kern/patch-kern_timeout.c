--- /usr/src/sys/kern/kern_timeout.c	Wed Feb 24 13:52:52 2010
+++ /usr/src/sys/kern/kern_timeout.c	Wed Feb 17 15:48:07 2010
@@ -232,6 +232,18 @@ timeout_add_sec(struct timeout *to, int secs)
 }
 
 void
+timeout_add_msec(struct timeout *to, int msecs)
+{
+	long long to_ticks;
+
+	to_ticks = (long long)msecs * 1000 / tick;
+	if (to_ticks > INT_MAX)
+		to_ticks = INT_MAX;
+
+	timeout_add(to, (int)to_ticks);
+}
+
+void
 timeout_add_usec(struct timeout *to, int usecs)
 {
 	int to_ticks = usecs / tick;

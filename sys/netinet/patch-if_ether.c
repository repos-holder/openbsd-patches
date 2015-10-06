--- if_ether.c.orig	Fri Apr 25 20:59:59 2014
+++ if_ether.c	Fri Apr 25 21:00:50 2014
@@ -389,9 +389,6 @@ arpresolve(struct arpcom *ac, struct rtentry *rt, stru
 	}
 	if (rt) {
 		la = (struct llinfo_arp *)rt->rt_llinfo;
-		if (la == NULL)
-			log(LOG_DEBUG, "arpresolve: %s: route without link "
-			    "local address\n", inet_ntoa(SIN(dst)->sin_addr));
 	} else {
 		if ((la = arplookup(SIN(dst)->sin_addr.s_addr, RT_REPORT, 0,
 		    ac->ac_if.if_rdomain)) != NULL)

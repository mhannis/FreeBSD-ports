--- lib/dynxdr/xdrutil.c.orig	2025-09-03 01:35:59 UTC
+++ lib/dynxdr/xdrutil.c
@@ -102,7 +102,7 @@
    xdrmem_create(&xdrs, src, srcLen, XDR_DECODE);
 
-   ret = (Bool) proc(&xdrs, dest, 0);
+   ret = (Bool) proc(&xdrs, dest);
 
    xdr_destroy(&xdrs);
 

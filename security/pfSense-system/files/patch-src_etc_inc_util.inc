--- src/etc/inc/util.inc.orig
+++ src/etc/inc/util.inc
@@ -3599,7 +3599,11 @@ function start_devd() {
 		unset($_gb);
 	}
 
 	/* Use the undocumented -q options of devd to quiet its log spamming */
-	$_gb = exec("/sbin/devd -q -f /etc/{$g['product_name']}-devd.conf");
+	$devd_conf = "/etc/{$g['product_name']}-devd.conf";
+	if (!file_exists($devd_conf) && file_exists('/etc/pfSense-devd.conf')) {
+		$devd_conf = '/etc/pfSense-devd.conf';
+	}
+	$_gb = exec("/sbin/devd -q -f {$devd_conf}");
 	sleep(1);
 	unset($_gb);
 }

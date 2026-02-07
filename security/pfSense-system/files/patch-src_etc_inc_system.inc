--- src/etc/inc/system.inc.orig
+++ src/etc/inc/system.inc
@@ -3314,12 +3314,15 @@
 	$uniqueid_file="{$g['vardb_path']}/uniqueid";
 
 	if (empty(g_get('uniqueid'))) {
-		if (!file_exists($uniqueid_file)) {
+		if (!file_exists($uniqueid_file) && is_executable('/usr/sbin/gnid')) {
 			mwexec("/usr/sbin/gnid > {$g['vardb_path']}/uniqueid " .
 			    "2>/dev/null");
 		}
 		if (file_exists($uniqueid_file)) {
-			$g['uniqueid'] = @file_get_contents($uniqueid_file);
+			$uniqueid = trim((string)@file_get_contents($uniqueid_file));
+			if (preg_match('/^[A-Za-z0-9]+$/', $uniqueid) === 1) {
+				$g['uniqueid'] = $uniqueid;
+			}
 		}
 	}
 

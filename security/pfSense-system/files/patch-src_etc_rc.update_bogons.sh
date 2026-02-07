--- src/etc/rc.update_bogons.sh.orig
+++ src/etc/rc.update_bogons.sh
@@ -27,8 +27,15 @@
 
 do_not_send_uniqueid=$(/usr/local/sbin/read_xml_tag.sh boolean system/do_not_send_uniqueid)
 if [ "${do_not_send_uniqueid}" != "true" ]; then
-	uniqueid=$(/usr/sbin/gnid)
-	export HTTP_USER_AGENT="${product}/${product_version}:${uniqueid}"
+	uniqueid=""
+	if [ -x /usr/sbin/gnid ]; then
+		uniqueid=$(/usr/sbin/gnid 2>/dev/null)
+	fi
+	if [ -n "${uniqueid}" ]; then
+		export HTTP_USER_AGENT="${product}/${product_version}:${uniqueid}"
+	else
+		export HTTP_USER_AGENT="${product}/${product_version}"
+	fi
 else
 	export HTTP_USER_AGENT="${product}/${product_version}"
 fi

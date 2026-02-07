--- src/etc/inc/pkg-utils.inc.orig
+++ src/etc/inc/pkg-utils.inc
@@ -1282,45 +1282,78 @@
 	}
 }
 
-/* Identify which meta package is installed */
-function get_meta_pkg_name() {
+/* Return candidate product names derived from runtime configuration. */
+function get_pkg_product_names() {
+	$names = [];
+
 	$product_name = g_get('product_name');
-	foreach (g_get('alternativemetaports') as $suffix) {
-		if (is_pkg_installed("{$product_name}-{$suffix}")) {
-			return "{$product_name}-{$suffix}";
+	if (!empty($product_name)) {
+		$names[] = $product_name;
+	}
+
+	$pkg_prefix = g_get('pkg_prefix');
+	if (!empty($pkg_prefix) && str_ends_with($pkg_prefix, '-pkg-')) {
+		$prefix_product = substr($pkg_prefix, 0, -5);
+		if (!empty($prefix_product) && !in_array($prefix_product, $names, true)) {
+			$names[] = $prefix_product;
 		}
 	}
-	if (is_pkg_installed($product_name)) {
-		return $product_name;
+
+	return $names;
+}
+
+/* Identify which meta package is installed */
+function get_meta_pkg_name() {
+	foreach (get_pkg_product_names() as $product_name) {
+		foreach (g_get('alternativemetaports') as $suffix) {
+			if (is_pkg_installed("{$product_name}-{$suffix}")) {
+				return "{$product_name}-{$suffix}";
+			}
+		}
+		if (is_pkg_installed($product_name)) {
+			return $product_name;
+		}
 	}
 	return false;
 }
 
 function get_core_pkg_names() {
-	$product_name = g_get('product_name');
+	$packages = [];
+
+	foreach (get_pkg_product_names() as $product_name) {
+		// This query mirrors pfSense-upgrade logic.
+		$rc = pkg_exec("query -e \"%n ~ {$product_name}-kernel-* || %n ~ {$product_name}-base*\" %n", $stdout, $stderr);
+		if ($rc != 0 || empty($stdout)) {
+			continue;
+		}
 
-	// This query mirrors pfSense-upgrade logic.
-	$rc = pkg_exec("query -e \"%n ~ {$product_name}-kernel-* || %n ~ {$product_name}-base*\" %n", $stdout, $stderr);
-	if ($rc != 0) {
+		foreach (explode(PHP_EOL, $stdout) as $pkg_name) {
+			$pkg_name = trim($pkg_name);
+			if (empty($pkg_name) || in_array($pkg_name, $packages, true)) {
+				continue;
+			}
+			$packages[] = $pkg_name;
+		}
+	}
+
+	if (empty($packages)) {
 		return false;
 	}
-	// Clean up the output.
-	$stdout = implode(' ', array_filter(explode(PHP_EOL, $stdout)));
 
-	return $stdout;
+	return implode(' ', $packages);
 }
 
 /* Identify which package has the product dependencies */
 function get_dep_pkg_name() {
 	$name = get_meta_pkg_name();
 
-	if ($name == false) {
+	if ($name === false) {
 		return false;
 	}
 
 	// The dependencies have been moved to another package.
-	if ($name == g_get('product_name')) {
-		$name .= '-system';
+	if (in_array($name, get_pkg_product_names(), true)) {
+		return "{$name}-system";
 	}
 
 	return $name;
@@ -1328,13 +1361,13 @@
 
 /* Identify which base package is installed */
 function get_base_pkg_name() {
-	global $g;
-
 	/* XXX: Use pkg annotation */
-	if (is_pkg_installed(g_get('product_name') . '-base-' . g_get('product_name'))) {
-		return g_get('product_name') . '-base-' . g_get('product_name');
-	} else if (is_pkg_installed(g_get('product_name') . '-base')) {
-		return g_get('product_name') . '-base';
+	foreach (get_pkg_product_names() as $product_name) {
+		if (is_pkg_installed("{$product_name}-base-{$product_name}")) {
+			return "{$product_name}-base-{$product_name}";
+		} else if (is_pkg_installed("{$product_name}-base")) {
+			return "{$product_name}-base";
+		}
 	}
 	return false;
 }

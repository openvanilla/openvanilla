<?php

$file = "CinDocument.mm";
$content = file_get_contents($file);
preg_match_all("/MSG\(@\"(.*)\"/i", $content, $matches);
$msgs = $matches[1];
foreach($msgs as $msg) {
  echo "\"$msg\" = \"$msg\";\n";
}

//export to Localizable.strings
?>

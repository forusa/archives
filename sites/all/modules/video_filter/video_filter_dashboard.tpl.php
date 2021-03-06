<?php

/**
 * @file
 * Video filter dashboard template, controls the output in the popup
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title><?php print $title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body id="video_filter">
  <?php if (!empty($messages)): print $messages; endif; ?>
  <?php if (!empty($help)): print $help; endif; ?>
  <div class="clear-block">
    <?php print $content; ?>
  </div>
</body>
</html>

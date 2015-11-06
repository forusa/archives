<?php
/**
 * This is the page-node-lightbox2.tpl.php from the lightbox2 module, renamed
 * and added to the theme so it can be called when needed, not just with
 * lightbox. It displays node content without a header, footer or sidebars.
 * There is a check for it in page.tpl.php. For more information,
 * see http://tinyurl.com/5uqughz
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>" class="simple">
  <head>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
  </head>
  <body>

<!-- Layout -->

    <div id="wrapper"  style="body-background: none; background-color: #fff;">
    <div id="container" class="clear-block" style="width: 560px; margin: 0 auto; padding: 20px 0;">

      <div id="center"><div id="squeeze"><div class="right-corner"><div class="left-corner">

<?php if ($page == 0): ?>
  <h2 style="margin-bottom: 1em;"><?php print $title ?></h2>
<?php endif; ?>

    <p class="directions-print"><a href="<?php print $node_url ?>" title="Open this in a separate window for printing" target="_blank">View print version</a></p>

    <?php print $content ?>

  <div class="clear-block clear">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>
</div>
</div>
</div>
</div> <!-- close container -->
</div> <!-- close wrapper -->
</body>
</html>
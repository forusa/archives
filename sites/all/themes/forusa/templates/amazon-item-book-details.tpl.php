<?php /*
* This file controls the display of Amazon book items.
* It MUST exist in both the marinelli and gnifetti themes (forusa's base themes)
* in order for it to take effect here.
*/ ?><div class="<?php print $classes; ?>">

<!-- Amazon image -->
<div class="amazon-image"><?php print l($largeimage, $detailpageurl, array('html' => TRUE, 'attributes' => array('target' => '_blank', 'rel' => 'nofollow'))); ?></div>

<!-- Amazon buy button -->
<p class="amazon-button"><a href="<?php print $detailpageurl; ?>" rel="nofollow" target="_blank"><img src="/sites/default/files/amazon-buy.png" width="236" height="72" alt="Buy <?php print $title; ?> on Amazon" /></a></p>

<!-- Amazon title -->
<h4><strong><?php print l($title, $detailpageurl, array('html' => TRUE, 'attributes' => array('target' => '_blank', 'rel' => 'nofollow'))); ?></strong></h4>

<!-- Amazon author -->
<p><strong>By <?php if (!empty($author)) { print $author; } elseif (!empty($creator)) { print $creator; } ?></strong></p>

<!-- Amazon book info -->
<p class="amazon-bookinfo"><?php print $publisher; ?> (<?php print $publicationyear; ?>). <?php print t('@binding, @pages pages', array('@binding' => $binding, '@pages' => !empty($numberofpages) ? $numberofpages : "")); ?>.<?php if (!empty($ean)) { print ' ISBN ' . $ean . '.'; } elseif (empty($ean)) { print ' ISBN ' . $isbn . '.'; } ?></p>

<!-- Amazon price -->
<p class="amazon-price"><strong><?php print $listpriceformattedprice; ?></strong> &#8212; <em><?php print l('View on Amazon', $detailpageurl, array('html' => TRUE, 'attributes' => array('target' => '_blank', 'rel' => 'nofollow'))); ?></em></p>

</div>

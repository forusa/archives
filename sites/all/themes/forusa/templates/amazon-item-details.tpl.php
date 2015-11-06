<?php /*
* This file controls the general display of Amazon items under the "full"
* presentation.
* 12/27/12: NO CHANGES to core file (other than this note).
* It MUST exist in both the marinelli and gnifetti themes (forusa's base themes)
* in order for it to take effect here.
*/ ?><div class="<?php print $classes; ?>">
<?php if (!empty($invalid_asin)) { print "<div class='invalid_asin'>This item is no longer valid on Amazon.</div>"; } ?>
<?php print $smallimage; ?>
<div><strong>Title: <?php print l($title, $detailpageurl, array('html' => TRUE, 'attributes' => array('rel', 'nofollow'))); ?></strong></div>
<div><strong><?php print t('Manufacturer'); ?>:</strong> <?php if (!empty($manufacturer)) { print $manufacturer; } ?></div>
<div><strong><?php print t('Part Number'); ?>:</strong> <?php if (!empty($mpn)) {print $mpn; }?></div>
<div><strong><?php print t('Price'); ?>:</strong> <?php if (!empty($listpriceformattedprice)) { print $listpriceformattedprice; }?></div>
</div>

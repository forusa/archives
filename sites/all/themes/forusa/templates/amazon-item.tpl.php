<?php /*
* This file controls the general display of Amazon items under default
* presentations.
* 12/27/12: NO CHANGES to core file (other than this note).
* It MUST exist in both the marinelli and gnifetti themes (forusa's base themes)
* in order for it to take effect here.
*/ ?><div class="<?php print $classes; ?>">
<?php if (!empty($invalid_asin)) { print "<div class='invalid_asin'>This item is no longer valid on Amazon.</div>"; } ?>
<?php if (!empty($smallimage)) { print $smallimage; } ?>
<div><strong>Title: <?php print l($title, $detailpageurl, array('html' => TRUE, 'attributes' => array('rel' => 'nofollow'))); ?></strong></div>
</div>

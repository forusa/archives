
 <div id="block-<?php print $block->module . '-' . $block->delta; ?>" class="block defaultblock <?php print $classes; ?>">
    <?php if ($title): ?><h2 class="block-title"><?php print $block->subject; ?></h2><!--block title--><?php endif; ?>
   <div class="blockcontent"><?php print $block->content; ?></div>
</div>

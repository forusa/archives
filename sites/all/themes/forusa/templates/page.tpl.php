<?php /* See the simple.tpl.php template for more information on this check. */
if ($_GET["format"] == "simple") {
  include ('simple.tpl.php');
  return;
}
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <title><?php print $head_title ?></title>

  <?php /* Facebook "share" image */ if($node->field_representative_image[0]['filepath']): ?><link rel="image_src" href="<?php print $GLOBALS['base_url'] . '/' . $node->field_representative_image[0]['filepath']; ?>" /><?php elseif($node->field_event_flyer_img[0]['filepath']): ?><link rel="image_src" href="<?php print $GLOBALS['base_url'] . '/' . $node->field_event_flyer_img[0]['filepath']; ?>" /><?php else: /* Facebook sharing logo */ ?><link rel="image_src" href="<?php print $GLOBALS['base_url'] . '/sites/default/files/forusa-sq-logo.png' ?>" /><?php endif; ?>

  <?php /* This should be replaced with the int_meta.module */
  if ($node->teaser): ?>
    <meta name="description" content="<?php print htmlspecialchars(strip_tags($node->teaser), ENT_QUOTES); ?>" />
  <?php else: ?>
    <meta name="description" content="The Fellowship of Reconciliation is the largest, oldest interfaith peace organization in the United States, working for peace, justice and nonviolence since 1915." />
  <?php endif; ?>

  <meta http-equiv="Content-Style-Type" content="text/css" />
  <?php /* Setting the default style sheet language for validation */  ?>

  <link rel="alternate" type="application/rss+xml" title="FOR: All Posts" href="/rss.xml" />

  <?php print $head ?>
  <?php print $styles ?>

  <!--[if IE 6]>
    <link rel="stylesheet" type="text/css" href="<?php print base_path(). path_to_theme(); ?>/css/ie6.css" />
<![endif]-->

  <!--[if IE 7]>
    <link rel="stylesheet" type="text/css" href="<?php print base_path(). path_to_theme(); ?>/css/ie7.css" />
<![endif]-->


  <!-- POPUP -->
  <link rel="stylesheet" type="text/css" href="<?php print base_path(). path_to_theme(); ?>/css/colorbox.css" />
  <link rel="stylesheet" type="text/css" href="<?php print base_path(). path_to_theme(); ?>/css/popup.css" />
  <!-- POPUP -->


  <?php print $scripts ?>


  <!-- POPUP -->
  <script language="javascript" src="<?php print base_path(). path_to_theme(); ?>/js/colorbox.js"></script>
  <script>

    $("document").ready(function (){

       // load the overlay

       if (document.cookie.indexOf('visited=true') == -1) {
        var fifteenDays = 1000*60*60*24*15;
        var expires = new Date((new Date()).valueOf() + fifteenDays);
        document.cookie = "visited=true;expires=" + expires.toUTCString();
        $.colorbox({width:"580px", inline:true, href:"#subscribe_popup"});
      }

      $(".open_popup").colorbox({width:"580px", inline:true, href:"#subscribe_popup"});
    });
  </script>
  <!-- POPUP -->


</head>

<body class="<?php print $body_classes; ?> <?php print $classes; ?>">

  <div id="utilities">

  <?php print $search_box ?>

  <?php if (isset($primary_links)) : ?>
  <?php print '<div id="plinks">'; ?>

             <?php

		if(theme_get_setting('menutype')== '0'){

			print theme('links', $primary_links, array('class' => 'links primary-links'));
		}

	   else {

			print forusa_get_primary_links();
		}

	   ?>

               <?php print '</div>'; ?>
        <?php endif; ?>
  <br clear="all" />
  </div>

<div id="page">

  <div id="header">

<?php if ($site_name) : ?>

  <div id="logo">
  <?php if ($logo) : ?>
  	<?php if ($is_front) : ?>
  	  <h1 class="sitetitle"><a href="<?php print $front_page ?>" title="<?php print $site_name ?>"><img src="<?php print $logo ?>" alt="<?php print $site_name ?>" /></a></h1>
  	<?php endif; if (!$is_front) : ?>
  	  <p class="sitetitle"><a href="<?php print $front_page ?>" title="<?php print $site_name ?>"><img src="<?php print $logo ?>" alt="<?php print $site_name ?>" /></a></p>
  	<?php endif; ?>
  <?php endif; ?>

  <?php if ($site_slogan){?>
    <p class="slogan"><a href="<?php print $front_page ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a> <?php print $site_slogan ?></p>
  <?php } ?>
  </div>

<?php endif; ?>
  </div>
  <?php /* this is actually not the true header, as the header items above are not blocks, but allows blocks to be placed directly below these items, above the menu and page items */
    if ($header){ print '<div id="header_content">' . $header . '</div>'; }
  ?>

 <?php /* The function theme('nice_menu', etc) prints the dropdown menu. It can be themed in the file css/forusa-menus.css in the forusa directory. Documentation: http://drupal.org/node/185543
dropdown.css is the dropdown css built into the Marinelli theme (for the primary links only) */ ?>
   <?php if (($secondary_links)) : ?>
      <div id="submenu">
          <?php
	$menu = theme('nice_menu', $id = 1, $mlid = 'secondary-links', $menu = NULL, $direction = 'down');
	print $menu['content'];
	  ?>
      </div>

      <div class="stopfloat"></div>
    <?php endif; ?>

  <div class="wrapper"><!-- wrapper:defines whole content margins -->

  <!-- left -->
        <?php if ($left && !$is_front) { ?>
          <div class="lsidebar">
            <?php print $left ?>
          </div><!-- end left -->
        <?php } ?>

   	<!-- right -->
        <?php if ($right && !$is_front) { ?>
          <div class="rsidebar">
            <?php print $right ?>
          </div><!-- end right -->
        <?php } ?>

    <div id="primary" class=<?php print '"'.forusa_width( $left, $right).'">' ?>
               <div class="singlepage">
	  <?php /* print $breadcrumb; */ ?>

   <?php if ($mission): print '<div id="sitemission"><p>'. $mission .'</p></div>'; endif; ?>

		 <?php
		 if ($title):

		 if (!$is_front){
		 /* if we are NOT on the front page, print node title */

			print '<h1'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h1>';
		 }

		 endif; ?>

        <?php if (!$is_front){
		 /* if we are NOT on the front page, print node tabs */
		      if ($tabs): print '<div class="tabs">'.$tabs.'</div>'; endif;
		    } ?>
        <?php if ($help) { ?><div class="help"><?php print $help ?></div><?php } ?>
        <?php if ($messages) { ?><div class="messages"><?php print $messages ?></div><?php } ?>
<div class="drdot">
<hr />
</div>

 <?php print $content_top ?>

 <?php print $content ?>
      </div>

    </div>


 <div class="clear"></div>

  </div>
</div>
<!-- Close Page -->

<div id="footer">
<?php print $footer ?>
<?php print $footer_message ?>
</div>
<?php print $closure ?>


<!-- POPUP -->
  <div style='display:none'>
    <div id='subscribe_popup' style='padding:10px;'>
      <h2 class="box-title">Donate to FOR-USA</h2>
      <p class="donate-modal__intro"><img src="http://org.salsalabs.com/o/2507/images/sekou-band_april-martin_250.jpg" width="150" height="102" class="donate-modal__image" />Now until Dec. 31, <strong>receive free albums of social justice–based music</strong> with your end-of-year donation to the Fellowship of Reconciliation!</p>
      <p class="donate-modal__button-wrap"><a href="/donate" title="Donate securely online" class="donate-modal__button">Make your tax-deductible donation now!</a></p>
      <p class="donate-modal__footer"><em>Prefer to donate by check? Postmark it by Dec. 31 and mail to:<br /><strong>Fellowship of Reconciliation<br />P.O. Box 271<br />Nyack, NY 10960</strong></em></p>
    </div>
  </div>
<!-- POPUP -->


<!--[if !(lte IE 8)]><!-->
   <script type="text/javascript">
  var tdwfb_config = {
    greeting: 'Dear Internet', // Sets the salutation at the top left
    disableDate: false, // If true, the banner shows even if the date is not yet 02/11/2014
    callOnly: true // If true, the banner only displays a form for calling congress
  };
  (function(){
    var e = document.createElement('script'); e.type='text/javascript'; e.async = true;
    e.src = document.location.protocol + '//d1agz031tafz8n.cloudfront.net/thedaywefightback.js/widget.min.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(e, s);
  })();

   </script>
<!--<![endif]-->
</body>
</html>

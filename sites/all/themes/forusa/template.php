<?php

//Jeremy: this function ensures that dropdown menus are enabled by skipping asking the theme.  Avoids modifying marinelli core.

function forusa_settings($saved_settings) {

  $defaults = array(
    'menutype' => 1,
    'cssPreload' =>0
  );
}

function forusa_get_primary_links() {
  return menu_tree(variable_get('menu_primary_links_source', 'primary-links'));
}


// retrieve custom theme settings


$preload = theme_get_setting('cssPreload'); // print the js file if we choose css image preload

if($preload == '1'){
	
	drupal_add_js(drupal_get_path('theme','marinelli').'/js/preloadCssImages.jQuery_v5.js'); // load the javascript
	drupal_add_js('$(document).ready(function(){
		
	$.preloadCssImages();
		
	});
	
	','inline');

}

	
$valore = theme_get_setting('menutype'); // if we choose dropdown

//if($valore == '1')

if (true)
{ 
 
    drupal_add_js(drupal_get_path('theme','marinelli').'/js/jquery.hoverIntent.minified.js'); // load the javascript
	drupal_add_js(drupal_get_path('theme','marinelli').'/js/marinellidropdown.js'); // load the javascript
	drupal_add_css(drupal_get_path('theme','marinelli').'/dropdown.css'); // load the css
	
}

function forusa_gmap_marker_popup($label) {
  //return $label;
  return "NOTHING";
}
function forusa_width($left, $right) {
  $width = "short";
  if (!$left ) {
    $width = "long";
  }  
  
   if (!$right) {
    $width = "long";
  }
  return $width;
}

  /**
   * Implementation of hook_form_alter().
   *
   * The function is named modulename_form_alter.
   * moved to module folder.
   */
//function hook_form_alter(&$form, $form_state, $form_id) {
    // Normally a switch is used because you may want to alter more than
    // one form and it is easy to add a new case for each form.
    //switch ($form_id) {
      // This is our form ID.
      //case 'group-node-form':
        //$form['author']['#type'] = 'hidden';
    //    break;
  //   }
  // }

 // function admin_group_node_form($form) {
   // dpm($form);
 // }


//adds a check for individual node template files like node-15.tpl.php
function forusa_preprocess_node(&$vars) {
    $vars['template_files'][] = 'node-' . $vars['nid'];
    return $vars;
}

//function forusa_preprocess_views_view__zip_code_proximity_test_2 (&$variables){	
//}



/* Implementation of theme_username (override) */

function forusa_username($object) {

  if ($object->uid && $object->name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($object->name) > 25) {
      $name = drupal_substr($object->name, 0, 25) . '...';
    }
    else {
      $name = $object->name;
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/' . $object->uid, array('attributes' => array('title' => t('Read more about this author'))));
    }
    else {
      $output = check_plain($name);
    }
  }
  else if ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if (!empty($object->homepage)) {
      $output = l($object->name, $object->homepage, array('attributes' => array('rel' => 'nofollow')));
    }
    else {
      $output = check_plain($object->name);
    }

    /* $output .= ' (' . t('not verified') . ')'; */
    $output .= '';
  }
  else {
    $output = check_plain(variable_get('anonymous', t('Anonymous')));
  }

  return $output;
}


/* Override date.module theming */

function forusa_date_repeat_display($field, $item, $node = NULL) {
  $output = '';
  if (!empty($item['rrule'])) {
    $output = date_repeat_rrule_description($item['rrule']);
    $output = '<div class="repeat">'. $output .'</div>';
  }
  return $output;
}

/**
* Sorts taxonomy terms for display.
*/
function phptemplate_preprocess_node(&$vars) {
  // If we have any terms...
  if ($vars['node']->taxonomy) {
    // Let's iterate through each term.
    foreach ($vars['node']->taxonomy as $term) {
      // We will build a new array where there will be as many
      // nested arrays as there are vocabularies
      // The key for each nested array is the vocabulary ID.     
      $vocabulary[$term->vid]['taxonomy_term_'. $term->tid]  = array(
        'title' => $term->name,
        'href' => taxonomy_term_path($term),
        'attributes' => array(
          'rel' => 'tag', 
          'title' => strip_tags($term->description),
        ),
      );       
    }
    // Making sure vocabularies appear in the same order.
    ksort($vocabulary, SORT_NUMERIC);
    // We will get rid of the old $terms variable.
    unset($vars['terms']);
    // And build a new $terms.
    foreach ($vocabulary as $vid => $terms) {
      // Getting the name of the vocabulary.
      $name = taxonomy_vocabulary_load($vid)->name;
      // Using the theme('links', ...) function to theme terms list.
      $terms = theme('links', $terms, array('class' => 'links inline'));
      // Wrapping the terms list.
      $vars['terms'] .= '<div class="vocabulary taxonomy_vid_';
      $vars['terms'] .= $vid;
      $vars['terms'] .= '"><span class="taxonomy-name">';
      $vars['terms'] .= $name;
      $vars['terms'] .= ':&nbsp;</span>';
      $vars['terms'] .= $terms;
      $vars['terms'] .= '</div>';
      
    // sorting vocabularies into individual variables
      $vars['taxonomy_campaigns']= theme('links',$vocabulary[12], array('class'=>'links inline'));
      $vars['taxonomy_issues']= theme('links',$vocabulary[11], array('class'=>'links inline'));
      $vars['taxonomy_faiths']= theme('links',$vocabulary[7], array('class'=>'links inline'));
      $vars['taxonomy_tags']= theme('links',$vocabulary[6], array('class'=>'links inline'));
    }  
  }    
}


function forusa_preprocess_page(&$vars, $hook) {
  // Add conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }


  // From zen.theme:
  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  if (!$vars['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    $vars['classes_array'][] = drupal_html_class('page-' . $path);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $vars['classes_array'][] = drupal_html_class('section-' . $section);
  }
  if (theme_get_setting('forusa_wireframes')) {
    $vars['classes_array'][] = 'with-wireframes'; // Optionally add the wireframes style.
  }
  // We need to re-do the $layout and body classes because
  // template_preprocess_page() assumes sidebars are named 'left' and 'right'.
  $vars['layout'] = 'none';
  if (!empty($vars['sidebar_first'])) {
    $vars['layout'] = 'first';
  }
  if (!empty($vars['sidebar_second'])) {
    $vars['layout'] = ($vars['layout'] == 'first') ? 'both' : 'second';
  }
  // If the layout is 'none', then template_preprocess_page() will already have
  // set a 'no-sidebars' class since it won't find a 'left' or 'right' sidebar.
  if ($vars['layout'] != 'none') {
    // Remove the incorrect 'no-sidebars' class.
    if ($index = array_search('no-sidebars', $vars['classes_array'])) {
      unset($vars['classes_array'][$index]);
    }
    // Set the proper layout body classes.
    if ($vars['layout'] == 'both') {
      $vars['classes_array'][] = 'two-sidebars';
    }
    else {
      $vars['classes_array'][] = 'one-sidebar';
      $vars['classes_array'][] = 'sidebar-' . $vars['layout'];
    }
  }
}



/**
 * From zen.theme:
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function forusa_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Drupal 7 uses a $content variable instead of $block->content.
  $vars['content'] = $block->content;
  // Drupal 7 should use a $title variable instead of $block->subject.
  $vars['title'] = $block->subject;

  // Special classes for blocks.
  $vars['classes_array'][] = 'block-' . $block->module;
  $vars['classes_array'][] = 'region-' . $vars['block_zebra'];
  $vars['classes_array'][] = $vars['zebra'];
  $vars['classes_array'][] = 'region-count-' . $vars['block_id'];
  $vars['classes_array'][] = 'count-' . $vars['id'];
  }

/**
 * Converts a string to a suitable html ID attribute.
 *
 * http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 * valid ID attribute in HTML. This function:
 *
 * - Ensure an ID starts with an alpha character by optionally adding an 'id'.
 * - Replaces any character except alphanumeric characters with dashes.
 * - Converts entire string to lowercase.
 *
 * @param $string
 *   The string
 * @return
 *   The converted string
 */
function forusa_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
  // If the first character is not a-z, add 'id' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id' . $string;
  }
  return $string;
}

if (!function_exists('drupal_html_class')) {
  /**
   * Prepare a string for use as a valid class name.
   *
   * Do not pass one string containing multiple classes as they will be
   * incorrectly concatenated with dashes, i.e. "one two" will become "one-two".
   *
   * @param $class
   *   The class name to clean.
   * @return
   *   The cleaned class name.
   */
  function drupal_html_class($class) {
    // By default, we filter using Drupal's coding standards.
    $class = strtr(drupal_strtolower($class), array(' ' => '-', '_' => '-', '/' => '-', '[' => '-', ']' => ''));

    // http://www.w3.org/TR/CSS21/syndata.html#characters shows the syntax for valid
    // CSS identifiers (including element names, classes, and IDs in selectors.)
    //
    // Valid characters in a CSS identifier are:
    // - the hyphen (U+002D)
    // - a-z (U+0030 - U+0039)
    // - A-Z (U+0041 - U+005A)
    // - the underscore (U+005F)
    // - 0-9 (U+0061 - U+007A)
    // - ISO 10646 characters U+00A1 and higher
    // We strip out any character not in the above list.
    $class = preg_replace('/[^\x{002D}\x{0030}-\x{0039}\x{0041}-\x{005A}\x{005F}\x{0061}-\x{007A}\x{00A1}-\x{FFFF}]/u', '', $class);

    return $class;
  }
} /* End of drupal_html_class conditional definition. */

if (!function_exists('drupal_html_id')) {
  /**
   * Prepare a string for use as a valid HTML ID and guarantee uniqueness.
   *
   * @param $id
   *   The ID to clean.
   * @return
   *   The cleaned ID.
   */
  function drupal_html_id($id) {
    $id = strtr(drupal_strtolower($id), array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));

    // As defined in http://www.w3.org/TR/html4/types.html#type-name, HTML IDs can
    // only contain letters, digits ([0-9]), hyphens ("-"), underscores ("_"),
    // colons (":"), and periods ("."). We strip out any character not in that
    // list. Note that the CSS spec doesn't allow colons or periods in identifiers
    // (http://www.w3.org/TR/CSS21/syndata.html#characters), so we strip those two
    // characters as well.
    $id = preg_replace('/[^A-Za-z0-9\-_]/', '', $id);

    return $id;
  }
} /* End of drupal_html_id conditional definition. */

// Cleaning up output of getdirections.module

function forusa_getdirections_direction_form($form) {
  if (isset($form['mto'])) {
    $form['mto']['#prefix'] = '<div class="container-inline getdirections_display">';
    $form['mto']['#suffix'] = '</div>';
  }
  if (isset($form['mfrom'])) {
    $form['mfrom']['#prefix'] = '<div class="container-inline getdirections_display">';
    $form['mfrom']['#suffix'] = '</div>';
  }
  if (isset($form['travelmode'])) {
    $form['travelmode']['#prefix'] = '<div class="container-inline getdirections_display">';
    $form['travelmode']['#suffix'] = '</div>';
  }
  if (isset($form['travelextras'])) {
    $form['travelextras']['#prefix'] = '<div class="container-inline getdirections_display">';
    $form['travelextras']['#suffix'] = '</div>';
  }
  if (getdirections_is_advanced()) {
    $desc = t('Get directions using the form below.');
    if (isset($form['country_from'])) {
      $desc = t('Get directions using the form below.');
      $form['country_from']['#prefix'] = '<div id="getdirections_start"><div class="container-inline getdirections_display">';
      $form['country_from']['#suffix'] = '</div>';
    }
    if (isset($form['from']) && $form['from']['#type'] == 'textfield' && module_exists('location')) {
      $form['from']['#suffix'] = '</div>';
    }
    if (isset($form['country_to'])) {
      $form['country_to']['#prefix'] = '<div id="getdirections_end"><div class="container-inline getdirections_display">';
      $form['country_to']['#suffix'] = '</div>';
    }
    if (isset($form['to']) && $form['to']['#type'] == 'textfield' && module_exists('location')) {
      $form['to']['#suffix'] = '</div>';
    }
  }
  else {
    if (isset($form['country_from'])) {
      $form['country_from']['#prefix'] = '<div class="container-inline getdirections_display">';
      $form['country_from']['#suffix'] = '</div>';
    }
    if (isset($form['country_to'])) {
      $form['country_to']['#prefix'] = '<div class="container-inline getdirections_display">';
      $form['country_to']['#suffix'] = '</div>';
    }
    $desc = t('Get directions using the form below.');
  }

  if (isset($form['trafficinfo'])) {
    $form['trafficinfo']['#prefix'] = '<div id="getdirections_trafficinfo">';
    $form['trafficinfo']['#suffix'] = '</div>';
  }
  if (isset($form['bicycleinfo'])) {
    $form['bicycleinfo']['#prefix'] = '<div id="getdirections_bicycleinfo">';
    $form['bicycleinfo']['#suffix'] = '</div>';
  }
  if (isset($form['switchfromto'])) {
    $form['switchfromto']['#prefix'] = '<div id="getdirections_switchfromto">';
    $form['switchfromto']['#suffix'] = '</div>';
  }
  if (isset($form['next'])) {
    $form['next']['#prefix'] = '<div id="getdirections_nextbtn">';
    $form['next']['#suffix'] = '</div>';
  }

  if (isset($form['submit'])) {
    $form['submit']['#prefix'] = '<div id="getdirections_btn">';
    $form['submit']['#suffix'] = '</div>';
  }
  $output = '<p class="description">'. $desc .'</p>';
  $output .= drupal_render($form);
  return $output;
}
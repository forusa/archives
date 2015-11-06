<?php
  /*  This file reorders the new user registration form.
  *  it also sets the location field to uncollapsed.
  *   
  */ 
?>
<h2>Create a New Account</h2>
<?php //dpm($form); ?>
<?php  $form['locations']['#collapsed'] = 0;
  $form['locations']['0']['#collapsed'] = 0; 
  $output = drupal_render( $form['user_registration_help'] );
  $output .= drupal_render( $form['account'] );
  $output .= drupal_render( $form['Personal Information'] );
  $output .= drupal_render( $form['locations'] );
  $output .= drupal_render( $form['More Information'] );
  $output .= drupal_render( $form['og_register'] );
  $output .= drupal_render( $form ); 
  print $output;
?>



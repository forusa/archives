<?php

/**
 * @file fb_social_comments.inc
 * 
 * Box class for "comments" plugin type
 */

class fb_social_comments extends fb_social_default {
  
  public $fb_plugin_name;

  protected function __construct() {
    parent::__construct();
    $this->fb_plugin_name = 'comments';
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return parent::options_defaults();
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form() {
    return parent::options_form();
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    return $block;
  }
}
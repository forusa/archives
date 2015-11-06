<?php
/**
 * @file
 * The field user link views handler for fasttoggle.
 *
 * This file defines the Views handler for rendering fasttoggle links.
 */

class fasttoggle_views_handler_field_user_link extends views_handler_field_user_link {
  var $fasttoggle_key = 'status';

  function construct() {
    parent::construct();

    // We need these fields for state checking later in the render() function.
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['status'] = 'status';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['text']);
  }

  function render($values) {

    $account = new stdClass();
    $account->uid = $values->{$this->aliases['uid']};
    
    $options = fasttoggle_get_options('user', $account);
    $key = $this->fasttoggle_key;
    if (!empty($options['status'][$key])) {
      $status = $values->{$this->aliases['status']};
      return fasttoggle($options['status'][$key]['label'][intval($status)], 'user/'. $account->uid .'/toggle/status/'. $key, FASTTOGGLE_FORMAT_HTML, 'status_' . $key .'_'. $account->uid, 'fasttoggle-status-user-status-' . $key . '-' . intval($status));
    }
  }
}

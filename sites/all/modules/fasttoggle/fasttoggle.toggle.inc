<?php

/**
 * @file
 * Handles toggling of options.
 */

/**
 * Menu callback. Toggle options for a node if the action is confirmed via
 * POST. Otherwise, display a confirmation form.
 */
function fasttoggle_node_option($node, $group, $option) {
  global $user;

  $options = fasttoggle_get_options('node', $node);

  // Check if the action is valid. This is essential to ensure the user has
  // access to the action.
  if (isset($options[$group][$option]) && isset($_GET['token']) && drupal_valid_token($_GET['token'], $group . '_' . $option . '_' . $node->nid, TRUE)) {
    // The action is confirmed: either via form submit or via AJAX/POST
    if (isset($_POST['confirm']) && $_POST['confirm']) {
      $selectorClass = 'fasttoggle-status-node-' . $node->nid . '-' . $group . '-' . $option;

      // Get the next ID.
      while (key($options[$group][$option]['label']) != $node->$option) {
        next($options[$group][$option]['label']);
      }

      if (next($options[$group][$option]['label']) === FALSE) {
        reset($options[$group][$option]['label']);
      }

      // Save the node.
      $node->$option = key($options[$group][$option]['label']);
      $newClass = 'fasttoggle-status-node-' . $group . '-' . $option . '-' . $node->$option;
      node_save($node);

      $labels = fasttoggle_fasttoggle_labels(FASTTOGGLE_LABEL_STATUS);
      watchdog('fasttoggle', 'Node @type "%title" @option toggled to @value by @username', array('@type' => $node->type, '%title' => $node->title, '@option' => $option, '@value' => $labels['node_' . $option][$node->$option], '@username' => $user->name));

      // Let other modules respond.
      module_invoke_all('fasttoggle_toggle', 'node', $node, $option);

      // Output the new status for the updated link text on AJAX changes
      if (isset($_POST['javascript']) && $_POST['javascript']) {
        drupal_set_header('Content-Type: text/javascript; charset=utf-8');
        echo drupal_to_js(array(
          'text' => $options[$group][$option]['label'][intval($node->$option)],
          'callback' => 'node',
          'option' => $option,
          'status' => $node->$option,
          'className' => 'fasttoggle-status-node-'. $group . '-' . $option .'-'. intval($node->$option),
        ));
        exit;
      }
      else {
        drupal_goto();
      }
    }
    else {
      // The action is not confirmed. The user came here through a regular link;
      // no AJAX was involved. That means, we need a confirmation form so that
      // we get a POST form.
      $labels = _fasttoggle_get_label('node_'. $group . '_' . $option, FASTTOGGLE_LABEL_STATUS);
      return drupal_get_form('fasttoggle_node_option_confirm', $node, $labels[intval(!$node->$option)]);
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
}


/**
 * Confirmation form for the option change of a node.
 */
function fasttoggle_node_option_confirm($form_state, $node, $option) {
  return confirm_form(array(),
    t('Are you sure you want to set the post %title to %option?', array('%title' => $node->title, '%option' => $option)),
    $_GET['destination'] ? $_GET['destination'] : 'node/' . $node->nid,
    '',
    t('Change'), t('Cancel')
  );
}


/**
 * Menu callback. Toggle the status of a user if the action is confirmed via
 * POST. Otherwise, display a confirmation form.
 */
function fasttoggle_user_option($obj_user, $group, $option) {
  global $user;
  $options = fasttoggle_get_options('user', $obj_user);

  // Check if the action is valid. This is essential to ensure the user has
  // access to the action.
  if (isset($options[$group][$option]) && isset($_GET['token']) && drupal_valid_token($_GET['token'], $group . '_' . $option . '_' . $obj_user->uid, TRUE)) {
    if (isset($_POST['confirm']) && $_POST['confirm']) {
      $selectorClass = 'fasttoggle-status-user-' . $obj_user->uid . '-' . $group . '-' . $option;

      if (isset($options[$group][$option]['array_fn'])) {
        $array = $options[$group][$option]['array_fn']($obj_user, $options[$group][$option]['value_key']);
      } else {
        $array = array($option => intval(!$obj_user->$option));
      }
      $obj_user = user_save($obj_user, $array);

      $labels = fasttoggle_fasttoggle_labels(FASTTOGGLE_LABEL_STATUS);

      if (isset($options[$group][$option]['value_fn'])) {
        $value = $options[$group][$option]['value_fn']($obj_user, $options[$group][$option]['value_key']);
        $label = $labels[$group][$value];
      } else {
        if (isset($options[$group][$option]['value_key'])) {
          $value = $obj_user->$options[$group][$option]['value_key'];
        } else {
          $value = intval($obj_user->$option);
        }
        $label = $labels['user_' . $option][$value];
      }

      watchdog('fasttoggle', 'User "@obj_user" @group "@option" toggled to @value by @username', array('@obj_user' => $obj_user->name, '@group' => $group, '@option' => $option, '@value' => $label, '@username' => $user->name));


      // Let other modules respond.
      module_invoke_all('fasttoggle_toggle', 'user', $obj_user, $option);

      // Output the new option for the updated link text on AJAX changes
      if (isset($_POST['javascript']) && $_POST['javascript']) {
        drupal_set_header('Content-Type: text/javascript; charset=utf-8');
        echo drupal_to_js(array(
          'text' => $options[$group][$option]['label'][$value],
          'className' => 'fasttoggle-status-user-'. $group . '-' . $option .'-'. $value,
        ));
        exit;
      }
      else {
        drupal_goto();
      }
    }
    else {
      // The action is not confirmed. The user came here through a regular link;
      // no AJAX was involved. That means, we need a confirmation form so that
      // we get a POST form.
      $labels = _fasttoggle_get_label('user_'. $group . '_' . $option, FASTTOGGLE_LABEL_STATUS);
      return drupal_get_form('fasttoggle_user_option_confirm', $obj_user, $labels[intval(!$obj_user->$option)]);
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
}


/**
 * Confirmation form for the status change of a user.
 */
function fasttoggle_user_option_confirm($form_state, $user, $option) {
  return confirm_form(array(),
    t('Are you sure you want to set the user %user to %option?', array('%user' => $user->name, '%option' => $option)),
    $_GET['destination'] ? $_GET['destination'] : 'user/' . $user->uid,
    '',
    t('Change'), t('Cancel')
  );
}


/**
 * Menu callback. Toggle options for a comment if the action is confirmed via
 * POST. Otherwise, display a confirmation form.
 */
function fasttoggle_comment_option($comment, $group, $option) {
  global $user;
  $options = fasttoggle_get_options('comment', $comment);

  // Check if the action is valid. This is essential to ensure the user has
  // access to the action.
  if (isset($options[$group][$option]) && isset($_GET['token']) && drupal_valid_token($_GET['token'], $group . '_' . $option .'_'. $comment->cid, TRUE)) {
    // The action is confirmed: either via form submit or via AJAX/POST
    if (isset($_POST['confirm']) && $_POST['confirm']) {
      $oldClass = 'fasttoggle-status-comment-' . $group . '-' . $option . '-' . intval($comment->$option);
      $comment->$option = !$comment->$option;
      $newClass = 'fasttoggle-status-comment-' . $group . '-' . $option . '-' . intval($comment->$option);
      comment_save((array)$comment);
      
      $labels = fasttoggle_fasttoggle_labels(FASTTOGGLE_LABEL_STATUS);
      watchdog('fasttoggle', 'Comment "@subject" @option toggled to @value by @username', array('@subject' => $comment->subject, '@option' => $option, '@value' => $labels['comment_' . $option][$comment->$option], '@username' => $user->name));

      // Let other modules respond.
      module_invoke_all('fasttoggle_toggle', 'comment', $comment, $option);

      // Output the new status for the updated link text on AJAX changes
      if (isset($_POST['javascript']) && $_POST['javascript']) {
        drupal_set_header('Content-Type: text/javascript; charset=utf-8');
        echo drupal_to_js(array(
          'text' => $options[$group][$option]['label'][intval($comment->$option)],
          'callback' => 'comment',
          'option' => $option,
          'status' => $comment->$option,
          'className' => 'fasttoggle-status-comment-'. $group . '-' . $option .'-'. (1 - intval($comment->$option)),
        ));
        exit;
      }
      else {
        drupal_goto();
      }
    }
    else {
      // The action is not confirmed. The user came here through a regular link;
      // no AJAX was involved. That means, we need a confirmation form so that
      // we get a POST form.
      $labels = _fasttoggle_get_label('comment_' . $group . '_' . $option, FASTTOGGLE_LABEL_STATUS);
      return drupal_get_form('fasttoggle_comment_option_confirm', $comment, $labels[intval(!$comment->$option)]);
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
}


/**
 * Confirmation form for the option change of a comment.
 */
function fasttoggle_comment_option_confirm($form_state, $comment, $option) {
  return confirm_form(array(),
    t('Are you sure you want to set the comment %title to %option?', array('%title' => $comment->subject, '%option' => $option)),
    $_GET['destination'] ? $_GET['destination'] : 'comment/'. $comment->cid,
    '',
    t('Change'), t('Cancel')
  );
}


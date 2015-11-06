<?php

/**
 * @file
 * Administration functions for the unitag module.
 */

/**
 * Menu callback: Unitag module settings form.
 */
function unitag_settings_form() {
  $vocabularies = _unitag_get_vocabularies();

  if (empty($vocabularies)) {
    drupal_set_message(t('The unitag module requires at least one free-tagging enabled vocabulary to be defined.'), 'error');
    drupal_goto('admin/content/taxonomy');
  }

  $unitag_config = unitag_variable_get();

  foreach ($vocabularies as $vid => $name) {
    $collapsed = in_array($vid, $unitag_config['readonly']) ||
      in_array($vid, $unitag_config['lower']) ||
      in_array($vid, $unitag_config['sanitise']) ||
      in_array($vid, $unitag_config['synonym']) ||
      in_array($vid, $unitag_config['related']) ||
      in_array($vid, $unitag_config['ancestor']);

    $fid = 'unitag_'. $vid;
    $form[$fid] = array(
      '#type' => 'fieldset',
      '#title' => $name,
      '#collapsible' => TRUE,
      '#collapsed' => !$collapsed
    );
    $form[$fid]['readonly'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable read-only mode'),
      '#description' => t('Limit free-tagging to existing terms only. Any suggested terms will be stored in a moderation queue and can be managed from the <em>Manage suggestions</em> tab.'),
      '#default_value' => in_array($vid, $unitag_config['readonly']),
      '#parents' => array('unitag', 'readonly', $vid)
    );
    $form[$fid]['lower'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force lower-case'),
      '#description' => t('Convert all tags (new and old) to lower-case prior to save.'),
      '#default_value' => in_array($vid, $unitag_config['lower']),
      '#parents' => array('unitag', 'lower', $vid)
    );
    $form[$fid]['sanitise'] = array(
      '#type' => 'checkbox',
      '#title' => t('Sanitise tags'),
      '#description' => t('At the moment, sanitisation only involves trimming excess whitespace within the string. This is applied to both new and old tags.'),
      '#default_value' => in_array($vid, $unitag_config['sanitise']),
      '#parents' => array('unitag', 'sanitise', $vid)
    );
    $form[$fid]['synonym'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable synonym replacement'),
      '#description' => t('Synonyms will be automatically replaced by the base term.'),
      '#default_value' => in_array($vid, $unitag_config['synonym']),
      '#parents' => array('unitag', 'synonym', $vid)
    );
    $form[$fid]['related'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable related terms insertion'),
      '#description' => t('Categorising a node under a term will also categorise it under any related terms.'),
      '#default_value' => in_array($vid, $unitag_config['related']),
      '#parents' => array('unitag', 'related', $vid)
    );
    $form[$fid]['ancestor'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable ancestral terms insertion'),
      '#description' => t('Categorising a node under a term will also categorise it under each of its ancestral terms (parents, grandparents etc.).'),
      '#default_value' => in_array($vid, $unitag_config['ancestor']),
      '#parents' => array('unitag', 'ancestor', $vid)
    );
  }

  $form['#validate'][] = 'unitag_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Validate unitag settings form submission.
 */
function unitag_settings_form_validate($form, &$form_state) {
  foreach ($form_state['values']['unitag'] as $key => $value) {
    $form_state['values']['unitag'][$key] = array_filter($value);
    form_set_value(array('#parents' => array('unitag', $key)), array_keys($form_state['values']['unitag'][$key]), $form_state);
  }
}

/**
 * Menu callback: Display the suggestion management form.
 *
 * @return $form
 */
function unitag_manage_suggestions_form() {
  // Display suggestions table.
  $form['term_suggestions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Term suggestions'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  // Grab available suggestions.
  $suggestions = unitag_suggestions_get();
  $utids = array();
  foreach ($suggestions as $suggestion) {
    $utids[$suggestion->utid] = '';
    $form['term_suggestions']['suggestion'][$suggestion->utid] = array('#value' => check_plain($suggestion->name));
    $form['term_suggestions']['nid'][$suggestion->utid] = array('#type' => 'value', '#value' => $suggestion->nid);
    $form['term_suggestions']['node_title'][$suggestion->utid] = array('#type' => 'value', '#value' => $suggestion->title);
    $form['term_suggestions']['vid'][$suggestion->utid] = array('#value' => $suggestion->vid);
    $form['term_suggestions']['root'][$suggestion->utid] = array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'taxonomy/autocomplete/'. $suggestion->vid,
      '#maxlength' => 255,
      '#size' => 24,
      '#parents' => array('root', $suggestion->utid)
    );
  }
  $form['term_suggestions']['utids'] = array('#type' => 'checkboxes', '#options' => $utids);

  $form['controls'] = array(
    '#type' => 'fieldset',
    '#description' => t('Approve or deny the selected suggestions. These operations will only work with vocabularies that support them.')
  );
  $form['controls']['operation'] = array(
    '#type' => 'select',
    '#title' => t('With selected'),
    '#options' => array(
      'new-term' => t('Approve as new terms'),
      'new-synonym' => t('Approve as synonyms of the root term'),
      'new-child' => t('Approve as children of the parent terms'),
      'deny' => t('Deny and delete'),
      'blacklist-current' => t('Deny and blacklist for the current vocabulary'),
      'blacklist-all' => t('Deny and blacklist for all vocabularies'),
    ),
    '#prefix' => '<div class="container-inline">'
  );

  $form['controls']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#suffix' => '</div>'
  );

  return $form;
}

/**
 * Validate the manage suggestions form.
 */
function unitag_manage_suggestions_form_validate($form, &$form_state) {
  $utids = array_filter($form_state['values']['utids']);
  if (empty($utids)) {
    form_set_error('', t('No suggestions were selected. Please select a suggestion and try again.'));
  }

  switch ($form_state['values']['operation']) {
    case 'new-synonym':
      foreach ($utids as $utid) {
        $tags = unitag_taxonomy_terms_parse_tags($form_state['values']['root'][$utid]);
        if (empty($tags)) {
          form_set_error("term_suggestions][root][$utid", t('The root term must be specified for synonyms.'));
        }
        else {
          // We only want 1 root term.
          form_set_value(array('#parents' => array('root', $utid)), $tags[0], $form_state);
        }
      }
      break;
    case 'new-child':
      foreach ($utids as $utid) {
        $tags = unitag_taxonomy_terms_parse_tags($form_state['values']['root'][$utid]);
        if (empty($tags)) {
          form_set_error("term_suggestions][root][$utid", t('The parent terms must be specified.'));
        }
        else {
          // Resave the parent terms.
          form_set_value(array('#parents' => array('root', $utid)), $tags, $form_state);
        }
      }
      break;
  }
}

/**
 * Process manage suggestions form submission.
 */
function unitag_manage_suggestions_form_submit($form, &$form_state) {
  $utids = array_filter($form_state['values']['utids']);
  foreach ($utids as $utid) {
    if ($suggestion = unitag_suggestion_load($utid)) {
      switch ($form_state['values']['operation']) {
        case 'new-term':
          // Save as a new term.
          $term = unitag_suggestion_save_term($suggestion);
          // Associate nodes that have made the suggestion with this term.
          unitag_association_register($suggestion, $term);
          // Delete all instances of this suggestion from the unitag table (for
          // this vocabulary).
          unitag_suggestion_delete($suggestion, TRUE);
          break;
        case 'new-synonym':
          $term = unitag_taxonomy_get_term_by_name($form_state['values']['root'][$utid], $suggestion->vid);
          if ($term) {
            unitag_suggestion_save_synonym($suggestion, $term);

            // Cast term as an array as _save_term functions return an array.
            $term = is_object($term) ? (array) $term : $term;
            // Associate nodes that have made the suggestion with the root term.
            unitag_association_register($suggestion, $term);

            // Delete all instances of this suggestion from the unitag table (for
            // this vocabulary).
            unitag_suggestion_delete($suggestion, TRUE);
          }
          else {
            drupal_set_message(t('Root term %term not found.', array('%term' => $form_state['values']['root'][$utid])), 'error');
          }
          break;
        case 'new-child':
          $parents = array();
          foreach ($form_state['values']['root'][$utid] as $tag) {
            $parent = unitag_taxonomy_get_term_by_name($tag, $suggestion->vid);
            if ($parent) {
              $parents[$parent->tid] = $parent->tid;
            }
            else {
              drupal_set_message(t('Parent term %term not found.', array('%term' => $tag)), 'error');
            }
          }
          $term = unitag_suggestion_save_term($suggestion, $parents);
          // Associate nodes that have made the suggestion with the root term.
          unitag_association_register($suggestion, $term);
          // Delete all instances of this suggestion from the unitag table (for
          // this vocabulary).
          unitag_suggestion_delete($suggestion, TRUE);
          break;
        case 'deny':
          unitag_suggestion_delete($suggestion);
          break;
        case 'blacklist-current':
          unitag_suggestion_blacklist($suggestion);
          break;
        case 'blacklist-all':
          unitag_suggestion_blacklist($suggestion, TRUE);
          break;
      }
    }
  }

  drupal_set_message(t('Suggestions updated successfully.'));
}

/**
 * Menu callback: Display the suggestion edit form.
 *
 * @return $form
 */
function unitag_edit_suggestion_form(&$form_state, $utid) {
  $form = array();

  $suggestion = unitag_suggestion_load($utid);

  if ($suggestion) {
    drupal_set_title(t('Edit suggestion: %name', array('%name' => $suggestion->name)));
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('New name'),
      '#default_value' => $suggestion->name,
      '#required' => TRUE
    );
    $form['suggestion'] = array('#type' => 'value', '#value' => $suggestion);

    $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));
  }

  return $form;
}

/**
 * Process the suggestion edit form submission.
 */
function unitag_edit_suggestion_form_submit($form, &$form_state) {
  db_query("UPDATE {unitag} SET name = '%s' WHERE utid = %d", trim($form_state['values']['name']), $form_state['values']['suggestion']->utid);
  drupal_set_message(t('Suggestion %oldname updated to %newname.', array('%oldname' => $form_state['values']['suggestion']->name, '%newname' => $form_state['values']['name'])));

  $form_state['redirect'] = 'admin/content/taxonomy/unitag/manage';
}

/**
 * Menu callback: Display the suggestion management form.
 *
 * @return $form
 */
function unitag_manage_blacklist_form() {
  // Display blacklist table.
  $form['term_blacklist'] = array();

  // Grab available suggestions.
  $suggestions = unitag_suggestions_get(TRUE);
  $utids = array();
  foreach ($suggestions as $suggestion) {
    $utids[$suggestion->utid] = '';
    $form['term_blacklist']['suggestion'][$suggestion->utid] = array('#value' => check_plain($suggestion->name));
    $form['term_blacklist']['vid'][$suggestion->utid] = array('#value' => $suggestion->vid);
  }
  $form['term_blacklist']['utids'] = array('#type' => 'checkboxes', '#options' => $utids);

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove')
  );

  return $form;
}

/**
 * Validate the manage blacklist form.
 */
function unitag_manage_blacklist_form_validate($form, &$form_state) {
  $utids = array_filter($form_state['values']['utids']);
  if (empty($utids)) {
    form_set_error('', t('No blacklisted terms were selected. Please select one or more and try again.'));
  }
}

/**
 * Process manage blacklist form submission.
 */
function unitag_manage_blacklist_form_submit($form, &$form_state) {
  $utids = array_filter($form_state['values']['utids']);
  foreach ($utids as $utid) {
    if ($suggestion = unitag_suggestion_load($utid)) {
      unitag_suggestion_delete($suggestion);
    }
  }

  drupal_set_message(t('Blacklisted term(s) successfully updated.'));
}

/**
 * Theme the manage suggestions form.
 */
function theme_unitag_manage_suggestions_form($form) {
  $rows = array();

  if (isset($form['term_suggestions']['suggestion'])) {
    foreach ($form['term_suggestions']['suggestion'] as $utid => $values) {
      if (is_numeric($utid)) {
        if ($form['term_suggestions']['vid'][$utid]['#value']) {
          $vocabulary = taxonomy_vocabulary_load($form['term_suggestions']['vid'][$utid]['#value']);
          $vocabulary_link = l($vocabulary->name, 'admin/content/taxonomy/'. $vocabulary->vid);
        }
        else {
          // Faux vocabulary with vid 0 which indicates "all" vocablaries.
          $vocabulary_link = t('All');
        }

        $rows[] = array(
          drupal_render($form['term_suggestions']['utids'][$utid]),
          drupal_render($form['term_suggestions']['suggestion'][$utid]),
          l($form['term_suggestions']['nid'][$utid]['#value'], 'node/'. $form['term_suggestions']['nid'][$utid]['#value'], array('attributes' => array('title' => check_plain($form['term_suggestions']['node_title'][$utid]['#value'])))),
          $vocabulary_link,
          drupal_render($form['term_suggestions']['root'][$utid]),
          l(t('Edit'), 'admin/content/taxonomy/unitag/manage/'. $utid .'/edit')
        );
      }
    }
  }
  if (!empty($rows)) {
    $header = array(theme('table_select_header_cell'), t('Name'), t('Node'), t('Vocabulary'), t('Root / Parents'), t('Operations'));
    $output = theme('table', $header, $rows);
    $output .= theme('pager', NULL, 25, 0);
    // Stick table inside the fieldset.
    $form['term_suggestions']['#children'] = $output;
    $output = drupal_render($form);
  }
  else {
    $output = t('No suggestions found.');
  }

  return $output;
}

/**
 * Theme the manage blacklist form.
 */
function theme_unitag_manage_blacklist_form($form) {
  $rows = array();

  if (isset($form['term_blacklist']['suggestion'])) {
    foreach ($form['term_blacklist']['suggestion'] as $utid => $values) {
      if (is_numeric($utid)) {
        if ($form['term_blacklist']['vid'][$utid]['#value']) {
          $vocabulary = taxonomy_vocabulary_load($form['term_blacklist']['vid'][$utid]['#value']);
          $vocabulary_link = l($vocabulary->name, 'admin/content/taxonomy/'. $vocabulary->vid);
        }
        else {
          // Faux vocabulary with vid 0 which indicates "all" vocablaries.
          $vocabulary_link = t('All');
        }
        $rows[] = array(
          drupal_render($form['term_blacklist']['utids'][$utid]),
          drupal_render($form['term_blacklist']['suggestion'][$utid]), $vocabulary_link
        );
      }
    }
  }
  if (!empty($rows)) {
    $header = array(theme('table_select_header_cell'), t('Name'), t('Vocabulary'));
    $output = theme('table', $header, $rows);
    $output .= theme('pager', NULL, 50, 0);
    // Stick table inside the fieldset.
    $form['term_blacklist']['#children'] = $output;
    $output = drupal_render($form);
  }
  else {
    $output = t('No blacklisted terms found.');
  }

  return $output;
}

<?php
// $Id: og_vocab.admin.inc,v 1.1.2.3 2009/01/05 14:37:43 amitaibu Exp $

/**
 * @file
 * Admin page callbacks for the og_vocab module.
 */

/**
 * List and manage vocabularies for a given group.
 */
function og_vocab_admin_overview_vocabularies($form_state, $groupnode) {
  $gid = $groupnode->nid;
  drupal_set_title($groupnode->title);
  drupal_set_breadcrumb(drupal_get_breadcrumb());

  if (user_access('add own group vocabulary')) {
    $form['add_vocab'] = array(
      '#value' => l(t('Add vocabulary'), "node/$gid/og/vocab/add/vocabulary", array('query' => drupal_get_destination())),
    );
  }
  $vocabularies = og_vocab_get_vocabularies($gid);
  foreach ($vocabularies as $vid => $vocabulary) {
    $types = array();
    foreach ($vocabulary->nodes as $type) {
      $node_type = node_get_types('name', $type);
      $types[] = $node_type ? $node_type : $type;
    }
    $row = array(
      check_plain($vocabulary->name),
      check_plain(implode(', ', $types)),
    );
    if (user_access('edit own group vocabulary')) {
      $row[] = l(t('edit vocabulary'), "node/$gid/og/vocab/edit/vocabulary/$vocabulary->vid", array('query' => drupal_get_destination()));
    }
    $row[] = l(t('list terms'), "node/$gid/og/vocab/terms/$vocabulary->vid");
    if (user_access('edit own group term')) {
      $row[] = l(t('add terms'), "node/$gid/og/vocab/terms/$vocabulary->vid/add_term");
    }
    $rows[] = $row;
  }
  if (!$rows) {
    $rows[] = array(array('data' => t('No categories'), 'colspan' => 5));
  }
  $header = array(t('Name'), t('Type'), array('data' => 'Operations', 'colspan' => 3));
  $form['table'] =  array(
    '#value' => theme('table', $header, $rows),
  );
  return $form;
}
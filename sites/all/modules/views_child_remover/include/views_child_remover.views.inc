<?php
/// (C) Michael Moritz <miiimooo@drupal.org>
function views_child_remover_views_data() {
  $data['node']['child_remover'] = array(
    'group' => t('Book'),
    'title' => t('Child remover'),
    'help' => t('Removes book children'),
    'filter' => array(
      'handler' => 'child_remover_filter_handler',
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers
 */
function views_child_remover_views_handlers() {
  return array(
    'handlers' => array(
      'child_remover_filter_handler' => array(
        'parent' => 'views_handler_filter',
        'file' => 'include/child_remover_filter_handler.inc'
      ),
    ),
  );
}

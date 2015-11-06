<?php

/**
 * @file
 * Integrates IMCE into FileField.
 */

/**
 * Outputs the IMCE browser for FileField.
 */
function imce_filefield_page($content_type, $field_name) {
  global $conf;

  // Check access.
  if (!imce_access() || !($field = content_fields($field_name, $content_type)) || empty($field['widget']['imce_filefield_on'])) {
    return drupal_access_denied();
  }

  $conf['imce_custom_scan'] = 'imce_filefield_custom_scan';
  module_load_include('inc', 'imce', 'inc/imce.page');
  return imce();
}

/**
 * Custom scan callback for IMCE.
 */
function imce_filefield_custom_scan($dirname, &$imce) {
  return imce_scan_directory($dirname, $imce);
}

/**
 * Extends filefield_widget element.
 */
function _imce_filefield_field_process($element, $edit, &$form_state, $form) {
  static $access;
  if (!isset($access)) {
    $access = imce_access();
  }

  // Check imce access.
  if (!$access) {
    return $element;
  }

  // Check field instance.
  if (!isset($element['#field_name']) || !($field = content_fields($element['#field_name'], $element['#type_name']))) {
    return $element;
  }

  // Check settings.
  if (empty($field['widget']['imce_filefield_on'])) {
    return $element;
  }

  $element['imce_filefield']['fid'] = array(
    '#attributes' => array('id' => $element['#id'] . '-imce-filefield-fid'),
    '#type' => 'hidden',
    '#value' => '',
  );
  $element['imce_filefield']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Select'),
    '#validate' => array(),
    '#submit' => array('node_form_submit_build_node'),
    '#name' => $element['#name'] . '[imce_filefield][submit]',
    '#id' => $element['#id'] . '-imce-filefield-submit',
    '#attributes' => array('style' => 'display: none;'),
    '#ahah' => array(
      'path' => 'filefield/ahah/' . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $element['#delta'],
      'wrapper' => $element['#id'] .'-ahah-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $element;
}

/**
 * Field value callback.
 */
function _imce_filefield_field_value($element, &$item) {
  if ($file = field_file_load($item['imce_filefield']['fid'])) {
    if (imce_filefield_element_validate($element, (object) $file)) {
      $item = array_merge($item, $file);
    }
  }
  else {
    form_error($element, t('The referenced file could not be used because the file does not exist in the database.'));
  }
  $item['imce_filefield']['fid'] = '';
}

/**
 * Pre render callback. Add page elements.
 */
function _imce_filefield_field_pre_render(&$element) {
  static $done;
  if (!empty($element['fid']['#value'])) {
    $element['imce_filefield']['#access'] = FALSE;
  }
  if (!isset($done[$element['#id']])) {
    $done[$element['#id']] = TRUE;
    imce_filefield_jscss();
    $jset['imce_filefield']['fields'][$element['#id']] = array('path' => $element['#type_name'] . '/' . $element['#field_name'] . '/' . $element['#delta']);
    drupal_add_js($jset, 'setting');
  }
}

/**
 * Validate a file with element validators.
 */
function imce_filefield_element_validate($element, $file) {
  $errors = array();

  // Check download access first
  $scheme = variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC);
  if ($scheme != FILE_DOWNLOADS_PUBLIC && !imce_filefield_download_access($file)) {
    form_error($element, t('You do not have permission to use the selected file.'));
    return FALSE;
  }

  // Validate.
  foreach ($element['#upload_validators'] as $function => $args) {
    array_unshift($args, NULL);
    $args[0] = &$file;
    $errors = array_merge($errors, call_user_func_array($function, $args));
  }

  // Set errors.
  if (!empty($errors)) {
    $message = isset($errors[1]) ? '<ul><li>' . implode('</li><li>', $errors) . '</li></ul>' : $errors[0];
    form_error($element, $message);
    return FALSE;
  }

  return TRUE;
}

/**
 * Adds main js, css files and settings.
 */
function imce_filefield_jscss() {
  static $done;
  if (!isset($done)) {
    $done = TRUE;
    $path = drupal_get_path('module', 'imce_filefield');
    drupal_add_css($path . '/imce_filefield.css');
    drupal_add_js($path . '/imce_filefield.js');
    drupal_add_js(array('imce_filefield' => array('url' => url('imce-filefield'), 'token' => drupal_get_token('imce_filefield'))), 'setting');
  }
}

/**
 * IMCE filefield operation.
 */
function _imce_js_imce_filefield(&$imce) {
  if (isset($_GET['token']) && drupal_valid_token($_GET['token'], 'imce_filefield')) {
    if (isset($_GET['filename']) && ($filename = rawurldecode($_GET['filename'])) && isset($imce['files'][$filename])) {
      $filepath = file_directory_path() . ($imce['dir'] == '.' ? '' : '/' . $imce['dir']) . '/' . $filename;
      // File is already in DB.
      if ($file = db_fetch_object(db_query("SELECT * FROM {files} WHERE filepath = '%s'", $filepath))) {
        if (field_file_references($file) < 1) {
          imce_file_register($file);
        }
      }
      // Add new entry for the file.
      else {
        $file = (object) array(
          'uid' => $imce['uid'],
          'filename' => $filename,
          'filepath' => $filepath,
          'filesize' => $imce['files'][$filename]['size'],
          'timestamp' => $imce['files'][$filename]['date'],
          'filemime' => file_get_mimetype($filepath),
          'status' => 1,
        );
        if (drupal_write_record('files', $file)) {
          imce_file_register($file);
        }
      }
      return array('fid' => empty($file->fid) ? 0 : $file->fid);
    }
  }
}

/**
 * Check download access to a file.
 */
function imce_filefield_download_access($file) {
  $headers = array();
  foreach (module_implements('file_download') as $module) {
    $function = $module . '_file_download';
    if ($result = $function($file->filepath)) {
      if ($result == -1) {
        return FALSE;
      }
      if (is_array($result)) {
        $headers = array_merge($headers, $result);
      }
    }
  }
  return !empty($headers);
}
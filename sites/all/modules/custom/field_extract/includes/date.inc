<?php
// $Id$

/**
 * @file
 * field_extract support for date field
 */

function field_extract_date_info() {
  return array(
    'callback' => 'field_extract_date_extract',
    'filepath' => drupal_get_path('module', 'field_extract') . '/includes/date.inc',
    'options' => array(
      'key' => 'format',
      'format' => 'medium',
    ),
  );
}

function field_extract_date_extract($item, $options = array()) {
  $date = date_formatter_process($options['format'], $options['parent_entity_type'],
    $options['parent_entity'], $options['field_info'], $options['instance_info'],
    $options['langcode'], $item, array());

  $timestamp = strtotime($date['value']['local']['datetime']);

  if ($options['key']=='format') {
    switch ($options['format']) {
      case 'timestamp':
        $value = $timestamp;
        break;
      default:
        $value = format_date($timestamp, $options['format']);
        break;
    }
  }
  else {
    $value = $item[$options['key']?$options['key']:'value'];
  }

  return $value;
}

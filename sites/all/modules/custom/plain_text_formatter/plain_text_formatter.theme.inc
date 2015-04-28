<?php

function theme_plain_text_formatter($variables) {
  $value = check_plain($variables['value']);
  $output = '';
  switch($variables['line_break']) {
    case 'br':
      $output = preg_split("/[\r\n]+/", $value, -1, PREG_SPLIT_NO_EMPTY);
      $output = implode('<br />', $output);
      break;
    case 'p':
      $output = preg_split("/[\r\n]+/", $value, -1, PREG_SPLIT_NO_EMPTY);
      $output = implode('</p><p>', $output);
      $output = '<p>'.$output.'</p>';
      break;
    case '_none':
    default:
      $output = $value;
  }
  return $output;
}
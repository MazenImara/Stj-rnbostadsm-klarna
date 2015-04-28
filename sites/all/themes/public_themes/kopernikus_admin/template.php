<?php

/**
 * Preprocessor for theme('html').
 */
function kopernikus_admin_preprocess_html(&$vars) {
  //Lägg till metatagg som ser till att IE körs i standards mode
  $ie_tag = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'http-equiv' => 'X-UA-Compatible', 
      'content' => 'IE=Edge',
    ),
  );
  drupal_add_html_head($ie_tag, 'ie_tag');
}

/**
 * Preprocessor for theme('page').
 */
function kopernikus_admin_preprocess_page(&$vars) {
  // Process local tasks. Only do this processing if the current theme is
  // indeed Rubik. Subthemes must reimplement this call.
  global $theme;
  if ($theme === 'kopernikus_admin') {
    _rubik_local_tasks($vars);
  }
}

function kopernikus_admin_theme() {
  $items = array();
  // Content theming.
  $items['help'] =
  $items['node'] =
  $items['comment'] =
  $items['comment_wrapper'] = array(
    'path' => drupal_get_path('theme', 'kopernikus_admin'),
    'template' => 'object',
  );

  return $items;
}
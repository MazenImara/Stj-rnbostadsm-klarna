<?php

function parallax_scroll_check_paths() {
  $page_match = true; //Om inga begränsningar har angivits så ska funktionen returnera true.
  $config = variable_get('parallax_scroll_config', array());
  if(!empty($config['visibility']['pages_list'])) {
    $pages = $config['visibility']['pages_list'];
    // Convert path to lowercase. This allows comparison of the same path
    // with different case. Ex: /Page, /page, /PAGE.
    $pages = drupal_strtolower($pages);
    $visibility = $config['visibility']['pages_setting'];
    // Convert the Drupal path to lowercase
    $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
    // Compare the lowercase internal and lowercase path alias (if any).
    $page_match = drupal_match_path($path, $pages);
    if ($path != $_GET['q']) {
      $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
    }
    // When $block->visibility has a value of 0 (BLOCK_VISIBILITY_NOTLISTED),
    // the block is displayed on all pages except those listed in $block->pages.
    // When set to 1 (BLOCK_VISIBILITY_LISTED), it is displayed only on those
    // pages listed in $block->pages.
    $page_match = !($visibility xor $page_match);
  }
  return $page_match;
}

function parallax_scroll_render() {
  if(parallax_scroll_check_paths()) {
    $layers = variable_get('parallax_scroll_layers', array());
    if($layers) {
      drupal_add_css(drupal_get_path('module', 'parallax_scroll').'/css/parallax_scroll.css');
      drupal_add_js(
        array(
          'parallax_scroll' => array(
            'layers' => $layers,
          ),
        ), 
      'setting');
      if ($path = libraries_get_path('jquery.mousewheel')) {
        drupal_add_js($path.'/jquery.mousewheel.min.js');
      }
      if ($path = libraries_get_path('scrollto')) {
        drupal_add_js($path.'/jquery.scrollTo.min.js');
      }
      drupal_add_js(drupal_get_path('module', 'parallax_scroll').'/js/parallax_scroll.js');
    }
  }
}
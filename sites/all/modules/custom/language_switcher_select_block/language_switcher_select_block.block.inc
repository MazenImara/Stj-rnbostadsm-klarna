<?php

function language_switcher_select_block_content() {
  $content = &drupal_static(__FUNCTION__);
  if (!isset($content)) {
    $content = _language_switcher_select_block_content();
  }
  return $content;
}
function _language_switcher_select_block_content() {
  $content = array();
  if (drupal_multilingual()) {
    $path = drupal_is_front_page() ? '<front>' : $_GET['q'];
    $links = language_negotiation_get_switch_links('language', $path);
    if (!empty($links->links)) {
      $content = array(
        '#theme' => 'language_switcher_select_block',
        '#links' => $links->links,
      );
    }
  }
  return render($content);
}

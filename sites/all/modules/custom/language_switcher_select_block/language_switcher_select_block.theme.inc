<?php

function theme_language_switcher_select_block($variables) {
  $output = '';
  if(!empty($variables['links'])) {
    global $language;
    $links = $variables['links'];
    $content = array(
      '#theme' => 'select',
      '#options' => array(),
      '#value' => $language->language,
      '#attributes' => array(
        'class' => array('language-switcher-select'),
      ),
    );
    $settings = array();
    foreach($links as $langcode => $link) {
      if($link) {
        $content['#options'][$langcode] = $link['language']->native;
        $settings[$langcode] = url($link['href'], array('language' => $link['language']));
      }
    }
    $content['#attached'] = array(
      'js' => array(
        array(
          'data' => array('language_switcher_select_block' => $settings),
          'type' => 'setting',
        ),
        drupal_get_path('module', 'language_switcher_select_block').'/js/language_switcher_select_block.js',
      ),
    );
    $output = render($content);
  }
  return $output;
}
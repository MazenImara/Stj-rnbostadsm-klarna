<?php
function hero_alpha_preprocess_html(&$vars) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Lato:400,700,900&subset=latin,latin-ext', array('type' => 'external'));
  drupal_add_css('http://fonts.googleapis.com/css?family=Raleway:300,500,600,700', array('type' => 'external'));
  //Lägg till heroscriptet på startsidan
  if(!empty($vars['is_front'])) {
    drupal_add_js(drupal_get_path('theme', 'hero') . '/js/hero.js');
  }
  //Lägg till klass om det finns en sidhuvudbild
  $node = menu_get_object();
  if($node) {
    $header_image_fields = array(
      'field_sidhuvud',
      'field_image',
    );
    $has_header_image = false;
    foreach($header_image_fields as $field_name) {
      if(!empty($vars['page']['content']['content']['content']['system_main']['nodes'][$node->nid][$field_name])) {
        $has_header_image = true;
        break;
      }
    }
    if($has_header_image) {
      $vars['classes_array'][] = 'has-header-image';
      $vars['attributes_array']['class'][] = 'has-header-image';
    }
  }
}
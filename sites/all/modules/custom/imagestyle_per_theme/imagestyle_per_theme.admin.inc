<?php
/**
 * Menu callback; Listing of all current image styles.
 */
function imagestyle_per_theme_image_style_list() {
  //Tar bort alla temaspecifika image styles förutom de som hör till det nuvarande defaulttemat
  module_load_include('inc', 'image', 'image.admin');
  $page = array();
  $themes = list_themes();
  $default_theme = variable_get('theme_default', '');
  $styles = image_styles();
  foreach($themes as $t_name => $theme) {
    if($t_name === $default_theme) continue;
    foreach($styles as $s_name => $style) {
      if(substr($s_name, 0, (strlen($t_name) + 1)) === $t_name.'_') {
        unset($styles[$s_name]);
      }
    }
  }
  $page['image_style_list'] = array(
    '#markup' => theme('image_style_list', array('styles' => $styles)),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'image') . '/image.admin.css' => array()),
    ),
  );

  return $page;
}

<?php
function kopernikus_alpha_preprocess_html(&$vars) {
  //Lägg till klass i body för sidtyp
  $menu_object = menu_get_object();
  if (isset($menu_object->field_sidtyp)) {
    if(function_exists('field_extract_value')) {
      $value = field_extract_value('node', $menu_object, 'field_sidtyp');
      if($value) {
        $class = 'sidtyp-'.str_replace('_', '-', strtolower($value->name));
        $vars['classes_array'][] = $class;
        if(isset($vars['attributes_array'])) {
          $vars['attributes_array']['class'][] = $class;
        }
      }
    }
  }
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
<?php
function kopernikus_alpha_preprocess_region(&$vars) {
  /*----------Gör det möjligt att rendera sidhuvudet i valfri region----------*/
  $menu_object = menu_get_object();
  if (isset($menu_object->type)) {
    $vars['theme_hook_suggestions'][] = 'region__'.$vars['region'].'__type__'.$menu_object->type;
    if(isset($menu_object->field_sidhuvud)) {
      if(!arg(2)) {
        $vars['field_sidhuvud'] = field_view_field('node', $menu_object, 'field_sidhuvud', 'default');
      }
    }
  }
  //dpm($vars['theme_hook_suggestions']);
}
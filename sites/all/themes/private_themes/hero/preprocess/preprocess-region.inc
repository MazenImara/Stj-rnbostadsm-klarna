<?php
function hero_alpha_preprocess_region(&$vars) {
  /*----------G�r det m�jligt att rendera titeln f�r startsidan i branding----------*/
  if(!empty($vars['is_front']) && $vars['region'] === 'branding') {
    global $theme_key;
    $node = menu_get_object();
    if($node) {    
      $title = field_view_field('node', $node, 'title_field', "{$theme_key}_full");
      $title['#contextual_links'] = array(
        'node' => array('node', array($node->nid)),
      );
      $vars['branding_title'] = render($title);
    }
  }
}
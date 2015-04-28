<?php
function kopernikus_alpha_preprocess_node(&$vars) {
  global $theme_key;
  $vars['active_theme'] = $theme_key;
  $trimmed_view_mode = $vars['view_mode'];
  if(substr($trimmed_view_mode, 0, strlen($theme_key)) === $theme_key) {
    $trimmed_view_mode = substr($trimmed_view_mode, strlen($theme_key) + 1);
  }
  $vars['trimmed_view_mode'] = $trimmed_view_mode;
  $node = $vars['node'];
  //Hanterar template suggestions
  $vars['theme_hook_suggestions'] = array();
  $vars['theme_hook_suggestions'][] = 'node__type__'.$node->type;
  $node_url = explode('/', $vars['node_url']);
  $node_url = array_pop($node_url);
  $vars['theme_hook_suggestions'][] = 'node__alias__'.$node_url;
  $vars['theme_hook_suggestions'][] = 'node__nid__'.$node->nid;

  /* Det här verkar inte längre behövas i och med Smart Trim som lägger till en egen länk
  //Lägger till "Läs mer"-länk för promo display
  global $theme_key;
  if($vars['view_mode'] === $theme_key . '_promo') {
    if(!isset($vars['content']['links']['node']['#links']['node-readmore'])) {
      $vars['content']['links']['node']['#links']['node-readmore'] = array(
        'title' => t('read more'),
        'href' => $GLOBALS['base_root'].$vars['node_url'],
        'attributes' => array(
          'rel' => 'tag',
          'title' => '',
        ),
      );
    }
  }
  */

  $vars['classes_array'][] = "view-mode-$trimmed_view_mode";
  $vars['attributes_array']['class'][] = "view-mode-$trimmed_view_mode";

  //Lägger till javascript för bildtext i galleribilder - än så länge gör vi bara det för kopernikustemat eftersom mallteman designas väldigt specifikt.
  if($theme_key === 'kopernikus') {
    if($node->type === 'galleribild' && $trimmed_view_mode === 'teaser') {
      drupal_add_js(drupal_get_path('theme', 'kopernikus').'/js/galleribildtext_width.js');
    }
  }
  //dpm($vars['theme_hook_suggestions']);
}
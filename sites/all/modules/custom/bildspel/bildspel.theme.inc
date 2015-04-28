<?php

function theme_bildspel($variables) {
  $items = $variables['items'];
  foreach($items as $i => $item) {
    $items[$i] = array(
      'data' => drupal_render($item),
    );
    if($i == 0) {
      $items[$i]['class'] = array('active');
    }
  }
  $elements = array(
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'bildspel').'/css/bildspel.css'),
      'js' => array(
        drupal_get_path('module', 'bildspel').'/js/bildspel.js' => array('scope' => 'footer'),
        array('data' => array('bildspel' => $variables['slider_settings']), 'type' => 'setting'),
      ),
    ),
    '#theme' => 'item_list',
    '#items' => $items,
    '#type' => 'ul',
    '#attributes' => array(
      'class' => array(
        'bildspel',
      ),
    ),
  );
  if($variables['slider_settings']['skin']) {
    $skin = $variables['slider_settings']['skin'];
    $elements['#attached']['css'][] = drupal_get_path('module', 'bildspel')."/skins/$skin/$skin.css";
  }
  return drupal_render($elements);
}
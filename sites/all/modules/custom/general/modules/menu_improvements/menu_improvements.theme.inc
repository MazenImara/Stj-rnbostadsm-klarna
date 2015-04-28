<?php

/**
 * Theme current menu links table.
 */
function theme_menu_improvements_current_links($variables) {
  $form = $variables['element'];

  $rows = array();
  foreach (element_children($form) as $key) {
    // Unset field titles and descriptions.
    foreach (array('link_title', 'description', 'parent', 'weight') as $field_name) {
      unset(
        $form[$key][$field_name]['#title'],
        $form[$key][$field_name]['#description']
      );
    }

    $rows[] = array(
    	array(
				'data' => drupal_render($form[$key]['link_title']) . drupal_render($form[$key]['description']),
				'class' => 'menu-improvements-current-links-title',
			),
      array(
      	'data' => drupal_render($form[$key]['parent']),
      	'class' => 'menu-improvements-current-links-parent',
			),
      drupal_render($form[$key]['weight']),
      drupal_render($form[$key]['remove']),
    );
  }  

  $headers = array(
    t('Menu link title'),
    t('Parent'),
    t('Weight'),
    t('Operations'),
  );

  return theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'menu-improvements-current-links')));
}
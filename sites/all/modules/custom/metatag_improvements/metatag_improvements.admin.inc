<?php
function metatag_improvements_config_overview() {
  module_load_include('inc', 'metatag', 'metatag.admin');
  $arrbody = array();

  $arrbody = array_merge($arrbody, drupal_get_form('metatag_improvements_admin_form'));
  if(function_exists('metatag_config_overview')) {
    $arrbody = array_merge($arrbody, metatag_config_overview());
  }

  return $arrbody;
}
function metatag_improvements_admin_form() {
  drupal_add_css(drupal_get_path('module', 'metatag_improvements').'/css/metatag_improvements_admin.css');
  drupal_add_js(drupal_get_path('module', 'metatag_improvements').'/js/metatag_improvements_admin.js');
  $types = node_type_get_names();
  $form['metatag_improvements_allowed_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display meta tags for these content types'),
    '#default_value' => variable_get('metatag_improvements_allowed_types', array_keys($types)),
    '#options' => $types,
    '#weight' => 1,
  );
  //Lägg in sidtyper
  $names = taxonomy_vocabulary_get_names();
  if(isset($names['sidtyper'])) {
    $vid = $names['sidtyper']->vid;
    $vocabulary = taxonomy_get_tree($vid);
    $terms = array();
    foreach($vocabulary as $term) {
      $terms[$term->tid] = $term->name;
    }
    $form['metatag_improvements_allowed_sidtyp'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Page types').':',
      '#default_value' => variable_get('metatag_improvements_allowed_sidtyp', array_keys($terms)),
      '#options' => $terms,
      '#weight' => 1,
    );
  }
  $form['metatag_improvements_title_maxlength'] = array(
    '#type' => 'textfield',
    '#title' => t('Max length for title tag'),
    '#default_value' => variable_get('metatag_improvements_title_maxlength', 65),
    '#weight' => 2,
  );
  $form['metatag_improvements_description_maxlength'] = array(
    '#type' => 'textfield',
    '#title' => t('Max length for description tag'),
    '#default_value' => variable_get('metatag_improvements_description_maxlength', 155),
    '#weight' => 3,
  );
  return system_settings_form($form);
}
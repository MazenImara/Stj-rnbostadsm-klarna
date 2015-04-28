<?php
function metatag_improvements_views_metatags_form($form, &$form_state, $view, $display_name) {
  $instance = 'views:' . $view->name;
  $default_metatags = metatag_config_load_with_defaults($instance);
  $defaults['title'] = array(
    'value' => $default_metatags['title']['value'],
    'maxlength' => variable_get('metatag_improvements_title_maxlength', 65),
  );
  $defaults['description'] = array(
    'value' => $default_metatags['description']['value'],
    'maxlength' => variable_get('metatag_improvements_description_maxlength', 155),
  );
  drupal_add_js(drupal_get_path('module', 'metatag_improvements').'/js/metatag_improvements.js');
  
  $js = '
    function metatag_improvements_get_defaults(type) {
      switch(type) {';
      foreach($defaults as $type => $fields) {
        $js .= "
        case '$type':
          defaults = new Array();
          defaults['value'] = '".$fields['value']."';
          defaults['maxlength'] = '".$fields['maxlength']."';
          return defaults;
        break;
        ";
      }
    $js .= '
      }
    }
  ';
  drupal_add_js($js, 'inline');
  if(isset($form['metatags']['title'])) {
    $form['metatags']['title']['value']['#maxlength'] = $defaults['title']['maxlength'];
  }
  if(isset($form['metatags']['description'])) {
    $form['metatags']['description']['value']['#maxlength'] = $defaults['description']['maxlength'];
  }


  $metatags = $view->display[$display_name]->display_options['metatags'];
  $options['token types'] = array('view');
  metatag_metatags_form($form, $instance, $metatags, $options);
  $form['#title'] = t('Meta Tags for !s', array('!s' => $view->display[$display_name]->display_options['title']));
  drupal_set_title($form['#title']);
  $form['metatags']['#type'] = 'container';
  $form['metatags']['#weight'] = 0;

  $form['#submit'][] = 'metatag_improvements_views_metatags_form_submit';
  $form_state['storage']['view'] = $view;
  $form_state['storage']['display_name'] = $display_name;
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );
  return $form;
}
function metatag_improvements_views_metatags_form_submit($form, &$form_state) {
  $view = $form_state['storage']['view'];
  $display_name = $form_state['storage']['display_name'];
  $instance = 'views:' . $view->name;
  foreach($form_state['values']['metatags'] as $name => &$metatag) {
    $value = $metatag['value'];
    if(!$value) {
      $defaults = metatag_config_load_with_defaults($instance);
      $metatag = $defaults[$name];
    }
  }
  $metatags = $form_state['values']['metatags'];
  metatag_filter_values_from_defaults($metatags);
  $view->display[$display_name]->display_options['metatags'] = $metatags;
  $view->save();
  drupal_set_message(t('Changes have been saved.'));
}
<?php

function bildspel_get_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  module_load_include('inc', 'bildspel', 'bildspel');
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $slider_settings = $settings['slider_settings'];
  $item_settings = (array) json_decode($settings['item_settings']);
  $elements = array();
  $elements['slider_settings'] = array(
    '#type' => 'fieldset',
    '#tree' => true,
    '#title' => t('Slider settings'),
  );
  $elements['slider_settings']['frame_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Frame length'),
    '#description' => t('Enter frame length in seconds for automatic sliding. Enter zero for no automatic sliding.'),
    '#field_suffix' => '<span> '.t('seconds').'</span>',
    '#default_value' => $slider_settings['frame_length'],
  );
  $options = array(
    'slide_horizontal' => t('Slide horizontally'),
    'fade' => t('Fade'),
  );
  $elements['slider_settings']['transition'] = array(
    '#type' => 'select',
    '#title' => t('Transition'),
    '#options' => $options,
    '#default_value' => $slider_settings['transition'],
  );
  
  $options = bildspel_get_skins();
  $elements['slider_settings']['skin'] = array(
    '#type' => 'select',
    '#title' => t('Skin'),
    '#options' => $options,
    '#empty_option' => t('None'),
    '#default_value' => $slider_settings['skin'],
  );
  $elements['slider_settings']['navigation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Arrows for navigation'),
    '#default_value' => $slider_settings['navigation'],
  );
  $elements['slider_settings']['pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dots for pager'),
    '#default_value' => $slider_settings['pager'],
  );
  $options = array();
  $field_type = $field['type'];
  $field_formatters = field_info_formatter_types();
  unset($field_formatters['bildspel']);
  foreach($field_formatters as $name => $formatter_data) {
    if(!empty($formatter_data['field types'])) {
      $formatter_data['field types'] = array_flip($formatter_data['field types']);
      if(isset($formatter_data['field types'][$field_type])) {
        $options[$name] = $formatter_data['label'];
      }
    }
  }
  if($options) {
    $elements['#attached']['js'][] = drupal_get_path('module', 'bildspel').'/js/bildspel_admin.js';
    $elements['item_display'] = array(
      '#type' => 'select',
      '#title' => t('View mode for slider items'),
      '#options' => $options,
      '#default_value' => $settings['item_display'],
      '#ajax' => array(
        'callback' => 'bildspel_ajax_update_item_settings',
        'wrapper' => 'item-settings-form',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#field' => $field,
      '#instance' => $instance,
      '#view_mode' => $view_mode,
    );

    $elements['item_settings'] = array(
      '#type' => 'hidden',
      '#attributes' => array('id' => 'item-settings'),
      '#default_value' => json_encode($item_settings),
    );

    $elements['item_settings_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Item settings'),
    );

    $elements['item_settings_fieldset']['item_settings'] = array(
      '#type' => 'container',
      '#attributes' => array('id' => 'item-settings-form'),
    );
    $selected = $settings['item_display'];
    if(!$selected) {
      $selected = array_shift(array_flip(array_slice($options, 0, 1)));
    }
    //Lägg in settings för det aktuella valet
    $item_display_instance = $instance;
    $item_display_instance['display'][$view_mode]['type'] = $selected;
    $item_settings_form = bildspel_get_item_settings_form($selected, $field, $item_display_instance, $view_mode, $form, $form_state);
    $elements['item_settings_fieldset']['item_settings'] = array_merge($elements['item_settings_fieldset']['item_settings'], $item_settings_form);
    //Sätt eventuella sparade värden
    foreach(element_children($elements['item_settings_fieldset']['item_settings']) as $element_name) {
      if(isset($item_settings[$element_name])) {
        $elements['item_settings_fieldset']['item_settings'][$element_name]['#default_value'] = $item_settings[$element_name];
      }
    }
  }
  else {
    $elements['no_options'] = array(
      '#markup' => t('There are no field formatters available for this field type. Please activate at least one before continuing.'),
    );
  }

  return $elements;
}

function bildspel_get_item_settings_form($item_display_name, $field, $instance, $view_mode, $form, &$form_state) {
  $item_settings_function = "{$item_display_name}_field_formatter_settings_form";
  $item_settings_form = call_user_func_array($item_settings_function, array($field, $instance, $view_mode, $form, $form_state));
  return $item_settings_form;
}
//Ajax callback for field formatter settings form
function bildspel_get_ajax_update_item_settings($form, &$form_state) {
  $field = $form_state['triggering_element']['#field'];
  $instance = $form_state['triggering_element']['#instance'];
  $view_mode = $form_state['triggering_element']['#view_mode'];
  $selected = $form_state['triggering_element']['#value'];
  $instance['display'][$view_mode]['type'] = $selected;
  $item_settings_form = bildspel_get_item_settings_form($selected, $field, $instance, $view_mode, $form, $form_state);
      
  foreach(element_children($item_settings_form) as $element) {
    //Lägger till #name
    $item_settings_form[$element]['#name'] = $element;
    //Lägger till empty option manuellt eftersom det försvinner annars.
    if(!empty($item_settings_form[$element]['#options']) && !empty($item_settings_form[$element]['#empty_option'])) {
      $item_settings_form[$element]['#options'] = array_merge(array($item_settings_form[$element]['#default_value'] => $item_settings_form[$element]['#empty_option']), $item_settings_form[$element]['#options']);
    }
  }
  
  $item_settings_form['#prefix'] = '<div id="item-settings-form">';
  $item_settings_form['#suffix'] = '</div>';
  
  return $item_settings_form;
}

function bildspel_get_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  //dpm($settings, 'settings');
  $slider_settings = $settings['slider_settings'];
  $summary = array(
    t('Frame length').': ' . (empty($slider_settings['frame_length']) ? 'No automatic sliding' : $slider_settings['frame_length'].' '.t('seconds')),
  );
  //Transition
  $summary[] = t('Transition').': '.$slider_settings['transition'];
  $summary[] = t('Skin').': '.$slider_settings['skin'];
  //Navigation
  $navigation = array();
  if(!empty($slider_settings['navigation'])) {
    $navigation[] = t('Arrows');
  }
  if(!empty($slider_settings['pager'])) {
    $navigation[] = t('Dots');
  }
  $navigation = t('Navigation').': '.implode(' '.t('and').' ', $navigation);
  $summary[] = $navigation;
  
  $summary[] = t('Item display').': '.$settings['item_display'];
  
  $output = implode('<br />', $summary);
  
  if(!empty($settings['item_display'])) {
    $summary_function = "{$settings['item_display']}_field_formatter_settings_summary";
    $item_display_instance = $instance;
    $item_display_instance['display'][$view_mode]['type'] = $settings['item_display'];
    $item_display_instance['display'][$view_mode]['settings'] = (array) json_decode($settings['item_settings']);
    $output .= '<br />' . call_user_func_array($summary_function, array($field, $item_display_instance, $view_mode));
  }

  return $output;
}
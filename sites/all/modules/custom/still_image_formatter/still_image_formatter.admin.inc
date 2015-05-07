<?php

function still_image_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $settings_form = array();
  
  //Force render
  $settings_form['force_render'] = array(
    '#title' => t('Render this field even if it is empty'),
    '#type' => 'checkbox',
    '#default_value' => $settings['force_render'],
  );
    
  //Reverse order
  $settings_form['reverse_order'] = array(
    '#title' => t('Reverse the order of the items'),
    '#type' => 'checkbox',
    '#default_value' => $settings['reverse_order'],
  );
  
  //Fixed height
  $settings_form['fixed_height'] = array(
    '#title' => t('Fixed height'),
    '#description' => t('Check this box if the image should have a fixed height, automatically cropping its sides if the screen is smaller than the image'),
    '#type' => 'checkbox',
    '#default_value' => $settings['fixed_height'],
  );
  
  //Deltas
  $settings_form['deltas'] = array(
    '#type' => 'textfield',
    '#title' => t('Delta'),
    '#description' => t('Optionally enter a delta, or a comma-separated list of deltas that should be shown. For example: 0, 1, 4. Leave empty to display all items'),
    '#size' => 10,
    '#default_value' => $settings['deltas'],
  );
  
  //Picture mapping
  if(module_exists('picture')) {
    $options = picture_get_mapping_options();
    if ($options) {
      $settings_form['picture_mapping'] = array(
        '#title' => t('Picture mapping'),
        '#type' => 'select',
        '#default_value' => $settings['picture_mapping'],
        '#options' => $options,
        '#empty_value' => '',
      );
    }
    // No picture mappings.
    else {
      $settings_form['picture_mapping'] = array(
        '#title' => t('Picture mapping'),
        '#type' => 'item',
        '#markup' => t(
          'There are no picture groups defined. !create_link.',
          array('!create_link' => l(t('Create a picture mapping'), 'admin/config/media/picture/add'))
        ),
      );
    }
  }
  
  //Image style
  $image_styles = image_style_options(FALSE);
  $settings_form['image_style'] = array(
    '#title' => t('Image style'),
    '#description' => t('If a picture group has been selected, this image style will be used as a fallback.'),
    '#type' => 'select',
    '#default_value' => $settings['image_style'],
    '#options' => $image_styles,
    '#empty_value' => '',
  );
  
  //Caption
  $settings_form['caption_type'] = array(
    '#type' => 'select',
    '#title' => t('Caption'),
    '#options' => array(
      'entity_title' => t('Entity title'),
      'property' => t('Field property'),
      'custom' => t('Custom'),
    ),
    '#empty_value' => '',
    '#default_value' => $settings['caption_type'],
    '#attributes' => array(
      'id' => drupal_html_id('still-image-formatter-settings-form-caption-type'),
    ),
  );
  $property_options = array_flip(array_keys($field['columns']));
  foreach($property_options as $key => &$value) {
    $value = $key;
  }
  $settings_form['caption_property'] = array(
    '#type' => 'select',
    '#default_value' => $settings['caption_property'],
    '#options' => $property_options,
    '#empty_option' => t('- Select a property -'),
    '#attributes' => array(
      'id' => drupal_html_id('still-image-formatter-settings-form-caption-property'),
    ),
  );
  $settings_form['caption_custom'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['caption_custom'],
    '#attributes' => array(
      'placeholder' => t('Write a custom caption here'),
      'id' => drupal_html_id('still-image-formatter-settings-form-caption-custom'),
    ),
  );
  
  //Image link
  $link_types = array(
    'content' => t('Content'),
    'file' => t('File'),
    'property' => t('Field property'),
    'custom' => t('Custom'),
  );
  $settings_form['link_type'] = array(
    '#title' => t('Link image to'),
    '#type' => 'select',
    '#default_value' => $settings['link_type'],
    '#empty_option' => t('Nothing'),
    '#options' => $link_types,
    '#attributes' => array(
      'id' => drupal_html_id('still-image-formatter-settings-form-link-type'),
    ),
  );
  $settings_form['link_property'] = array(
    '#type' => 'select',
    '#default_value' => $settings['link_property'],
    '#options' => $property_options,
    '#empty_option' => t('- Select a property -'),
    '#attributes' => array(
      'id' => drupal_html_id('still-image-formatter-settings-form-link-property'),
    ),
  );
  $settings_form['link_custom'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['link_custom'],
    '#attributes' => array(
      'placeholder' => t('Write a custom url here'),
    ),
    '#element_validate' => array('still_image_formatter_validate_url_form_element'),
    '#attributes' => array(
      'id' => drupal_html_id('still-image-formatter-settings-form-link-custom'),
    ),
  );
  
  //#states är så trasigt att jag implementerar min egen variant i stället. Det kräver fasta id på de berörda elementen.
  
  $ids = array();
  foreach($settings_form as $element_name => $element) {
    if(!empty($element['#attributes']['id'])) {
      $ids[$element_name] = $element['#attributes']['id'];
    }
  }
  $settings_form['#attached']['js'][] = drupal_get_path('module', 'still_image_formatter').'/js/still_image_formatter.admin.js';  
  $settings_form['#attached']['js'][] = array(
    'data' => array(
      'still_image_formatter' => array(
        'field_name' => $instance['field_name'],
        'ids' => $ids,
      ),
    ),
    'type' => 'setting',
  );
  
  return $settings_form;
}

function still_image_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = array();
  
  //Force render
  if(!empty($settings['force_render'])) {
    $summary[] = t('Force render');
  }
  
  //Reverse order
  if(!empty($settings['reverse_order'])) {
    $summary[] = t('Reverse item order');
  }
  
  //Fixed height
  if(!empty($settings['fixed_height'])) {
    $summary[] = t('Fixed height');
  }
  
  //Deltas
  $deltas = trim($settings['deltas']);
  if(!empty($deltas) || $deltas === '0') {
    $delta_label = strpos($deltas, ',') ? 'Deltas' : 'Delta';
    $deltas_output = t($delta_label . ': @deltas', array('@deltas' => $deltas));
    $summary[] = $deltas_output;
  }
  
  //Picture mapping
  $valid_picture_mapping = false;
  if(module_exists('picture') && !empty($settings['picture_mapping'])) {
    $picture_mapping = picture_mapping_load($settings['picture_mapping']);
    if ($picture_mapping) {
      $summary[] = t('Picture mapping: @picture_mapping', array('@picture_mapping' => $picture_mapping->label()));
      $valid_picture_mapping = true;
    }
  }
  
  //Image style
  $image_styles_output = '';
  $image_styles = image_style_options(FALSE);
  // Unset possible 'No defined styles' option.
  unset($image_styles['']);
  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  if (isset($image_styles[$settings['image_style']])) {
    if($valid_picture_mapping) {
      $image_styles_output = t('Fallback image style: @style', array('@style' => $image_styles[$settings['image_style']]));
    }
    else {
      $image_styles_output = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
    }
    $summary[] = $image_styles_output;
  }
  
  //Caption
  if($settings['caption_type']) {
    $summary[] = t('Caption: @caption', array('@caption' => $settings['caption_type']));
  }

  //Image link
  if ($settings['link_type']) {
    $summary[] = t('Link: @link_type', array('@link_type' => $settings['link_type']));
  }
  
  $output = implode('<br />', $summary);
  //Någon output måste alltid finnas, annars går det inte att gå in på settingsformuläret.
  if(!$output) {
    $output = t('Original settings');
  }
    
  return $output;
}

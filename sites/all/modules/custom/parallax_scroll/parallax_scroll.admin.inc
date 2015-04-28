<?php

function parallax_scroll_admin_form($form, $form_state) {
  $layers = variable_get('parallax_scroll_layers', array());
  $config = variable_get('parallax_scroll_config', array());
  /*---------Lager---------*/
  $form['layers'] = array(
    '#type' => 'container',
  );
  $form['layers']['title'] = array(
    '#markup' => '<h2>'.t('Layers').'</h2>',
  );
  $antal_lager = count($layers);
  if($antal_lager < 4) $antal_lager = 4;
  for($i = 0; $i < $antal_lager; $i++) {
    $form['layers'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Layer nr %nr', array('%nr' => ($i+1))),
      '#collapsible' => true,
      '#collapsed' => true,
    );
    $layer = array();
    if(!empty($layers[$i])) {
      $layer = $layers[$i];
    }
    //Nuvarande bild
    $current_path = '';
    if(!empty($layer['current_path'])) {
      $current_path = $layer['current_path'];
    }
    if($current_path) {
      $layer_data = array(
        'style_name' => 'thumbnail',
        'width' => '',
        'height' => '',
        'alt' => '',
        'title' => '',
        'path' => $current_path,
      );
      $layer_data = theme('image_style',$layer_data);
      $form['layers'][$i]["layer_current_image_$i"] = array(
        '#markup' => '<div class="current-image"><div class="description">'.t('Current image').':</div>'.$layer_data.'</div>',
      );
    }
    //Upload
    $form['layers'][$i]["layer_upload_$i"] = array(
      '#type' => 'file',
      '#title' => t('Upload an image for this layer'),
      '#size' => 0,
    );
    $options = array();
	  $breakpoint_groups = breakpoints_breakpoint_group_load_all();
	  foreach ($breakpoint_groups as $breakpoint_name => $breakpoint_group) {
      if(substr($breakpoint_name, 0, 8) === 'parallax') {
        $options[$breakpoint_name] = $breakpoint_name;
      }
	  }
    $form['layers'][$i]["layer_picture_$i"] = array(
      '#type' => 'select',
      '#title' => t('Responsive image'),
      '#options' => $options,
      '#empty_value' => '_none',
      '#default_value' => !empty($layers[$i]['picture']) ? $layers[$i]['picture'] : null,
    );
    $form['layers'][$i]["layer_speed_$i"] = array(
      '#type' => 'textfield',
      '#title' => t('Scroll speed'),
      '#default_value' => !empty($layers[$i]['speed']) ? $layers[$i]['speed'] : 1,
      '#description' => t('Smaller number means slower scroll speed. 1 = normal scroll speed'),
    ); 
    $form['layers'][$i]["layer_delete_$i"] = array(
      '#type' => 'submit',
      '#layer' => $i,
      '#value' => t('Delete layer'),
    );
  }
  /*---------Config----------*/
  $form['config'] = array(
    '#type' => 'container',
    '#tree' => 'true',
  );
  $form['config']['title'] = array(
    '#markup' => '<h2>'.t('Configuration').'</h2>',
  );
  $form['config']['visibility'] = array(
    '#title' => t('Visibility'),
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('collapsible', 'collapsed')),
  );
  $form['config']['visibility']['pages_setting'] = array(
    '#title' => t('Display on pages'),
    '#type' => 'radios',
    '#options' => array(
      BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
      BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
    ),
    '#default_value' => !empty($config['visibility']) ? $config['visibility']['pages_setting'] : BLOCK_VISIBILITY_LISTED,
  );
  $form['config']['visibility']['pages_list'] = array(
    '#type' => 'textarea',
    '#default_value' => !empty($config['visibility']) ? $config['visibility']['pages_list'] : '<front>',
  );
  /*-----------Övrigt----------*/
  $form['antal_lager'] = array(
    '#type' => 'hidden',
    '#value' => $antal_lager,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}
function parallax_scroll_admin_form_validate($form, &$form_state) {
  $values =& $form_state['values'];
  $validators = array('file_validate_extensions' => array('jpg jpeg png'));
  $values['layers'] = array();
  for($i = 0; $i < $form_state['values']['antal_lager']; $i++) {
    //Hantera upload
    $file = file_save_upload("layer_upload_$i", $validators);
    if (isset($file)) {
      if ($file) {
        $values["layer_upload_$i"] = $file;
      }
      else {
        form_set_error("layer_upload_$i", t('The file could not be uploaded.'));
      }
    }
    //Scrollhastighet
    if(isset($values["layer_speed_$i"])) {
      if($values["layer_speed_$i"] === '0') {
        form_set_error("layer_speed_$i", t('This field cannot be 0.'));
      }
    }
    //Omstrukturera till träd
    $values['layers'][$i] = array();
    foreach($values as $key => $value) {
      $split = explode('_', $key);
      if($split[0] === 'layer' && $split[2] == $i) {
        $values['layers'][$i][$split[1]] = $value;
        unset($values[$key]);
      }
    }
  }
}
function parallax_scroll_admin_form_submit($form, &$form_state) {
  $layers = variable_get('parallax_scroll_layers', array());
  $op = $form_state['clicked_button']['#value'];
  if($op === t('Save')) {
    module_load_include('inc', 'imagecache_actions', 'utility');
    $basedir = 'public://parallax_scroll_images';
    if (!file_exists($basedir)) {
      drupal_mkdir($basedir, NULL, TRUE);
      file_prepare_directory($basedir);
    }
    $values =& $form_state['values'];
    for($i = 0; $i < $form_state['values']['antal_lager']; $i++) {
      if(empty($layers[$i])) {
        $layers[$i] = array();
      }
      $layer =& $layers[$i];
      foreach($values['layers'][$i] as $key => $value) {
        if($key === "upload") {
          if ($file = $values['layers'][$i]["upload"]) {
            $filepath = file_unmanaged_copy($file->uri, "$basedir/$file->filename");
            $layer['current_path'] = $filepath;
          }
        }
        else {
          $layer[$key] = $value;
        }
      }
      //Ta bort de poster som inte har någon bild
      if(empty($layer['current_path'])) {
        unset($layers[$i]);
      }
      else {
        if(file_exists($layer['current_path'])) {
          $image_size = getimagesize($layer['current_path']);
          $layer['width'] = $image_size[0];
          $layer['height'] = $image_size[1];
        }
      }
    }
    variable_set('parallax_scroll_layers', $layers);
    if(!empty($values['config'])) {
      variable_set('parallax_scroll_config', $values['config']);
    }
    //Generera css-fil
    $css = '/*--------Generated css for parallax scroll. Please don\'t edit this file.-----------*/
.parallax-layer {
  width: 100%;
  position: fixed;
  background-position: 50% 0;
  background-repeat: no-repeat;
  z-index: -1;
}
';
    $relative_adjustment = '../../../../../default/files/';
    foreach($layers as $i => &$layer) {
      if($layer['picture'] !== '_none') {
        $picture_data = picture_mapping_load($layer['picture']);
        $layer['images'] = array();
        if(!empty($picture_data->mapping)) {
          foreach($picture_data->mapping as $mapping_name => $multipliers) {
            foreach($multipliers as $style_name) {
              $mapping_name_array = explode('.', $mapping_name);
              $layout = array_pop($mapping_name_array);
              $image_path = $layer['current_path'];    
              if($style_name) {
                $new_image_path = image_style_path($style_name, $layer['current_path']);
                //Generera bild för bildstilen om ingen sådan finns
                $success = true;
                if(!file_exists($new_image_path)) {
                  $new_dir = dirname($new_image_path);
                  if (!file_exists($new_dir)) {
                    drupal_mkdir($new_dir, NULL, TRUE);
                    file_prepare_directory($new_dir);
                  }
                  if(file_unmanaged_copy($image_path, $new_image_path, FILE_EXISTS_REPLACE)) {
                    $oImage = imagecache_actions_image_load($new_image_path);
                    $image_style = image_style_load($style_name);
                    if(!empty($image_style['effects'])) {
                      foreach($image_style['effects'] as $ieid => $effect) {
                        image_effect_apply($oImage, $effect);
                      }
                    }
                    if(!image_save($oImage, drupal_realpath($new_image_path))) {
                      $success = false;
                      drupal_set_message("Failed to save image \"$new_image_path\"", 'error');
                    }
                  }
                  else {
                    $success = false;
                    drupal_set_message("Failed to copy image to \"$new_image_path\"", 'error');
                  }
                }
                if($success) {
                  $image_path = $new_image_path;
                }
              }
              if(file_exists($image_path)) {
                $image_size = getimagesize($image_path);
                $layer['width'] = $image_size[0];
                $layer['height'] = $image_size[1];
                $image_path = $relative_adjustment . str_replace("public://", '', $image_path);
                $css .= <<<TEXT
body.responsive-layout-$layout #parallax-layer-$i {
  background-image: url('$image_path');
  max-width: {$layer['width']}px;
  height: {$layer['height']}px;
}

TEXT;
              }
            }
          }
        }
      }
      else { //Man har valt att inte använda någon responsiv bild, visa originalbilden
        $image_path = $layer['current_path'];
        if(file_exists($image_path)) {
          $image_path = $relative_adjustment . str_replace("public://", '', $image_path);
          $css .= <<<TEXT
.parallax-layer-$i {
  position: absolute;
  background-image: url('$image_path');
  height: {$layer['height']}px;
}

TEXT;
        }
      }
    }
    if($css) {
      $css_dir = drupal_realpath(drupal_get_path('module', 'parallax_scroll')).'/css';
      if (!file_exists($css_dir)) {
        drupal_mkdir($css_dir, NULL, TRUE);
        if(!file_prepare_directory($css_dir)) {
          drupal_set_message("Could not write to directory \"$css_dir\".", 'error');
        }
      }
      file_unmanaged_save_data($css, "$css_dir/parallax_scroll.css", FILE_EXISTS_REPLACE);
    }
    else {
      drupal_set_message('No css was generated.', 'error');
    }
  }
  else if($op === t('Delete layer')) {
    if(!empty($layers[$form_state['clicked_button']['#layer']]['current_path'])) {
      $i = $form_state['clicked_button']['#layer'];
      $image_path = $layers[$i]['current_path'];
      file_unmanaged_delete($image_path);
      unset($layers[$i]);
      $layers = array_values($layers);
      variable_set('parallax_scroll_layers', $layers);
    }
  }
}

//Startpunkt för en framtida funktion för att lägga till fler lager via formuläret och ajax.
function parallax_scroll_admin_form_js() {
  // We're starting in step #3, preparing for #4.
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Step #4.
  $form = form_get_cache($form_build_id, $form_state);
  // Preparing for #5.
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  // Step #5.
  drupal_process_form($form_id, $form, $form_state);
  // Step #6 and #7 and #8.
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  // Step #9.
  $choice_form = $form['choice_wrapper']['choice'];
  unset($choice_form['#prefix'], $choice_form['#suffix']);
  $output = theme('status_messages') . drupal_render($choice_form);
  // Final rendering callback.
  drupal_json(array('status' => TRUE, 'data' => $output));
}
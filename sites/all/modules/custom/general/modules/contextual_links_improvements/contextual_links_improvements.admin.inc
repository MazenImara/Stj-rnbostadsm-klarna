<?php
function contextual_links_edit_logo_form($form, $form_state) {
  //Logga och favikon
  $default_theme = variable_get('theme_default', 'kopernikus');
  // Logo settings
  $form['logo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logo image settings'),
    '#description' => t('If toggled on, the following logo will be displayed.'),
    '#attributes' => array('class' => array('theme-settings-bottom')),
  );
  $logo_path = '';
  $default_logo = theme_get_setting('default_logo', $default_theme);
  if($default_logo) {
    $logo_path = general_get_default_logo_path($default_theme);
  }
  else {
    $logo_path = theme_get_setting('logo_path', $default_theme);
  }
  if($logo_path) {
    $form['logo']['current_logo'] = array(
      '#path' => $logo_path,
      '#alt' => '',
      '#title' => '',
      '#theme' => 'image',
      '#prefix' => '<div class="description">'.t('Current logo').':</div>',
    );
    if (!$default_logo) {
      $form['logo']['current_logo']['#style_name'] = 'thumbnail';
      $form['logo']['current_logo']['#theme'] = 'image_style';
    }
  }
  $form['logo']['default_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default logo'),
    '#default_value' => theme_get_setting('default_logo', $default_theme),
    '#tree' => FALSE,
    '#description' => t('Check here if you want the theme to use the logo supplied with it.')
  );
  $form['logo']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the logo settings when using the default logo.
      'invisible' => array(
        'input[name="default_logo"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['logo']['settings']['logo_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom logo'),
    '#description' => t('The path to the file you would like to use as your logo file instead of the default logo.'),
    '#default_value' => theme_get_setting('logo_path', $default_theme),
  );
  $form['logo']['settings']['logo_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload logo image'),
    '#maxlength' => 40,
    '#size' => 0,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your logo."),
  );

  // Inject human-friendly values for logo
  foreach (array('logo' => 'logo.png') as $type => $default) {
    if (isset($form[$type]['settings'][$type . '_path'])) {
      $element = &$form[$type]['settings'][$type . '_path'];

      // If path is a public:// URI, display the path relative to the files
      // directory; stream wrappers are not end-user friendly.
      $original_path = $element['#default_value'];
      $friendly_path = NULL;
      if (file_uri_scheme($original_path) == 'public') {
        $friendly_path = file_uri_target($original_path);
        $element['#default_value'] = $friendly_path;
      }
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}
function contextual_links_edit_logo_form_validate($form, &$form_state) {
  module_load_include('inc', 'system', 'system.admin');
  //Logga och favikon
  // Handle file uploads.
  $validators = array('file_validate_is_image' => array());

  // Check for a new uploaded logo.
  $file = file_save_upload('logo_upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['logo_upload'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('logo_upload', t('The logo could not be uploaded.'));
    }
  }

  $validators = array('file_validate_extensions' => array('ico png gif jpg jpeg apng svg'));

  // If the user provided a path for a logo file, make sure a file
  // exists at that path.
  if ($form_state['values']['logo_path']) {
    $path = _system_theme_settings_validate_path($form_state['values']['logo_path']);
    if (!$path) {
      form_set_error('logo_path', t('The custom logo path is invalid.'));
    }
  }
}
function contextual_links_edit_logo_form_submit($form, &$form_state) {
  module_load_include('inc', 'system', 'system.admin');
  global $theme_key;
  //Logga
  $values =& $form_state['values'];

  // If the user uploaded a new logo or favicon, save it to a permanent location
  // and use it in place of the default theme-provided file.
  if ($file = $values['logo_upload']) {
    unset($values['logo_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['default_logo'] = 0;
    $values['logo_path'] = $filename;
  }

  // If the user entered a path relative to the system files directory for
  // a logo or favicon, store a public:// URI so the theme system can handle it.
  if (!empty($values['logo_path'])) {
    $values['logo_path'] = _system_theme_settings_validate_path($values['logo_path']);
  }

  $current_settings = variable_get('theme_'.$theme_key.'_settings', array());
  $changed = false;
  if($current_settings) {
    $vars = array(
      'default_logo' => 'default_logo',
      'logo_path' => 'logo_path',
    );
    foreach($vars as $var) {
      if(isset($values[$var])) {
        if($values[$var] !== $current_settings[$var]) {
          $changed = true;
          break;
        }
      }
    }
  }
  if($changed) {
    $themes = list_themes();
    foreach($themes as $name => $theme) {
      if($theme->status) {
        $settings = variable_get('theme_'.$name.'_settings', array());
        if(isset($values['default_logo'])) $settings['default_logo'] = $values['default_logo'];
        if(isset($values['logo_path'])) $settings['logo_path'] = $values['logo_path'];
        variable_set('theme_'.$name.'_settings', $settings);
      }
    }
  }
}
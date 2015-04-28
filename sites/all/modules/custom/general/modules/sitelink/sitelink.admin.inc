<?php
function sitelink_config_form($form, $form_state) {
  $form['sitelink_anchor'] = array(
    '#title' => t('Anchor links'),
    '#type' => 'textarea',
    '#default_value' => variable_get('sitelink_anchor', ''),
  );
  $form['sitelink_title'] = array(
    '#title' => t('Title attributes'),
    '#type' => 'textarea',
    '#default_value' => variable_get('sitelink_title', ''),
  );
  return system_settings_form($form);
}
<?php

function theme_views_ui_expose_filter_fields_compare_form($variables) {
  $form = $variables['form'];
  $more = drupal_render($form['more']);

  $output = drupal_render($form['form_description']);
  $output .= drupal_render($form['expose_button']);
  $output .= drupal_render($form['group_button']);
  if (isset($form['required'])) {
    $output .= drupal_render($form['required']);
  }
  $output .= drupal_render($form['label']);
  $output .= drupal_render($form['description']);

  $output .= drupal_render($form['left_field']);
  $output .= drupal_render($form['operator']);
  $output .= drupal_render($form['right_field']);
  $output .= drupal_render($form['value']);

  if (isset($form['use_operator'])) {
    $output .= '<div class="views-left-40">';
    $output .= drupal_render($form['use_operator']);
    $output .= '</div>';
  }

  // Only output the right column markup if there's a left column to begin with
  if (!empty($form['operator']['#type'])) {
    $output .= '<div class="views-right-60">';
    $output .= drupal_render_children($form);
    $output .= '</div>';
  }
  else {
    $output .= drupal_render_children($form);
  }

  $output .= $more;

  return $output;
}
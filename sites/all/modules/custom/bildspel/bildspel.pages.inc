<?php

function bildspel_get_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $view_function = "{$settings['item_display']}_field_formatter_view";
  $item_display = $display;
  $item_display['type'] = $settings['item_display'];
  $item_display['settings'] = (array) json_decode($settings['item_settings']);
  $processed_items = call_user_func_array($view_function, array($entity_type, $entity, $field, $instance, $langcode, $items, $item_display));
  $elements = array(
    array(
      '#theme' => 'bildspel',
      '#items' => $processed_items,
      '#slider_settings' => $settings['slider_settings'],
    ),
  );
  return $elements;
}
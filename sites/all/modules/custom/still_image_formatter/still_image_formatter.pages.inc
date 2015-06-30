<?php

function still_image_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $wrapper = entity_metadata_wrapper($entity_type, $entity);
  $title_value = $wrapper->title_field->value();
  if(!empty($title_value)) {
    $entity_title = $title_value;
  }
  else {
    $entity_title = $entity->title;
  }
  $entity_uri = '';
  if($entity_type === 'commerce_product') {
    //Använd produktdisplayen i stället
    $product_wrapper = entity_metadata_wrapper('commerce_product', $entity);
    $nodes = $product_wrapper->field_product_node->value();
    $product_display = array_shift($nodes);
    if($product_display) {
      $uri = entity_uri('node', $product_display);
      if(!empty($uri['path'])) {
        $entity_uri = $uri['path'];
      }
      $product_display_wrapper = entity_metadata_wrapper('node', $product_display); 
      $entity_title = $product_display_wrapper->title_field->value();
    }
  }
  else {
    $uri = entity_uri($entity_type, $entity);
    if(!empty($uri['path'])) {
      $entity_uri = $uri['path'];
    }
  }
  
  //Reverse order
  if (!empty($settings['reverse_order'])) {
    $items = array_reverse($items);
  }
  
  //Delta
  $use_delta = false;
  $deltas = trim($settings['deltas']);
  if(!empty($deltas) || $deltas === '0') {
    // Prepare an array of selected deltas from the entered string.
    if (strpos($deltas, ',')) {
      $deltas = explode(',', $deltas);
      $deltas = array_map('trim', $deltas);
    }
    else {
      $deltas = array($deltas);
    }
    $use_delta = true;
  }
  
  //Picture mapping
  $breakpoints = array();
  if(!empty($settings['picture_mapping'])) {
    $mapping_name = $settings['picture_mapping'];
    // If we haven't saved a picture mapping for this field previously and this isn't a field formatter screen (i.e. panels), load a default mapping.
    $picture_mapping = FALSE;
    if (isset($mapping_name) && !empty($mapping_name)) {
      $picture_mapping = picture_mapping_load($mapping_name);
      if (!$picture_mapping) {
        trigger_error("Unable to load picture mapping $mapping_name.", E_USER_ERROR);
        return $element;
      }
    }
    else {
      $all_mappings = picture_mapping_load_all();
      $picture_mapping = reset($all_mappings);
      if (!$picture_mapping) {
        trigger_error("No picture mappings have been defined yet.", E_USER_ERROR);
        return $element;
      }
    }
    $breakpoints = picture_get_mapping_breakpoints($picture_mapping, $settings['image_style']);
  }
  
  //Caption
  $caption = '';
  if($settings['caption_type'] === 'entity_title') {
    $caption = $entity_title;
  }
  else if($settings['caption_type'] === 'custom') {
    $caption = t($settings['caption_custom']);
  }
  
  //Image link
  $link = '';
  $link_type = $settings['link_type'];
  if ($link_type === 'content') {
    $link = $entity_uri;
  }
  else if($link_type === 'custom') {
    $link = $settings['link_custom'];
  }
  
  $elements = array();
  if(!empty($items)) {
    foreach($items as $delta => $item) {
      
      //Delta
      if($use_delta) {
        // Skip deltas that weren't selected for display.
        if (!in_array($delta, $deltas)) {
          continue;
        }
      }
      
      //Caption
      if($settings['caption_type'] === 'property') {
        if(!empty($item[$settings['caption_property']])) {
          $caption = $item[$settings['caption_property']];
        }
      }
      
      //Image link
      if ($link_type === 'file') {
        $link = file_create_url($item['uri']);
      }
      else if($link_type === 'property') {
        if(!empty($item[$settings['link_property']])) {
          $link = $item[$settings['link_property']];
        }
      }
      
      $elements[] = array(
        '#theme' => 'still_image_formatter',
        '#item' => $item,
        '#fixed_height' => !empty($settings['fixed_height']),
        '#breakpoints' => $breakpoints,
        '#image_style' => $settings['image_style'],
        '#caption' => $caption,
        '#image_link' => $link,
      );
    }
  }
  else {
    if(!empty($settings['force_render'])) {
      $elements[] = array();
    }
  }

  return $elements;
}
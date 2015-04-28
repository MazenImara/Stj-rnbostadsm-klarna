<?php

function theme_responsive_bg_formatter($variables) {
  static $instance_index = 0;
  if (!isset($variables['breakpoints']) || empty($variables['breakpoints'])) {
    return theme('image_formatter', $variables);
  }
  
  extract($variables);
  if(!empty($item['height'])) {
    $height = $item['height'];
  }
  else {
    $image_size = getimagesize($item['uri']);
    $height = $image_size[1];
  }
  $id = "responsive-bg-$instance_index";
  $instance_index++;
  $css = "
#$id {
  height: {$height}px;
}
";
  $relative_adjustment = '/sites/default/files/';
  $images = array();
  foreach($breakpoints as $mapping_name => $multipliers) {
    foreach($multipliers as $style_name) {
      $mapping_name_array = explode('.', $mapping_name);
      $layout = array_pop($mapping_name_array);
      $image_path = $item['uri'];    
      if($style_name) {
        $new_image_path = image_style_path($style_name, $image_path);
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
        $image_data = array('width' => $image_size[0], 'height' => $image_size[1]);
        $image_path = $relative_adjustment . str_replace("public://", '', $image_path);
        $css .= <<<TEXT
body.responsive-layout-$layout #$id {
  background-image: url('$image_path');
  height: {$image_data['height']}px;
}

TEXT;
      }
    }
  }
  if($css) {
    drupal_add_css($css, array('group' => CSS_THEME, 'type' => 'inline'));
  }
  $img = array(
    '#theme' => 'image',
    '#path' => drupal_get_path('module', 'responsive_bg').'/images/transparent.gif',
    '#alt' => !empty($item['alt']) ? $item['alt'] : '',
    '#title' => !empty($item['title']) ? $item['title'] : '',
    '#attributes' => array(
      'id' => $id, 
      'class' => array('responsive-bg'),
    ),
  );
  if(!empty($item['caption'])) { //Skapa en separat caption
    if(empty($item['title'])) {
      $img['#title'] = $item['caption'];
    }
    $img['#suffix'] = '<span class="responsive-bg-caption"><span class="responsive-bg-caption-bg"></span><span class="responsive-bg-caption-text">'.$item['caption'].'</span></span>';
  }
  if(!empty($item['url'])) { //Skapa en länk utifrån fältvärden
    $output = array(
      '#theme' => 'link',
      '#text' => drupal_render($img),
      '#path' => $item['url'],
      '#options' => array(
        'attributes' => array(
          'class' => array('responsive-bg-link'),
          'title' => $img['#title'],
        ),
        'html' => TRUE,
      ),
    );
  }
  else if (isset($variables['path']['path'])) { //Skapa en länk utifrån settings
    $path = $variables['path']['path'];
    $output = array(
      '#theme' => 'link',
      '#text' => drupal_render($img),
      '#path' => $path,
      '#options' => array(
        'attributes' => array(
          'class' => array('responsive-bg-link'),
          'title' => $img['#title'],
        ),
        'html' => TRUE,
      ),
    );
  }
  else {
    $output = $img;
  }
  return drupal_render($output);
}
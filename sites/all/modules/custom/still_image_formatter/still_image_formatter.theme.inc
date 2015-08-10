<?php

function theme_still_image_formatter($variables) {
  $output = array();

  if($variables['fixed_height']) {
    $image = array(
      '#theme' => 'still_image_formatter_fixed_height_image',
      '#item' => $variables['item'],
      '#breakpoints' => $variables['breakpoints'],
      '#image_style' => $variables['image_style'],
    );
  }
  else {
    if($variables['breakpoints']) {
      $image = array(
        '#theme' => 'picture_formatter',
        '#item' => $variables['item'],
        '#path' => NULL,
        '#image_style' => $variables['image_style'],
        '#breakpoints' => $variables['breakpoints'],
      );
    }
    else {
      $image = array(
        '#theme' => 'image_formatter',
        '#item' => $variables['item'],
        '#image_style' => $variables['image_style'],
      );
    }
  }
  $output['image'] = $image;
  
  if(!empty($variables['caption'])) {
    $caption = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('caption-wrapper'),
      ),
      '#attached' => array(
        'css' => array(drupal_get_path('module', 'still_image_formatter').'/css/still_image_formatter_caption.css'),
      ),
      'caption_bg' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('caption-bg'),
        ),
      ),
      'caption_text' => array(
        '#theme' => 'html_tag',
        '#tag' => 'span',
        '#value' => check_plain($variables['caption']),
        '#attributes' => array(
          'class' => array('caption-text'),
        ),
      ),
    );
    $output['caption'] = $caption;
  }
  if (!empty($variables['image_link'])) {
    $link = array(
      '#theme' => 'link',
      '#path' => $variables['image_link'],
      '#options' => array(
      'attributes' => array(
        'class' => array('image-link'),
      ),
      'html' => true,
    ),
      '#text' => render($output),
    );
    $output = array(
      'link' => $link,
    );
  }

  $output['#type'] = 'container';
  $output['#attributes'] = array(
    'class' => array('still-image-formatter-item'),
  );
  $output['#attached'] = array('css' => array(drupal_get_path('module', 'still_image_formatter').'/css/still_image_formatter.css'));
  return render($output);
}

function theme_still_image_formatter_fixed_height_image($variables) {
  $instance_index = &drupal_static(__FUNCTION__, 0);
  $item = $variables['item'];
  $id = "still-image-formatter-bg-$instance_index";
  $instance_index++;
  
  $image_styles = array();
  if($variables['breakpoints']) {
    foreach($variables['breakpoints'] as $mapping_name => $multipliers) {
      foreach($multipliers as $multiplier) {
        if($multiplier) {
          $mapping_name_array = explode('.', $mapping_name);
          $layout = array_pop($mapping_name_array);
          $mapping_type = $multiplier['mapping_type'];
          $style_name = $multiplier[$mapping_type];
          $image_styles[$layout] = $style_name;
        }
      }
    }
  }
  else {
    $image_styles['all'] = $variables['image_style'];
  }
  
  $css = '';
  global $base_url; 
  
  foreach($image_styles as $layout => $style_name) {
    $image_path = $item['uri']; 
    if($style_name) {
      $new_image_path = image_style_path($style_name, $image_path);
    }
    else {
      $new_image_path = $image_path;
    }
    //Generera bild för bildstilen om ingen sådan finns
    $success = true;
    if(!file_exists($new_image_path)) {
      $new_dir = dirname($new_image_path);
      file_prepare_directory($new_dir, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
      if(file_unmanaged_copy($image_path, $new_image_path, FILE_EXISTS_REPLACE)) {
        module_load_include('inc', 'imagecache_actions', 'utility');
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
    if(file_exists($image_path)) {
      $image_size = getimagesize($image_path);
      $image_data = array('width' => $image_size[0], 'height' => $image_size[1]);
      $image_url = file_create_url($image_path); 
      //$image_url = preg_replace('#' . $base_url . '#', '', $image_url); //Commented out to support sites in subdirectories
      if($layout === 'all') {
        $layout_conditions = array('');
      }
      else {
        $layout_conditions = array(
          "body.layout-$layout ",
          "body.responsive-layout-$layout ",
        );
      }
      foreach($layout_conditions as $layout_condition) {
        $css .= <<<TEXT
{$layout_condition}#$id {
  background-image: url('$image_url');
  width: {$image_data['width']}px;
  height: {$image_data['height']}px;
}
TEXT;
      }
    }
  }

  $img = array(
    '#theme' => 'image',
    '#path' => drupal_get_path('module', 'still_image_formatter').'/images/transparent.gif',
    '#alt' => !empty($item['alt']) ? $item['alt'] : '',
    '#title' => !empty($item['title']) ? $item['title'] : '',
    '#attributes' => array(
      'id' => $id, 
      'class' => array('bg-image'),
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'still_image_formatter').'/css/still_image_formatter_bg_image.css'),
    ),
  );
  if($css) {
    //Det funkar inte att använda #attached för inline css
    drupal_add_css($css, array('group' => CSS_THEME, 'type' => 'inline'));
  }
  return render($img);
}
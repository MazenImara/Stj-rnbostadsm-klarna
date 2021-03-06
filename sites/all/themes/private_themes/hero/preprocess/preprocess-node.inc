<?php
function hero_alpha_preprocess_node(&$vars) {
  if($vars['type'] === 'nyhet') {
    //Supply user picture for news items
    if(!empty($vars['uid'])) {
      $account = user_load($vars['uid']);
      $wrapper = entity_metadata_wrapper('user', $account);
      $picture_value = $wrapper->field_enterprise_blog_picture->value();
      if(!empty($picture_value)) {
        $picture_data = $picture_value;
        $picture = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('user-picture-container'),
          ),
          'image' => array(
            '#theme' => 'image_style',
            '#style_name' => 'hero_news_slideshow_user_picture',
            '#path' => $picture_data['uri'],
            '#width' => $picture_data['width'],
            '#height' => $picture_data['height'],
            '#alt' => $picture_data['alt'],
            '#title' => $picture_data['title'],
            '#attributes' => array(
              'class' => array('user-picture'),
            ),
          ),
        );
        $vars['user_picture'] = render($picture);
      }
    }
    //Add class if node has header image
    if(!empty($vars['field_sidhuvud'])) {
      $vars['classes_array'][] = 'has-header-image';
      $vars['attributes_array']['class'][] = 'has-header-image';
    }
  }
  else if($vars['type'] === 'enterprise_blog') {
    //Add class if node has header image
    if(!empty($vars['field_image'])) {
      $vars['classes_array'][] = 'has-header-image';
      $vars['attributes_array']['class'][] = 'has-header-image';
    }
    unset($vars['content']['links']);
  }
}
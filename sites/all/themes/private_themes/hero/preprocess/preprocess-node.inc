<?php
function hero_alpha_preprocess_node(&$vars) {
  //Supply user picture for news items
  if($vars['type'] === 'nyhet') {
    if(!empty($vars['uid'])) {
      $account = user_load($vars['uid']);
      $wrapper = entity_metadata_wrapper('user', $account);
      if(!empty($wrapper->field_enterprise_blog_picture->value())) {
        $picture_data = $wrapper->field_enterprise_blog_picture->value();
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
  }
}
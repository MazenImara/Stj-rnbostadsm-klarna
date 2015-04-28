<?php
function theme_nivoslider_container(&$vars) {
  $settings = $vars['settings'];
  $nivoslider_id = &drupal_static('nivoslider_id', 0);
  $vars['id'] = ++$nivoslider_id;

  // Attach nivoslider JavaScripts
  if ($path = libraries_get_path('nivo-slider')) {
    drupal_add_js($path . '/jquery.nivo.slider.pack.js');
    $js = '
      (function($) {
        $(document).ready(function(){
          $("#nivoslider_'.$vars['id'].'").nivoSlider({
            pauseTime: '.$settings['nivoslider_pausetime'].',
            directionNav: true,
            effect: "'.$settings['nivoslider_transition'].'",
            controlNav: '.($settings['nivoslider_display_pagination'] ? 'true' : 'false').'
          });';
          if($settings['nivoslider_use_swipe']) {
            if($swipe_path = libraries_get_path('jquery.touchswipe')) {
              drupal_add_js($swipe_path . '/jquery.touchSwipe.min.js');
            }
            drupal_add_js(drupal_get_path('module', 'linkimagefield_nivoslider') . '/js/linkimagefield_nivoslider_swipe.js');
            $js .= '
              $("#nivoslider_'.$vars['id'].'").linkimagefield_nivoslider_bindswipe();
            ';
          }
          $js .= '
        });
      })(jQuery);
    ';
    drupal_add_js($js, 'inline');
    drupal_add_css($path . '/nivo-slider.css');
    drupal_add_css(drupal_get_path('module', 'linkimagefield_nivoslider') . '/css/linkimagefield_nivoslider.css');
    if($settings['nivoslider_skin'] !== '_none') {
      if(file_exists($path . '/themes/'.$settings['nivoslider_skin'].'/'.$settings['nivoslider_skin'].'.css')) {
        drupal_add_css($path . '/themes/'.$settings['nivoslider_skin'].'/'.$settings['nivoslider_skin'].'.css');
      }
      else {
        drupal_add_css(drupal_get_path('module', 'linkimagefield_nivoslider') . '/themes/'.$settings['nivoslider_skin'].'/'.$settings['nivoslider_skin'].'.css');
      }
    }
  }

  // Prepare image elements
  $items = $vars['items'];
  $classes = array();
  if(!$settings['nivoslider_display_controls']) {
    $classes[] = 'hide-dircontrols';
  }
  $vars['items'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'nivoslider_'.$vars['id'], 'class' => $classes),
    '#prefix' => '<div class="slider-wrapper theme-'.$settings['nivoslider_skin'].'">',
    '#suffix' => '</div>',
  );
  foreach ($items as $delta => $item) {
    $vars['items'][$delta] = array(
      '#theme' => 'linkimage_formatter',
      '#item' => $item, 
      '#image_style' => ($settings['image_style'] === '_none' ? null : $settings['image_style']),
    );
  }
  return drupal_render($vars['items']);
}
function theme_nivoslider_wrapper($variables) {
  $element = $variables['element'];
  $link = $element['#url'];
  if ($link) {
    return l($element['#children'], $link, array(
      'absolute' => TRUE,
      'attributes' => array('title' => $element['#title']),
      'html' => TRUE));
  }
  return $element['#children'];
}

<?php
function theme_caroufredsel_container(&$vars) {
  $settings = $vars['settings'];
  $caroufredsel_id = &drupal_static('caroufredsel_id', 0);
  $vars['id'] = ++$caroufredsel_id;
  if(!$settings['caroufredsel_items_max']) {
    $settings['caroufredsel_items_max'] = '1';
  }
  
  // Attach caroufredsel JavaScripts
  if ($path = libraries_get_path('caroufredsel')) {
    drupal_add_js($path . '/jquery.carouFredSel-packed.js');
    drupal_add_js($path . '/helper-plugins/jquery.ba-throttle-debounce.min.js');
    if($settings['caroufredsel_use_swipe']) {
      if($swipe_path = libraries_get_path('jquery.touchswipe')) {
        drupal_add_js($swipe_path . '/jquery.touchSwipe.min.js');
      }
    }
    if($settings['caroufredsel_use_mouse']) {
      drupal_add_js($path . '/helper-plugins/jquery.mousewheel.min.js');
    }
    $js = '
      (function($) {
        $(window).load(function() {
          $(".caroufredsel").show();
          $("#caroufredsel_'.$vars['id'].'").carouFredSel({
            width: "100%",
            auto: {
              pauseOnHover: "resume",
              timeoutDuration: '.$settings['caroufredsel_timeoutduration'].'
            },
            responsive: false,
            items : {
              visible : { min : 1, max : '.$settings['caroufredsel_items_max'].' }
            },
            circular: '.($settings['caroufredsel_circular'] ? 'true' : 'false');
            if($settings['caroufredsel_display_controls']) {
              $js .= ',
              prev: "#caroufredsel_prev_'.$vars['id'].'",
              next: "#caroufredsel_next_'.$vars['id'].'"';
            }
            if($settings['caroufredsel_display_pagination']) {
              $js .= ',
              pagination: "#caroufredsel_pager_'.$vars['id'].'"';
            }
            if($settings['caroufredsel_scroll']) {
              $js .= ',
              scroll: '.$settings['caroufredsel_scroll'];
            }
            $js .= ',
            swipe: {
              onTouch: '.($settings['caroufredsel_use_swipe'] ? 'true' : 'false').',
              onMouse: '.($settings['caroufredsel_use_mouse'] ? 'true' : 'false').'
            }';
            $js .= '
          });
        });
      })(jQuery);
    ';
    drupal_add_js($js, 'inline');
    drupal_add_css(drupal_get_path('module', 'linkimagefield_caroufredsel').'/css/linkimagefield_caroufredsel.css');
    if($settings['caroufredsel_skin'] !== '_none') {
      drupal_add_css(drupal_get_path('module', 'linkimagefield_caroufredsel').'/themes/'.$settings['caroufredsel_skin'].'/'.$settings['caroufredsel_skin'].'.css');
    }
  }


  // Prepare image elements
  $items = $vars['items'];
  $vars['items'] = array(
    '#theme' => 'item_list',
    '#attributes' => array('class' => array('caroufredsel'), 'id' => 'caroufredsel_'.$vars['id']),
    '#items' => array(),
    '#prefix' => '<div class="caroufredsel-outer caroufredsel-theme-'.$settings['caroufredsel_skin'].'">',
    '#suffix' => '',
  );
  if($settings['caroufredsel_display_controls']) {
    $vars['items']['#suffix'] = '<div class="caroufredsel_controls"><a id="caroufredsel_prev_'.$vars['id'].'" class="caroufredsel_prev" href="#">&lt;</a><a id="caroufredsel_next_'.$vars['id'].'" class="caroufredsel_next" href="#">&gt;</a></div>';
  }
  if($settings['caroufredsel_display_pagination']) {
    $vars['items']['#suffix'] .= '<div id="caroufredsel_pager_'.$vars['id'].'" class="caroufredsel_pager"></div>';
  }
  $vars['items']['#suffix'] .= '</div>';
  foreach ($items as $delta => $item) {
    $vars['items']['#items'][$delta] = array(
      'data' => theme('linkimage_formatter', array('item' => $item, 'image_style' => ($settings['image_style'] === '_none' ? null : $settings['image_style']), 'separate_caption' => true)),
    );
  }
  return drupal_render($vars['items']);
}
function theme_caroufredsel_wrapper($variables) {
  return $element['#children'];
}

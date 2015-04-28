<?php
function _vertical_scroll_autocomplete($string) {
  $matches = array();
  global $language;
  // Some fantasy DB table which holds cities
  $query = db_select('node', 'n');
  // Select rows that match the string
  $return = $query
    ->fields('n', array('nid', 'title'))
    ->condition('n.title', '%' . db_like($string) . '%', 'LIKE')
    ->condition('n.language', $language->language)
    ->range(0, 10)
    ->execute();
  
  // add matches to $matches  
  foreach ($return as $row) {
    $matches[$row->title] = check_plain($row->title." (nid: $row->nid)");
  }
  
  // return for JS
  drupal_json_output($matches);
}
function vertical_scroll_map_form($form, $form_state, $current_destination_nid = 0) {
  global $language;
  $settings = vertical_scroll_get_data();
  $node_types = node_type_get_names();
  $display_types = variable_get('vertical_scroll_map_display_types', array());
  if(!$display_types) {
    foreach($node_types as $name => $obj) {
      $display_types[$name] = $name;
    }
  }
  if(variable_get('general_language_settings', 'single')==='multi') {
    $language_list = language_list();
    $tabs = array(
      'vertical_scroll_map_tabs' => array(
        '#prefix' => '<div class="vertical-scroll-map-tabs"><ul>',
        '#suffix' => '</ul></div>',
      ),
    );
    $vertical_scroll_map_tabs =& $tabs['vertical_scroll_map_tabs'];
    $current_path = current_path();
    foreach($language_list as $key => $lang_full) {
      if($lang_full->enabled) {
        $active = $key===$language->language ? 'active' : '';
        $vertical_scroll_map_tabs[$key] = array(
          '#markup' => "<li class='menu-tab $key $active'>".l(t($lang_full->name), url($current_path, array('absolute' => true, 'language' => $lang_full))).'</li>',
        );
      }
    }
    $form['#prefix'] = render($tabs);
  }
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'vertical_scroll') . '/css/vertical_scroll_map_form.css',
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'vertical_scroll') . '/js/vertical_scroll_map_form.js',
  );
  $temp = array();
  foreach($settings as $setting) {
    if(!isset($temp[$setting['destination_nid']])) $temp[$setting['destination_nid']] = array('node' => node_load($setting['destination_nid']), 'sources' => array());
    $temp[$setting['destination_nid']]['sources'][$setting['source_nid']] = $setting;
    $temp[$setting['destination_nid']]['sources'][$setting['source_nid']]['node'] = node_load($setting['source_nid']);
  }
  $hierarchy = array();
  foreach($temp as $destination_nid => $destination) {
		if(isset($destination['node']) && $destination['node']) {
	    if(!isset($hierarchy[$destination['node']->language])) $hierarchy[$destination['node']->language] = array();
	    $hierarchy[$destination['node']->language][$destination_nid] = $destination;
		}
  }
  unset($temp);
  $form['destinations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Destinations'),
  );
  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sources'),
  );
  $entity_info = entity_get_info('node');
  $view_modes = array();
  foreach($entity_info['view modes'] as $key => $mode) {
    $view_modes[$key] = $mode['label'];
  }
  $index = 0;
  if(isset($hierarchy[$language->language])) {
    foreach($hierarchy[$language->language] as $destination_nid => $destination) {
      $hidden = '';
      $active = 'active';
      if(!$current_destination_nid) {
        if($index>0) {
          $hidden = 'hidden';
          $active = '';
        }
      }
      $index++;
      $form['destinations'][$destination_nid] = array(
        '#markup' => theme('vertical_scroll_destination', array('node' => $destination['node'], 'active' => $active)),
      );
      if($current_destination_nid) {
        if($current_destination_nid != $destination_nid) continue;
      }
      usort($destination['sources'], 'vertical_scroll_sort_array_by_weight');
      $form['data'][$destination_nid] = array(
        '#tree' => TRUE,
        '#theme' => 'vertical_scroll_sources',
        '#hidden' => $hidden,
        'sources' => array(),
      );
      foreach($destination['sources'] as $i => $source) {
        $source_nid = $source['source_nid'];
        $form['data'][$destination_nid]['sources'][$source_nid] = array(
          'title' => array(
            '#markup' => $source['node']->title,
          ),
          'source_nid' => array(
            '#type' => 'hidden', 
            '#value' => $source['source_nid'],
          ),
          'destination_nid' => array(
            '#type' => 'hidden',
            '#value' => $source['destination_nid'],
          ),
          'view_mode' => array(
            '#type' => 'select',
            '#options' => $view_modes,
            '#default_value' => $source['view_mode'],
          ),
          'weight' => array(
            '#type' => 'weight',
            '#title' => t('Weight'),
            '#title_display' => 'invisible',
            '#default_value' => $source['weight'],
            '#attributes' => array(
              'class' => array('source-weight'),
            ),
          ),
        );
      }
    }
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}
function theme_vertical_scroll_destination($variables) {
  $node = $variables['node'];
  $body = '';
  $destination = drupal_get_destination();
  $body .= l($node->title, "admin/structure/vertical-scroll/map/$node->nid", array('absolute' => true, 'attributes' => array('class' => array('destination', "destination-$node->nid", $variables['active']))));
  return $body;
}
function theme_vertical_scroll_sources($variables) {
  $header = array(t('Title'), t('View mode'), t('Weight'));
  $rows = array();
  $sources = $variables['elements']['sources'];
  $destination_nid = 0;
  foreach(element_children($sources) as $key) {
    $element = $sources[$key];
    if(!$destination_nid) $destination_nid = $element['destination_nid']['#value'];
    $rows[] = array(
      'data' => array(
        drupal_render($element['title']),
        drupal_render($element['view_mode']),
        drupal_render($element['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => "sources-$destination_nid", 'class' => array($variables['elements']['#hidden']))));
  drupal_add_tabledrag("sources-$destination_nid", 'order', 'sibling', 'source-weight');
  return $output;
}
function vertical_scroll_map_form_submit($form, &$form_state) {
  $update = array();
  $delete = array();
  foreach($form_state['values'] as $destination_nid => $destination) {
    if(is_numeric($destination_nid)) {
      foreach($destination['sources'] as $source_nid => $source) {
        $source_nid = $source['source_nid'];
        if(isset($form['data'][$destination_nid]['sources'][$source_nid])) {
          $old_source = $form['data'][$destination_nid]['sources'][$source_nid];
          $changed = false;
          if(!$changed) {
            if($source['view_mode'] !== $old_source['view_mode']['#default_value']) {
              $changed = true;
            }
          }
          if(!$changed) {
            if($source['weight'] !== $old_source['weight']['#default_value']) {
              $changed = true;
            }
          }
          if($changed) {
            $update[] = $source;
          }
        }
        else { //Ny källa har lagts till i formuläret
          $update[] = $source;
        }
      }
      foreach($form['data'] as $destination_nid => $destination) {
        if(is_numeric($destination_nid)) {
          foreach($destination['sources'] as $source_nid => $source) {
            if(!isset($form_state['values']['sources'][$source_nid])) { //En källa har tagits bort i formuläret
              $delete[] = $source_nid;
            }
          }
        }
      }
    }
  }
  if($update) {
    vertical_scroll_add_rows($update);
  }
}
function vertical_scroll_settings_form($form, $form_state) {
  $form['vertical_scroll_duration'] = array(
    '#title' => t('Duration'),
    '#type' => 'textfield',
    '#default_value' => variable_get('vertical_scroll_duration', 500),
  );
  $form['vertical_scroll_margin'] = array(
    '#title' => t('Negate border and margin'),
    '#type' => 'select',
    '#options' => array(
      false => t('No'),
      true => t('Yes'),
    ),
    '#default_value' => variable_get('vertical_scroll_margin', false),
  );
  $form['vertical_scroll_offset'] = array(
    '#title' => t('Offset'),
    '#type' => 'textfield',
    '#default_value' => variable_get('vertical_scroll_offset', 0),
  );
  $form['vertical_scroll_sync_translations'] = array(
    '#title' => t('Synchronize between translations'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('vertical_scroll_sync_translations', true),
  );
  $form['vertical_scroll_all_links'] = array(
    '#title' => t('Scroll all fragment links'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('vertical_scroll_all_links', true),
  );
  return system_settings_form($form);
}
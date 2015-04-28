<?php
/*
Feedbackfrågor:
- När man skapar eller översätter en nod via adminformuläret, ska man sedan komma till den noden eller tillbaka till adminformuläret?
- Ska adminformuläret alltid visa samma språk vid start eller ska det vara flexibelt?
- Är redigeringslänkarna tillräckligt tydliga?
- Hur kan vi göra sammanfattningen av underlänkarna tydligare?
- Snyggare/större pil för att visa underlänkar?
- Vilka ska de nya färgerna vara? Vi har ju lagt till menyvyn nu.
Att göra:
- Teckensymboler för opublicerade resp. oöversatta resp. både och underlänkar i stället för siffror som visar antal publicerade och översatta.
*/
function adminpanel_settings_form($form, $form_state) {
  $form['#attached']['css'] = array(drupal_get_path('module', 'adminpanel').'/css/adminpanel_admin.css');
  $form['#attached']['js'] = array(drupal_get_path('module', 'adminpanel').'/js/adminpanel_admin.js');
  $result = db_select('menu_custom', 'mc')
    ->fields('mc', array('menu_name', 'title'))
    ->execute();
  $menus = array();
  while($record = $result->fetchAssoc()) {
    $menus[$record['menu_name']] = $record['title'];
  }
  $form['adminpanel_menus'] = array(
    '#title' => t('Select menus for admin panel'),
    '#type' => 'checkboxes',
    '#options' => $menus,
    '#default_value' => variable_get('adminpanel_menus', array()),
    '#weight' => 0,
  );
  $types = node_type_get_names();
  $form['adminpanel_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select content types for admin panel'),
    '#default_value' => variable_get('adminpanel_node_types', array_keys($types)),
    '#options' => $types,
    '#weight' => 1,
  );
  //Lägg in sidtyper
  $names = taxonomy_vocabulary_get_names();
  if(isset($names['sidtyper'])) {
    $vid = $names['sidtyper']->vid;
    $vocabulary = taxonomy_get_tree($vid);
    $terms = array();
    foreach($vocabulary as $term) {
      $terms[$term->tid] = $term->name;
    }
    $form['adminpanel_sidtyper'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Page types').':',
      '#default_value' => variable_get('adminpanel_sidtyper', array_keys($terms)),
      '#options' => $terms,
      '#weight' => 2,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );

  return $form;
}
function adminpanel_settings_form_submit($form, &$form_state) {
  if(isset($form_state['values']['adminpanel_menus'])) {
    variable_set('adminpanel_menus', $form_state['values']['adminpanel_menus']);
  }
  if(isset($form_state['values']['adminpanel_node_types'])) {
    variable_set('adminpanel_node_types', $form_state['values']['adminpanel_node_types']);
  }
  if(isset($form_state['values']['adminpanel_sidtyper'])) {
    variable_set('adminpanel_sidtyper', $form_state['values']['adminpanel_sidtyper']);
  }
  adminpanel_rebuild_all();
  drupal_set_message(t('Your changes have been saved.'));
}
function adminpanel_page() {
  global $user;
  global $language;
  if($user->language !== $language->language) {
    $current_path = current_path();
    drupal_goto($current_path, array('language' => user_preferred_language($user), 'query' => array('lang' => $language->language)));
    return;
  }
  $visual_lang = $language->language;
  if(isset($_GET['lang'])) {
    $visual_lang = check_plain($_GET['lang']);
  }
  //Viktiga variabler som bara behöver hämtas en gång
  $node_types = adminpanel_get_allowed_node_types();
  //Lägg till lite extra info i nodtyperna som bara behöver initieras en gång men som kommer användas senare.
  foreach($node_types as $name => $node_type) {
    $node_type->allowed_menus = variable_get("menu_options_$name", array());
    $node_type->access_create = user_access("create $name content");
  }
  $languages = i18n_improvements_get_enabled_languages();
  $destination = adminpanel_get_destination();
  $multilanguage = (variable_get('general_language_settings', 'single') === 'multi') ? true : false;

  $settings = array('lang' => $visual_lang, 'language_count' => count($languages), 'sync_languages' => variable_get('menu_improvements_sync_order', false), 'sync_status' => variable_get('menu_improvements_sync_status', false), 'destination' => $destination);
  drupal_add_js(array('adminpanel' => $settings), 'setting');
  drupal_add_js(drupal_get_path('module', 'adminpanel').'/js/adminpanel.js');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  if ($path = libraries_get_path('jquery_url')) {
    drupal_add_js($path . '/jquery.url.js');
  }
  if ($path = libraries_get_path('scrollto')) {
    drupal_add_js($path . '/jquery.scrollTo.min.js');
  }
  drupal_add_css(drupal_get_path('module', 'adminpanel').'/css/adminpanel.css');

  $regions = array(
    'main' => array(
      'menu_view' => 'adminpanel_menu_view',
    ),
    'col_left' => array(
      'content_type_view' => 'adminpanel_content_type_view',
      'webform_email_view' => 'adminpanel_webform_email_view',
    ),
    'col_right' => array(
      'search_view' => 'adminpanel_search_content',
      'create_content_view' => 'adminpanel_create_content_content',
      'edit_account_view' => 'adminpanel_edit_account_content',
      'edit_site_config_view' => 'adminpanel_edit_site_config_content',
    ),
  );
  foreach (module_implements('adminpanel_regions') as $module) {
    $function = $module . '_adminpanel_regions';
    $function($regions);
  }
  $page = array(
    '#show_messages' => TRUE,
    '#theme' => 'page',
    '#type' => 'page',
    'content' => array(),
  );
  $vars = array('node_types' => $node_types, 'languages' => $languages, 'destination' => $destination, 'multilanguage' => $multilanguage);
  //----//
  foreach($regions as $name => $region) {
    foreach($region as $key => $callback) {
      if(function_exists($callback)) {
        $returned = call_user_func($callback, $vars);
        if($returned) {
          if(!isset($page['content'][$name])) {
            $page['content'][$name] = array(
              '#sorted' => true,
              '#type' => 'container',
              '#attributes' => array('class' => array('adminpanel-'.$name)),
            );
          }
          $page['content'][$name][$key] = $returned;
        }
      }
    }
  }
  return $page;
}
function adminpanel_menu_view($vars) {
  $return = array();
  $menus = variable_get('adminpanel_menus', array());
  if($menus) {
    $all_menus = menu_load_all();
    foreach($menus as $name => $value) {
      if($value) {
        if(isset($all_menus[$name])) {
          $menu = $all_menus[$name];
          $return[$name] = array(
            '#theme' => 'fieldset',
            '#title' => $menu['title'],
            '#attributes' => array(
              'class' => array('menu-view', 'collapsible'),
            ),
          );
          $form = drupal_get_form('adminpanel_menu_overview_form_'.$name, $menu, $vars);
          $return[$name]['#children'] = render($form);
        }
      }
    }
  }
  return $return;
}
/**
 * Form for editing an entire menu tree at once.
 *
 * Shows for one menu the menu links accessible to the current user and
 * relevant operations.
 */
function adminpanel_menu_overview_form($form, &$form_state, $menu, $vars) {
  $form['#attributes']['class'] = 'menu-overview-form';
  $form['#attributes']['data-menu'] = str_replace('-','_', $menu['menu_name']);
  module_load_include('inc', 'menu', 'menu.admin');
  $menu_form = drupal_get_form('menu_overview_form', $menu);
  $menu_form_submit = $menu_form['#submit'];
  foreach($menu_form_submit as $i => $callback) {
    if($callback === 'menu_overview_form_submit') {
      unset($menu_form_submit[$i]);
    }
  }
  $form['#submit'] = array_merge(array('adminpanel_menu_overview_form_submit'), $menu_form_submit);
  global $language;
  global $menu_admin;
  $language_list = $vars['languages'];
  $all_links = array();
  $multilanguage = $vars['multilanguage'];

  $tabs = array(
    'menu_tabs' => array(
      '#prefix' => '<div class="document-tabs menu-tabs"><ul>',
      '#suffix' => '</ul></div>',
    ),
  );
  $menu_tabs =& $tabs['menu_tabs'];
  $current_path = current_path();
  
  $visual_lang = $language->language;
  if(isset($_GET['lang'])) {
    $visual_lang = check_plain($_GET['lang']);
  }

  foreach($language_list as $key => $lang_full) {
    if($lang_full->enabled) {
      $all_links[$key] = array();
      if($multilanguage) {
        $active = $key===$visual_lang ? 'active' : '';
        $menu_tabs[$key] = array(
          '#markup' => "<li class='menu-tab $key $active'>".l(t($lang_full->name), url($current_path, array('absolute' => true, 'language' => $lang_full)), array('attributes' => array('class' => array($key)))).'</li>',
        );
      }
    }
  }
  $translation_data = array();
  if($multilanguage) {
    $form['#prefix'] = render($tabs);

    $query = db_select('node', 'n')
      ->fields('mn', array('mlid'))
      ->fields('n', array('tnid'));
    $query->innerJoin('menu_node', 'mn', 'n.nid = mn.nid');
    $result = $query->execute();
    while($record = $result->fetchAssoc()) {
			$translation_data[$record['mlid']] = $record['tnid'];
    }
  }

  $retrieved_nodes = adminpanel_get_nodes_by_menu($menu['menu_name']);
  $nodes = $retrieved_nodes['nodes'];
  $tnodes = $retrieved_nodes['tnodes'];
  $form['#nodes'] = $nodes;
  $form['#tnodes'] = $tnodes;

  $form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css');
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu' => $menu['menu_name']), array('fetch' => PDO::FETCH_ASSOC));

  foreach ($result as $item) {
    $all_links[$item['language']][] = $item;
  }
  $form['links'] = array('#tree' => TRUE);
  foreach($all_links as $lang => $links) {
    $tree = menu_tree_data($links);
    $node_links = array();
    menu_tree_collect_node_links($tree, $node_links);
    // We indicate that a menu administrator is running the menu access check.
    $menu_admin = TRUE;
    adminpanel_menu_tree_check_access($tree, $node_links, $lang);
    $menu_admin = FALSE;
    $form['links'][$lang] = _adminpanel_menu_overview_tree_form($tree, $translation_data, $nodes, true);
  }
  $form['#vars'] = $vars;
  $form['#menu'] =  $menu;
  $has_children = false;
  if(isset($form['links'])) {
    foreach(element_children($form['links']) as $lang) {
      if(element_children($form['links'][$lang])) {
        $has_children = true;
        break;
      }
    }
  }
  if($has_children) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = t('There are no menu links yet.');
  }
  
  //dpm($form, 'final form');
  return $form;
}
/**
 * Check access and perform other dynamic operations for each link in the tree.
 *
 * @param $tree
 *   The menu tree you wish to operate on.
 * @param $node_links
 *   A collection of node link references generated from $tree by
 *   menu_tree_collect_node_links().
 */
function adminpanel_menu_tree_check_access(&$tree, $node_links = array(), $lang) {
  if ($node_links) {
    $dead_nodes = variable_get('general_dead_nodes', array());
    $nids = array_keys($node_links);
    $nids = array_diff($nids, $dead_nodes);
    if($nids) {
      $select = db_select('node', 'n');
      $select->addField('n', 'nid');
      $select->condition('n.nid', $nids, 'IN');
      $select->addTag('node_access');
      $nids = $select->execute()->fetchCol();
      foreach ($nids as $nid) {
        foreach ($node_links[$nid] as $mlid => $link) {
          $node_links[$nid][$mlid]['access'] = TRUE;
        }
      }
    }
  }
  //Ändrar det aktuella språket tillfälligt till menylänkarnas språk för att undvika att _menu_tree_check_access döljer dem i onödan
  global $language;
  $current_lang = $language->language;
  $language->language = $lang;
  _menu_tree_check_access($tree);
  $language->language = $current_lang;
}
/**
 * Recursive helper function for menu_overview_form().
 *
 * @param $tree
 *   The menu_tree retrieved by menu_tree_data.
 */
function _adminpanel_menu_overview_tree_form($tree, $translation_data = array(), $nodes, $reset = false) {
  $form = &drupal_static(__FUNCTION__, array('#tree' => TRUE));
  if($reset) {
    $form = array('#tree' => TRUE);
  }
  foreach ($tree as $data) {
    $title = '';
    $item = $data['link'];
    // Don't show callbacks; these have $item['hidden'] < 0.
    if ($item && $item['hidden'] >= 0) {
      $mlid = 'mlid:' . $item['mlid'];
      $form[$mlid]['#item'] = $item;
      $form[$mlid]['#attributes'] = $item['hidden'] ? array('class' => array('menu-disabled')) : array('class' => array('menu-enabled'));
      $mlid_short = $item['mlid'];
      if(isset($nodes[$mlid_short])) { //Nod
        $form[$mlid]['#node'] = $nodes[$mlid_short];
      }
      else { //Menylänk
        $form[$mlid]['#attributes']['class'][] = 'type-menu-link';
      }
      if(isset($translation_data[$item['mlid']])) {
        $form[$mlid]['#tnid'] = $translation_data[$item['mlid']];
      }
      $form[$mlid]['title']['#markup'] = l($item['title'], $item['href'], $item['localized_options']);
      $form[$mlid]['hidden'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable @title menu link', array('@title' => $item['title'])),
        '#title_display' => 'invisible',
        '#default_value' => !$item['hidden'],
      );
      if(isset($nodes[$mlid_short])) {
        $form[$mlid]['hidden']['#default_value'] = $nodes[$mlid_short]->status ? TRUE : FALSE;
        $form[$mlid]['hidden']['#checked'] = $nodes[$mlid_short]->status ? TRUE : FALSE;
      }
      $form[$mlid]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 50,
        '#default_value' => $item['weight'],
        '#title_display' => 'invisible',
        '#title' => t('Weight for @title', array('@title' => $item['title'])),
      );
      $form[$mlid]['mlid'] = array(
        '#type' => 'hidden',
        '#value' => $item['mlid'],
      );
      $form[$mlid]['plid'] = array(
        '#type' => 'hidden',
        '#default_value' => $item['plid'],
      );
      // Build a list of operations.
      $operations = array();
      $operations['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/edit');
      // Only items created by the menu module can be deleted.
      if ($item['module'] == 'menu' || $item['updated'] == 1) {
        $operations['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/delete');
      }
      // Set the reset column.
      elseif ($item['module'] == 'system' && $item['customized']) {
        $operations['reset'] = array('#type' => 'link', '#title' => t('reset'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/reset');
      }
      $form[$mlid]['operations'] = $operations;
    }

    if ($data['below']) {
      $form[$mlid]['#attributes']['class'][] = 'link-parent';
      _adminpanel_menu_overview_tree_form($data['below'], $translation_data, $nodes);
    }
  }
  //dpm($form);
  return $form;
}
/**
 * Returns HTML for the menu overview form into a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_adminpanel_menu_overview_form($variables) {
  global $language;
  $real_form = $variables['form'];
  $menu_name = str_replace('-', '_', $real_form['#menu']['menu_name']);
  $menu_name_org = $real_form['#menu']['menu_name'];
  $multilanguage = $real_form['#vars']['multilanguage'];
  $node_types = $real_form['#vars']['node_types'];
  $destination = $real_form['#vars']['destination'];
  $languages = $real_form['#vars']['languages'];
  $nodes = $real_form['#nodes'];
  $tnodes = $real_form['#tnodes'];
  $menulink_translations = adminpanel_get_menulink_translations($real_form, $languages);

  $header = array(
    t('Menu link'),
    t('Type'),
    array('data' => t('Published'), 'class' => array('checkbox')),
    t('Weight'),
  );
  if($multilanguage) {
    $header[] = t('Languages');
  }
  $header[] = t('Sublinks');
  $header[] = t('Add sublink');

  $rows = array();
  $output = '';
  foreach(element_children($real_form['links']) as $lang) {
    if(!$multilanguage) {
      if($lang !== $language->language) {
        continue;
      }
    }
    $add_link = adminpanel_theme_add_links($menu_name_org, 0, $lang, $node_types, false);
    $output .= $add_link;
    drupal_add_tabledrag('menu-overview-'.$menu_name.'-'.$lang, 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
    drupal_add_tabledrag('menu-overview-'.$menu_name.'-'.$lang, 'order', 'sibling', 'menu-weight');
    $form = $real_form['links'][$lang];
    $rows = array();
    foreach (element_children($form) as $mlid) {
      if (isset($form[$mlid]['hidden'])) {
        $element = &$form[$mlid];
        $mlid_short = $element['mlid']['#value'];

        // Add special classes to be used for tabledrag.js.
        $element['plid']['#attributes']['class'] = array('menu-plid');
        $element['mlid']['#attributes']['class'] = array('menu-mlid');
        $element['weight']['#attributes']['class'] = array('menu-weight');
  
        // Change the parent field to a hidden. This allows any value but hides the field.
        $element['plid']['#type'] = 'hidden';


        $type = array('data' => t('Menu link'), 'class' => array('col-content-type', 'type-menu-link'));
        $col_languages = '';
        $col_edit = '';
        if(isset($nodes[$mlid_short])) { //Nod
          $this_node = $nodes[$mlid_short];
          //if(count($this_node->menu_node_links) > 1) dpm($this_node, 'node with multiple menu links');
          //Titellänk
          if(node_access('update', $this_node)) {
            $element['title'] = array(
              'data' => array(
                '#type' => 'link',
                '#title' => $element['#item']['title'],
                '#href' => 'node/'.$this_node->nid.'/edit',
                '#options' => array_merge($element['#item']['localized_options'], array('attributes' => array('class' => array('edit-link'), 'title' => t('Edit !title', array('!title' => $this_node->title))), 'query' => array('destination' => $destination))),
              ),
            );
          }
          else {
            $element['title'] = array(
              'data' => array(
                '#type' => 'markup',
                '#markup' => $element['#item']['title'],
                '#options' => $element['#item']['localized_options'],
              ),
            );
          }
          $image = array(
            'path' => drupal_get_path('module', 'adminpanel').'/images/door_arrow_icon.png',
          );
          $element['title']['#suffix'] = '';
          $frontpage = variable_get('site_frontpage', '');
          //Typ
          $type = array('data' => $node_types[$this_node->type]->name, 'class' => 'col-content-type');
          if($multilanguage) {
            $col_languages = adminpanel_theme_language_column_node($this_node->translations, $languages, $destination, $tnodes);
            foreach(element_children($this_node->translations) as $translation_lang) {
              if($frontpage === 'node/'.$this_node->translations[$translation_lang]['nid']) {
                $element['title']['#suffix'] .= ' (front)';
                break;
              }
            }
          }
          else {
            if($frontpage === 'node/'.$this_node->nid) {
              $element['title']['#suffix'] .= ' (front)';
            }
          }
          $element['title']['#suffix'] .= l(theme('image', $image), $element['#item']['href'], array_merge($element['#item']['localized_options'], array('attributes' => array('class' => array('goto-link'), 'title' => t('Go to the page')), 'html' => true, 'language' => $languages[$this_node->language])));
        }
        else { //Menylänk
          if(user_access("administer $menu_name_org menu items")) {
            $element['title']['#markup'] = l($element['#item']['title'],'admin/structure/menu/item/'.$element['#item']['mlid'].'/edit', array('language' => $language, 'attributes' => array('title' => t('Edit @node', array('@node' => $element['#item']['title']))), 'query' => array('destination' => $destination)));
          }
          else {
            $element['title']['#markup'] = $element['#item']['title'];
          }
          if($element['#item']['href'] !== '<nolink>' && $element['#item']['href'] !== '<separator>') {
            $image = array(
              'path' => drupal_get_path('module', 'adminpanel').'/images/door_arrow_icon.png',
            );
            $element['title']['#suffix'] = l(theme('image', $image), $element['#item']['href'], array_merge($element['#item']['localized_options'], array('attributes' => array('class' => array('goto-link'), 'title' => t('Go to the page')), 'html' => true)));
          }

          if($multilanguage) {
            if(isset($element['#item']['i18n_tsid'])) {
              $translations = array();
              if(isset($menulink_translations[$element['#item']['i18n_tsid']])) {
                $translations = $menulink_translations[$element['#item']['i18n_tsid']];
              }
              $parent_translations = array();
              $this_plid = $element['#item']['plid'];
              if($this_plid > 0) {
                if(isset($nodes[$this_plid])) { //Nod
                  $parent_translations = $nodes[$this_plid]->translations;
                }
                else { //Menylänk
                  if(isset($form['mlid:'.$this_plid])) {
                    $this_parent = $form['mlid:'.$this_plid]['#item'];
                    if(isset($this_parent['i18n_tsid']) && isset($menulink_translations[$this_parent['i18n_tsid']])) {
                      $parent_translations = $menulink_translations[$this_parent['i18n_tsid']];
                    }
                  }
                }
              }
              $col_languages = adminpanel_theme_language_column_menulink($element['#item'], $translations, $parent_translations, $languages, $destination, $menu_name_org);
            }
          }
        }
        $row = array();
        $row[] = array('data' => theme('indentation', array('size' => $element['#item']['depth'] - 1)) . '<div class="open-children" title="'.t('Show/hide sublinks').'"></div>' . drupal_render($element['title']), 'class' => array('col-title'));
        $row[] = $type;
        $row[] = array('data' => drupal_render($element['hidden']), 'class' => array('checkbox', 'menu-enabled'));
        $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);
        if($multilanguage) {
          $row[] = array('data' => $col_languages, 'class' => 'col-languages');
        }
        $row[] = array('data' => '<span title="'.t('Total number of sublinks').'" class="menu-sublinks-total"></span><span title="'.t('The number of published sublinks').'" class="menu-sublinks-published"></span><span  title="'.t('The number of fully translated sublinks').'"class="menu-sublinks-translated"></span>', 'class' => array('menu-sublinks'), 'title' => t('Show/hide sublinks'));
        $row[] = array('data' => adminpanel_theme_add_links($menu_name_org, $element['mlid']['#value'], $element['#item']['language'], $node_types), 'class' => 'col-create-sublink');
        $element['#attributes']['data-mlid'] = $element['mlid']['#value'];
        $element['#attributes']['data-plid'] = $element['plid']['#value'];
        $element['#attributes']['data-lang'] = $element['#item']['language'];
        $element['#attributes']['data-title'] = $element['#item']['title'];
        $element['#attributes']['data-menu'] = $menu_name;
        if(!empty($element['#item']['i18n_tsid'])) {
          $element['#attributes']['data-tsid'] = $element['#item']['i18n_tsid'];
        }
        $row = array_merge(array('data' => $row), $element['#attributes']);
        $row['class'][] = 'draggable';
        $rows[] = $row;
      }
    }
    if (empty($rows)) {
			if(!empty($real_form['#empty_text'])) {
      	$rows[] = array(array('data' => $real_form['#empty_text'], 'colspan' => '7'));
			}
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'menu-overview-'.$menu_name.'-'.$lang, 'class' => array('menu-overview-table', $lang))));
  }
  foreach(element_children($real_form) as $key) {
    if($key !== 'links') {
      $output .= drupal_render($real_form[$key]);
    }
  }
  return $output;
}
/**
 * Submit handler for the menu overview form.
 *
 * This function takes great care in saving parent items first, then items
 * underneath them. Saving items in the incorrect order can break the menu tree.
 *
 * @see menu_overview_form()
 */
function adminpanel_menu_overview_form_submit($form, &$form_state) {
  if(isset($form_state['values']['links'])) {
    foreach($form_state['values']['links'] as $lang => $links) {
      $form_state['values'] = array_merge($form_state['values'], $links);
    }
    unset($form_state['values']['links']);
  }
  $form_links = array();
  foreach(element_children($form['links']) as $lang) {
    foreach(element_children($form['links'][$lang]) as $link) {
      $form_links[$link] = $form['links'][$lang][$link];
    }
  }
  // When dealing with saving menu items, the order in which these items are
  // saved is critical. If a changed child item is saved before its parent,
  // the child item could be saved with an invalid path past its immediate
  // parent. To prevent this, save items in the form in the same order they
  // are sent by $_POST, ensuring parents are saved first, then their children.
  // See http://drupal.org/node/181126#comment-632270
  $order = array_flip(array_keys($form_state['input'])); // Get the $_POST order.
  $form = array_merge($order, $form_links); // Update our original form with the new order.
  $updated_items = array();
  $fields = array('weight', 'plid');
  foreach (element_children($form_links) as $mlid) {
    if (isset($form_links[$mlid]['#item'])) {
      $element = $form_links[$mlid];
      // Update any fields that have changed in this menu item.
      foreach ($fields as $field) {
        if ($element[$field]['#value'] != $element[$field]['#default_value']) {
          $element['#item'][$field] = $element[$field]['#value'];
          $updated_items[$mlid] = $element['#item'];
        }
      }
      //Uppdatera publiceringsstatus beroende på om länken hör till en nod eller inte
      if ($element['hidden']['#value'] != $element['hidden']['#default_value']) {
        if(isset($element['#node'])) {
          //Uppdatera nodstatus
          $element['#node']->status = $element['hidden']['#value'];
          node_save($element['#node']);
        }
        //Uppdatera menylänkstatus
        // Convert to integer rather than boolean due to PDO cast to string.
        $element['#item']['hidden'] = $element['hidden']['#value'] ? 0 : 1;
        $updated_items[$mlid] = $element['#item'];
      }
    }
  }
  // Save all our changed items to the database.
  foreach ($updated_items as $item) {
    $item['customized'] = 1;
    menu_link_save($item);
  }
  drupal_set_message(t('Your configuration has been saved.'));
}
function adminpanel_search_content($vars) {
  $form = drupal_get_form('adminpanel_search_content_form', $vars);
  return array(
    'default' => array(
      '#theme' => 'fieldset',
      '#title' => t('Search for content title'),
      '#attributes' => array(
        'class' => array('collapsible', 'search-view'),
      ),
      '#children' => render($form),
    ),
  );
}
function adminpanel_search_content_form($form, $form_state, $vars) {
  $form['adminpanel_search'] = array(
    '#type' => 'textfield',
  );
  $form['adminpanel_result'] = array(
    '#markup' => '<div id="adminpanel-search-result"></div>',
  );
  return $form;
}
function adminpanel_ajax_search_callback() {
  $GLOBALS['conf']['cache'] = FALSE;
  user_cookie_save(array('adminpanel_cookie'=>'nocache'));
  $content = '';
  if(isset($_GET['search'])) {
    $node_types = adminpanel_get_allowed_node_types();
    $languages = i18n_improvements_get_enabled_languages();
    $destination = adminpanel_get_destination();
    $string = check_plain($_GET['search']);
    $header = array(t('Title'), t('Type'));
    $rows = array();

    //Hämta noder
    $query = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->fields('mn', array('mlid'))
      ->condition('n.title', '%'.$string.'%', 'LIKE')
      ->condition('n.type', array_keys($node_types))
      ->condition('n.language', array_keys($languages));
    $query->leftJoin('menu_node', 'mn', 'n.nid = mn.nid');
    $result = $query->execute();
    $nids = array();
    $used_mlids = array();
    while($record = $result->fetchAssoc()) {
      $nids[] = $record['nid'];
      if($record['mlid']) {
        $used_mlids[] = $record['mlid'];
      }
    }
    if($nids) {
      $nodes = node_load_multiple($nids);
      foreach($nodes as $this_node) {
        $l_options = $this_node->language != LANGUAGE_NONE && isset($languages[$this_node->language]) ? array('language' => $languages[$this_node->language]) : array();
        $l_options['query'] = array('destination' => $destination);
        $l_options['attributes'] = array('title' => t('Edit @node', array('@node' => $this_node->title)));
        if(node_access('update', $this_node)) {
          $col_title = array(
            '#type' => 'link',
            '#title' => $this_node->title,
            '#href' => 'node/'.$this_node->nid.'/edit',
            '#options' => $l_options,
          );
          $col_title['#options']['attributes']['class'] = array('edit-link');
        }
        else {
          $col_title = array(
            '#type' => 'markup',
            '#markup' => $this_node->title,
            '#options' => $l_options,
          );
        }
        $image = array(
          'path' => drupal_get_path('module', 'adminpanel').'/images/door_arrow_icon.png',
        );
        $col_title['#suffix'] = l(theme('image', $image), "node/$this_node->nid", array('attributes' => array('class' => array('goto-link'), 'title' => t('Go to the page')), 'html' => true, 'language' => $languages[$this_node->language]));
        $col_type = $node_types[$this_node->type]->name;
        $rows[] = array('title' => array('data' => drupal_render($col_title), 'data-title' => $this_node->title), 'type' => array('data' => $col_type));
      }
    }
    
    //Hämta menylänkar
    $menus = variable_get('adminpanel_menus', array());
    $allowed_menus = array();
    foreach($menus as $name => $value) {
      if($value) {
        $allowed_menus[] = $value;
      }
    }
    if($allowed_menus) {
      $query = db_select('menu_links', 'ml')
        ->fields('ml', array('mlid'))
        ->condition('menu_name', $allowed_menus)
        ->condition('language', array_keys($languages))
        ->condition('link_title', '%'.$string.'%', 'LIKE');
        if($used_mlids) {
          $query->condition('mlid', $used_mlids, 'NOT IN');
        }
      $result = $query->execute();
      $links = array();
      while($mlid = $result->fetchField()) {
        $links[] = menu_link_load($mlid);
      }
      if($links) {
        foreach($links as $link) {
          if(user_access('administer '.$link['menu_name'].' menu items')) {
            $col_title = l($link['title'],'admin/structure/menu/item/'.$link['mlid'].'/edit', array('language' => $languages[$link['language']], 'attributes' => array('title' => t('Edit @node', array('@node' => $link['title']))), 'query' => array('destination' => $destination)));
          }
          else {
            $col_title = $link['title'];
          }
          if($link['href'] !== '<nolink>' && $link['href'] !== '<separator>') {
            $image = array(
              'path' => drupal_get_path('module', 'adminpanel').'/images/door_arrow_icon.png',
            );
            $col_title .= l(theme('image', $image), $link['href'],  array('attributes' => array('class' => array('goto-link'), 'title' => t('Go to the page')), 'html' => true, 'language' => $languages[$link['language']]));
          }
          $col_type = t('Menu link');
          $rows[] = array('title' => array('data' => $col_title, 'data-title' => $link['title']), 'type' => array('data' => $col_type, 'class' => 'type-menu-link'));
        }
      }
    }
    if($rows) {
      //Sortera raderna efter titel
      usort($rows, 'adminpanel_sort_search_result');
      $content = theme('table', array('header' => $header, 'rows' => $rows));
    }
  }
  drupal_json_output ($content);
}
function adminpanel_sort_search_result($a, $b) {
  return strcmp($a['title']['data-title'], $b['title']['data-title']);
}
function adminpanel_content_type_view($vars) {
  $node_types = $vars['node_types'];
  $languages = $vars['languages'];
  $destination = $vars['destination'];
  $multilanguage = $vars['multilanguage'];
  $default_theme = variable_get('theme_default', '');

  $content = array();

	$params = array();
  $query = '
  SELECT COUNT(ap.nid) AS nid_count,
  (SELECT COUNT(ap.nid) FROM {adminpanel} ap INNER JOIN {node} ON ap.nid = node.nid LEFT JOIN {field_data_field_node_per_theme} npt ON ap.nid = npt.entity_id WHERE ap.status = 1 AND node.language IN (:languages) AND (npt.entity_type = \'node\' OR npt.entity_type IS NULL) AND (npt.field_node_per_theme_value = \''.$default_theme.'\' OR npt.field_node_per_theme_value IS NULL)) AS status_count';
  $params[':languages'] = array_keys($languages);
  if($multilanguage) {
    $languages = i18n_improvements_get_enabled_languages();
    $query .= ', (SELECT COUNT(ap.nid) FROM {adminpanel} ap INNER JOIN {node} ON ap.nid = node.nid LEFT JOIN {field_data_field_node_per_theme} npt ON ap.nid = npt.entity_id WHERE ap.translations = :max AND node.language IN (:languages) AND (npt.entity_type = \'node\' OR npt.entity_type IS NULL) AND (npt.field_node_per_theme_value = \''.$default_theme.'\' OR npt.field_node_per_theme_value IS NULL)) AS translations_count';
    $params[':max'] = count($languages);
  }
  $query .= ' FROM {adminpanel} ap INNER JOIN {node} ON ap.nid = node.nid LEFT JOIN {field_data_field_node_per_theme} npt ON ap.nid = npt.entity_id WHERE node.language IN (:languages) AND (npt.entity_type = \'node\' OR npt.entity_type IS NULL) AND (npt.field_node_per_theme_value = \''.$default_theme.'\' OR npt.field_node_per_theme_value IS NULL)';

  $count = db_query($query, $params)->fetchAssoc();

  $count_page = $count['nid_count'];
  $count_published = $count['status_count'];
  $count_unpublished = $count_page - $count_published;
  $content['default_count'] = array(
    '#markup' => adminpanel_theme_subblock(t('Node Count'), t('!count nodes', array('!count' => '<span class="number adminpanel-green-color">'.$count_page.'</span>'))),
  );
  $content['published_count'] = array(
    '#markup' => adminpanel_theme_subblock(t('Published Count'), '<p><span class="number adminpanel-green-color">'.$count_published.'</span> '.t('published nodes').'</p><p><span class="number adminpanel-red-color">'.$count_unpublished.'</span> '.t('unpublished nodes').'</p>'),
  );

  if($multilanguage) {
    $count_translated = $count['translations_count'];
    $count_untranslated = $count_page - $count_translated;
    $content['translated_count'] = array(
      '#markup' => adminpanel_theme_subblock(t('Translated Count'), '
  <p><span class="number adminpanel-green-color">'.$count_translated.'</span> '.t('translated nodes').'</p>
  <p><span class="number adminpanel-red-color">'.$count_untranslated.'</span> '.t('nodes lack translations').'</p>
'),
    );
  }
  //Val av innehållstyp för filtrering
  $tabs = array(
    '#prefix' => '<div class="bullet-tabs">',
    '#suffix' => '</div>',
    'title' => array(
      '#markup' => '<h2>'.t('Display content of type...').'</h2>',
    ),
    'bullet_tabs' => array(
      '#prefix' => '<ul>',
      '#suffix' => '</ul>',
    ),
  );
  $select_content_type =& $tabs['bullet_tabs'];
  $select_content_type['_none'] = array(
    '#markup' => '<li class="bullet-tab none active" data-type="none"><span>'.t('All').'</span></li>',
  );
  foreach($node_types as $name => $type) {
    $select_content_type[$name] = array(
      '#markup' => '<li class="bullet-tab '.str_replace('_', '-', $name).'" data-type="'.str_replace('_', '-', $name).'"><span>'.$type->name.'</span></li>',
    );
  }
  $content['select_content_type'] = $tabs;

  $content['node_list_default'] = drupal_get_form('adminpanel_generate_node_list_none', 'none');
  foreach($node_types as $name => $type) {
    $content["node_list_$name"] = drupal_get_form('adminpanel_generate_node_list_'.$name, $name);
  }

  $return = array(
    'default' => array(
      '#theme' => 'fieldset',
      '#title' => t('Content'),
      '#attributes' => array(
        'class' => array('collapsible', 'content-type-view'),
      ),
      '#children' => drupal_render($content),
    ),
  );
  return $return;
}
function adminpanel_ajax_refresh_table_callback() {
  $GLOBALS['conf']['cache'] = FALSE;
  user_cookie_save(array('adminpanel_cookie'=>'nocache'));
  $node_type = 'none';
  if(isset($_GET['node_type'])) {
    $node_type = check_plain($_GET['node_type']);
  }
  $result = drupal_get_form("adminpanel_generate_node_list_$node_type", $node_type);
  drupal_json_output (drupal_render($result));
}
function adminpanel_generate_node_list($form, &$form_state, $node_type = 'none') {
  $id = &drupal_static(__FUNCTION__);
  if(!isset($id)) {
    $id = 0;
  }
  global $language;
  //Hämta alla viktiga variabler igen för att det ska funka med AJAX
  $multilanguage = (variable_get('general_language_settings', 'single') === 'multi') ? true : false;
  $node_types = adminpanel_get_allowed_node_types();
  $languages = i18n_improvements_get_enabled_languages();
  $destination = adminpanel_get_destination();
  $form = array();

  $result_chunk = 40;
  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'a.title'),
  );
  if($node_type === 'none') {
    $header['type'] = array('data' => t('Type'), 'field' => 'a.type');
  }
  $header['status'] = array('data' => t('Status'), 'field' => 'a.status');
  if ($multilanguage) {
    $header['language'] = array('data' => t('Languages'), 'field' => 'a.translation_points');
  }
  $result = db_select('adminpanel', 'a');
  $result->addField('a', 'nid', 'nid');
  if($node_type !== 'none') {
    $result->condition('type', $node_type);
  }
  $result = $result->extend('PagerDefault')->limit($result_chunk);
  $result = $result->extend('TableSort')->orderByHeader($header);
  $result = $result->execute();
  $nids = array();
  if($result) {
    while($nid = $result->fetchField()) {
      $nids[] = $nid;
    }
  }
  $nodes = array();
  $t_nodes = array();
  if($nids) {
    $nodes = node_load_multiple($nids);
    //Rensa bort noder enligt Menu Per Theme
    $default_theme = variable_get('theme_default', '');
    foreach($nodes as $nid => $node) {
      if(isset($node->field_node_per_theme)) {
        $allowed_themes = field_extract_values('node', $node, 'field_node_per_theme');
        if($allowed_themes) {
          $keep = 0;
          foreach($allowed_themes as $i => $value) {
            if($default_theme === $value) {
              $keep = 1;
              break;
            }
          }
          if(!$keep) {
            unset($nids[$nid]);
            unset($nodes[$nid]);
          }
        }
      }
    }
    if($multilanguage) {
      $t_result = db_select('node', 'n')
      ->fields('n', array('nid', 'type', 'language', 'title', 'status', 'tnid'))
      ->condition('n.tnid', $nids)
      ->execute();
      $translations = array();
      $t_nids = array();
      while($record = $t_result->fetchAssoc()) {
        $t_nids[] = $record['nid'];
        if(!isset($translations[$record['tnid']])) $translations[$record['tnid']] = array();
        if($record['tnid']==$record['nid']) { //tsource
          $translations[$record['tnid']]['#tsource_nid'] = $record['nid'];
          $translations[$record['tnid']]['#tsource_type'] = $record['type'];
        }
        $translations[$record['tnid']][$record['language']] = array(
          'nid' => $record['nid'],
          'title' => $record['title'],
          'status' => $record['status'],
        );
      }
      $t_nodes = node_load_multiple($t_nids);

      foreach($nodes as $nid => &$node) {
        $node->translations = $translations[$node->nid];
      }
    }
  }
  // Enable language column if translation module is enabled or if we have any
  // node with language.
  $admin_access = user_access('administer nodes');

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'a.title'),
  );
  if($node_type === 'none') {
    $header['type'] = array('data' => t('Type'), 'field' => 'a.type');
  }
  $header['status'] = array('data' => t('Status'), 'field' => 'a.status');
  if ($multilanguage) {
    $header['language'] = array('data' => t('Languages'), 'field' => 'a.translation_points');
  }
  // Prepare the list of nodes.
  $options = array();
  foreach ($nodes as $this_node) {
    if(isset($languages[$this_node->language])) {
      $l_options = $this_node->language != LANGUAGE_NONE && isset($languages[$this_node->language]) ? array('language' => $languages[$this_node->language]) : array();
      $l_options['query'] = array('destination' => $destination);
      $l_options['attributes'] = array('title' => t('Edit @node', array('@node' => $this_node->title)));
      if(node_access('update', $this_node)) {
        $options[$this_node->nid] = array(
          'title' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => $this_node->title,
              '#href' => 'node/'.$this_node->nid.'/edit',
              '#options' => $l_options,
            ),
          ),
        );
        $options[$this_node->nid]['title']['data']['#options']['attributes']['class'] = array('edit-link');
      }
      else {
        $options[$this_node->nid] = array(
          'title' => array(
            'data' => array(
              '#type' => 'markup',
              '#markup' => $this_node->title,
              '#options' => $l_options,
            ),
          ),
        );
      }
      $options[$this_node->nid]['title']['data']['#suffix'] = '';
      $frontpage = variable_get('site_frontpage', '');
      if($frontpage === 'node/'.$this_node->nid) {
        $options[$this_node->nid]['title']['data']['#suffix'] .= ' (front)';
      }
      $image = array(
        'path' => drupal_get_path('module', 'adminpanel').'/images/door_arrow_icon.png',
      );
      $options[$this_node->nid]['title']['data']['#suffix'] .= l(theme('image', $image), "node/$this_node->nid", array('attributes' => array('class' => array('goto-link'), 'title' => t('Go to the page')), 'html' => true, 'language' => $languages[$this_node->language]));
      if($node_type === 'none') {
        $options[$this_node->nid]['type'] = $node_types[$this_node->type]->name;
      }
      $options[$this_node->nid]['status'] = $this_node->status ? '<span class="node-status">'.t('published').'</span>' : '<span class="node-status adminpanel-red-color">'.t('not published').'</span>';
  
      if ($multilanguage) {
        $options[$this_node->nid]['language'] = adminpanel_theme_language_column_node($this_node->translations, $languages, $destination, $t_nodes);
      }
    }
  }
  $form['nodes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No content available.'),
    '#attributes' => array('class' => array('node-list-table')),
  );

  $form['pager'] = array(
    '#markup' => theme('pager', array('element' => $id)),
  );
  //Workaround för bugg i theme_pager
  if($id > 0) {
    if(isset($form['pager']['#markup'])) {
      $form['pager']['#markup'] = theme('pager');
    }
  }
  $form['#attributes']['class'][] = 'node-list-form';
  $id++;
  return $form;
}
function adminpanel_webform_email_view($vars) {
  if(variable_get('general_functions_contact', false)) {
    $content = array();
    global $language;
    $languages = $vars['languages'];
    $multilanguage = $vars['multilanguage'];
    $tabs = array(
      '#prefix' => '<div class="document-tabs webform-tabs"><ul>',
      '#suffix' => '</ul></div>',
    );
    $current_path = current_path();
    
    $visual_lang = $language->language;
    if(isset($_GET['lang'])) {
      $visual_lang = check_plain($_GET['lang']);
    }
    $webform_node_types = webform_variable_get('webform_node_types');
    foreach($webform_node_types as $i => $type) {
      if($type === 'webform_template') {
        unset($webform_node_types[$i]);
        break;
      }
    }
    $query = db_select('webform_emails', 'we')
    ->fields('we', array('nid', 'email'))
    ->fields('n', array('language'))
    ->condition('n.type', $webform_node_types);
    $query->innerJoin('node', 'n', 'we.nid = n.nid');
    $result = $query->execute()->fetchAll();
    $sorted_results = array();
    foreach(element_children($result) as $i) {
      if(!isset($sorted_results[$result[$i]->language])) $sorted_results[$result[$i]->language] = array();
      if(!isset($sorted_results[$result[$i]->language][$result[$i]->nid])) $sorted_results[$result[$i]->language][$result[$i]->nid] = array();
      $sorted_results[$result[$i]->language][$result[$i]->nid][] = $result[$i]->email;
    }

    foreach($languages as $langcode => $lang_full) {
      if($lang_full->enabled) {
        $all_links[$langcode] = array();
        if($multilanguage) {
          $active = $langcode===$visual_lang ? 'active' : '';
          $tabs[$langcode] = array(
            '#markup' => "<li class='webform-tab $langcode $active'>".l(t($lang_full->name), url($current_path, array('absolute' => true, 'language' => $lang_full)), array('attributes' => array('class' => array($langcode)))).'</li>',
          );
        }
      }
      $rows = array();
      if(!empty($sorted_results[$langcode])) {
	      foreach($sorted_results[$langcode] as $nid => $data) {
					foreach($data as $i => $address) {
						if(strpos($address, '@') === false) {
							$data[$i] = t('Variable');
						}
					}
	        $this_node = node_load($nid);
	        $node_access = node_access('update', $this_node);
	        if($node_access) {
	          $l_options = $this_node->language != LANGUAGE_NONE && isset($languages[$this_node->language]) ? array('language' => $languages[$this_node->language]) : array();
	          $l_options['attributes'] = array('title' => t('Edit @node', array('@node' => $this_node->title)));
	          $row = array(
	            'title' => array(
	              'data' => array(
	                '#type' => 'link',
	                '#title' => $this_node->title,
	                '#href' => 'node/'.$this_node->nid.'/edit',
	                '#options' => $l_options,
	              ),
	            ),
	          );
	          $row['title']['data']['#options']['attributes']['class'] = array('edit-link', 'webform-item-title');
	        }
	        else {
	          $row = array(
	            'title' => array(
	              'data' => array(
	                '#markup' => '<span class="webform-item-title">'.$this_node->title.'</span>',
	              ),
	            ),
	          );
	        }
	        $row['title']['data']['#suffix'] = '';
	        $frontpage = variable_get('site_frontpage', '');
	        if($frontpage === 'node/'.$this_node->nid) {
	          $children['title']['data']['#suffix'] .= ' (front)';
	        }
	        $image = array(
	          'path' => drupal_get_path('module', 'adminpanel').'/images/door_arrow_icon.png',
	        );
	        $row['title']['data']['#suffix'] .= l(theme('image', $image), "node/$this_node->nid", array('attributes' => array('class' => array('goto-link'), 'title' => t('Go to the page')), 'html' => true, 'language' => $languages[$this_node->language]));
	        if($node_access && user_access('webform_improvements_subtab_email')) {
	          $l_options = $this_node->language != LANGUAGE_NONE && isset($languages[$this_node->language]) ? array('language' => $languages[$this_node->language]) : array();
	          $l_options['attributes'] = array('title' => t('Edit email settings'));
	          $row['emails'] = array(
	            'data' => array(
	              '#type' => 'link',
	              '#title' => implode(', ', $data),
	              '#href' => 'node/'.$this_node->nid.'/webform/emails',
	              '#options' => $l_options,
	            ),
	          );
	          $row['emails']['data']['#options']['attributes']['class'] = array('webform-item-emails');
	        }
	        else {
	          $row['emails'] = array(
	            'data' => array(
	              '#markup' => '<span class="webform-item-emails">'.implode(', ', $data).'</span>',
	            ),
	          );
	        }
	        if($row) {
	          $rows[] = $row;
	        }
	      }
			}
      $content[$langcode] = array(
        '#theme' => 'table',
        '#empty' => t('No contact forms have been defined.'),
        '#attributes' => array('id' => 'webform-email-view-table-'.$langcode, 'class' => array('webform-email-view-table', $langcode)),
        '#header' => array(t('Title'), t('E-mail addresses')),
        '#rows' => $rows,
      );
    }
    $return = array(
      '#theme' => 'fieldset',
      '#title' => t('E-mails for contact forms'),
      '#attributes' => array('class' => array('collapsible', 'webform-email-view')),
      '#children' => drupal_render($content),
    );
    if($multilanguage) {
      $return['#children'] = drupal_render($tabs) . $return['#children'];
    }
    return $return;

  }
  return null;
}
function adminpanel_theme_subblock($title, $content) {
  return "
    <div class='adminpanel-subblock'>
    <h2>$title</h2>
    <div class='content'>$content</div>
    </div>
  ";
}
function adminpanel_theme_language_column_menulink($item, $translations, $parent_translations, $languages, $destination, $menu_name) {
	$base_path = base_path();
  $content = '';
  foreach($languages as $langcode => $language) {
    $class = '';
    $output = '';
    if(isset($translations[$langcode])) {
      $translation = $translations[$langcode];
      if($translation['status']) {
        $class = 'published';
        if(user_access("administer $menu_name menu items")) {
          $output = l(strtoupper($langcode),'admin/structure/menu/item/'.$translation['mlid'].'/edit', array('language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Edit @node', array('@node' => $translation['title']))), 'query' => array('destination' => $destination)));
        }
        else {
          $output = "<div class='adminpanel-translation $class'>".strtoupper($langcode).'</div>';
        }
      }
      else {
        $class = 'unpublished adminpanel-red-color';
        if(user_access("administer $menu_name menu items")) {
          $output = l(strtoupper($langcode),'admin/structure/menu/item/'.$translation['mlid'].'/edit', array('language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Edit @node (unpublished)', array('@node' => $translation['title']))), 'query' => array('destination' => $destination)));
        }
        else {
          $output = "<div class='adminpanel-translation $class'>".strtoupper($langcode).'</div>';
        }
      }
    }
    else {
      if($item['language'] === $langcode) {
        if(!$item['hidden']) {
          $class = 'published';
          if(user_access("administer $menu_name menu items")) {
            $output = l(strtoupper($langcode),'admin/structure/menu/item/'.$item['mlid'].'/edit', array('language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Edit @node', array('@node' => $item['link_title']))), 'query' => array('destination' => $destination)));
          }
          else {
            $output = "<div class='adminpanel-translation $class'>".strtoupper($langcode).'</div>';
          }
        }
        else {
          $class = 'unpublished adminpanel-red-color';
          if(user_access("administer $menu_name menu items")) {
            $output = l(strtoupper($langcode),'admin/structure/menu/item/'.$item['mlid'].'/edit', array('language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Edit @node (unpublished)', array('@node' => $item['link_title']))), 'query' => array('destination' => $destination)));
          }
          else {
            $output = "<div class='adminpanel-translation $class'>".strtoupper($langcode).'</div>';
          }
        }
      }
      else {
        //Sätt plid till en översatt parent, om en sådan hittas.
        $plid = 0;
        if(isset($parent_translations[$langcode])) {
          $plid = $parent_translations[$langcode]['mlid'];
        }
        $class = 'empty';
        if(user_access("administer $menu_name menu items")) {
          $output = l('<img src="'.$base_path.'sites/all/modules/custom/general/images/button_add_mini_red.png" />','admin/structure/menu/manage/'.$menu_name.'/add', array('html' => true, 'language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Create a translation in').' '.t($language->name)), 'query' => array('translation' => $item['mlid'], 'target' => $langcode, 'plid' => $plid, 'weight' => $item['weight'], 'destination' => $destination)));
        }
      }
    }
    $content .= $output;
  }
  return $content;
}
function adminpanel_theme_language_column_node($translations, $languages, $destination, $t_nodes) {
	$base_path = base_path();
  $content = '';
  foreach($languages as $langcode => $language) {
    $class = '';
    $output = '';
    if(isset($translations[$langcode])) {
      $translation = $translations[$langcode];
      if($translation['status']) {
        $class = 'published';
        if(node_access('update', $t_nodes[$translation['nid']])) {
          $output = l(strtoupper($langcode),'node/'.$translation['nid'].'/edit', array('language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Edit @node', array('@node' => $translation['title']))), 'query' => array('destination' => $destination)));
        }
        else {
          $output = "<div class='adminpanel-translation $class'>".strtoupper($langcode).'</div>';
        }
      }
      else {
        $class = 'unpublished adminpanel-red-color';
        if(node_access('update', $t_nodes[$translation['nid']])) {
          $output = l(strtoupper($langcode),'node/'.$translation['nid'].'/edit', array('language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Edit @node (unpublished)', array('@node' => $translation['title']))), 'query' => array('destination' => $destination)));
        }
        else {
          $output = "<div class='adminpanel-translation $class'>".strtoupper($langcode).'</div>';
        }
      }
    }
    else {
      $class = 'empty';
      if(user_access('translate content') && user_access('create '.$translations['#tsource_type'].' content')) {
        $output = l('<img src="'.$base_path.'sites/all/modules/custom/general/images/button_add_mini_red.png" />','node/add/'.str_replace('_', '-', $translations['#tsource_type']), array('html' => true, 'language' => $language, 'attributes' => array('data-lang' => $langcode, 'class' => "adminpanel-translation $class", 'title' => t('Create a translation in').' '.t($language->name)), 'query' => array('translation' => $translations['#tsource_nid'], 'target' => $langcode)));
      }
    }
    $content .= $output;
  }
  return $content;
}
function adminpanel_theme_add_links($menu_name, $plid, $langcode, $node_types, $sublink = true) {
/*
  Lägg till kolumn med th = '' som har en selectlist för varje rad med alternativen ('Lägg till underlänk', 'Menylänk', {nodtyper som användaren har behörighet att skapa och som har tillåtelse att ligga i den här menyn}). Menylänken som skapas (utöver en möjlig nod) ska automatiskt lägga sig som child till den aktuella raden.
  */
  $content = '';
  $options = array();
  //Lägg till menylänk
  if(user_access("administer $menu_name menu items")) {
    $options['create_menulink_'.$menu_name.'_'.$plid.'_'.$langcode] = t('Menu link');
    //Lägg till noder
    foreach($node_types as $name => $node_type) {
      if($node_type->access_create) {
        if(in_array($menu_name, $node_type->allowed_menus)) {
          $options['create_node_'.$name.'_'.$menu_name.'_'.$plid.'_'.$langcode] = $node_type->name;
        }
      }
    }
  }
  //Sist: lägg till alternativet "Lägg till underlänk" om $options inte är tom
  if($options) {
    if($sublink) {
      array_unshift($options, ' - '.t('Add sublink').' - ');
      $content = theme('select', array('element' => array('#options' => $options)));
    }
    else {
      array_unshift($options, ' - '.t('Add link').' - ');
      $content = theme('select', array('element' => array('#attributes' => array('class' => array('menu-add-link')), '#options' => $options)));
    }
  }

  return $content;
}
function adminpanel_get_menulink_translations($real_form, $languages) {
  //Translations måste vara menyöverskridande så att adminpanelen kan hålla reda på översättningar som ligger i olika menyer.
  //Gör en statisk variabel av translations så att databasanropen inte blir bortkastade när nästa menyformulär anropar den här funktionen.
  //Hela funktionen måste skrivas om.
  $translations = &drupal_static(__FUNCTION__);
  if (!isset($translations)) {
    $translations = array();
  }
  foreach(element_children($real_form['links']) as $lang) {
    $form = $real_form['links'][$lang];
    foreach (element_children($form) as $mlid) {
      if (isset($form[$mlid]['hidden'])) {
        $link = $form[$mlid]['#item'];
        if(isset($link['i18n_tsid']) && $link['i18n_tsid']) {
          if(!isset($translations[$link['i18n_tsid']])) $translations[$link['i18n_tsid']] = array();
          $translations[$link['i18n_tsid']][$link['language']] = array(
            'mlid' => $link['mlid'],
            'title' => $link['title'],
            'status' => $link['hidden'] ? false : true,
          );
        }
      }
    }
  }
  foreach(element_children($translations) as $tsid) {
    foreach($languages as $langcode => $langobj) {
      if(!isset($translations[$tsid][$langcode])) {
        $item = db_select('menu_links', 'ml')
          ->fields('ml', array('mlid', 'link_title', 'hidden'))
          ->condition('i18n_tsid', $tsid)
          ->condition('language', $langcode)
          ->range(0, 1)
          ->execute()
          ->fetchAssoc();
        if($item) {
          $translations[$tsid][$langcode] = array(
            'mlid' => $item['mlid'],
            'title' => $item['link_title'],
            'status' => $item['hidden'] ? false : true,
          );
        }
      }
    }
  }
  return $translations;
}
function adminpanel_get_destination() {
  $destination = drupal_get_destination();
  $destination = $destination['destination'];
  $exploded = explode('?', $destination);
  $destination = array_shift($exploded);
  return $destination;
}
function adminpanel_get_allowed_node_types() {
  $node_types = node_type_get_types();
  //Filtrera node types mot tillåtna nodtyper
  $allowed_node_types = variable_get('adminpanel_node_types', array_keys($node_types));
  foreach($allowed_node_types as $name => $value) {
    if(!$value) {
      unset($node_types[$name]);
    }
  }
  return $node_types;
}
function adminpanel_get_nodes_by_menu($menu_name) {
  $mnodes = array();
  $retrieved_nodes = array('nodes' => array(), 'tnodes' => array());
  $mlids = menu_node_improvements_get_nodes_by_menu($menu_name);
  if($mlids) {
    $retrieved_nodes = adminpanel_load_nodes_with_translations($mlids);
    $mnodes = $mlids;
    foreach($mnodes as $mlid => $nid) {
      $mnodes[$mlid] = $retrieved_nodes['nodes'][$nid];
    }
  }
  return array('nodes' => $mnodes, 'tnodes' => $retrieved_nodes['tnodes']);
}
function adminpanel_load_nodes_with_translations($nids) {
  $nodes = array();
  $t_nodes = array();
  if($nids) {
    $multilanguage = (variable_get('general_language_settings', 'single')==='multi') ? true : false;
    $nodes = node_load_multiple($nids);
    if($multilanguage) {
      $tnids = array();
      foreach($nodes as $nid => $node) {
        if(isset($node->tnid)) {
          $tnids[] = $node->tnid;
        }
      }
      $query = db_select('node', 'n')
      ->fields('n', array('nid', 'type', 'language', 'title', 'status', 'tnid'))
      ->fields('mn', array('mlid'))
      ->condition('n.tnid', $tnids);
      $query->leftJoin('menu_node', 'mn', 'n.nid = mn.nid');
      $t_result = $query->execute();
      $translations = array();
      $t_nids = array();
      while($record = $t_result->fetchAssoc()) {
        $t_nids[] = $record['nid'];
        if(!isset($translations[$record['tnid']])) $translations[$record['tnid']] = array();
        if($record['tnid']==$record['nid']) { //tsource
          $translations[$record['tnid']]['#tsource_nid'] = $record['nid'];
          $translations[$record['tnid']]['#tsource_type'] = $record['type'];
        }
        $translations[$record['tnid']][$record['language']] = array(
          'mlid' => $record['mlid'],
          'nid' => $record['nid'],
          'title' => $record['title'],
          'status' => $record['status'],
        );
      }
      $t_nodes = node_load_multiple($t_nids);

      foreach($nodes as $nid => &$node) {
        $node->translations = array();
        if(isset($translations[$node->nid])) {
          $node->translations = $translations[$node->nid];
        }
        else {
          //Kolla om noden tillhör en översättning
          if($node->tnid) {
            if(isset($translations[$node->tnid])) {
              $node->translations = $translations[$node->tnid];
            }
          }
        }
      }
    }
  }
  //dpm($nodes, 'nodes');
  //dpm($t_nodes, 't_nodes');
  return array('nodes' => $nodes, 'tnodes' => $t_nodes);
}
function adminpanel_create_content_content($vars) {
  $content = array();
  $content_types = $vars['node_types'];
  $destination = $vars['destination'];
  $content['create_content'] = array(
    '#type' => 'fieldset',
    '#attributes' => array(
      'class' => array('create-content', 'collapsible'),
    ),
    '#title' => t('Add content'),
  );
  foreach($content_types as $name => $obj) {
    if(user_access("create $name content")) {
      $content['create_content']["create_$name"] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $obj->name,
          '#href' => 'node/add/'.str_replace('_', '-', $name),
          '#options' => array('query' => array('destination' => $destination), 'attributes' => array('title' => t('Create !type', array('!type' => strtolower($obj->name))))),
        ),
      );
    }
  }
  return $content;
}

function adminpanel_edit_account_content($vars) {
  return array(
    '#type' => 'fieldset',
    '#title' => t('Edit your account'),
    '#attributes' => array('class' => array('edit-account', 'collapsible')),
    'form' => drupal_get_form('adminpanel_edit_your_account_form', $vars),
  );
}
function adminpanel_edit_your_account_form($form, &$form_state, $vars) {
  global $user;
  $form['#attached']['js'][] = drupal_get_path('module', 'adminpanel').'/js/adminpanel.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'adminpanel').'/css/adminpanel.css';
  $account = user_load($user->uid);
  $form['#user'] = $account;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, apostrophes, and underscores.'),
    '#required' => TRUE,
    '#attributes' => array('class' => array('username')),
    '#default_value' => $account->name,
    '#weight' => 0,
  );
  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#default_value' => field_extract_value('user', $account, 'field_enterprise_blog_firstname'),
  );
  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#default_value' => field_extract_value('user', $account, 'field_enterprise_blog_lastname'),
  );
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t("E-mail address"),
    '#maxlength' => EMAIL_MAX_LENGTH,
    '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
    '#required' => TRUE,
    '#default_value' => $account->mail,
    '#weight' => 2,
  );
  $protected_values = array();
  $current_pass_description = '';
  $protected_values['mail'] = $form['mail']['#title'];
  $protected_values['pass'] = t('Password');
  $request_new = l(t('Request new password'), 'user/password', array('attributes' => array('title' => t('Request new password via e-mail.'))));
  $current_pass_description = t('Enter your current password to change the %mail or %pass. !request_new.', array('%mail' => $protected_values['mail'], '%pass' => $protected_values['pass'], '!request_new' => $request_new));

  $form['current_pass_required_values'] = array(
      '#type' => 'value',
      '#value' => $protected_values,
    );
  $form['current_pass'] = array(
    '#type' => 'password',
    '#title' => t('Current password'),
    '#access' => !empty($protected_values),
    '#description' => $current_pass_description,
    '#attributes' => array('autocomplete' => 'off', 'class' => array('fluid')),
    '#weight' => 1,
  );
  $form['new_pass'] = array(
    '#title' => t('New password'),
    '#type' => 'fieldset',
    '#weight' => 4,
  );
  $form['new_pass']['pass'] = array(
    '#type' => 'password_confirm',
    '#description' => t('To change the current user password, enter the new password in both fields.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );
  $form['#validate'][] = 'user_validate_current_pass';

  return $form;
}
function adminpanel_edit_your_account_form_submit($form, &$form_state) {
  if(isset($form['#user'])) {
    $edit = array();
    if($form_state['values']['name']!==$form['#user']->name) {
      $edit['name'] = $form_state['values']['name'];
    }

    if($form_state['values']['mail']!==$form['#user']->mail) {
      $edit['mail'] = $form_state['values']['mail'];
    }
    if($form_state['values']['pass']) {
      $edit['pass'] = $form_state['values']['pass'];
    }
    if(isset($form_state['values']['firstname'])) {
      if($form_state['values']['firstname']!==field_extract_value('user', $form['#user'], 'field_enterprise_blog_firstname')) {
        $language = field_language('user', $form['#user'], 'field_enterprise_blog_firstname');
        $edit['field_enterprise_blog_firstname'] = array(
          $language => array(
            array(
              'value' => $form_state['values']['firstname'],
            ),
          ),
        );
      }
    }
    if(isset($form_state['values']['lastname'])) {
      if($form_state['values']['lastname']!==field_extract_value('user', $form['#user'], 'field_enterprise_blog_lastname')) {
        $language = field_language('user', $form['#user'], 'field_enterprise_blog_lastname');
        $edit['field_enterprise_blog_lastname'] = array(
          $language => array(
            array(
              'value' => $form_state['values']['lastname'],
            ),
          ),
        );
      }
    }
    if($edit) {
      user_save($form['#user'], $edit);
      drupal_set_message(t('The changes to your account have been saved successfully!'));
    }
  }
}
function adminpanel_edit_site_config_content($vars) {
  return array(
    '#type' => 'fieldset',
    '#title' => t('Edit site configuration'),
    '#attributes' => array('class' => array('edit-site-config', 'collapsible')),
    'form' => drupal_get_form('adminpanel_edit_site_configuration_form', $vars),
  );
}
function adminpanel_edit_site_configuration_form($form, &$form_state, $vars) {
  module_load_include('inc', 'system', 'system.admin');
  $system_site_information_settings = drupal_get_form('system_site_information_settings');
  $form['#realm_variables'] = $system_site_information_settings['#realm_variables'];
  $form['#realm_keys'] = $system_site_information_settings['#realm_keys'];
  $form['#submit'][] = 'adminpanel_edit_site_configuration_form_submit';
  $form['#submit'] = array_merge($form['#submit'], $system_site_information_settings['#submit']);

  $form['site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#default_value' => variable_get('site_name', 'Drupal'),
    '#required' => TRUE,
  );
  $form['site_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Site e-mail address'),
    '#default_value' => variable_get('site_mail', ini_get('sendmail_from')),
    '#description' => t("The <em>From</em> address in automated e-mails sent during registration and new password requests, and other notifications. (Use an address ending in your site's domain to help prevent this e-mail being flagged as spam.)"),
    '#required' => TRUE,
  );
  //Logga och favikon
  $default_theme = variable_get('theme_default', 'kopernikus');
  // Logo settings
  $form['logo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logo image settings'),
    '#description' => t('If toggled on, the following logo will be displayed.'),
    '#attributes' => array('class' => array('theme-settings-bottom')),
  );
  $logo_path = '';
  $default_logo = theme_get_setting('default_logo', $default_theme);
  if($default_logo) {
    $logo_path = general_get_default_logo_path($default_theme);
  }
  else {
    $logo_path = theme_get_setting('logo_path', $default_theme);
  }
  if($logo_path) {
    $form['logo']['current_logo'] = array(
      '#path' => $logo_path,
      '#alt' => '',
      '#title' => '',
      '#theme' => 'image',
      '#prefix' => '<div class="description">'.t('Current logo').':</div>',
    );
    if (!$default_logo) {
      $form['logo']['current_logo']['#style_name'] = 'thumbnail';
      $form['logo']['current_logo']['#theme'] = 'image_style';
    }
  }
  $form['logo']['default_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default logo'),
    '#default_value' => theme_get_setting('default_logo', $default_theme),
    '#tree' => FALSE,
    '#description' => t('Check here if you want the theme to use the logo supplied with it.')
  );
  $form['logo']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the logo settings when using the default logo.
      'invisible' => array(
        'input[name="default_logo"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['logo']['settings']['logo_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom logo'),
    '#description' => t('The path to the file you would like to use as your logo file instead of the default logo.'),
    '#default_value' => theme_get_setting('logo_path', $default_theme),
  );
  $form['logo']['settings']['logo_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload logo image'),
    '#maxlength' => 40,
    '#size' => 0,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your logo."),
  );

  $form['favicon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shortcut icon settings'),
    '#description' => t("Your shortcut icon, or 'favicon', is displayed in the address bar and bookmarks of most browsers."),
  );
  $favicon_path = '';
  $default_favicon = theme_get_setting('default_favicon', $default_theme);
  if($default_favicon) {
    $favicon_path = 'misc/favicon.ico';
  }
  else {
    $favicon_path = theme_get_setting('favicon_path', $default_theme);
  }
  if($favicon_path) {
    $form['favicon']['current_favicon'] = array(
      '#path' => $favicon_path,
      '#alt' => '',
      '#title' => '',
      '#theme' => 'image',
      '#prefix' => '<div class="description">'.t('Current favicon').':</div>',
    );
  }
  $form['favicon']['default_favicon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default shortcut icon.'),
    '#default_value' => theme_get_setting('default_favicon', $default_theme),
    '#description' => t('Check here if you want the theme to use the default shortcut icon.')
  );
  $form['favicon']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the favicon settings when using the default favicon.
      'invisible' => array(
        'input[name="default_favicon"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['favicon']['settings']['favicon_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom icon'),
    '#description' => t('The path to the image file you would like to use as your custom shortcut icon.'),
    '#default_value' => theme_get_setting('favicon_path', $default_theme),
  );
  $form['favicon']['settings']['favicon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload icon image'),
    '#size' => 0,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your shortcut icon.")
  );

  // Inject human-friendly values for logo and favicon.
  foreach (array('logo' => 'logo.png', 'favicon' => 'favicon.ico') as $type => $default) {
    if (isset($form[$type]['settings'][$type . '_path'])) {
      $element = &$form[$type]['settings'][$type . '_path'];

      // If path is a public:// URI, display the path relative to the files
      // directory; stream wrappers are not end-user friendly.
      $original_path = $element['#default_value'];
      $friendly_path = NULL;
      if (file_uri_scheme($original_path) == 'public') {
        $friendly_path = file_uri_target($original_path);
        $element['#default_value'] = $friendly_path;
      }
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}
function adminpanel_edit_site_configuration_form_validate(&$form, &$form_state) {
  global $language;
  module_load_include('inc', 'system', 'system.admin');
  $form_state['values']['i18n_variable_language'] = $language;

  if(isset($form_state['values']['site_mail'])) {
    if ($error = user_validate_mail($form_state['values']['site_mail'])) {
      form_set_error('site_mail', $error);
    }
  }
  
  //Logga och favikon
  // Handle file uploads.
  $validators = array('file_validate_is_image' => array());

  // Check for a new uploaded logo.
  $file = file_save_upload('logo_upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['logo_upload'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('logo_upload', t('The logo could not be uploaded.'));
    }
  }

  $validators = array('file_validate_extensions' => array('ico png gif jpg jpeg apng svg'));

  // Check for a new uploaded favicon.
  $file = file_save_upload('favicon_upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['favicon_upload'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('favicon_upload', t('The favicon could not be uploaded.'));
    }
  }

  // If the user provided a path for a logo or favicon file, make sure a file
  // exists at that path.
  if ($form_state['values']['logo_path']) {
    $path = _system_theme_settings_validate_path($form_state['values']['logo_path']);
    if (!$path) {
      form_set_error('logo_path', t('The custom logo path is invalid.'));
    }
  }
  if ($form_state['values']['favicon_path']) {
    $path = _system_theme_settings_validate_path($form_state['values']['favicon_path']);
    if (!$path) {
      form_set_error('favicon_path', t('The custom favicon path is invalid.'));
    }
  }
}
function adminpanel_edit_site_configuration_form_submit($form, &$form_state) {
  module_load_include('inc', 'system', 'system.admin');
  global $theme_key;
  //Logga och favikon
  $values =& $form_state['values'];

  // If the user uploaded a new logo or favicon, save it to a permanent location
  // and use it in place of the default theme-provided file.
  if ($file = $values['logo_upload']) {
    unset($values['logo_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['default_logo'] = 0;
    $values['logo_path'] = $filename;
  }
  if ($file = $values['favicon_upload']) {
    unset($values['favicon_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['default_favicon'] = 0;
    $values['favicon_path'] = $filename;
    $values['toggle_favicon'] = 1;
  }

  // If the user entered a path relative to the system files directory for
  // a logo or favicon, store a public:// URI so the theme system can handle it.
  if (!empty($values['logo_path'])) {
    $values['logo_path'] = _system_theme_settings_validate_path($values['logo_path']);
  }
  if (!empty($values['favicon_path'])) {
    $values['favicon_path'] = _system_theme_settings_validate_path($values['favicon_path']);
  }

  if (empty($values['default_favicon']) && !empty($values['favicon_path'])) {
    $values['favicon_mimetype'] = file_get_mimetype($values['favicon_path']);
  }
  $current_settings = variable_get('theme_'.$theme_key.'_settings', array());
  $changed = false;
  if($current_settings) {
    $vars = array(
      'default_logo' => 'default_logo',
      'logo_path' => 'logo_path',
      'default_favicon' => 'default_favicon',
      'favicon_path' => 'favicon_path',
      'toggle_favicon' => 'toggle_favicon',
      'favicon_mimetype' => 'favicon_mimetype',
    );
    foreach($vars as $var) {
      if(isset($current_settings[$var])) {
        if(isset($values[$var])) {
          if($values[$var] !== $current_settings[$var]) {
            $changed = true;
            break;
          }
        }
      }
      else {
        $changed = true;
        break;
      }
    }
  }
  if($changed) {
    $themes = list_themes();
    foreach($themes as $name => $theme) {
      if($theme->status) {
        $settings = variable_get('theme_'.$name.'_settings', array());
        if(isset($values['default_logo'])) $settings['default_logo'] = $values['default_logo'];
        if(isset($values['logo_path'])) $settings['logo_path'] = $values['logo_path'];
        if(isset($values['default_favicon'])) $settings['default_favicon'] = $values['default_favicon'];
        if(isset($values['favicon_path'])) $settings['favicon_path'] = $values['favicon_path'];
        if(isset($values['toggle_favicon'])) $settings['toggle_favicon'] = $values['toggle_favicon'];
        if(isset($values['favicon_mimetype'])) $settings['favicon_mimetype'] = $values['favicon_mimetype'];
        variable_set('theme_'.$name.'_settings', $settings);
      }
    }
  }
  foreach($vars as $var) {
    unset($values[$var]);
  }
}
function adminpanel_initialize() {
  global $base_url;
  $base = rtrim($base_url, '/');
  if(variable_get('general_page', 0)) {
    if(isset($_GET['key']) && $_GET['key'] === variable_get('cron_key')) {}
    else {
      drupal_goto($base.'/index.html');
    }
  }
}
function adminpanel_node_insert_callback($node) {
  $allowed_types = variable_get('adminpanel_node_types', array());
  if(isset($allowed_types[$node->type]) && $allowed_types[$node->type]) {
    $has_sidtyper = array();
    $allowed_sidtyp = variable_get('adminpanel_sidtyper', array());
    foreach($allowed_types as $type => $value) {
      $fields = field_info_instances("node",$type);
      if(isset($fields['field_sidtyp'])) {
        $has_sidtyper[$type] = true;
      }
    }
    if(isset($has_sidtyper[$node->type])) {
      $term_sidtyp = field_extract_value('node', $node, 'field_sidtyp');
      if(!$allowed_sidtyp[$term_sidtyp->tid]) {
        return;
      }
    }
    $multilanguage = (variable_get('general_language_settings', 'single')==='multi') ? true : false;
    if(isset($node->translation_source)) { //tslave
      if($multilanguage) {
        //Öka translations och translation points hos tsource
        $tnid = $node->translation_source->nid;
        $points = 5;
        if($node->status) $points += 1;
        db_update('adminpanel')
        ->expression('translations', 'translations + 1')
        ->expression('translation_points', "translation_points + $points")
        ->condition('nid', $tnid)
        ->execute();
      }
      else { //Enspråkig
        //Lägg in en ny rad i databasen
        $save = array(
          'nid' => $node->nid,
          'type' => $node->type,
          'title' => $node->title,
          'status' => $node->status,
          'translations' => 0,
          'translation_points' => 0,
        );
        drupal_write_record('adminpanel', $save);
      }
    }
    else { //tsource
      //Lägg in en ny rad i databasen
      $save = array(
        'nid' => $node->nid,
        'type' => $node->type,
        'title' => $node->title,
        'status' => $node->status,
        'translations' => 0,
        'translation_points' => 0,
      );
      if($multilanguage) {
        $save['translations'] = 1;
        $save['translation_points'] += 5;
        if($save['status']) $save['translation_points'] += 1;
      }
      drupal_write_record('adminpanel', $save);
    }
  }
}
function adminpanel_node_update_callback($node) {
  $allowed_types = variable_get('adminpanel_node_types', array());
  if($allowed_types[$node->type]) {
    $has_sidtyper = array();
    $allowed_sidtyp = variable_get('adminpanel_sidtyper', array());
    foreach($allowed_types as $type => $value) {
      $fields = field_info_instances("node",$type);
      if(isset($fields['field_sidtyp'])) {
        $has_sidtyper[$type] = true;
      }
    }
    if(isset($has_sidtyper[$node->type])) {
      $term_sidtyp = field_extract_value('node', $node, 'field_sidtyp');
      if(!$allowed_sidtyp[$term_sidtyp->tid]) {
        return;
      }
    }

    $update = false;
    $multilanguage = (variable_get('general_language_settings', 'single')==='multi') ? true : false;
    $fields = array();

    //Uppdatera eventuell ändring av titeln
    if($node->title!==$node->original->title) {
      if($multilanguage) {
        if($node->nid==$node->tnid) { //tsource
          $update = true;
          $fields['title'] = $node->title;
        }
      }
      else {
        $update = true;
        $fields['title'] = $node->title;      
      }
    }
    $query = db_update('adminpanel');
    if($multilanguage) {
      $query->condition('nid', $node->tnid);
    }
    else {
      $query->condition('nid', $node->nid);
    }

    //Kolla om statusen har ändrats
    if($node->status!=$node->original->status) { //Statusen har ändrats
      $update = true;
      if($multilanguage) {
        if($node->nid==$node->tnid) { //tsource
          $fields['status'] = $node->status;
        }
      }
      else {
        $fields['status'] = $node->status;
      }
      if($multilanguage) {
        if($node->status) { //Noden har publicerats
          //Öka translation points med 1
          $query->expression('translation_points', 'translation_points + 1');
        }
        else { //Noden har avpublicerats
          //Minska translation points med 1
          $query->expression('translation_points', 'translation_points - 1');
        }
      }
    }
    
    if($update) {
      $query->fields($fields)
      ->execute();
    }
  }
}
//Implements hook_node_delete()
function adminpanel_node_delete_callback($node) {
  $dead_nodes = variable_get('general_dead_nodes', array());
  if(isset($dead_nodes[$node->nid])) {
    unset($dead_nodes[$node->nid]);
    variable_set('general_dead_nodes', $dead_nodes);
  }
  $allowed_types = variable_get('adminpanel_node_types', array());
  if(isset($allowed_types[$node->type]) && $allowed_types[$node->type]) {
    $has_sidtyper = array();
    $allowed_sidtyp = variable_get('adminpanel_sidtyper', array());
    foreach($allowed_types as $type => $value) {
      $fields = field_info_instances("node",$type);
      if(isset($fields['field_sidtyp'])) {
        $has_sidtyper[$type] = true;
      }
    }
    if(isset($has_sidtyper[$node->type])) {
      $term_sidtyp = field_extract_value('node', $node, 'field_sidtyp');
      if(!$allowed_sidtyp[$term_sidtyp->tid]) {
        return;
      }
    }
    
    $multilanguage = (variable_get('general_language_settings', 'single')==='multi') ? true : false;
    if($node->tnid==$node->nid || $node->tnid==0) { //tsource
      if($multilanguage) {
        //Hämta alla översättningar till noden
        $result = db_select('node', 'n')
          ->fields('n', array('nid'))
          ->condition('n.tnid', $node->tnid)
          ->execute();
        $translation_count = $result->rowCount();
        if($translation_count > 1) { //Noden har översättningar
          $new_tsource = db_query_range('SELECT nid, title, status FROM {node} WHERE tnid = :tnid AND nid != :tnid ORDER BY translate ASC, nid ASC', 0, 1, array(':tnid' => $node->tnid))->fetchAssoc();
          //Uppdatera alla översatta noder via node_save() så att entity cache uppdateras
          $nids = array();
          while($nid = $result->fetchField()) {
            $nids[] = $nid;
          }
          if($nids) {
            $translated_nodes = node_load_multiple($nids);
            foreach($translated_nodes as $tnode) {
              if($tnode->nid!=$node->nid) {
                $tnode->tnid = $new_tsource['nid'];
                node_save($tnode);
              }
            }
          }

          //Räkna ut minskningen av translation points
          $points = 5;
          if($node->status) $points += 1;
          //Uppdatera nid och translation points för tsourceraden
          db_update('adminpanel')
          ->fields(array('nid' => $new_tsource['nid'], 'title' => $new_tsource['title'], 'status' => $new_tsource['status']))
          ->expression('translations', 'translations - 1')
          ->expression('translation_points', "translation_points - $points")
          ->condition('nid', $node->tnid)
          ->execute();
        }
        else { //Noden har inga översättningar, ta bort raden
          db_delete('adminpanel')
          ->condition('nid', $node->nid)
          ->execute();
        }
      }
      else { //Enspråkig, ta bort raden
        db_delete('adminpanel')
        ->condition('nid', $node->nid)
        ->execute();
      }
    }
    else { //tslave
      if($multilanguage) {
        //Minska translation points hos tsource
        $points = 5;
        if($node->status) $points += 1;
        db_update('adminpanel')
        ->expression('translations', 'translations - 1')
        ->expression('translation_points', "translation_points - $points")
        ->condition('nid', $node->tnid)
        ->execute();
      }
      else {
        //Enspråkig, ta bort raden
        db_delete('adminpanel')
        ->condition('nid', $node->nid)
        ->execute();
      }
    }
  }
}

<?php
/**
 * Form for editing an entire menu tree at once.
 *
 * Shows for one menu the menu links accessible to the current user and
 * relevant operations.
 */
function menu_improvements_menu_overview_form($form, &$form_state, $menu) {
  module_load_include('inc', 'menu', 'menu.admin');
  global $menu_admin;
  global $language;
  $form['#submit'][] = 'menu_improvements_menu_overview_form_submit';
  if(variable_get('general_language_settings', 'single')==='multi') {
    $language_list = language_list();
    $tabs = array(
      'menu_tabs' => array(
        '#prefix' => '<div class="menu-tabs"><ul>',
        '#suffix' => '</ul></div>',
      ),
    );
    $menu_tabs =& $tabs['menu_tabs'];
    $current_path = current_path();
    foreach($language_list as $key => $lang_full) {
      if($lang_full->enabled) {
        $active = $key===$language->language ? 'active' : '';
        $menu_tabs[$key] = array(
          '#markup' => "<li class='menu-tab $key $active'>".l(t($lang_full->name), url($current_path, array('absolute' => true, 'language' => $lang_full))).'</li>',
        );
      }
    }
    $form['#prefix'] = render($tabs);
  }
  $form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css', drupal_get_path('module', 'menu_improvements') . '/css/menu_improvements_overview.css');
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu AND ml.language IN ('$language->language', 'und')
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu' => $menu['menu_name']), array('fetch' => PDO::FETCH_ASSOC));

  $links = array();
  foreach ($result as $item) {
    $links[] = $item;
  }
  $tree = menu_tree_data($links);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
  // We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;
  $form = array_merge($form, _menu_overview_tree_form($tree));
  $form['#menu'] =  $menu;

  if (element_children($form)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
    $form['#submit'][] = 'menu_overview_form_submit';
  }
  else {
    $form['#empty_text'] = t('There are no menu links yet. <a href="@link">Add link</a>.', array('@link' => url('admin/structure/menu/manage/'. $form['#menu']['menu_name'] .'/add')));
  }
  if(!user_access('administer menu')) {
    $form['#hide_enabled'] = true;
    foreach($form as $key => &$link) {
      if(substr($key, 0, 4)==='mlid') {
        if(isset($link['hidden'])) {
          $link['hidden']['#access'] = false;
        }
      }
    }
  }
  return $form;
}

function menu_improvements_menu_overview_form_submit($form, &$form_state) {
  //Jag lämnar kvar den här funktionen ifall jag skulle behöva den i framtiden
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
function theme_menu_improvements_menu_overview_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('menu-overview', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('menu-overview', 'order', 'sibling', 'menu-weight');
  $header = array();
  if(!isset($form['#hide_enabled'])) {
    $header[] = t('Menu link');
    $header[] = array('data' => t('Enabled'), 'class' => array('checkbox'));
  }
  $header[] = t('Weight');
  $header[] = array('data' => t('Operations'), 'colspan' => '3');

  $rows = array();
  foreach (element_children($form) as $mlid) {
    if (isset($form[$mlid]['hidden'])) {
      $element = &$form[$mlid];
      // Build a list of operations.
      $operations = array();
      foreach (element_children($element['operations']) as $op) {
        $operations[] = array('data' => drupal_render($element['operations'][$op]), 'class' => array('menu-operations'));
      }
      while (count($operations) < 2) {
        $operations[] = '';
      }

      // Add special classes to be used for tabledrag.js.
      $element['plid']['#attributes']['class'] = array('menu-plid');
      $element['mlid']['#attributes']['class'] = array('menu-mlid');
      $element['weight']['#attributes']['class'] = array('menu-weight');

      // Change the parent field to a hidden. This allows any value but hides the field.
      $element['plid']['#type'] = 'hidden';

      $row = array();
      $row[] = theme('indentation', array('size' => $element['#item']['depth'] - 1)) . drupal_render($element['title']);
      if(!isset($form['#hide_enabled'])) {$row[] = array('data' => drupal_render($element['hidden']), 'class' => array('checkbox', 'menu-enabled'));}
      $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);
      $row = array_merge($row, $operations);

      $row = array_merge(array('data' => $row), $element['#attributes']);
      $row['class'][] = 'draggable';
      $rows[] = $row;
    }
  }
  $output = '';
  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#empty_text'], 'colspan' => '7'));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'menu-overview')));
  $output .= drupal_render_children($form);
  return $output;
}
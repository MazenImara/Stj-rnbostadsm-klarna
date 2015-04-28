<?php

/**
 * Helper function to save menu items. Used on hook_node_insert() and
 * hook_node_update().
 *
 * @param $node
 *   Node object to save menu links for.
 */
function menu_improvements_save($node) {
  if (!empty($node->menu_improvements_links_deleted)) {
    foreach ($node->menu_improvements_links_deleted as $deleted_link) {
      menu_link_delete($deleted_link);
    }
  }

  if (empty($node->menu_improvements_links)) {
    return;
  }
	if(!empty($node->menu_improvements_links)) {
	  foreach ($node->menu_improvements_links as $delta => &$link) {
	    if (!empty($link['link_title'])) {
	      // Set path to node being saved.
	      $link['link_path'] = "node/$node->nid";
	
	      // If 'Enabled' checkbox was ticked. Set hidden to FALSE and vice-versa. 
	      if (isset($link['enabled'])) {
	        $link['hidden'] = (int) !$link['enabled'];
	      }
	
	      if (!empty($link['description'])) {
	        $link['options']['attributes']['title'] = $link['description'];
	      }
	      else {
	        unset($link['options']['attributes']['title']);
	      }
	
	      // Add support for i18n_menu and other language aware modules. Set link
	      // language to node language.
	      $link['language'] = empty($node->language) ? LANGUAGE_NONE : $node->language;
	
	      // Actually save the link.
	      if (!menu_link_save($link)) {
	        // @todo Not sure in what situations we should get here, but it doesn't
	        // look quite right to display error messages on a loop like this.
	        drupal_set_message(t('There was an error saving the menu link.'), 'error');
	      }
	    }
	  }
	}
}
function menu_improvements_node_form_alter_callback(&$form, $form_state) {
	if(isset($form['menu'])) {
		menu_improvements_node_form_alter_single_item($form, $form_state);
  }
  else {
		menu_improvements_node_form_alter_multiple_items($form, $form_state);
	}
}
function menu_improvements_node_form_alter_single_item(&$form, $form_state) {
  $link = $form['#node']->menu;
  $type = $form['#node']->type;
  $returned_values = menu_improvements_menu_parent_options(menu_get_menus(), $link['mlid'] ? $link : $type, $type);
  $options = $returned_values['options'];
  $sorted_tree = $returned_values['sorted_tree'];
  $form['menu']['link']['parent']['#options'] = $options;

  //Dölj menyalternativ om det är en översättning som håller på att skapas
  if(isset($_GET['translation'])) {
    if(isset($form['menu'])) {
      $form['menu']['#access'] = false;
    }
  }
  else {
    //Dölj vissa menyelement för obehöriga
    if(isset($form['menu']['link'])) {
      $elements = array(
        'expanded',
        'xmlsitemap',
        'options--id',
        'options--name',
        'options--class',
        'options--style',
        'options--accesskey',
      );
      $permission = user_access('menu_improvements_link_display_options');
      foreach($elements as $element) {
        $element = explode('--', $element);
        if($element[0] !== 'options') {
          $element = $element[0];
          if(isset($form['menu']['link'][$element])) {
            $form['menu']['link'][$element]['#access'] = $permission;
          }
        }
      }
      foreach($elements as $element) {
        $element = explode('--', $element);
        if($element[0] === 'options') {
          $element = $element[1];
          if(isset($form['menu']['link']['options']['attributes'][$element])) {
            $form['menu']['link']['options']['attributes'][$element]['#access'] = $permission;
          }
        }
      }
    }

		menu_improvements_node_form_add_parent_js($form, $sorted_tree);

    //Lägg in eventuella förinställda alternativ
    if(isset($_GET['menu-name']) && isset($_GET['plid'])) {
      $menu_name = check_plain($_GET['menu-name']);
      $plid = check_plain($_GET['plid']);
      //Sätt en custommenylänk
      $form['menu']['enabled']['#default_value'] = 1;
      if(isset($form['menu']['link']['parent']['#options'][$menu_name.':'.$plid])) {
        $form['menu']['link']['parent']['#default_value'] = $menu_name.':'.$plid;
      }
    }
  }
}
function menu_improvements_node_form_alter_multiple_items(&$form, $form_state) {
	$node = $form['#node'];
  $form['menu_improvements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($node->menu_improvements_links),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'menu') . '/menu.js'),
    ),
    '#tree' => TRUE,
    '#weight' => -2,
    '#attributes' => array('class' => array('menu-link-form')),
  );

  $form['menu_improvements']['link'] = array(
    '#type' => 'container',
    '#parents' => array('menu_improvements'),
    '#prefix' => '<div id="menu-improvements-wrapper">',
    '#suffix' => '</div>',
  );
	$menu_parents = menu_improvements_menu_parent_options(menu_get_menus(), $node->type);

	menu_improvements_node_form_add_parent_js($form, $menu_parents['sorted_tree']);
	if(empty($node->menu_improvements_links)) {
	  if(isset($_GET['translation'])) {
	    if(isset($form['menu_improvements'])) {
	      //Lägg in standardalternativ
	      if(variable_get('menu_improvements_sync_order', false)) {
		      $tnid = check_plain($_GET['translation']);
		      $tnode = node_load($tnid);
		      if(!empty($tnode->menu_node_links)) {
						foreach($tnode->menu_node_links as $mlid => $link) {
							$link = (array) $link;
							$item = array(
								'weight' => $link['weight'],
								'menu_name' => $link['menu_name'],
								'language' => check_plain($_GET['target']),
								'link_title' => $link['link_title'],
							);
							$item['translation'] = $link['mlid'];
	            $plid = $link['plid'];
              $menu_name = $link['menu_name'];
	            if($plid) {
	              $node_parent = menu_node_get_node($plid);
	              $parent_links = array();
	              if($node_parent) { //Parentlänken hör till en nod
	                //------------Hämta alla översättningar till parentnoden
	                $parent_translations = i18n_improvements_get_node_translations($node_parent);
	                if($parent_translations) { //Det finns översättningar till parentnoden
	                  //Hämta översatta länkar
	                  $query = db_select('menu_node', 'mn')
	                    ->fields('mn', array('mlid'))
	                    ->fields('ml', array('language'))
	                    ->condition('mn.nid', $parent_translations);
	                  $query->innerJoin('menu_links', 'ml', 'mn.mlid = ml.mlid');
	                  //Det här gör att menylänken inte påverkas om den läggs i en ny meny.
	                  //$query->condition('ml.menu_name', $menu_name);
	                  $result = $query->execute();
	                  while($link = $result->fetchAssoc()) {
	                    $parent_links[$link['language']] = $link['mlid'];
	                  }
	                }
	              }
	              else { //Parentlänken hör inte till en nod
	                //Hämta översatta länkar till parentlänken
	                $links = menu_improvements_get_link_translations($plid, $menu_name);
	                if($links) {
	                  //Det finns översättningar till länken
	                  foreach($links as $link) {
	                    $parent_links[$link['language']] = $link['mlid'];
	                  }
	                }
	              }
	              if(isset($parent_links[$item['language']])) {
	                $item['plid'] = $parent_links[$item['language']];
	              }
	            }
	            _menu_improvements_set_defaults($node, $item);
    					$node->menu_improvements_links[] = $item;
						}
					}
				}
	    }
	  }
	}

  if(!empty($node->menu_improvements_links)) {
    $form['menu_improvements']['link']['current_links'] = menu_improvements_current_links_form($node, $menu_parents);
    $form['menu_improvements']['link']['current_links']['#prefix'] = '<h3>' . t('Current menu links') . '</h3>';
  }

  $form['menu_improvements']['link']['add_link'] = menu_improvements_link_form($node, $menu_parents);
  if($form['menu_improvements']['link']['add_link']) {
	  $form['menu_improvements']['link']['add_link']['#prefix'] = '<h3>' . t('Add new menu link') . '</h3>';
	  $form['menu_improvements']['link']['add_link']['add_link_submit'] = array(
	    '#type' => 'submit',
	    '#value' => t('Add new menu link'),
	    '#submit' => array('menu_improvements_add_link_submit'),
	    // Restrict validation to appropriate fields only.
	    '#limit_validation_errors' => array(array('menu_improvements', 'add_link')),
	    '#ajax' => array(
	      'callback' => 'menu_improvements_ajax_callback',
	      'wrapper' => 'menu-improvements-wrapper',
	    ),
	  );
	}
	else {
		$form['menu_improvements']['link']['add_link'] = array(
			'#markup' => t('Menu links are disabled for this content type.'),
		);
	}
}
function menu_improvements_node_form_add_parent_js(&$form, $sorted_tree) {
	//Uppdatera automatiskt möjliga parentalternativ beroende på vilket språk man väljer
  $form['#attached']['js'][] = drupal_get_path('module', 'menu_improvements').'/js/menu_improvements_node_form.js';
  $mlids = array();
	$translations = array('tsid' => array(), 'tnid' => array());
  $hierarchy = array();
  $flatlist = array();
  foreach(element_children($sorted_tree) as $lang) {
    $temp = menu_improvements_sort_out_links_recursive($sorted_tree[$lang]);
    foreach(element_children($temp['translations']['tsid']) as $tsid) {
      foreach($temp['translations']['tsid'][$tsid] as $link_lang => $mlid) {
        if(!isset($translations['tsid'][$tsid])) $translations['tsid'][$tsid] = array();
        $translations['tsid'][$tsid][$link_lang] = $mlid;
      }
    }
    foreach(element_children($temp['translations']['tnid']) as $tnid) {
      foreach($temp['translations']['tnid'][$tnid] as $link_lang => $mlid) {
        if(!isset($translations['tnid'][$tnid])) $translations['tnid'][$tnid] = array();
        $translations['tnid'][$tnid][$link_lang] = $mlid;
      }
    }
    foreach(element_children($temp['hierarchy']) as $link_lang) {
      foreach(element_children($temp['hierarchy'][$link_lang]) as $plid) {
        if(!isset($hierarchy[$link_lang])) $hierarchy[$link_lang] = array();
        if(!isset($hierarchy[$link_lang][$plid])) $hierarchy[$link_lang][$plid] = array();
        $hierarchy[$link_lang][$plid] = array_merge($hierarchy[$link_lang][$plid], $temp['hierarchy'][$link_lang][$plid]);
      }
    }
    foreach(element_children($temp['flatlist']) as $link_lang) {
      if(!isset($flatlist[$link_lang])) $flatlist[$link_lang] = array();
      $flatlist[$link_lang] += $temp['flatlist'][$link_lang];
    }
  }
  //dpm($translations, 'translations');
  //dpm($hierarchy, 'hierarchy');
  $form['#attached']['js'][] = array(
    'data' => array('menu_improvements_translations' => $translations),
    'type' => 'setting',
  );
  $form['#attached']['js'][] = array(
    'data' => array('menu_improvements_hierarchy' => $hierarchy),
    'type' => 'setting',
  );
  $form['#attached']['js'][] = array(
    'data' => array('menu_improvements_flatlist' => $flatlist),
    'type' => 'setting',
  );
}
/**
 * Generate a form for editing a menu links.
 */
function menu_improvements_current_links_form($node, $menu_parents) {
  $form = array(
    '#tree' => TRUE,
    '#theme' => 'menu_improvements_current_links',
  );

  foreach ($node->menu_improvements_links as $delta => $link) {
    $form[$delta] = menu_improvements_link_form($node, $menu_parents, $link, TRUE);
    $form[$delta]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => !$link['hidden'],
      '#access' => false,
    );
    $form[$delta]['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#name' => 'remove_' . $delta,
      '#submit' => array('menu_improvements_remove_link_submit'),
      // Bypass validation for 'Delete' button.
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'menu_improvements_ajax_callback',
        'wrapper' => 'menu-improvements-wrapper',
      ),
    );
  }

  return $form;
}
/**
 * Generate a form for adding/editing a menu link.
 */
function menu_improvements_link_form($node, $menu_parents = array(), $link = array(), $edit = FALSE) {
  // Set default values.
  _menu_improvements_set_defaults($node, $link);

	$options = $menu_parents['options'];
  $sorted_tree = $menu_parents['sorted_tree'];

  // If no possible parent menu items were found, there is nothing to display.
  if (empty($options)) {
    return;
  }

  $form = array();

  // Populate the element with the link data.
  foreach (array('mlid', 'module', 'hidden', 'has_children', 'customized', 'options', 'expanded', 'hidden', 'parent_depth_limit') as $key) {
    $form[$key] = array('#type' => 'value', '#value' => $link[$key]);
  }

	//Skapa översättning
  if(!empty($link['translation']) && ($source_item = menu_link_load($link['translation']))) {
		if (!empty($source_item['i18n_tsid'])) {
      $translation_set = i18n_translation_set_load($source_item['i18n_tsid']);
    }
    else {
      // Create object and stick the source information in the translation set.
      $translation_set = i18n_translation_set_build('menu_link')
        ->add_item($source_item);
    }
	}
	elseif (!empty($link['i18n_tsid'])) {
    $translation_set = i18n_translation_set_load($link['i18n_tsid']);
  }
  // Add the translation set to the form so we know the new menu item
  // needs to be added to that set.
  if (!empty($translation_set)) {
    $form['translation_set'] = array(
      '#type' => 'value',
      '#value' => $translation_set,
    );
  }

  $form['link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu link title'),
    '#default_value' => $link['link_title'],
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => !empty($link['description']) ? $link['description'] : '',
    '#rows' => 1,
    '#description' => t('Shown when hovering over the menu link.'),
    '#access' => false,
  );

  $default = (!empty($link['menu_name']) ? $link['menu_name'] . ':' . $link['plid'] : variable_get('menu_parent_' . $node->type, 'main-menu:0'));

  // If the current parent menu item is not present in options, use the first
  // available option as default value.
  // @todo User should not be allowed to access menu link settings in such a
  // case.
  if (!isset($options[$default])) {
    $array = array_keys($options);
    $default = reset($array);
  }
  $form['parent'] = array(
    '#type' => 'select',
    '#title' => t('Parent item'),
    '#default_value' => $default,
    '#options' => $options,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 50,
    '#default_value' => $link['weight'],
    '#description' => t('Menu links with smaller weights are displayed before links with larger weights.'),
  );

  if(!isset($_GET['translation'])) {
		/*
    //Dölj vissa menyelement för obehöriga
    if(isset($form['menu']['link'])) {
      $elements = array(
        'expanded',
        'xmlsitemap',
        'options--id',
        'options--name',
        'options--class',
        'options--style',
        'options--accesskey',
      );
      $permission = user_access('menu_improvements_link_display_options');
      foreach($elements as $element) {
        $element = explode('--', $element);
        if($element[0] !== 'options') {
          $element = $element[0];
          if(isset($form['menu']['link'][$element])) {
            $form['menu']['link'][$element]['#access'] = $permission;
          }
        }
      }
      foreach($elements as $element) {
        $element = explode('--', $element);
        if($element[0] === 'options') {
          $element = $element[1];
          if(isset($form['menu']['link']['options']['attributes'][$element])) {
            $form['menu']['link']['options']['attributes'][$element]['#access'] = $permission;
          }
        }
      }
    }
    */

    //Uppdatera automatiskt möjliga parentalternativ beroende på vilket språk man väljer
    $form['#attached']['js'][] = drupal_get_path('module', 'menu_improvements').'/js/menu_improvements_node_form.js';
    $mlids = array();
    $translations = array('tsid' => array(), 'tnid' => array());
    $hierarchy = array();
    $flatlist = array();
    foreach(element_children($sorted_tree) as $lang) {
      $temp = menu_improvements_sort_out_links_recursive($sorted_tree[$lang]);
      foreach(element_children($temp['translations']['tsid']) as $tsid) {
        foreach($temp['translations']['tsid'][$tsid] as $link_lang => $mlid) {
          if(!isset($translations['tsid'][$tsid])) $translations['tsid'][$tsid] = array();
          $translations['tsid'][$tsid][$link_lang] = $mlid;
        }
      }
      /*
      foreach(element_children($temp['translations']['tnid']) as $tnid) {
        foreach($temp['translations']['tnid'][$tnid] as $link_lang => $mlid) {
          if(!isset($translations['tnid'][$tnid])) $translations['tnid'][$tnid] = array();
          $translations['tnid'][$tnid][$link_lang] = $mlid;
        }
      }
      */
      foreach(element_children($temp['hierarchy']) as $link_lang) {
        foreach(element_children($temp['hierarchy'][$link_lang]) as $plid) {
          if(!isset($hierarchy[$link_lang])) $hierarchy[$link_lang] = array();
          if(!isset($hierarchy[$link_lang][$plid])) $hierarchy[$link_lang][$plid] = array();
          $hierarchy[$link_lang][$plid] = array_merge($hierarchy[$link_lang][$plid], $temp['hierarchy'][$link_lang][$plid]);
        }
      }
      foreach(element_children($temp['flatlist']) as $link_lang) {
        if(!isset($flatlist[$link_lang])) $flatlist[$link_lang] = array();
        $flatlist[$link_lang] += $temp['flatlist'][$link_lang];
      }
    }
    //dpm($translations, 'translations');
    //dpm($hierarchy, 'hierarchy');
    $form['#attached']['js'][] = array(
      'data' => array('menu_improvements_translations' => $translations),
      'type' => 'setting',
    );
    $form['#attached']['js'][] = array(
      'data' => array('menu_improvements_hierarchy' => $hierarchy),
      'type' => 'setting',
    );
    $form['#attached']['js'][] = array(
      'data' => array('menu_improvements_flatlist' => $flatlist),
      'type' => 'setting',
    );

    //Lägg in eventuella förinställda alternativ
    if(isset($_GET['menu-name']) && isset($_GET['plid'])) {
      $menu_name = check_plain($_GET['menu-name']);
      $plid = check_plain($_GET['plid']);
      //Sätt en custommenylänk
      //$form['menu']['enabled']['#default_value'] = 1;
      if(isset($form['parent']['#options'][$menu_name.':'.$plid])) {
        $form['parent']['#default_value'] = $menu_name.':'.$plid;
      }
    }
  }

  return $form;
}
/**
 * Set default values to menu link array().
 */
function _menu_improvements_set_defaults($node, &$link) {
  $menu_default = explode(':', variable_get('menu_parent_' . $node->type, 'main-menu:0'));
  $menu_name = $menu_default[0];
  $plid = $menu_default[1];
  $link += array(
    'link_title' => '',
    'mlid' => 0,
    'plid' => $plid,
    'menu_name' => $menu_name,
    'weight' => 0,
    'options' => array(),
    'module' => 'menu',
    'expanded' => 0,
    'hidden' => 0,
    'has_children' => 0,
    'customized' => 0,
    'translation' => 0,
  );

  // Find the depth limit for the parent select.
  if (!isset($link['parent_depth_limit'])) {
    $link['parent_depth_limit'] = _menu_parent_depth_limit($link);
  }
}
/**
 * Load all menu items associated with a node.
 *
 * @param $node
 *   The source path to look up.
 *
 * @return
 *   Array of paths or NULL if none found.
 */
function _menu_improvements_load($node) {
  $type_menus = variable_get('menu_options_' . $node->type, array('main-menu' => 'main-menu'));
  if (empty($type_menus)) {
    return NULL;
  }

  $links = db_select('menu_links')
    ->condition('module', 'menu')
    ->condition('router_path', 'node/%')
    ->condition('link_path', 'node/' . $node->nid)
    ->condition('menu_name', $type_menus, 'IN')
    ->fields('menu_links')
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  foreach ($links as &$link) {
    // Unserialize options array.
    $link['options'] = unserialize($link['options']);

    // Set values used in the form.
    if (!empty($link['options']['attributes']['title'])) {
      $link['description'] = $link['options']['attributes']['title'];
    }
    $link['enabled'] = !$link['hidden'];
  }

  return $links;
}
<?php
/*
När jag automatiserar det hela med superadminpanelen så bör bara alla användarnamn behöva skrivas in. Ändra sedan i den här filen och i bytbil_xml_preprocess_node() (för finansieringsknappen) så att rätt värde alltid läggs in.
*/

function bytbil_xml_init() {
	$xml_data = bytbil_xml_init_data();
	if(!$xml_data) return false;

  if(module_exists('redirect_anonymous')) {
		variable_set('redirect_anonymous_open', false);
	}
  // Clear page caches.
  if (module_exists('block')) {
    cache_clear_all('*', 'cache_block', TRUE);
  }
  cache_clear_all('*', 'cache_page', TRUE);

  _bytbil_flush_imagecache();

  watchdog('bytbil_xml', 'Running XML Sync');
  
  //Ta bort alla bilar först så att vi vet att alla bilar uppdateras som de ska.
  $result = db_select('node', 'n')
  	->fields('n', array('nid'))
  	->condition('type', 'bytbil_bil')
  	->execute();
  while($nid = $result->fetchField()) {
    node_delete($nid);
  }

	$data = bytbil_xml_load_data($xml_data);
  $items = array();
  //dpm($data, 'data');

  $befintliga_bilar = $data['befintliga_bilar'];
  $nya_bilar = $data['nya_bilar'];
  $gamla_bilar = $data['gamla_bilar'];
  
  foreach($nya_bilar as $bil) {
		$items[] = array('bytbil_xml_perform_task', array('add', null, $bil));
  }
  foreach($befintliga_bilar as $nid) {

  }
  foreach($gamla_bilar as $nid) {
    $items[] = array('bytbil_xml_perform_task', array('delete', $nid));
  }

	//Förbered minmax-arrayen
	$minmax = array(
		'miltal' => array('min' => -1, 'max' => -1),
		'pris' => array('min' => -1, 'max' => -1),
		'arsmodell' => array('min' => -1, 'max' => -1),
	);
	variable_set('bytbil_minmax', $minmax);

	//Gör i ordning taxonomier
	$taxonomies = array();
	$vocabularies = db_select('taxonomy_vocabulary', 'tv')
		->fields('tv', array('vid', 'machine_name'))
		->condition('machine_name', db_like('bytbil_') . '%', 'LIKE')
		->execute()
		->fetchAll();
	if($vocabularies) {
		foreach($vocabularies as $row) {
		  $tree = taxonomy_get_tree($row->vid);
		  if($row->machine_name === 'bytbil_drivhjul') {
				$taxonomies[$row->machine_name] = array(
					'vid' => $row->vid,
					'terms' => $tree,
				);
			}
			else { //Töm alla berörda vokabulärer så att det finns bilar för alla termer man kan filtrera på.
				foreach($tree as $term) {
					taxonomy_term_delete($term->tid);
				}
				$taxonomies[$row->machine_name] = array(
					'vid' => $row->vid,
					'terms' => array(),
				);
			}
		}
	}
	variable_set('bytbil_taxonomies', $taxonomies);

  watchdog('bytbil_xml', 'Starting working on jobs...');
  
  return $items;

}
function _bytbil_flush_imagecache() {
	//Flushar alla imagecachegenererade bilder som har med Byt Bil att göra
	watchdog('bytbil_xml', t('Flushing all bytbil related images'));

	//Flushar hämtade externa bilder
  $scheme = file_default_scheme();
  $wrapper= file_stream_wrapper_get_instance_by_scheme($scheme);
  $path   = realpath($wrapper->getDirectoryPath() . '/imagecache');
  if (is_dir($path)) {
    file_unmanaged_delete_recursive($path);
  }
  //Flushar genererade bilder
  $image_styles = image_styles();
  foreach($image_styles as $name => $style) {
		if(substr($name, 0, 7) !== 'bytbil_') {
			unset($image_styles[$name]);
		}
	}
	foreach ($image_styles as $name => $style) {
		if(isset($style['name']) && $style['name']) {
			$style_directory = drupal_realpath(file_default_scheme() . '://styles/' . $style['name']);
		  if (is_dir($style_directory)) {
		    file_unmanaged_delete_recursive($style_directory);
		  }
	  	// Let other modules update as necessary on flush.
	  	module_invoke_all('image_style_flush', $style);
		}
		else {
			watchdog('bytbil_xml', t('Style @name cannot be found!', array('@name' => $name)));
		}
  }
  // Clear image style and effect caches.
  cache_clear_all('image_styles', 'cache');
  cache_clear_all('image_effects:', 'cache', TRUE);
  drupal_static_reset('image_styles');
  drupal_static_reset('image_effects');

  // Clear field caches so that formatters may be added for this style.
  field_info_cache_clear();
  drupal_theme_rebuild();

  watchdog('bytbil_xml', t('Flush completed successfully!'));
}
function bytbil_xml_init_data() {
	$xml_data = array();
  $urls = array(
  	'http://data.bytbil.com/sellers/autoitalia/xml/autoitalia.xml',
  );
  foreach($urls as $key => $url) {
	  $temp = bytbil_xml_fetch_data($url);
	  $xml_data = array_merge($xml_data, $temp);
	}
	return $xml_data;
}
function bytbil_xml_load_data($xml_data) {
  $data = array();
  $befintliga_bilar = array();
  $nya_bilar = array();
  $gamla_bilar = array();
  //Hämta befintliga bilar
	$query = db_select('node', 'node')
		->fields('node', array('nid'))
		->fields('field_regnr', array('field_bytbil_regnr_value'))
		->condition('node.type', 'bytbil_bil')
		->condition('field_regnr.bundle', 'bytbil_bil');
	$query->innerJoin('field_data_field_bytbil_regnr', 'field_regnr', 'node.nid = field_regnr.entity_id');
	$result =	$query->execute()->fetchAll();
	if($result) {
		foreach($result as $row) {
			$befintliga_bilar[$row->field_bytbil_regnr_value] = $row->nid;
		}
	}

  foreach($xml_data as $regnr => $bil) {
    if(!isset($befintliga_bilar[$regnr])) {
      $nya_bilar[$regnr] = $bil;
    }
  }
  foreach($befintliga_bilar as $regnr => $nid) {
    if(!isset($xml_data[$regnr])) {
      $gamla_bilar[$regnr] = $nid;
      unset($befintliga_bilar[$regnr]);
    }
  }
  
  $data = array(
    'befintliga_bilar' => $befintliga_bilar,
    'nya_bilar' => $nya_bilar,
    'gamla_bilar' => $gamla_bilar,
  );

  return $data;
}
function bytbil_xml_perform_task($op, $local_data = null, $foreign_data = null) {
  watchdog('bytbil_xml_perform_task', "op: $op");
  switch($op) {
    case 'add': //Lägg till ny bil
			$current_minmax = array();

      $node = new stdClass();
      $node->type = 'bytbil_bil'; //This can be any node type
			node_object_prepare($node); // Set some default values
      //Huvudsakliga fält
      $node->title = '';
			if($foreign_data['brand']) {
				$node->title .= $foreign_data['brand'].' ';
			}
			if($foreign_data['model']) {
				$node->title .= $foreign_data['model'].' ';
			}
			if($foreign_data['modeldescription']) {
				$node->title .= str_replace('/', ' / ', $foreign_data['modeldescription']);
			}
      $node->language = 'sv';

      if($foreign_data['yearmodel']) {
				$node->field_bytbil_arsmodell[LANGUAGE_NONE][0]['value'] = $foreign_data['yearmodel'];
      	$current_minmax['arsmodell'] = $node->field_bytbil_arsmodell[LANGUAGE_NONE][0]['value'];
			}
			if(!$foreign_data['price-sek']) $foreign_data['price-sek'] = '0';
			$node->field_bytbil_pris[LANGUAGE_NONE][0]['value'] = $foreign_data['price-sek'];
      $node->field_bytbil_pris[LANGUAGE_NONE][0]['value'] = str_replace('.', ';', $node->field_bytbil_pris[LANGUAGE_NONE][0]['value']);
      $node->field_bytbil_pris[LANGUAGE_NONE][0]['value'] = str_replace(',', '.', $node->field_bytbil_pris[LANGUAGE_NONE][0]['value']);
      $node->field_bytbil_pris[LANGUAGE_NONE][0]['value'] = str_replace(';', '', $node->field_bytbil_pris[LANGUAGE_NONE][0]['value']);
      $current_minmax['pris'] = (int) $node->field_bytbil_pris[LANGUAGE_NONE][0]['value'];

			if(!$foreign_data['miles']) $foreign_data['miles'] = '0';
			$node->field_bytbil_miltal[LANGUAGE_NONE][0]['value'] = $foreign_data['miles'];
      $node->field_bytbil_miltal[LANGUAGE_NONE][0]['value'] = str_replace('.', ';', $node->field_bytbil_miltal[LANGUAGE_NONE][0]['value']);
     	$node->field_bytbil_miltal[LANGUAGE_NONE][0]['value'] = str_replace(',', '.', $node->field_bytbil_miltal[LANGUAGE_NONE][0]['value']);
      $node->field_bytbil_miltal[LANGUAGE_NONE][0]['value'] = str_replace(';', '', $node->field_bytbil_miltal[LANGUAGE_NONE][0]['value']);
      $current_minmax['miltal'] = (int) $node->field_bytbil_miltal[LANGUAGE_NONE][0]['value'];

      if($foreign_data['color']) $node->field_bytbil_colorname[LANGUAGE_NONE][0]['value'] = $foreign_data['color'];
      if($foreign_data['regno']) $node->field_bytbil_regnr[LANGUAGE_NONE][0]['value'] = $foreign_data['regno'];
      if($foreign_data['info']) $node->field_bytbil_info[LANGUAGE_NONE][0]['value'] = $foreign_data['info'];
      //Bilder
      if($foreign_data['images']) {
				foreach($foreign_data['images'] as $paths) {
	        foreach($paths as $filepath) {
	          if(strpos(substr($filepath, -5), '.') !== false) { //Kollar att det är en bildfil
							//Kollar att filen finns på servern
							$result = drupal_http_request($filepath);
							$valid_image = false;
							if($result->code == 200) {
								$valid_image = true;
		            $node->field_bytbil_bilder[LANGUAGE_NONE][] = array(
									'url' => $filepath,
									'title' => '',
									'alt' => '',
								);
							}
							else {
								//Prova med xxx1.xxx i stället
								$filepath_split = explode('.', $filepath);
								$filepath_split[count($filepath_split) - 2] = substr_replace($filepath_split[count($filepath_split) - 2], '1', -1);
								$filepath = implode('.', $filepath_split);
								$result = drupal_http_request($filepath);
								if($result->code == 200) {
									$valid_image = true;
			            $node->field_bytbil_bilder[LANGUAGE_NONE][] = array(
										'url' => $filepath,
										'title' => '',
										'alt' => '',
									);
								}
							}
							if($valid_image) { //Generera bildstilar
								$styles = image_styles();
								foreach($styles as $name => $style) {
									if(isset($style['name']) && $style['name']) {
										if(substr($name, 0, 7) === 'bytbil_') {
											$uri = imagecache_external_generate_path($filepath, $style['name']);
											if($uri) {
												if($dest = image_style_path($style['name'], $uri)) {
													image_style_create_derivative($style, $uri, $dest);
												}
											}
										}
									}
								}
							}
	          }
	        }
	      }
			}

      //Taxonomier
      $taxonomies = variable_get('bytbil_taxonomies', array());
      if($taxonomies) {

	      //Drivhjul
	      $vocname = 'bytbil_drivhjul';
	      $fieldname = 'field_'.$vocname;
	      if(isset($taxonomies[$vocname])) {
		      if(strpos($foreign_data['info'], '4WD') === false) {
						$termname = '2-hjulsdriven';
						foreach($taxonomies[$vocname]['terms'] as $term) {
							if($term->name === $termname) {
								$node->$fieldname = array(
									LANGUAGE_NONE => array(
										0 => array(
											'tid' => $term->tid,
										),
									),
								);
								break;
							}
						}
		      }
		      else {
						$termname = '4-hjulsdriven';
						foreach($taxonomies[$vocname]['terms'] as $term) {
							if($term->name === $termname) {
								$node->$fieldname = array(
									LANGUAGE_NONE => array(
										0 => array(
											'tid' => $term->tid,
										),
									),
								);
								break;
							}
						}
		      }
				}
	
	      //Drivmedel
	      $vocname = 'bytbil_drivmedel';
	      $fieldname = 'field_'.$vocname;
	      $termname = $foreign_data['fueltype'];
	      $tid = bytbil_xml_get_term_tid($taxonomies, $vocname, $termname);
	      if($tid !== false) {
					$node->$fieldname = array(
						LANGUAGE_NONE => array(
							0 => array(
								'tid' => $tid,
							),
						),
					);
				}
	
	      //Karosseri
	      $vocname = 'bytbil_karosseri';
	      $fieldname = 'field_'.$vocname;
	      $termname = $foreign_data['bodytype'];
	      $tid = bytbil_xml_get_term_tid($taxonomies, $vocname, $termname);
	      if($tid !== false) {
					$node->$fieldname = array(
						LANGUAGE_NONE => array(
							0 => array(
								'tid' => $tid,
							),
						),
					);
				}
	
	      //Växellåda
	      $vocname = 'bytbil_vaxellada';
	      $fieldname = 'field_'.$vocname;
	      $termname = $foreign_data['gearboxtype'];
	      $tid = bytbil_xml_get_term_tid($taxonomies, $vocname, $termname);
	      if($tid !== false) {
					$node->$fieldname = array(
						LANGUAGE_NONE => array(
							0 => array(
								'tid' => $tid,
							),
						),
					);
				}
	
	      //Märke/modell
	      $vocname = 'bytbil_bilmodell';
	      $fieldname = 'field_'.$vocname;
	      $marke = $foreign_data['brand'];
	      $modell = $foreign_data['model'];
	      if(!$marke) {
					$marke = t('Other');
				}
				if(!$modell) {
					$modell = t('Other');
				}
	      if(isset($taxonomies[$vocname])) {
					$markemodell = array();
					//Hitta först märket
					$parent = bytbil_xml_get_term_tid($taxonomies, $vocname, $marke);
					if($parent !== false) {
						$markemodell = array(
							LANGUAGE_NONE => array(
								0 => array(
									'tid' => $parent,
								),
							),
						);
			    	//Hitta sedan modellen
			    	$tid = bytbil_xml_get_term_tid($taxonomies, $vocname, $modell, $parent);
			    	if($tid !== false) {
					    $markemodell[LANGUAGE_NONE][1] = array(
								'tid' => $tid,
							);
						}
					}
					if($markemodell) {
						$node->$fieldname = $markemodell;
					}
				}
				variable_set('bytbil_taxonomies', $taxonomies);
			}

      //Sparar noden
			if($node = node_submit($node)) { // Prepare node for saving
				if($foreign_data['added']) $node->created = $foreign_data['added']; //Det här måste ligga här för att Drupal inte ska ändra created i node_submit().
		    node_save($node);
				$minmax = variable_get('bytbil_minmax', array());
				if($minmax) {
					foreach($current_minmax as $key => $value) {
						if($minmax[$key]['min'] == -1 || $value < $minmax[$key]['min']) {
							$minmax[$key]['min'] = (int) $value;
						}
						if($minmax[$key]['max'] == -1 || $value > $minmax[$key]['max']) {
							$minmax[$key]['max'] = (int) $value;
						}
					}
					variable_set('bytbil_minmax', $minmax);
				}
			}
      else {
        return false;
      }

    break;
    case 'update': //Uppdatera bil
    	/* Det här stycket behöver uppdateras till Drupal 7 om det ska användas.
      //Uppdatera informationen i noden utifrån curdata
      $local_data->name = $foreign_data['brand'].' '.$foreign_data['model'].' '.$foreign_data['modeldescription'];
      $local_data->name = $local_data->name;
      $local_data->title = $local_data->name;
      
      $local_data->field_bytbil_model[0]['value'] = $foreign_data['yearmodel'];
      $local_data->field_bytbil_pris[0]['value'] = $foreign_data['price-sek'];
      $local_data->field_bytbil_miltal[0]['value'] = $foreign_data['miles'];
      $local_data->field_bytbil_colorname[0]['value'] = $foreign_data['color'];
      $local_data->field_bytbil_regnr[0]['value'] = $foreign_data['regno'];
      $local_data->field_bytbil_utrustning[0]['value'] = $foreign_data['info'];
      $local_data->field_bytbil_personal_anlaggning[0]['nid'] = $foreign_data['anlaggning_nid'];
  
      node_save($local_data);
      */
    break;
    case 'delete': //Radera bil
      node_delete($local_data);
    break;
  }
  return true;
}
function bytbil_xml_get_term_tid(&$taxonomies, $vocname, $termname, $parent = -1, $create_if_not_found = true) {
  if(isset($taxonomies[$vocname])) {
		foreach($taxonomies[$vocname]['terms'] as $term) {
			if($parent != -1) {
				if(isset($term->parent)) {
					if(!in_array($parent, $term->parent)) continue;
				}
				elseif(isset($term->parents)) {
					if(!in_array($parent, $term->parents)) continue;
				}
			}
			if($term->name === $termname) {
				return $term->tid;
			}
		}
    //Skapa ny term eftersom den uppenbarligen inte har hittats
    if($create_if_not_found) {
      $new_term = bytbil_xml_create_taxonomy_term($termname, $taxonomies[$vocname]['vid'], $vocname, ($parent != -1 ? $parent : 0));
      $taxonomies[$vocname]['terms'][] = $new_term;
      return $new_term->tid;
		}
	}
	return false;
}
function bytbil_xml_create_taxonomy_term($termname, $vid, $vocname, $parent = 0) {
	$term = new stdClass();
	$term->name = $termname;
	$term->vid = $vid;
	$term->vocabulary_machine_name = $vocname;
	if($parent) $term->parent = $parent;
	taxonomy_term_save($term);
	return $term;
}
function bytbil_xml_fetch_data($url) {
  $xml_data = drupal_http_request($url);
  if($xml_data->code == 200) {
    $xml_data = bytbil_xml_xmlstr_to_array($xml_data->data);
    if($xml_data) {
	    $new_xml = array();
	    foreach($xml_data['car'] as $i => $car) {
	      if($car['regno']) {
	        $new_xml[$car['regno']] = $car;
	      }
	    }
	    $xml_data = $new_xml;
	    unset($new_xml);
	    if(is_array($xml_data)) { //XML-datan har framgångsrikt översatts till en array
	      return $xml_data;
	    }
		}
		else {
			watchdog('bytbil_xml', "XML file error: $url, no elements in XML file");
		}
  }
  else {
    watchdog("bytbil_xml", "XML file error: $url, status: $xml_data->code $xml_data->error");
  }
  return array();
}
/**
 * convert xml string to php array - useful to get a serializable value
 *
 * @param string $xmlstr
 * @return array
 * @author Adrien aka Gaarf
 */
function bytbil_xml_xmlstr_to_array($xmlstr) {
  $doc = new DOMDocument();
  $return = array();
  if($doc->loadXML($xmlstr)) {
	  $return = bytbil_xml_domnode_to_array($doc->documentElement);
	}
	else {
		watchdog('bytbil_xml', 'XML file error: Could not load XML from file');
	}
  unset($doc);
  return $return;
}
function bytbil_xml_domnode_to_array($node) {
  $output = array();
  switch ($node->nodeType) {
   case XML_CDATA_SECTION_NODE:
   case XML_TEXT_NODE:
    $output = trim($node->textContent);
   break;
   case XML_ELEMENT_NODE:
    for ($i=0, $m=$node->childNodes->length; $i<$m; $i++) {
     $child = $node->childNodes->item($i);
     $v = bytbil_xml_domnode_to_array($child);
     if (isset($child->tagName)) {
       $t = $child->tagName;
       if (!isset($output[$t])) {
        $output[$t] = array();
       }
       if(empty($v)) {
        $v = "";
       }
       $output[$t][] = $v;
      } elseif ($v || $v === "0") {
       $output = (string) $v;
     }
    }

    if(is_array($output)) {
     if($node->attributes->length) {
      $a = array();
      foreach($node->attributes as $attrName => $attrNode) {
       $a[$attrName] = (string) $attrNode->value;
      }
      $output['@attributes'] = $a;
     }
     foreach ($output as $t => $v) {
      if(is_array($v) && count($v)==1 && $t!='@attributes') {
       $output[$t] = $v[0];
      }
     }
    }
   break;
  }
  return $output;
}
function bytbil_xml_intresseanmalan($farg='', $marke='', $modell='') {
	$nid = db_select('node', 'n')
		->fields('n', array('nid'))
		->condition('n.title', utf8_encode('Intresseanmälan'))
		->condition('n.type', 'webform')
		->execute()
		->fetchField();
	if($nid) {
		$_GET['farg'] = $farg;
		$_GET['marke'] = $marke;
		$_GET['modell'] = $modell;
		$content = general_draw_node($nid, TRUE);
		print general_deliver_limited_page($content);
	}
}
function bytbil_xml_tipsa($nid=0, $marke='', $modell='', $modellar='') {
	if($nid) {
		$tipsa_nid = db_select('node', 'n')
		->fields('n', array('nid'))
		->condition('n.title', utf8_encode('Tipsa en vän'))
		->condition('n.type', 'webform')
		->execute()
		->fetchField();
		if($tipsa_nid) {
			$_GET['url'] = url("node/$nid", array('absolute' => true));
			$_GET['marke'] = $marke;
			$_GET['modell'] = $modell;
			$_GET['modellar'] = $modellar;
			$content = general_draw_node($tipsa_nid, TRUE);
			print general_deliver_limited_page($content);
		}
	}
}
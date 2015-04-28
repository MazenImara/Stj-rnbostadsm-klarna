<?php
/**
 * @file
 * bytbil.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function bytbil_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function bytbil_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function bytbil_image_default_styles() {
  $styles = array();

  // Exported image style: bytbil_medium.
  $styles['bytbil_medium'] = array(
    'name' => 'bytbil_medium',
    'effects' => array(
      121 => array(
        'label' => 'Change file format',
        'help' => 'Choose to save the image as a different filetype.',
        'effect callback' => 'coloractions_convert_image',
        'dimensions passthrough' => TRUE,
        'form callback' => 'coloractions_convert_form',
        'summary theme' => 'coloractions_convert_summary',
        'module' => 'imagecache_coloractions',
        'name' => 'coloractions_convert',
        'data' => array(
          'format' => 'image/png',
          'quality' => '75',
        ),
        'weight' => '1',
      ),
      122 => array(
        'label' => 'Skala',
        'help' => 'Skalning kommer att bibehålla proportionerna i den ursprungliga bilden. Om enbart ett mått anges kommer det andra måttet att räknas ut.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => '575',
          'height' => '383',
          'upscale' => 1,
        ),
        'weight' => '2',
      ),
      123 => array(
        'label' => 'Define canvas',
        'help' => 'Define the size of the working canvas and background color, this controls the dimensions of the output image.',
        'effect callback' => 'canvasactions_definecanvas_effect',
        'dimensions callback' => 'canvasactions_definecanvas_dimensions',
        'form callback' => 'canvasactions_definecanvas_form',
        'summary theme' => 'canvasactions_definecanvas_summary',
        'module' => 'imagecache_canvasactions',
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '575',
            'height' => '383',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => '3',
      ),
    ),
  );

  // Exported image style: bytbil_puffbild.
  $styles['bytbil_puffbild'] = array(
    'name' => 'bytbil_puffbild',
    'effects' => array(
      124 => array(
        'label' => 'Change file format',
        'help' => 'Choose to save the image as a different filetype.',
        'effect callback' => 'coloractions_convert_image',
        'dimensions passthrough' => TRUE,
        'form callback' => 'coloractions_convert_form',
        'summary theme' => 'coloractions_convert_summary',
        'module' => 'imagecache_coloractions',
        'name' => 'coloractions_convert',
        'data' => array(
          'format' => 'image/png',
          'quality' => '75',
        ),
        'weight' => '-10',
      ),
      125 => array(
        'label' => 'Skala',
        'help' => 'Skalning kommer att bibehålla proportionerna i den ursprungliga bilden. Om enbart ett mått anges kommer det andra måttet att räknas ut.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => '263',
          'height' => '175',
          'upscale' => 1,
        ),
        'weight' => '-9',
      ),
      126 => array(
        'label' => 'Define canvas',
        'help' => 'Define the size of the working canvas and background color, this controls the dimensions of the output image.',
        'effect callback' => 'canvasactions_definecanvas_effect',
        'dimensions callback' => 'canvasactions_definecanvas_dimensions',
        'form callback' => 'canvasactions_definecanvas_form',
        'summary theme' => 'canvasactions_definecanvas_summary',
        'module' => 'imagecache_canvasactions',
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '263',
            'height' => '175',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => '-7',
      ),
    ),
  );

  // Exported image style: bytbil_thumbnail.
  $styles['bytbil_thumbnail'] = array(
    'name' => 'bytbil_thumbnail',
    'effects' => array(
      120 => array(
        'label' => 'Skala',
        'help' => 'Skalning kommer att bibehålla proportionerna i den ursprungliga bilden. Om enbart ett mått anges kommer det andra måttet att räknas ut.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => '100',
          'height' => '100',
          'upscale' => 1,
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function bytbil_node_info() {
  $items = array(
    'bytbil_bil' => array(
      'name' => t('Bil från Byt Bil'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Titel'),
      'help' => '',
    ),
    'page' => array(
      'name' => t('Sida'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Rubrik'),
      'help' => '',
    ),
    'webform' => array(
      'name' => t('Formulär'),
      'base' => 'node_content',
      'description' => t('Create a new form or questionnaire accessible to users. Submission results and statistics are recorded and accessible to privileged users.'),
      'has_title' => '1',
      'title_label' => t('Rubrik'),
      'help' => '',
    ),
  );
  return $items;
}

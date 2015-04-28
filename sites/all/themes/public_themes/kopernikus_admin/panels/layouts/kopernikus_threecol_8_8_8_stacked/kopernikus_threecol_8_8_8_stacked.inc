<?php

/**
 * Implements hook_panels_layouts().
 */
function kopernikus_kopernikus_threecol_8_8_8_stacked_panels_layouts() {
  $items['kopernikus_threecol_8_8_8_stacked'] = array(
    'title' => t('Three column 8-8-8 stacked'),
    'category' => t('Kopernikus Custom Layouts'),
    'icon' => 'kopernikus_threecol_8_8_8_stacked.png',
    'theme' => 'kopernikus_threecol_8_8_8_stacked',
    'admin css' => 'kopernikus_threecol_8_8_8_stacked.css',
    'regions' => array(
      'top' => t('Top'),
      'left' => t('Left side'),
      'middle' => t('Middle column'),
      'right' => t('Right side'),
      'bottom' => t('Bottom'),
    ),
  );

  return $items;
}

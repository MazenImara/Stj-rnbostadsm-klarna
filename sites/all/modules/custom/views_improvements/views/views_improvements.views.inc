<?php

//Implements hook_views_data_alter().
function views_improvements_views_data_alter(&$data) {
  if ( isset($data['views']) && !isset($data['views']['fields_compare_improvements']) ) {
    $data['views']['fields_compare_improvements'] = array(
      'title' => t('Fields comparison (improved)'),
      'help' => t('Compare database fields against each other.'),
      'filter' => array(
        'help' => t('Use fields comparison to filter the result of the view.'),
        'handler' => 'views_improvements_handler_filter_fields_compare',
      )
    );
  }
}
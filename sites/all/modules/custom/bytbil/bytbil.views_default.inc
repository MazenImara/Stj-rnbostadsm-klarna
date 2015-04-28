<?php
/**
 * @file
 * bytbil.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function bytbil_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'bytbil_bilar';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Byt Bil - bilar';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_text'] = 'mer';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Återställ';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortering';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Stigande';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Fallande';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Alla -';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = 1;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* Fält: Innehåll: Titel */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Innehåll: Titel */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['title']['expose']['label'] = 'Märke/modell';
  /* Sort criterion: Innehåll: Mätarställning (field_bytbil_miltal) */
  $handler->display->display_options['sorts']['field_bytbil_miltal_value']['id'] = 'field_bytbil_miltal_value';
  $handler->display->display_options['sorts']['field_bytbil_miltal_value']['table'] = 'field_data_field_bytbil_miltal';
  $handler->display->display_options['sorts']['field_bytbil_miltal_value']['field'] = 'field_bytbil_miltal_value';
  $handler->display->display_options['sorts']['field_bytbil_miltal_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['field_bytbil_miltal_value']['expose']['label'] = 'Mätarställning';
  /* Sort criterion: Innehåll: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Datum inlagd';
  $handler->display->display_options['sorts']['created']['granularity'] = 'day';
  /* Sort criterion: Innehåll: Pris (field_bytbil_pris) */
  $handler->display->display_options['sorts']['field_bytbil_pris_value']['id'] = 'field_bytbil_pris_value';
  $handler->display->display_options['sorts']['field_bytbil_pris_value']['table'] = 'field_data_field_bytbil_pris';
  $handler->display->display_options['sorts']['field_bytbil_pris_value']['field'] = 'field_bytbil_pris_value';
  $handler->display->display_options['sorts']['field_bytbil_pris_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['field_bytbil_pris_value']['expose']['label'] = 'Pris';
  /* Filter criterion: Innehåll: Publicerad */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Innehåll: Typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'bytbil_bil' => 'bytbil_bil',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Innehåll: Bilmodell (field_bytbil_bilmodell) */
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['id'] = 'field_bytbil_bilmodell_tid';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['table'] = 'field_data_field_bytbil_bilmodell';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['field'] = 'field_bytbil_bilmodell_tid';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['expose']['operator_id'] = 'field_bytbil_bilmodell_tid_op';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['expose']['label'] = 'Märke/modell (i lager just nu)';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['expose']['operator'] = 'field_bytbil_bilmodell_tid_op';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['expose']['identifier'] = 'field_bytbil_bilmodell_tid';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['vocabulary'] = 'bytbil_bilmodell';
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['hierarchy'] = 1;
  $handler->display->display_options['filters']['field_bytbil_bilmodell_tid']['error_message'] = 1;
  /* Filter criterion: Innehåll: Karosseri (field_bytbil_karosseri) */
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['id'] = 'field_bytbil_karosseri_tid';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['table'] = 'field_data_field_bytbil_karosseri';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['field'] = 'field_bytbil_karosseri_tid';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['expose']['operator_id'] = 'field_bytbil_karosseri_tid_op';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['expose']['label'] = 'Karosseri';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['expose']['operator'] = 'field_bytbil_karosseri_tid_op';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['expose']['identifier'] = 'field_bytbil_karosseri_tid';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['vocabulary'] = 'bytbil_karosseri';
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['hierarchy'] = 1;
  $handler->display->display_options['filters']['field_bytbil_karosseri_tid']['error_message'] = 1;
  /* Filter criterion: Innehåll: Drivhjul (field_bytbil_drivhjul) */
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['id'] = 'field_bytbil_drivhjul_tid';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['table'] = 'field_data_field_bytbil_drivhjul';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['field'] = 'field_bytbil_drivhjul_tid';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['expose']['operator_id'] = 'field_bytbil_drivhjul_tid_op';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['expose']['label'] = 'Drivhjul';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['expose']['operator'] = 'field_bytbil_drivhjul_tid_op';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['expose']['identifier'] = 'field_bytbil_drivhjul_tid';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['vocabulary'] = 'bytbil_drivhjul';
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['hierarchy'] = 1;
  $handler->display->display_options['filters']['field_bytbil_drivhjul_tid']['error_message'] = 1;
  /* Filter criterion: Innehåll: Drivmedel (field_bytbil_drivmedel) */
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['id'] = 'field_bytbil_drivmedel_tid';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['table'] = 'field_data_field_bytbil_drivmedel';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['field'] = 'field_bytbil_drivmedel_tid';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['expose']['operator_id'] = 'field_bytbil_drivmedel_tid_op';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['expose']['label'] = 'Drivmedel';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['expose']['operator'] = 'field_bytbil_drivmedel_tid_op';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['expose']['identifier'] = 'field_bytbil_drivmedel_tid';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['vocabulary'] = 'bytbil_drivmedel';
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['hierarchy'] = 1;
  $handler->display->display_options['filters']['field_bytbil_drivmedel_tid']['error_message'] = 1;
  /* Filter criterion: Innehåll: Växellåda (field_bytbil_vaxellada) */
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['id'] = 'field_bytbil_vaxellada_tid';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['table'] = 'field_data_field_bytbil_vaxellada';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['field'] = 'field_bytbil_vaxellada_tid';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['expose']['operator_id'] = 'field_bytbil_vaxellada_tid_op';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['expose']['label'] = 'Växellåda';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['expose']['operator'] = 'field_bytbil_vaxellada_tid_op';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['expose']['identifier'] = 'field_bytbil_vaxellada_tid';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['vocabulary'] = 'bytbil_vaxellada';
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['hierarchy'] = 1;
  $handler->display->display_options['filters']['field_bytbil_vaxellada_tid']['error_message'] = 1;
  /* Filter criterion: Innehåll: Årsmodell (field_bytbil_arsmodell) */
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['id'] = 'field_bytbil_arsmodell_value';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['table'] = 'field_data_field_bytbil_arsmodell';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['field'] = 'field_bytbil_arsmodell_value';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['expose']['operator_id'] = 'field_bytbil_arsmodell_value_op';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['expose']['label'] = 'Årsmodell';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['expose']['operator'] = 'field_bytbil_arsmodell_value_op';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['expose']['identifier'] = 'field_bytbil_arsmodell_value';
  $handler->display->display_options['filters']['field_bytbil_arsmodell_value']['expose']['multiple'] = FALSE;
  /* Filter criterion: Innehåll: Pris (field_bytbil_pris) */
  $handler->display->display_options['filters']['field_bytbil_pris_value']['id'] = 'field_bytbil_pris_value';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['table'] = 'field_data_field_bytbil_pris';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['field'] = 'field_bytbil_pris_value';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_pris_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_pris_value']['expose']['operator_id'] = 'field_bytbil_pris_value_op';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['expose']['label'] = 'Pris';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['expose']['operator'] = 'field_bytbil_pris_value_op';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['expose']['identifier'] = 'field_bytbil_pris_value';
  $handler->display->display_options['filters']['field_bytbil_pris_value']['expose']['multiple'] = FALSE;
  /* Filter criterion: Innehåll: Mätarställning (field_bytbil_miltal) */
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['id'] = 'field_bytbil_miltal_value';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['table'] = 'field_data_field_bytbil_miltal';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['field'] = 'field_bytbil_miltal_value';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['group'] = 1;
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['expose']['operator_id'] = 'field_bytbil_miltal_value_op';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['expose']['label'] = 'Mätarställning';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['expose']['operator'] = 'field_bytbil_miltal_value_op';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['expose']['identifier'] = 'field_bytbil_miltal_value';
  $handler->display->display_options['filters']['field_bytbil_miltal_value']['expose']['multiple'] = FALSE;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $export['bytbil_bilar'] = $view;

  return $export;
}

<?php
/**
 * @file
 * bytbil.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function bytbil_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_bytbil_bil';
  $strongarm->value = '0';
  $export['comment_anonymous_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_page';
  $strongarm->value = '0';
  $export['comment_anonymous_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_webform';
  $strongarm->value = '0';
  $export['comment_anonymous_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_bytbil_bil';
  $strongarm->value = '1';
  $export['comment_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_bytbil_bil';
  $strongarm->value = 1;
  $export['comment_default_mode_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_page';
  $strongarm->value = 1;
  $export['comment_default_mode_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_webform';
  $strongarm->value = 1;
  $export['comment_default_mode_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_bytbil_bil';
  $strongarm->value = '50';
  $export['comment_default_per_page_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_page';
  $strongarm->value = '50';
  $export['comment_default_per_page_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_webform';
  $strongarm->value = '50';
  $export['comment_default_per_page_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_bytbil_bil';
  $strongarm->value = 1;
  $export['comment_form_location_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_page';
  $strongarm->value = 1;
  $export['comment_form_location_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_webform';
  $strongarm->value = 1;
  $export['comment_form_location_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_page';
  $strongarm->value = '1';
  $export['comment_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_bytbil_bil';
  $strongarm->value = '1';
  $export['comment_preview_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_page';
  $strongarm->value = '1';
  $export['comment_preview_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_webform';
  $strongarm->value = '1';
  $export['comment_preview_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_bytbil_bil';
  $strongarm->value = 1;
  $export['comment_subject_field_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_page';
  $strongarm->value = 1;
  $export['comment_subject_field_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_webform';
  $strongarm->value = 1;
  $export['comment_subject_field_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_webform';
  $strongarm->value = '1';
  $export['comment_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__bytbil_bil';
  $strongarm->value = array(
    'view_modes' => array(
      'kopernikus_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_promo' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_full' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_rss' => array(
        'custom_settings' => FALSE,
      ),
      'kopernikus_token' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'language' => array(
          'weight' => '13',
        ),
        'metatags' => array(
          'weight' => '14',
        ),
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '17',
        ),
        'redirect' => array(
          'weight' => '15',
        ),
        'xmlsitemap' => array(
          'weight' => '16',
        ),
      ),
      'display' => array(
        'language' => array(
          'default' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'kopernikus_teaser' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
          'kopernikus_full' => array(
            'weight' => '12',
            'visible' => FALSE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_node__bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__page';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => TRUE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'promo' => array(
        'custom_settings' => TRUE,
      ),
      'carousel' => array(
        'custom_settings' => TRUE,
      ),
      'slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'india_full' => array(
        'custom_settings' => TRUE,
      ),
      'india_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'india_promo' => array(
        'custom_settings' => TRUE,
      ),
      'india_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'india_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'india_rss' => array(
        'custom_settings' => FALSE,
      ),
      'india_token' => array(
        'custom_settings' => FALSE,
      ),
      'kopernikus_full' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_promo' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_rss' => array(
        'custom_settings' => FALSE,
      ),
      'kopernikus_token' => array(
        'custom_settings' => FALSE,
      ),
      'pure_white_full' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_promo' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_rss' => array(
        'custom_settings' => FALSE,
      ),
      'pure_white_token' => array(
        'custom_settings' => FALSE,
      ),
      'waterfall_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'waterfall_promo' => array(
        'custom_settings' => TRUE,
      ),
      'waterfall_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'waterfall_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'waterfall_full' => array(
        'custom_settings' => TRUE,
      ),
      'waterfall_rss' => array(
        'custom_settings' => FALSE,
      ),
      'waterfall_token' => array(
        'custom_settings' => FALSE,
      ),
      'big_apple_full' => array(
        'custom_settings' => TRUE,
      ),
      'big_apple_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'big_apple_promo' => array(
        'custom_settings' => TRUE,
      ),
      'big_apple_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'big_apple_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'big_apple_rss' => array(
        'custom_settings' => FALSE,
      ),
      'big_apple_token' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'language' => array(
          'weight' => '2',
        ),
        'title' => array(
          'weight' => '1',
        ),
        'path' => array(
          'weight' => '7',
        ),
        'metatags' => array(
          'weight' => '8',
        ),
        'redirect' => array(
          'weight' => '9',
        ),
        'xmlsitemap' => array(
          'weight' => '10',
        ),
      ),
      'display' => array(
        'language' => array(
          'default' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'teaser' => array(
            'weight' => '2',
            'visible' => FALSE,
          ),
          'full' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'great_india_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'india_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'india_full' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'kopernikus_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'kopernikus_full' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'pure_white_full' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
          'pure_white_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'waterfall_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'waterfall_full' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
          'big_apple_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'big_apple_full' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_node__page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__webform';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'promo' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'kopernikus_full' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_promo' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'kopernikus_rss' => array(
        'custom_settings' => FALSE,
      ),
      'kopernikus_token' => array(
        'custom_settings' => FALSE,
      ),
      'pure_white_teaser' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_promo' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_carousel' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_slideshow' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_full' => array(
        'custom_settings' => TRUE,
      ),
      'pure_white_rss' => array(
        'custom_settings' => FALSE,
      ),
      'pure_white_token' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'language' => array(
          'weight' => '2',
        ),
        'metatags' => array(
          'weight' => '7',
        ),
        'title' => array(
          'weight' => '1',
        ),
        'path' => array(
          'weight' => '6',
        ),
        'redirect' => array(
          'weight' => '8',
        ),
        'webform_template' => array(
          'weight' => '5',
        ),
        'xmlsitemap' => array(
          'weight' => '30',
        ),
      ),
      'display' => array(
        'webform' => array(
          'default' => array(
            'weight' => '10',
            'visible' => TRUE,
          ),
          'promo' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
          'great_india_promo' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
          'svartinge_teaser' => array(
            'weight' => '10',
            'visible' => TRUE,
          ),
          'svartinge_promo' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
          'india_promo' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
          'kopernikus_full' => array(
            'weight' => '10',
            'visible' => TRUE,
          ),
          'kopernikus_promo' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
          'pure_white_promo' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
          'pure_white_teaser' => array(
            'weight' => '10',
            'visible' => TRUE,
          ),
          'pure_white_full' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
          'big_apple_teaser' => array(
            'weight' => '10',
            'visible' => TRUE,
          ),
        ),
        'language' => array(
          'default' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'great_india_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'svartinge_teaser' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'svartinge_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'india_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'kopernikus_full' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'kopernikus_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'pure_white_promo' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'pure_white_teaser' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
          'pure_white_full' => array(
            'weight' => '3',
            'visible' => FALSE,
          ),
          'big_apple_teaser' => array(
            'weight' => '0',
            'visible' => FALSE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_node__webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_bytbil_bil';
  $strongarm->value = '1';
  $export['i18n_node_extended_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_page';
  $strongarm->value = '1';
  $export['i18n_node_extended_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_webform';
  $strongarm->value = '1';
  $export['i18n_node_extended_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_bytbil_bil';
  $strongarm->value = array(
    0 => 'current',
    1 => 'required',
  );
  $export['i18n_node_options_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_page';
  $strongarm->value = array(
    0 => 'current',
    1 => 'required',
  );
  $export['i18n_node_options_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_webform';
  $strongarm->value = array(
    0 => 'current',
    1 => 'required',
  );
  $export['i18n_node_options_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_sync_node_type_bytbil_bil';
  $strongarm->value = array();
  $export['i18n_sync_node_type_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_sync_node_type_page';
  $strongarm->value = array(
    0 => 'field_sidhuvud',
    1 => 'field_view',
    2 => 'field_sidtyp',
  );
  $export['i18n_sync_node_type_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_sync_node_type_webform';
  $strongarm->value = array(
    0 => 'field_sidhuvud',
    1 => 'field_view',
  );
  $export['i18n_sync_node_type_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_bytbil_bil';
  $strongarm->value = '2';
  $export['language_content_type_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_page';
  $strongarm->value = '2';
  $export['language_content_type_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_webform';
  $strongarm->value = '2';
  $export['language_content_type_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_bytbil_bil';
  $strongarm->value = array();
  $export['menu_options_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_page';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_webform';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_bytbil_bil';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_page';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_webform';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_bytbil_bil';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_page';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_webform';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_bytbil_bil';
  $strongarm->value = '1';
  $export['node_preview_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_page';
  $strongarm->value = '1';
  $export['node_preview_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_webform';
  $strongarm->value = '1';
  $export['node_preview_webform'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_bytbil_bil';
  $strongarm->value = FALSE;
  $export['node_submitted_bytbil_bil'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_page';
  $strongarm->value = 0;
  $export['node_submitted_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_webform';
  $strongarm->value = 0;
  $export['node_submitted_webform'] = $strongarm;

  return $export;
}

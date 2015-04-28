<?php
/**
 * @file
 * bytbil.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function bytbil_taxonomy_default_vocabularies() {
  return array(
    'bytbil_bilmodell' => array(
      'name' => 'Byt Bil - Bilmodell',
      'machine_name' => 'bytbil_bilmodell',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'language' => 'und',
      'i18n_mode' => '1',
      'metatags' => array(),
    ),
    'bytbil_drivhjul' => array(
      'name' => 'Byt Bil - Drivhjul',
      'machine_name' => 'bytbil_drivhjul',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'language' => 'und',
      'i18n_mode' => '1',
      'metatags' => array(),
    ),
    'bytbil_drivmedel' => array(
      'name' => 'Byt Bil - Drivmedel',
      'machine_name' => 'bytbil_drivmedel',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'language' => 'und',
      'i18n_mode' => '1',
      'metatags' => array(),
    ),
    'bytbil_karosseri' => array(
      'name' => 'Byt Bil - Karosseri',
      'machine_name' => 'bytbil_karosseri',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'language' => 'und',
      'i18n_mode' => '1',
      'metatags' => array(),
    ),
    'bytbil_vaxellada' => array(
      'name' => 'Byt Bil - Växellåda',
      'machine_name' => 'bytbil_vaxellada',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'language' => 'und',
      'i18n_mode' => '1',
      'metatags' => array(),
    ),
    'sidtyper' => array(
      'name' => 'Sidtyper',
      'machine_name' => 'sidtyper',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'language' => 'und',
      'i18n_mode' => '0',
      'metatags' => array(),
    ),
  );
}

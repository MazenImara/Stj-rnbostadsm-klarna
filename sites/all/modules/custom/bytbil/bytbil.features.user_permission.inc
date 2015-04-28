<?php
/**
 * @file
 * bytbil.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function bytbil_user_default_permissions() {
  $permissions = array();

  // Exported permission: create bytbil_bil content.
  $permissions['create bytbil_bil content'] = array(
    'name' => 'create bytbil_bil content',
    'roles' => array(
      0 => 'administratör',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any bytbil_bil content.
  $permissions['delete any bytbil_bil content'] = array(
    'name' => 'delete any bytbil_bil content',
    'roles' => array(
      0 => 'administratör',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own bytbil_bil content.
  $permissions['delete own bytbil_bil content'] = array(
    'name' => 'delete own bytbil_bil content',
    'roles' => array(
      0 => 'administratör',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any bytbil_bil content.
  $permissions['edit any bytbil_bil content'] = array(
    'name' => 'edit any bytbil_bil content',
    'roles' => array(
      0 => 'administratör',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own bytbil_bil content.
  $permissions['edit own bytbil_bil content'] = array(
    'name' => 'edit own bytbil_bil content',
    'roles' => array(
      0 => 'administratör',
    ),
    'module' => 'node',
  );

  return $permissions;
}

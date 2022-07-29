<?php
/**
 * @file
 * dedikam_v2.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function dedikam_v2_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create slider content'.
  $permissions['create slider content'] = array(
    'name' => 'create slider content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any slider content'.
  $permissions['delete any slider content'] = array(
    'name' => 'delete any slider content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own slider content'.
  $permissions['delete own slider content'] = array(
    'name' => 'delete own slider content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any slider content'.
  $permissions['edit any slider content'] = array(
    'name' => 'edit any slider content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own slider content'.
  $permissions['edit own slider content'] = array(
    'name' => 'edit own slider content',
    'roles' => array(),
    'module' => 'node',
  );

  return $permissions;
}

<?php
/**
 * @file
 * dedikam_v2.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function dedikam_v2_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['views-slider-block'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'slider-block',
    'module' => 'views',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'bartik' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'bartik',
        'weight' => 0,
      ),
      'corporateclean' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'corporateclean',
        'weight' => 0,
      ),
      'garland' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'garland',
        'weight' => 0,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  return $export;
}

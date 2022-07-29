<?php
/**
 * @file
 * dedikam_v2.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dedikam_v2_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function dedikam_v2_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function dedikam_v2_image_default_styles() {
  $styles = array();

  // Exported image style: 1920x280.
  $styles['1920x280'] = array(
    'label' => '1920x280',
    'effects' => array(
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 1920,
          'height' => 280,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function dedikam_v2_node_info() {
  $items = array(
    'slider' => array(
      'name' => t('Slider'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

<?php

/**
 * @file
 * Custom price administration menu callbacks.
 */


/**
 * Configures custom price settings.
 *
 * @see uc_custom_price_admin_settings_validate()
 * @see uc_custom_price_admin_settings_submit()
 * @ingroup forms
 */
function uc_custom_price_admin_settings($form, &$form_state) {
  $form = array();

  $form['custom_price_fieldset'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Custom Price Calculation'),
    '#access'      => user_access('administer custom price code'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );

  // Each product type has its own default calculation
  foreach ($types = uc_product_types() as $type) {
    $form['custom_price_fieldset']['uc_custom_price_' . $type . '_default_code'] = array(
      '#type'          => 'textarea',
      '#title'         => t('Custom code for type @type', array('@type' => $type)),
      '#access'        => user_access('administer custom price code'),
      '#description'   => t('Enter PHP code to be used for dynamic price calculation.'),
      '#default_value' => variable_get('uc_custom_price_' . $type . '_default_code', '$item->price = $item->price;'),
      '#wysiwyg'       => FALSE,
    );
  }

  return system_settings_form($form);
}

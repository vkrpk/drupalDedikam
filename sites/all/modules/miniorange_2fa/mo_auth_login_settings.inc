<?php
/**
 * @file
 * Contains support form for miniOrange 2FA Login Module.
 */

 /**
 * Showing Support form info.
 */
 function mo_auth_login_settings($form, &$form_state) {
  if (! MoAuthUtilities::isCustomerRegistered()) {
    $form['header'] = array(
      '#markup' => '<center><h3>You need to register with miniOrange before using this module.</h3></center>',
    );
    return $form;
  }
  $loginSettings = new MoAuthLoginSettings();
  $form['mo_auth_enable_two_factor'] = array(
  	'#type' => 'checkbox',
  	'#value' => $loginSettings->getEnable2Factor(),
  	'#prefix' => '<div><div class="mo2f-setup-header"><div class="mo2f-setup-header-top-left">Enable Two-Factor module</div></div><div class="mo2f-info">',
  	'#suffix' => '</div></div>',
  	'#title' => t('Enable Two-Factor plugin. <span style="color:red;">(If you disable this checkbox, Two-Factor module will not be invoked for any user during login.)</span>'),
  );
  $is_free = variable_get('mo_auth_2fa_license_type', 'DEMO') == 'PREMIUM' ? FALSE : TRUE;
  $is_free_class = $is_free ? " mo2f-grayed-out" : "";
  $form['mo_auth_enforce_inline_registration'] = array(
  	'#type' => 'checkbox',
  	'#value' => $loginSettings->getEnforceInlineRegistration(),
  	'#prefix' => '<div><div class="mo2f-setup-header"><div class="mo2f-setup-header-top-left">Invoke Inline Registration to setup 2nd factor for users</div><div style="float:right;display:inline-block;color:#0073aa;"><a href="?q=admin/config/people/mo_auth/licensing"><b>PREMIUM*</b></a></div></div><div class="mo2f-info ' . $is_free_class . '">',
  	'#suffix' => '</div></div>',
  	'#disabled' => $is_free,
  	'#title' => t('Enforce 2 Factor registration for users at login time.'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );
  return $form;
 }

 /**
  * Send support query.
  */
 function mo_auth_login_settings_submit($form, &$form_state) {
   $form_state['rebuild'] = TRUE;
   $enable2Factor = FALSE;
   $enforceInlineRegistration = FALSE;
   if (array_key_exists('mo_auth_enable_two_factor', $form_state['input'])) {
     $enable2Factor = TRUE;
   }
   if (array_key_exists('mo_auth_enforce_inline_registration', $form_state['input'])) {
   	 $enforceInlineRegistration = TRUE;
   }
   $loginSettings = MoAuthLoginSettings::withValues($enable2Factor, $enforceInlineRegistration);
   $loginSettings->updateLoginSettings();
   drupal_set_message(t("Login settings updated."), 'status');
 }
<?php
/**
 * @file
 * Contains Setup Two-Factor page for miniOrange 2FA Login Module.
 */

/**
 * Showing Setup Two-Factor page.
 */
function mo_auth_setup_twofactor($form, &$form_state) {
	
  if (! MoAuthUtilities::isCustomerRegistered()) {
	$form['header'] = array(
	  '#markup' => '<center><h3>You need to register with miniOrange before using this module.</h3></center>'
	);
	return $form;
  }
  global $user, $base_url;
  $user = user_load($user->uid);
  
  $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  $customer = new MiniorangeCustomerProfile();
  $user_api_handler = new UsersAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
  $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $user_email, '', '', '');
  $response = $user_api_handler->get($miniorange_user);
  $users_active_method = AuthenticationType::getAuthType($response->authType);
  $configured_methods = mo_auth_get_configured_methods($user->uid);
  
  
  $form['header_top']['#markup'] = '<div class="mo2f-setup-header">
      <span class="mo2f-setup-header-top-left">Setup Two-Factor</span>
      <span class="mo2f-setup-header-top-center">Active Method - <b>' . drupal_strtoupper($users_active_method['name']) . '</b></span>
      <span class="mo2f-setup-header-top-right">' . l(t('Need Support?'), 'admin/config/people/mo_auth/support') . '</span>
    </div>
    <div style="width:80%;display:inline-block"><div class="mo2f-info" style="padding:15px 0px;">Select any Two-Factor of your choice below and complete its setup.</div></div><div style="width:20%;display:inline-block;text-align: right;"><input type="submit" value="Save" class="form-submit"/></div>';
  
  $form['header_methods']['#markup'] = '<div class="mo2f-setup-methods-info-wrap">
        <div class="mo2f-setup-methods-info-left"><span class="mo2f-color-icon mo2f-active-method"></span>- Active Method</div>
        <div class="mo2f-setup-methods-info-center"><span class="mo2f-color-icon mo2f-configured-method"></span>- Configured Method</div>
        <div class="mo2f-setup-methods-info-right"><span class="mo2f-color-icon mo2f-unconfigured-method"></span>- Unconfigured Method</div>
      </div>';
  
  $emailVerificationOption = mo_auth_create_auth_type($user, AuthenticationType::$EMAIL_VERIFICATION, $configured_methods, $users_active_method['code'], FALSE, $base_url);
  $kbaAuth = mo_auth_create_auth_type($user, AuthenticationType::$KBA, $configured_methods, $users_active_method['code'], TRUE, $base_url);
  $googleAuthenticatorOption = mo_auth_create_auth_type($user, AuthenticationType::$GOOGLE_AUTHENTICATOR, $configured_methods, $users_active_method['code'], TRUE, $base_url);
  $qrCodeAuthenticatorOption = mo_auth_create_auth_type($user, AuthenticationType::$QR_CODE, $configured_methods, $users_active_method['code'], TRUE, $base_url);
  $softTokenOption = mo_auth_create_auth_type($user, AuthenticationType::$SOFT_TOKEN, $configured_methods, $users_active_method['code'], TRUE, $base_url);
  $pushNotificationsOption = mo_auth_create_auth_type($user, AuthenticationType::$PUSH_NOTIFICATIONS, $configured_methods, $users_active_method['code'], TRUE, $base_url);
  $OTPOverSMSOption = mo_auth_create_auth_type($user, AuthenticationType::$SMS, $configured_methods, $users_active_method['code'], TRUE, $base_url);
  $OTPOverSMSandEMAILOption = mo_auth_create_auth_type($user, AuthenticationType::$SMS_AND_EMAIL, $configured_methods, $users_active_method['code'], TRUE, $base_url);
  
  $options = array(
    AuthenticationType::$EMAIL_VERIFICATION['code'] => $emailVerificationOption,
    AuthenticationType::$GOOGLE_AUTHENTICATOR['code'] =>  $googleAuthenticatorOption,
    AuthenticationType::$QR_CODE['code'] => $qrCodeAuthenticatorOption,
    AuthenticationType::$KBA['code'] => $kbaAuth,
	AuthenticationType::$SOFT_TOKEN['code'] => $softTokenOption,
	AuthenticationType::$PUSH_NOTIFICATIONS['code'] => $pushNotificationsOption,
	AuthenticationType::$SMS['code'] => $OTPOverSMSOption,
	AuthenticationType::$SMS_AND_EMAIL['code'] => $OTPOverSMSandEMAILOption
  );
  
  $form['mo_auth_method'] = array(
    '#type' => 'radios',
    '#default_value' => $users_active_method['code'],
    '#options' => $options,
    '#required' => TRUE,
  );
  
  $path = $base_url . '/?q=admin/config/people/mo_auth/setup/user/' . $user->uid . '/configure/';
  $form['#attached']['js'][] = array(
    'data' => array(
      'mo_settings' => array(
      'path' => $path,
      ),
    ),
    'type' => 'setting',
  );
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit_bottom'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('style' => 'display:inline-block;'),
  );
  return $form;
}

function mo_auth_setup_twofactor_submit($form, &$form_state) {
	global $base_url, $user;
  $form_state['rebuild'] = TRUE;
  $authType = $form_state['input']['mo_auth_method'];
  $configured_methods = mo_auth_get_configured_methods($user->uid);
  if (in_array($authType, $configured_methods)) {
    $user = user_load($user->uid);
    $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  
    $customer = new MiniorangeCustomerProfile();
    $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $user_email, NULL, NULL, $authType);
    $user_api_handler = new UsersAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    $response = $user_api_handler->update($miniorange_user);
    if ($response->status == 'SUCCESS') {
      drupal_set_message(t('Authentication method updated successfully.'), 'status');
      drupal_goto('admin/config/people/mo_auth/setup');
    } else {
      drupal_set_message(t('An error occured while updating the authentication type. Please try again.'), 'error');
      drupal_goto('admin/config/people/mo_auth/setup');
    }
  } else {
  	drupal_set_message(t('Please configure this authentication method first to enable it.'), 'error');
  	drupal_goto('admin/config/people/mo_auth/setup');
  }
}

function mo_auth_create_auth_type($user, $authType, $configuredMethods, $active_method, $reconfigure_allowed, $base_url) {
  $label_title = 'Supported in ' . implode(', ', $authType['supported-for']);;
  $supported_for_icon_class = '';
  if (in_array('Landline', $authType['supported-for'])) {
    $supported_for_icon_class = 'mo2f-landline';
  } elseif (in_array('Feature Phones', $authType['supported-for'])) {
    $supported_for_icon_class = 'mo2f-smartphone-feature-phone';
  } elseif (in_array('Laptops', $authType['supported-for'])) {
    $supported_for_icon_class = 'mo2f-laptop';
  } elseif (in_array('Smartphones', $authType['supported-for'])) {
    $supported_for_icon_class = 'mo2f-smartphone';
  }
  
  $config_type_class = 'mo2f-unconfigured-method';
  if ($authType['code'] == $active_method) {
    $config_type_class = 'mo2f-active-method';
  } elseif (in_array($authType['code'], $configuredMethods)) {
    $config_type_class = 'mo2f-configured-method';
  }
  
  $configured = 'false';
  $classes = $supported_for_icon_class . ' ' . $config_type_class;
  $test = '<a href="' . $base_url . '/?q=admin/config/people/mo_auth/setup/user/' . $user->uid . '/configure/' . $authType['id'] . '">Configure</a>';
  if (! empty($configuredMethods) && in_array($authType['code'], $configuredMethods)) {
    $test = '<a href="' . $base_url . '/?q=admin/config/people/mo_auth/setup/user/' . $user->uid . '/test/' . $authType['id'] .'">Test</a>';
    if ($reconfigure_allowed === TRUE) {
      $test .= ' | <a href="' . $base_url . '/?q=admin/config/people/mo_auth/setup/user/' . $user->uid . '/configure/' . $authType['id'] .'">Re-configure</a>';
    }
    $configured = 'true';
  }
  $html = '<div>
        <div class="mo2f-method" data-id="' . $authType['id'] . '" data-configured="'. $configured .'">' . $authType['name'] . '</div>
        <p>' . $authType['description'] . '</p>
        <div class="' . $classes . '" title="' . $label_title . '">'
          . $test . '</div>
      </div>';
  
  return $html;
}

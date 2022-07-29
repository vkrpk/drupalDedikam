<?php

/**
 * @file
 * Email verification functions.
 */

/**
 * Menu callback for email verification.
 */
function mo_auth_test_google_authenticator($form, &$form_state) {
  global $base_url, $user;
  $user = user_load($user->uid);
  $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  
  /* To check which method (Soft Token, Google Authenticator') is being tested by user
   * $authTypeCode:- Code of the authentication Type
   * $messageHeader:- Info related to auth Type in the page
   */
  $query_param = $_GET["q"];
  $url_parts = explode('/',$query_param);
  
  $authTypeCode = '';
  $messageHeader = '';
  if (in_array("soft-token",$url_parts)) {
	$authTypeCode = AuthenticationType::$SOFT_TOKEN['code'];
	$messageHeader = '<div class="mo2f-setup-header-top-left">Test Soft Token</div></div><div class="mo2f-info">Please enter the 6 digit passcode generated on your miniOrange Authenticator app.</div>';
  } else {
	$authTypeCode = AuthenticationType::$GOOGLE_AUTHENTICATOR['code'];
    $messageHeader = '<div class="mo2f-setup-header-top-left">Test Google Authenticator</div></div><div class="mo2f-info">Please enter the 6 digit passcode generated on your Google Authenticator app.</div>';	
  }

  $form['header']['#markup'] = t('<div class="mo2f-setup-header">' . $messageHeader);

  $form['mo_auth_googleauth_token'] = array (
    '#type' => 'textfield',
    '#attributes' => array (
      'placeholder' => t('Enter 6 digit passcode.'),
      'class' => array (
        'mo2f-textbox',
        'mo2f-textbox-otp'
      ),
      'style' => 'width:200px',
      'autofocus' => 'true'
    )
  );
  
  $form['authTypeCode'] = array (
      '#type' => 'hidden',
      '#value' => $authTypeCode
    );

  $form['actions'] = array (
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array (
    '#type' => 'submit',
    '#value' => t('Verify')
  );
  $form['actions']['cancel'] = array (
    '#markup' => l(t('Cancel Test'), 'admin/config/people/mo_auth/setup'),
    '#suffix' => '</div>'
  );
  return $form;
}

/**
 * Form submit handler for email verify.
 */
function mo_auth_test_google_authenticator_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  global $base_url, $user;
  $user = user_load($user->uid);
  $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  $token = $form_state['input']['mo_auth_googleauth_token'];
  $authTypeCode = $form_state['input']['authTypeCode'];
  
  $customer = new MiniorangeCustomerProfile();
  $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $user_email, NULL, NULL, $authTypeCode);
  $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
  $response = $auth_api_handler->validate($miniorange_user, '', $token);

  // Clear all the messages
  drupal_get_messages();

  // read API response
  if ($response->status == 'SUCCESS') {
    drupal_set_message(t('You have successfully completed the test.'), 'status');
    drupal_goto('admin/config/people/mo_auth/setup');
  } elseif ($response->status == 'FAILED') {
    form_set_error('form', t('The passcode you have entered is incorrect. Please try again.'));
    return;
  } else {
    form_set_error('form', t('An error occured while processing your request. Please try again.'));
    drupal_goto('admin/config/people/mo_auth/setup');
  }
}

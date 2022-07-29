	<?php

/**
 * @file
 * OTP Over SMS(test) functions.
 */

/**
 * Menu callback for testing OTP Over SMS.
 */
function mo_auth_test_otp_over_sms($form, &$form_state) {
  $form['actions'] = array('#type' => 'actions');
  if (array_key_exists('txId', $form_state['input']) === FALSE) {
    global $base_url, $user;
    $user = user_load($user->uid);
    $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
	
    $authTypeCode = '';
    $divMessage = '';
	$messageHeader = '';
    $user_phone = variable_get('mo_phone');
	
	$authTypeCode = AuthenticationType::$SMS['code'];
	$divMessage = '<div class="mo2f-setup-header"><div class="mo2f-setup-header-top-left">Test OTP Over SMS</div></div>';
    $messageHeader = 'An OTP has been sent to ' . $user_phone . '. Please enter it here to complete the test.';
	
    $customer = new MiniorangeCustomerProfile();
    $miniorange_user = new MiniorangeUser($customer->getCustomerID(), NULL , $user_phone, $authTypeCode, NULL, $user_email);
    $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    $response = $auth_api_handler->challenge($miniorange_user);
   
    if ($response->status == 'SUCCESS') {
        drupal_set_message(t($messageHeader), 'status');
     
	 $form['header']['#markup'] = t($divMessage);
		
	  $form['#attached']['js'][] = array(
        'data' => array(
          'mo_authentication' => array(
            'txId' => $response->txId,
            'url' => MoAuthConstants::$AUTH_STATUS_API,
          ),
        ),
        'type' => 'setting',
      );
		
		$form['mo_auth_otpoversms_token'] = array (
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
    
	   $form['actions'] = array (
		'#type' => 'actions'
	  );
	  
	  $form['actions']['submit'] = array (
		'#type' => 'submit',
		'#value' => t('Verify')
	  );
	  
	  $form['txId'] = array(
        '#type' => 'hidden',
        '#value' => $response->txId,
      );
	  
	  $form['actions']['cancel'] = array (
		'#markup' => l(t('Cancel Test'), 'admin/config/people/mo_auth/setup'),
		'#suffix' => '</div>'
	  );
	}
	return $form;
 }
 $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Test'),
    '#attributes' => array('class' => array('element-invisible')),
  );
  return $form;

}

/**
 * Form submit handler for otp over sms validation.
 */
function mo_auth_test_otp_over_sms_submit($form, &$form_state) {
	
  $form_state['rebuild'] = TRUE;
  global $base_url, $user;
  
  $txId = $form_state['input']['txId'];;
  
  $user = user_load($user->uid);
  $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  $token = $form_state['input']['mo_auth_otpoversms_token'];
  $authTypeCode = AuthenticationType::$SMS['code'];
  $user_phone = variable_get('mo_phone');
  
  $customer = new MiniorangeCustomerProfile();
  $miniorange_user = new MiniorangeUser($customer->getCustomerID(), NULL, $user_phone, $authTypeCode, $user_email);
  $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
  $response = $auth_api_handler->validate($miniorange_user, $txId, $token);
  
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
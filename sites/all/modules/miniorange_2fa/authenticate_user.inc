<?php
/**
 * @file
 *  This is used to authenticate user during login.
 */
function mo_auth_authenticate_user($form, &$form_state, $user) {
  global $base_url;
  if (user_is_logged_in()) {
    drupal_goto('user');
    exit;
  }
  $form_state['uid'] = $user->uid;
  $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  if (isset($_SESSION['mo_auth']['status']) && $_SESSION['mo_auth']['status'] === '1ST_FACTOR_AUTHENTICATED') {
    $challenge_response = $_SESSION['mo_auth']['mo_challenge_response'];
    $form['actions'] = array('#type' => 'actions');
    if (! empty($challenge_response->authType)) {
      $form['authType'] = array(
        '#type' => 'hidden',
        '#value' => $challenge_response->authType,
      );
      $authType = AuthenticationType::getAuthType($challenge_response->authType);
      $form = mo_auth_build_form($form, $base_url, $authType, $challenge_response);
      unset($form['mo_message']);
    } else {
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('element-invisible')),
      );
    }
    return $form;
  }
  drupal_goto('user');
  exit;
}

function mo_auth_authenticate_user_validate($form, &$form_state) {
  // $user_api_response = $_SESSION['mo_auth']['user'];
  $challenge_response = $_SESSION['mo_auth']['mo_challenge_response'];
  if ($challenge_response->authType == AuthenticationType::$GOOGLE_AUTHENTICATOR['code'] || $challenge_response->authType == AuthenticationType::$SOFT_TOKEN['code']) {
  	$token = $form_state['input']['token'];
  	if (empty($token) || strlen($token) != 6 || ! preg_match('/[0-9]/', $token)) {
  	  $new_form = mo_auth_build_form_with_error_message($form_state);
  	  form_set_error('form', t($new_form['mo_message']));
  	  unset($new_form['mo_message']);
  	}
  }
  elseif($challenge_response->authType == AuthenticationType::$SMS['code'] ||
  		 $challenge_response->authType == AuthenticationType::$SMS_AND_EMAIL['code']){
	  $token = $form_state['input']['token'];
	  
	  if (empty($token) || ! preg_match('/[0-9]/', $token)) {
		  $new_form = mo_auth_build_form_with_error_message($form_state);
		  form_set_error('form', t($new_form['mo_message']));
		  unset($new_form['mo_message']);
  	}
  }
  elseif ($challenge_response->authType == AuthenticationType::$KBA['code']) {
  	$i=0;
  	foreach ($challenge_response->questions as $question){
  	  $i++;
  	  $ques = $form_state['input']['mo2f_kbaquestion' . $i];
  	  if ($ques != $question->question) {
  	  	form_set_error('form', t('An error occured while processing your request.'));
  	  	drupal_goto("user");
  	  	break;
  	  }
  	}
  }
}

function mo_auth_authenticate_user_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $uid = $form_state['uid'];
  $user = user_load($uid);
  $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  // $user_api_response = $_SESSION['mo_auth']['user'];
  $challenge_response = $_SESSION['mo_auth']['mo_challenge_response'];
  $authType = AuthenticationType::getAuthType($challenge_response->authType);
  if ($authType['oob'] === FALSE) {
  	$token = '';
  	if (array_key_exists('token', $form_state['input']))
  	  $token = $form_state['input']['token'];
    $txId = '';
    $kba = array();
    if ($authType['challenge'] === TRUE) {
      $txId = $challenge_response->txId;
      if ($challenge_response->authType == AuthenticationType::$KBA['code']) {
      	$count = count($challenge_response->questions);
      	for ($i = 1; $i<=$count; $i++) {
      	  $ques = $form_state['input']['mo2f_kbaquestion' . $i];
      	  $ans = $form_state['input']['mo2f_kbaanswer' . $i];
      	  $qa = array(
      	    "question" => $ques,
      	    "answer" => $ans,
      	  );
      	  array_push($kba, $qa);
      	}
      }
    }

    $customer = new MiniorangeCustomerProfile();
    $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $user_email, NULL, NULL, NULL);
    $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    $response = $auth_api_handler->validate($miniorange_user, $txId, $token, $kba);
  } else {
    $txId = $form_state['input']['txId'];
    $customer = new MiniorangeCustomerProfile();
    $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    $response = $auth_api_handler->getAuthStatus($txId);
  }
  
  // read API response
  if ($response->status == 'SUCCESS') {
    user_login_submit(array(), $form_state);
    unset($_SESSION['mo_auth']);
  } elseif ($response->status == 'DENIED') {
    form_set_error('form', t('Authentication denied.'));
    unset($_SESSION['mo_auth']);
    drupal_goto('user');
  } elseif ($response->status == 'FAILED') {
    $new_form = mo_auth_build_form_with_error_message($form_state);
    form_set_error('form', t($new_form['mo_message']));
    unset($new_form['mo_message']);
  } else {
    form_set_error('form', t('An error occured while processing your request. Please try again.'));
    unset($_SESSION['mo_auth']);
    drupal_goto('user');
  }
}

function mo_auth_build_form($form, $base_url, $authType, $challenge_response, $success_form = TRUE) {
  $form['main-header']['#markup'] = t('<style>#messages div.messages{visibility:hidden;}</style>');
  $form['header']['#markup'] = t('<div class="mo2f-modal">
              <div class="mo2f-modal-content">
                <div class="mo2f-modal-container mo2f-modal-header">Verify your identity</div>
                <div class="mo2f-modal-container">');

  $submit_attributes = array();

  $form = mo_auth_build_form_content($form, $base_url, $authType, $challenge_response, $success_form);
  if(isset($form['submit_attributes'])) {
  $submit_attributes = $form['submit_attributes']; }
  unset($form['submit_attributes']);
  $form['loader']['#markup'] = '</div><div class="mo2f-modal-container mo2f-modal-footer">';
  $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Verify'),
      '#attributes' => $submit_attributes,
  );
  $form['actions']['cancel'] = array(
      '#markup' => l(t('Back to Login'), 'user'),
      '#suffix' => '</div></div></div>',
  );
  return $form;
}

function mo_auth_build_form_content($form, $base_url, $authType, $challenge_response, $success_form = TRUE) {

  switch($authType['code']) {
    case AuthenticationType::$EMAIL_VERIFICATION['code']:
      return mo_auth_build_oobemail_form($form, $base_url, $challenge_response);
    case AuthenticationType::$GOOGLE_AUTHENTICATOR['code']:
      return mo_auth_build_google_authenticator_form($form, $base_url, $challenge_response, $success_form);
    case AuthenticationType::$QR_CODE['code']:
      return mo_auth_build_qrcode_authentication_form($form, $base_url, $challenge_response);
    case AuthenticationType::$KBA['code']:
      return mo_auth_build_kba_authentication_form($form, $base_url, $challenge_response, $success_form);
	case AuthenticationType::$SOFT_TOKEN['code']:
	  return mo_auth_build_soft_token_form($form, $base_url, $challenge_response, $success_form);
	case AuthenticationType::$PUSH_NOTIFICATIONS['code']:
      return mo_auth_build_push_notifications_form($form, $base_url, $challenge_response);
    case AuthenticationType::$SMS['code']:
      return mo_auth_build_otp_over_sms_form($form, $base_url, $challenge_response, $success_form);
	case AuthenticationType::$SMS_AND_EMAIL['code']:
      return mo_auth_build_otp_over_sms_and_email_form($form, $base_url, $challenge_response, $success_form);
	default:
      return $form;
  }
}

function mo_auth_build_form_with_error_message(&$form_state) {
  global $base_url;
  $challenge_response = $_SESSION['mo_auth']['mo_challenge_response'];
  $authType = AuthenticationType::getAuthType($challenge_response->authType);

  $form = array();
  $form = mo_auth_build_form($form, $base_url, $authType, $challenge_response, FALSE);

  $form_state['complete form']['header']['#markup'] = $form['header']['#markup'];

  return $form;
}

function mo_auth_build_google_authenticator_form($form, $base_url, $challenge_response, $success_message = TRUE) {
  if ($success_message === TRUE)
	$message = t('Please enter the 6 digit passcode generated on your Google Authenticator app.');
  else
  	$message = t('The passcode you have entered is incorrect. Please enter the valid 6 digit passcode.');

  $message_div_class = $success_message === TRUE ? 'mo2f-message-status' : 'mo2f-message-error';
  $form['header']['#markup'] .= t('<div class="mo2f-message ' . $message_div_class . '">' . $message . '</div>');
  $form['header']['#markup'] .= t('<div class="mo2f-info">Enter the passcode:</div>');
  $form['token'] = array(
    '#type' => 'textfield',
    '#attributes' => array('placeholder' => t('Enter 6 digit passcode.'), 'class' => array('mo2f-textbox', 'mo2f-textbox-otp'), 'autofocus' => 'true'),
  );
  $submit_attributes = array();
  $form['submit_attributes'] = $submit_attributes;
  $form['mo_message'] = $message;
  return $form;
}

function mo_auth_build_qrcode_authentication_form($form, $base_url, $challenge_response) {
  $message = t('Please scan the below QR Code from miniOrange Authenticator app.');
  $form['#attached']['js'][] = array(
    'data' => array(
      'mo_authentication' => array(
        'txId' => $challenge_response->txId,
        'url' => MoAuthConstants::$AUTH_STATUS_API,
      ),
    ),
    'type' => 'setting',
  );

  $form['header']['#markup'] .= t('<div class="mo2f-message mo2f-message-status">' . $message . '</div>');

  $form['header']['#markup'] .= t('<div class="mo2f-info mo2f-text-center">Please scan the below QR Code with miniOrange Authenticator app to authenticate yourself.</div>');
  $form['header']['#markup'] .= '<div class="mo2f-text-center"><img src="data:image/jpg;base64,' . $challenge_response->qrCode . '"></div>';

  $form['txId'] = array(
    '#type' => 'hidden',
    '#value' => $challenge_response->txId,
  );
  $submit_attributes = array('class' => array('element-invisible'));
  $form['submit_attributes'] = $submit_attributes;
  $form['mo_message'] = $message;
  return $form;
}

function mo_auth_build_oobemail_form($form, $base_url, $challenge_response) {
  $response = $_SESSION['mo_auth']['mo_challenge_response'];
  $user_email = $challenge_response->emailDelivery->contact;
  $hidden_email = MoAuthUtilities::getHiddenEmail($user_email);
  $message = t('A verification email is sent to <b>' . $hidden_email . '</b>. Please click on accept link to verify your email.');
  $form['#attached']['js'][] = array(
    'data' => array(
      'mo_authentication' => array(
        'txId' => $response->txId,
        'url' => MoAuthConstants::$AUTH_STATUS_API,
      ),
    ),
    'type' => 'setting',
  );

  $form['header']['#markup'] .= t('<div class="mo2f-message mo2f-message-status">' . $message . '</div>');

  $form['header']['#markup'] .= t('<div class="mo2f-info mo2f-text-center">A verification email is sent to your registered email.</div>
                  <div class="mo2f-info mo2f-text-center">We are waiting for your approval...</div>');
  $image_path = file_create_url($base_url . '/' . drupal_get_path('module', 'mo_auth') . '/includes/images/ajax-loader-login.gif');
  $form['header']['#markup'] .= '<div class="mo2f-text-center"><img src="' . $image_path . '"></div>';

  $form['txId'] = array(
    '#type' => 'hidden',
    '#value' => $response->txId,
  );
  $submit_attributes = array('class' => array('element-invisible'));
  $form['submit_attributes'] = $submit_attributes;
  $form['mo_message'] = $message;
  return $form;
}

function mo_auth_build_kba_authentication_form($form, $base_url, $challenge_response, $success_message = TRUE) {
	if ($success_message === TRUE)
		$message = t('Please answer the following questions.');
	else
		$message = t('The answers you have entered are incorrect.');

	$message_div_class = $success_message === TRUE ? 'mo2f-message-status' : 'mo2f-message-error';
	$form['header']['#markup'] .= t('<div class="mo2f-message ' . $message_div_class . '">' . $message . '</div>');
	$form['header']['#markup'] .= t('<div class="mo2f-info">Please answer the following questions:</div>');
	$form['header']['#markup'] .= '<div class="mo2f-kba-header mo2f-kba-row">
  			<div class="mo2f-kba-srno">No.</div>
  			<div class="mo2f-kba-question">Questions</div>
			<div class="mo2f-kba-answer">Answers</div>
  			</div>';
	$i = 0;
	$questions = $challenge_response->questions;
	foreach ($questions as $ques) {
	  $i++;
	  $form['header']['#markup'] .= '<div class="mo2f-kba-row">
  			<div class="mo2f-kba-srno">' . $i . '.</div>
  			<div class="mo2f-kba-question">'
	  					. $ques->question .
	  					'</div>
			<div class="mo2f-kba-answer"><input class="form-control form-text" type="text" name="mo2f_kbaanswer' . $i . '" title="Only alphanumeric letters with special characters(_@.$#&amp;+-) are allowed." pattern="(?=\S)[A-Za-z0-9_@.$#&amp;+-\s]{1,100}" required="true" autofocus="true" placeholder="Enter your answer"></div>
  		<input type="hidden" name="mo2f_kbaquestion' . $i .'" value="' . $ques->question . '"/></div>';
	}
	$form['txId'] = array(
	  '#type' => 'hidden',
	  '#value' => $challenge_response->txId,
	);
	$submit_attributes = array();
	$form['submit_attributes'] = $submit_attributes;
	$form['mo_message'] = $message;
	return $form;
}

function mo_auth_build_soft_token_form($form, $base_url, $challenge_response, $success_message = TRUE) {
  if ($success_message === TRUE)
	$message = t('Please enter the 6 digit passcode generated on your miniOrange Authenticator app.');
  else
  	$message = t('The passcode you have entered is incorrect. Please enter the valid 6 digit passcode.');

  $message_div_class = $success_message === TRUE ? 'mo2f-message-status' : 'mo2f-message-error';
  $form['header']['#markup'] .= t('<div class="mo2f-message ' . $message_div_class . '">' . $message . '</div>');
  $form['header']['#markup'] .= t('<div class="mo2f-info">Enter the passcode:</div>');
  $form['token'] = array(
    '#type' => 'textfield',
    '#attributes' => array('placeholder' => t('Enter 6 digit passcode.'), 'class' => array('mo2f-textbox', 'mo2f-textbox-otp'), 'autofocus' => 'true'),
  );
  $submit_attributes = array();
  $form['submit_attributes'] = $submit_attributes;
  $form['mo_message'] = $message;
  return $form;
}

function mo_auth_build_push_notifications_form($form, $base_url, $challenge_response) {
  $response = $_SESSION['mo_auth']['mo_challenge_response'];
  //$user_email = $challenge_response->emailDelivery->contact;
  //$hidden_email = MoAuthUtilities::getHiddenEmail($user_email);
  $message = t('Please accept the push notification sent to your miniOrange Authenticator App.');
  $form['#attached']['js'][] = array(
    'data' => array(
      'mo_authentication' => array(
        'txId' => $response->txId,
        'url' => MoAuthConstants::$AUTH_STATUS_API,
      ),
    ),
    'type' => 'setting',
  );

  $form['header']['#markup'] .= t('<div class="mo2f-message mo2f-message-status">' . $message . '</div>');

  $form['header']['#markup'] .= t('<div class="mo2f-info mo2f-text-center">A Push Notification has been sent to your miniOrange Authenticator App.</div>
                  <div class="mo2f-info mo2f-text-center">We are waiting for your approval...</div>');
  $image_path = file_create_url($base_url . '/' . drupal_get_path('module', 'mo_auth') . '/includes/images/ajax-loader-login.gif');
  $form['header']['#markup'] .= '<div class="mo2f-text-center"><img src="' . $image_path . '"></div>';

  $form['txId'] = array(
    '#type' => 'hidden',
    '#value' => $response->txId,
  );
  $submit_attributes = array('class' => array('element-invisible'));
  $form['submit_attributes'] = $submit_attributes;
  $form['mo_message'] = $message;
  return $form;
}

function mo_auth_build_otp_over_sms_form($form, $base_url, $challenge_response, $success_message = TRUE) {
   
	if ($success_message === TRUE)
		$message = t('Please enter the 6 digit passcode sent to your mobile phone.');
	else
		$message = t('The passcode you have entered is incorrect. Please enter the valid 6 digit passcode.');
	
	$message_div_class = $success_message === TRUE ? 'mo2f-message-status' : 'mo2f-message-error';
    $form['header']['#markup'] .= t('<div class="mo2f-message ' . $message_div_class . '">' . $message . '</div>');
	$form['header']['#markup'] .= t('<div class="mo2f-info">Enter the passcode:</div>');
	
	$form['token'] = array (
      '#type' => 'textfield',
      '#title' => t('Please enter the OTP you received:'),
      '#attributes' => array('autofocus' => 'true'),
    );
	
	$submit_attributes = array();
	$form['submit_attributes'] = $submit_attributes;
	$form['mo_message'] = $message;
	return $form;
	
}

function mo_auth_build_otp_over_sms_and_email_form($form, $base_url, $challenge_response, $success_message = TRUE) {
   
	if ($success_message === TRUE)
		$message = t('Please enter the 6 digit passcode sent to your mobile phone and email.');
	else
		$message = t('The passcode you have entered is incorrect. Please enter the valid 6 digit passcode.');
	
	$message_div_class = $success_message === TRUE ? 'mo2f-message-status' : 'mo2f-message-error';
    $form['header']['#markup'] .= t('<div class="mo2f-message ' . $message_div_class . '">' . $message . '</div>');
	$form['header']['#markup'] .= t('<div class="mo2f-info">Enter the passcode:</div>');
	
	$form['token'] = array (
      '#type' => 'textfield',
      '#title' => t('Please enter the OTP you received:'),
      '#attributes' => array('autofocus' => 'true'),
    );
	
	$submit_attributes = array();
	$form['submit_attributes'] = $submit_attributes;
	$form['mo_message'] = $message;
	return $form;
	
}
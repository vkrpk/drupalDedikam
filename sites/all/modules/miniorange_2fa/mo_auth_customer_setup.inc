<?php
/**
 * @file
 * Contains form for customer setup.
 */

/**
 * Customer setup form().
 */
function mo_auth_customer_setup($form, &$form_state) {
  global $user;
  $user_obj = user_load($user->uid);
  $current_status = variable_get('mo_auth_status', '');
  if ($current_status == 'VALIDATE_OTP') {
    $form['mo_auth_customer_otp_token'] = array (
      '#type' => 'textfield',
      '#title' => t('Please enter the OTP you received:'),
      '#attributes' => array('autofocus' => 'true'),
    );
    $form['mo_auth_customer_validate_otp_button'] = array (
      '#type' => 'submit',
      '#value' => t('Validate OTP'),
      '#submit' => array (
        'mo_auth_validate_otp_submit',
      ),
    );
    $form['mo_auth_customer_setup_resendotp'] = array (
      '#type' => 'submit',
      '#value' => t('Resend OTP'),
      '#submit' => array (
        'mo_auth_resend_otp',
      ),
    );
    $form['mo_auth_customer_setup_back'] = array (
      '#type' => 'submit',
      '#value' => t('Back'),
      '#submit' => array (
        'mo_auth_back'
      )
    );

    return $form;
  } elseif ($current_status == 'PLUGIN_CONFIGURATION') {
    // Show customer configuration here.
    $user_email = $user_obj->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
    $customer = new MiniorangeCustomerProfile();
    $user_api_handler = new UsersAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $user_email, '', '', '');

    $response = $user_api_handler->get($miniorange_user);
    $authType = AuthenticationType::getAuthType($response->authType);
    $form['markup_top'] = array (
      '#markup' => '<div>Thank you for registering with miniOrange</div>' . '<h4>Your Profile: </h4>'
    );

    $header = array (
      'attribute' => array (
        'data' => t('Attribute')
      ),
      'value' => array (
        'data' => t('Value')
      )
    );

    $options = array ();
    $options[0] = array (
      'attribute' => '2 Factor Registered Email',
      'value' => variable_get('mo_auth_customer_admin_email', '')
    );
    $options[1] = array (
      'attribute' => 'Activated 2nd Factor',
      'value' => $authType['name']
    );
    $options[2] = array (
      'attribute' => 'miniOrange Registered Email',
      'value' => $user_email
    );
    $options[3] = array (
      'attribute' => 'Customer ID',
      'value' => variable_get('mo_auth_customer_id', '')
    );
    $options[4] = array (
      'attribute' => 'API Key',
      'value' => variable_get('mo_auth_customer_api_key', '')
    );
    $options[5] = array (
      'attribute' => 'Token Key',
      'value' => variable_get('mo_auth_customer_token_key', '')
    );
    $options[6] = array (
      'attribute' => 'App Secret',
      'value' => variable_get('mo_auth_customer_app_secret', '')
    );

    $form['fieldset']['customerinfo'] = array (
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options
    );

    $license_header = array (
      'attribute' => array (
        'data' => t('Attribute')
      ),
      'value' => array (
        'data' => t('Value')
      )
    );

    $license_options = array ();
    $license_options[0] = array (
      'attribute' => 'License Type',
      'value' => variable_get('mo_auth_2fa_license_type', 'DEMO')
    );
    $license_options[1] = array (
      'attribute' => 'License Plan',
      'value' => variable_get('mo_auth_2fa_license_plan', 'DEMO')
    );
    $license_options[2] = array (
      'attribute' => 'No. of Users',
      'value' => variable_get('mo_auth_2fa_license_no_of_users', 1)
    );

    $form['fieldset']['customer-license'] = array (
      '#theme' => 'table',
      '#header' => $license_header,
      '#rows' => $license_options,
      '#prefix' => '<div style="margin-top: 20px;"><h4>Your license info: </h4></div>'
    );
    $form['fecth_customer_license'] = array (
      '#type' => 'submit',
      '#value' => t('Check License'),
      '#submit' => array (
        'mo_auth_fetch_customer_license'
      )
    );

    return $form;
  }

  $form['markup_14'] = array (
    '#markup' => '<h3>Register with miniOrange</h3>'
  );

  $form['markup_15'] = array (
    '#markup' => 'Just complete the short registration below to configure' . ' the Two-Factor Module. Please enter a valid email ID <br>that you have' . ' access to. You will be able to move forward after verifying an OTP' . ' that we will send to this email.'
  );

  $form['mo_auth_customer_setup_username'] = array (
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
    '#attributes' => array('required' => 'true', 'autofocus' => 'true'),
  );

  $form['mo_auth_customer_setup_phone'] = array (
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#attributes' => array('pattern' => '[\+]?[0-9]{1,4}\s?[0-9]{7,12}'),
  );

  $form['markup_16'] = array (
    '#markup' => '<b>NOTE:</b> We will only call if you need support.'
  );

  $form['mo_auth_customer_setup_password'] = array (
    '#type' => 'password_confirm',
    '#required' => TRUE,
    '#attributes' => array('required' => 'true'),
  );

  $form['mo_auth_customer_setup_button'] = array (
    '#type' => 'submit',
    '#value' => t('Register'),
    '#submit' => array (
      'mo_auth_customer_setup_submit'
    ),
    '#validate' => array(
      'mo_auth_validate_register_form',
    ),
  );

  return $form;
}

/**
 * Validate OTP.
 */
function mo_auth_validate_otp_submit(&$form, $form_state) {
  global $user;
  $user = user_load($user->uid);
  $otp_token = $form['mo_auth_customer_otp_token']['#value'];
  $username = variable_get('mo_auth_customer_admin_email', NULL);
  $phone = variable_get('mo_auth_customer_admin_phone', NULL);
  $txId = variable_get('mo_auth_tx_id', NULL);
  $customerSetup = new MiniorangeCustomerSetup($username, $phone, NULL, $otp_token);

  // Validate OTP.
  $validate_otp_response = json_decode($customerSetup->validate_otp_token($txId, $otp_token, MoAuthConstants::$DEFAULT_CUSTOMER_ID, MoAuthConstants::$DEFAULT_CUSTOMER_API_KEY ));
  

  if ($validate_otp_response->status == 'SUCCESS') {
    // OTP Validated. Show Configuration page.
    $current_status = 'PLUGIN_CONFIGURATION';
    variable_set('mo_auth_status', $current_status);
    variable_del('mo_auth_tx_id');

    // OTP Validated. Create customer.
    $password = variable_get('mo_auth_customer_admin_password', '');
    $customer_config = new MiniorangeCustomerSetup($username, $phone, $password, NULL);
    $create_customer_response = $customer_config->createCustomer();
    if ($create_customer_response->status == 'SUCCESS') {
      // Customer created.
      mo_auth_save_customer($user, $create_customer_response, $username, $phone);

      drupal_set_message(t('Customer account created. Email Verification has been set as your default 2nd factor method.'));
      drupal_goto('admin/config/people/mo_auth/licensing');
    } else {
      drupal_set_message(t('An error occured while creating your account. Please try again or contact us at <a href="mailto:info@miniorange.com">info@miniorange.com</a>.'), 'error');
    }
  } else {
    drupal_set_message(t('The OTP you have entered is incorrect. Please try again.'), 'error');
  }
}

/**
 * Resend OTP.
 */
function mo_auth_resend_otp(&$form, $form_state) {
  variable_del('mo_auth_tx_id');
  $username = variable_get('mo_auth_customer_admin_email', NULL);
  $phone = variable_get('mo_auth_customer_admin_phone', NULL);
  $customer_config = new MiniorangeCustomerSetup($username, $phone, NULL, NULL);
  $send_otp_response = json_decode($customer_config->sendOtp());
  if ($send_otp_response->status == 'SUCCESS') {
    // Store txID.
    variable_set('mo_auth_tx_id', $send_otp_response->txId);
    $current_status = 'VALIDATE_OTP';
    variable_set('mo_auth_status', $current_status);
    drupal_set_message(t('We have sent an OTP to @username. Please enter the OTP to verify your email.', array (
      '@username' => $username
    )));
  }
}

function mo_auth_validate_register_form($form, &$form_state) {
  $username = $form['mo_auth_customer_setup_username']['#value'];
  if(! valid_email_address($username)) {
    form_set_error('invalid_email', 'The email address <i>' . $username . '</i> is not valid.');
  }
}

/**
 * Handle submit for customer setup.
 */
function mo_auth_customer_setup_submit(&$form, $form_state) {
  global $user;
  $user = user_load($user->uid);
  $username = $form['mo_auth_customer_setup_username']['#value'];
  $phone = $form['mo_auth_customer_setup_phone']['#value'];
  $password = $form['mo_auth_customer_setup_password']['#value']['pass1'];
  $customer_config = new MiniorangeCustomerSetup($username, $phone, $password, NULL);
  $check_customer_response = $customer_config->checkCustomer();
  if ($check_customer_response->status == 'CUSTOMER_NOT_FOUND') {
    // Create customer.
    // Store email and phone.
    variable_set('mo_auth_customer_admin_email', $username);
    variable_set('mo_auth_customer_admin_phone', $phone);
    variable_set('mo_auth_customer_admin_password', $password);

    $send_otp_response = $customer_config->sendOtp();
    if ($send_otp_response->status == 'SUCCESS') {
      // Store txID.
      variable_set('mo_auth_tx_id', $send_otp_response->txId);
      $current_status = 'VALIDATE_OTP';
      variable_set('mo_auth_status', $current_status);
      drupal_set_message(t('We have sent an OTP to @username. Please enter the OTP to verify your email.', array (
        '@username' => $username
      )));
    }
  } elseif ($check_customer_response->status == 'CURL_ERROR') {
    drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
  } else {
    // Customer exists. Retrieve keys.
    $customer_keys_response = $customer_config->getCustomerKeys();
    if (json_last_error() == JSON_ERROR_NONE) {
      mo_auth_save_customer($user, $customer_keys_response, $username, $phone);

      drupal_set_message(t('Your account has been retrieved successfully. Email Verification has been set as your default 2nd factor method.'));
      drupal_goto('admin/config/people/mo_auth');
    } else {
      drupal_set_message(t('Invalid credentials'), 'error');
    }
  }
}

/**
 * Handle back button submit for customer setup.
 */
function mo_auth_back($form, &$form_state) {
  $current_status = 'CUSTOMER_SETUP';
  variable_set('mo_auth_status', $current_status);
  variable_del('mo_auth_customer_admin_email');
  variable_del('mo_auth_customer_admin_phone');
  variable_del('mo_auth_tx_id');
  drupal_set_message(t('Register/Login with your miniOrange Account'));
}
function mo_auth_save_customer($user, $json, $username, $phone) {
  variable_set('mo_auth_customer_id', $json->id);
  variable_set('mo_auth_customer_token_key', $json->token);
  variable_set('mo_auth_customer_admin_email', $username);
  variable_set('mo_auth_customer_admin_phone', $phone);
  variable_set('mo_auth_customer_api_key', $json->apiKey);
  variable_set('mo_auth_customer_app_secret', $json->appSecret);
  $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'] = $username;
  $user->configured_auth_methods[LANGUAGE_NONE][0]['value'] = array (
    AuthenticationType::$EMAIL_VERIFICATION['code']
  );
  $current_status = 'PLUGIN_CONFIGURATION';
  variable_set('mo_auth_status', $current_status);
  user_save($user);

  // Update the customer second factor to OOB Email in miniOrange
  $customer = new MiniorangeCustomerProfile();
  $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $username, '', '', AuthenticationType::$EMAIL_VERIFICATION['code']);
  $user_api_handler = new UsersAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
  $user_api_handler->update($miniorange_user);

  $license_response = $user_api_handler->fetchLicense();
  
  $license_type = 'DEMO';
  $license_plan = 'DEMO';
  $no_of_users = 1;
  if ($license_response->status == 'SUCCESS') {
  
    $license_type = $license_response->licenseType;
    if ($license_type == 'PREMIUM') {
      $license_plan = $license_response->licensePlan;
    }
    $no_of_users = $license_response->noOfUsers;
  }
  variable_set('mo_auth_2fa_license_type', $license_type);
  variable_set('mo_auth_2fa_license_plan', $license_plan);
  variable_set('mo_auth_2fa_license_no_of_users', $no_of_users);
  variable_set('mo_auth_enable_two_factor', TRUE);
  variable_set('mo_auth_enforce_inline_registration', FALSE);
}
function mo_auth_fetch_customer_license($form, &$form_state) {
  $customer = new MiniorangeCustomerProfile();
  $user_api_handler = new UsersAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
  $response = $user_api_handler->fetchLicense();
  if ($response->status == 'SUCCESS') {
    $license_type = $response->licenseType;
    $license_plan = 'DEMO';
    if ($license_type == 'PREMIUM') {
      $license_plan = $response->licensePlan;
    }
    $no_of_users = $response->noOfUsers;
    variable_set('mo_auth_2fa_license_type', $license_type);
    variable_set('mo_auth_2fa_license_plan', $license_plan);
    variable_set('mo_auth_2fa_license_no_of_users', $no_of_users);
  }
  drupal_set_message(t('License fetched successfully.'), 'status');
}

<?php

function mo_auth_configure_otp_over_sms($form, &$form_state)
{
    
    global $base_url, $user;
    $user = user_load($user->uid);
    
    $user_email       = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
    $customer         = new MiniorangeCustomerProfile();
    $miniorange_user  = new MiniorangeUser($customer->getCustomerID(), $user_email, NULL, NULL, AuthenticationType::$SMS['code']);
    $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    
    $form['header']['#markup'] = '<div class="mo2f-setup-header"><div class="mo2f-setup-header-top-left">Configure OTP Over SMS</div></div>';
    
    $form['miniorange_phone'] = array(
        '#type' => 'textfield',
        '#title' => t('Verify Your Phone'),
        '#attributes' => array('pattern' => '[\+]?[0-9]{1,4}\s?[0-9]{7,12}','placeholder' => 'Enter number with country code Eg. +00xxxxxxxxxx'),
    );
    
    $form['verifyphone'] = array(
        '#type' => 'submit',
        '#value' => t('Verify'),
        '#submit' => array(
            'mo_auth_configure_otp_over_sms_submit'
        )
    );
    
    $form['miniorange_saml_customer_setup_resendotp'] = array(
        '#type' => 'submit',
        '#value' => t('Resend OTP'),
        '#submit' => array(
            'miniorange_saml_resend_otp'
        )
    );
    
    $form['miniorange_OTP'] = array(
        '#type' => 'textfield',
        '#title' => t('OTP')
    );
    
    $form['miniorange_saml_customer_validate_otp_button'] = array(
        '#type' => 'submit',
        '#value' => t('Validate OTP'),
        '#submit' => array(
            'miniorange_saml_validate_otp_submit'
        )
    );
    
    $form['actions']['cancel'] = array(
        '#markup' => l(t('Back'), 'admin/config/people/mo_auth/setup')
    );
    return $form;
}

function mo_auth_configure_otp_over_sms_submit($form, &$form_state)
{
    $form_state['rebuild'] = TRUE;
    global $base_url, $user;
    
    $customer = new MiniorangeCustomerProfile();
    $custID   = $customer->getCustomerID();
    $api_key  = $customer->getAPIKey();
    
    $user            = user_load($user->uid);
    $user_email      = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
    $phone_number    = $form['miniorange_phone']['#value'];
    $currentMethod   = "OTP_OVER_SMS";
    $customer_config = new MiniorangeCustomerSetup($user_email, $phone_number, NULL, NULL);
    
	$response          = $customer_config->send_otp_token($phone_number, $currentMethod, $custID, $api_key);
	
    $send_otp_response = json_decode($response);
    if ($send_otp_response->status == 'SUCCESS') {
        // Store txID.
        variable_set('mo_auth_tx_id', $send_otp_response->txId);
        drupal_set_message(t('We have sent an OTP to @phone. Please enter the OTP to verify your phone number.', array(
            '@phone' => $phone_number
        )));
    } elseif ($send_otp_response->status == 'CURL_ERROR') {
        drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
    }
    
}

function miniorange_saml_resend_otp($form, &$form_state)
{
    $form_state['rebuild'] = TRUE;
    global $base_url, $user;
    
    $customer = new MiniorangeCustomerProfile();
    $custID   = $customer->getCustomerID();
    $api_key  = $customer->getAPIKey();
    
    $user            = user_load($user->uid);
    $user_email      = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
    $phone_number    = $form['miniorange_phone']['#value'];
    $currentMethod   = "OTP_OVER_SMS";
    $customer_config = new MiniorangeCustomerSetup($user_email, $phone_number, NULL, NULL);
    
    $response          = $customer_config->send_otp_token($phone_number, $currentMethod, $custID, $api_key);
    $send_otp_response = json_decode($response);
    if ($send_otp_response->status == 'SUCCESS') {
        // Store txID.
        variable_set('mo_auth_tx_id', $send_otp_response->txId);
        drupal_set_message(t('We have sent an OTP to @phone. Please enter the OTP to verify your phone number.', array(
            '@phone' => $phone_number
        )));
    } elseif ($send_otp_response->status == 'CURL_ERROR') {
        drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
    }
}

function miniorange_saml_validate_otp_submit($form, &$form_state)
{
    
    global $base_url, $user;
    
    $customer       = new MiniorangeCustomerProfile();
    $cKey           = $customer->getCustomerID();
    $customerApiKey = $customer->getAPIKey();
    $otpToken       = $form['miniorange_OTP']['#value'];
    $user           = user_load($user->uid);
    $user_email     = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
    $phone_number   = $form['miniorange_phone']['#value'];
    variable_set('mo_phone', $phone_number);
    $authType      = "SMS";
    $transactionId = variable_get('mo_auth_tx_id');
    
    $customer_config = new MiniorangeCustomerSetup($user_email, $phone_number, NULL, NULL);
    
    $response       = $customer_config->validate_otp_token($transactionId, $otpToken, $cKey, $customerApiKey);
    $otp_validation = json_decode($response);
    $txId           = $otp_validation->txId;
  
    if ($otp_validation->status == 'FAILED') {
        drupal_set_message(t("Validation Failed. Please enter the correct OTP."),'error');
    } else if ($otp_validation->status == 'SUCCESS') {
        
        $form_state['rebuild'] = TRUE;
        $authTypeCode          = AuthenticationType::$SMS['code'];
        global $base_url, $user;
        $user = user_load($user->uid);
        
        $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
        
        $customer         = new MiniorangeCustomerProfile();
        $miniorange_user  = new MiniorangeUser($customer->getCustomerID(), $user_email, NULL, NULL, AuthenticationType::$SMS['code']);
        $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
        
        $configured_methods = mo_auth_get_configured_methods($user->uid);
        
        if (!in_array(AuthenticationType::$SMS['code'], $configured_methods)) {
            array_push($user->configured_auth_methods[LANGUAGE_NONE], array(
                'value' => AuthenticationType::$SMS['code']
            ));
        }
        
        $user_api_handler = new UsersAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
        
        // Updating the authentication method for the user
        $miniorange_user->setAuthType($authTypeCode);
        $response = $user_api_handler->update($miniorange_user);
        
        if ($response->status == 'SUCCESS') {
            user_save($user);
            if ($authTypeCode == AuthenticationType::$SMS['code']) {
                drupal_set_message(t('OTP configured successfully.'), 'status');
            }
            
            drupal_goto('admin/config/people/mo_auth/setup');
        }
        return;
        drupal_set_message(t('An error occured while processing your request. Please try again.'), 'error');
        drupal_goto('admin/config/people/mo_auth/setup');
    }
    
}
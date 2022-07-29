<?php
function mo_auth_configure_google_authenticator($form, &$form_state) {
  global $base_url, $user;
  $user = user_load($user->uid);

  if (array_key_exists('secret', $form_state['input']) === FALSE) {
    $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
    $customer = new MiniorangeCustomerProfile();
    $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $user_email, NULL, NULL, AuthenticationType::$GOOGLE_AUTHENTICATOR['code']);
    $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    $response = $auth_api_handler->getGoogleAuthSecret($miniorange_user);
    $qrCode = $response->qrCodeData;
    $secret = $response->secret;
  } else {
    $secret = $form_state['input']['secret'];
    $qrCode = $form_state['input']['qrCode'];
  }
  $form['header']['#markup'] = '<div class="mo2f-setup-header"><div class="mo2f-setup-header-top-left">Configure Google Authenticator</div></div>' . mo_auth_create_form($base_url, $qrCode, $secret);

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
  $form['secret'] = array (
    '#type' => 'hidden',
    '#value' => $secret
  );
  $form['qrCode'] = array (
    '#type' => 'hidden',
    '#value' => $qrCode
  );
  $form['actions'] = array (
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array (
    '#type' => 'submit',
    '#value' => t('Verify and Save')
  );
  $form['actions']['cancel'] = array (
    '#markup' => l(t('Cancel'), 'admin/config/people/mo_auth/setup')
  )
  // '#suffix' => '</div>',
  ;
  return $form;
}
function mo_auth_configure_google_authenticator_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $secret = $form_state['input']['secret'];
  global $base_url, $user;
  $user = user_load($user->uid);
  $user_email = $user->miniorange_registered_email[LANGUAGE_NONE][0]['value'];
  $otpToken = $form_state['input']['mo_auth_googleauth_token'];

  $customer = new MiniorangeCustomerProfile();
  $miniorange_user = new MiniorangeUser($customer->getCustomerID(), $user_email, NULL, NULL, AuthenticationType::$GOOGLE_AUTHENTICATOR['code']);
  $auth_api_handler = new AuthenticationAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
  $response = $auth_api_handler->register($miniorange_user, AuthenticationType::$GOOGLE_AUTHENTICATOR['code'], $secret, $otpToken, NULL);

  // Clear all the messages
  drupal_get_messages();
  // read API response
  if ($response->status == 'SUCCESS') {
    drupal_set_message(t(''), 'status');
    $configured_methods = mo_auth_get_configured_methods($user->uid);

    if (! in_array(AuthenticationType::$GOOGLE_AUTHENTICATOR['code'], $configured_methods)) {
      array_push($user->configured_auth_methods[LANGUAGE_NONE], array (
        'value' => AuthenticationType::$GOOGLE_AUTHENTICATOR['code']
      ));
    }

    $user_api_handler = new UsersAPIHandler($customer->getCustomerID(), $customer->getAPIKey());
    $response = $user_api_handler->update($miniorange_user);
	//print_r($user); exit;
    if ($response->status == 'SUCCESS') {
      user_save($user);
	  drupal_set_message(t('Google Authenticator configured successfully.'), 'status'); 
      drupal_goto('admin/config/people/mo_auth/setup');
    }
  } elseif ($response->status == 'FAILED') {
    form_set_error('form', t('The passcode you have entered is incorrect. Please try again.'));
    // $form_state['redirect'] = FALSE;
    // drupal_goto('admin/config/people/mo_auth/setup-twofactor');
  } else {
    form_set_error('form', t('An error occured while processing your request. Please try again.'));
    // drupal_goto('admin/config/people/mo_auth/setup-twofactor');
  }
}
function mo_auth_create_form($base_url, $qrCode, $secret) {
  $androidAppLink = file_create_url($base_url . '/' . drupal_get_path('module', 'mo_auth') . '/includes/images/android-google-authenticator-app-link.png');
  $iPhoneAppLink = file_create_url($base_url . '/' . drupal_get_path('module', 'mo_auth') . '/includes/images/iphone-google-authenticator-app-link.png');
  $androidAppQR = file_create_url($base_url . '/' . drupal_get_path('module', 'mo_auth') . '/includes/images/android-google-authenticator-app-qr.jpg');
  $iPhoneAppQR = file_create_url($base_url . '/' . drupal_get_path('module', 'mo_auth') . '/includes/images/iphone-google-authenticator-app-qr.jpg');

  $step1 = '<div>
        <div class="googleauth-steps"><b>Step 1:</b> Download & Install the Google Authenticator app.</div>
        <div style="margin-bottom:20px;">
        <div style="width:50%;float:left;min-width:450px;">
          <div class="googleauth-download-manual">
            <div class="googleauth-download-header">Manual Installation</div>
            <div>
              <div style="font-size:1.1em;padding:10px;">iPhone Users</div>
              <div>
              <div style="width:60%;float:left">
                <ul>
                  <li>Go to App Store.</li>
                  <li>Search for Google Authenticator.</li>
                  <li>Download and install the App.</li>
                </ul>
              </div>
              <div style="width:40%;float:left;">
                <a target="_blank" href="https://itunes.apple.com/in/app/google-authenticator/id388497605?mt=8"><img src="' . $iPhoneAppLink . '"></a>
              </div>
              </div>
              <br><br><br><br>
              <div style="font-size:1.1em;padding:10px;">Android Users</div>
              <div>
              <div style="width:60%;float:left;">
                <ul>
                  <li>Go to Google Play Store.</li>
                  <li>Search for Google Authenticator.</li>
                  <li>Download and install the App.</li>
                </ul>
              </div>
              <div style="width:40%;float:left;">
                <a target="_blank" href="https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2"><img src="' . $androidAppLink . '"></a>
              </div>
              </div>
            </div>
          </div>
        </div>
        <div style="width:50%;float:left;min-width:450px;">
          <div class="googleauth-download-qrcode">
            <div class="googleauth-download-header">Scan QR Code</div>
            <div style="text-align:center;padding:25px;">
            <div style="width:50%;float:left">
              <div>
                <img src="' . $iPhoneAppQR . '">
              </div>
              <span>Apple App Store<br />(iOS)</span>
            </div>
            <div style="width:50%;float:left">
              <div>
                <img src="' . $androidAppQR . '">
              </div>
              <span>Google Play Store<br />(Android)</span>
            </div>
            </div>
          </div>
        </div>
        </div>
        </div>';

  $secret = MoAuthUtilities::indentSecret($secret);
  $step2 = '<div style="width: 100%;float:left;padding-top: 10px;">
        <div class="googleauth-steps"><b>Step 2:</b> Scan the below QR Code from Google Authenticator app.</div>
        <div style="text-align:center">
        <div style="display:inline-block;width:33%">
          <img src="data:image/jpg;base64,' . $qrCode . '">
        </div>
        <div style="vertical-align: top;display: inline-flex;padding: 75px 0px;">
          <p style="margin: auto 10px;font-size:12pt;font-weight:bold">OR</p>
        </div>
        <div style="display:inline-block;width:33%">
          <div class="googleauth-secret">
          <span style="padding:10px;">
            <p style="font-size: 10pt;">Use the following secret</p><p>
            </p><p id="googleAuthSecret" style="font-size: 13pt; font-weight: bold">' . $secret . '</p>
            <p style="font-size: 9pt;">(Spaces don&#39;t matter)</p><p>

        </p></span></div>
        </div>
        </div>
        </div>';

  $step3 = '<div style="width: 100%;float:left;padding-top: 10px;">
        <div class="googleauth-steps"><b>Step 3:</b> Enter the Passcode generated by Google Authenticator app.</div>
        </div>';
  return $step1 . $step2 . $step3;
}

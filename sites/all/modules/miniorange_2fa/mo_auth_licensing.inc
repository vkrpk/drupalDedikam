<?php

/**
 * @file
 * Contains support form for miniOrange 2FA Login Module.
 */

/**
 * Showing Support form info.
 */
function mo_auth_licensing($form, &$form_state) {
  global $base_url;
  $license_type = variable_get('mo_auth_2fa_license_type', 'DEMO');
  $license_plan = variable_get('mo_auth_2fa_license_plan', 'DEMO');
  $free = '<div class="mo2f-licensing-thumbnail mo2f-licensing-light-tab">
    <h3 class="mo2f-pricing-header">Free</h3>';
  if ($license_type != 'PREMIUM') {
    $free .= '<h4 style="margin: 5px;padding: 12px 12px 2px 0px;">(You are automatically on this plan)</h4>';
  } else {
    $free .= 'Thank you for upgrading.<br /><br />';
  }
  $free .= '<hr>
    <p>For 1 user - Forever</p><hr>
    <p style="margin-bottom: 43px;">$0 - Subscription Fees</p>
    <hr>
    <p style="font-weight:bold">Features:</p>
    <p>
    All Authentication Methods,<br>
    Remember Device<br><br><br><br><br><br><br>
    </p><hr>
    <p style="font-weight:bold;">Backup Method:</p>
    <p>Security Questions(KBA)</p><br>
    <hr>
    <p>Basic Support by Email</p>
    </div>';

  $doItYourself = '<div class="mo2f-licensing-thumbnail mo2f-licensing-dark-tab">
    <h3 class="mo2f-pricing-header">Do it yourself</h3>';
  if ($license_type == 'PREMIUM' && strpos($license_plan, 'Do it Yourself') !== false) {
    $doItYourself .= '<h4 style="margin: 5px;padding: 12px 12px 2px 0px;">(You are on this plan)</h4>';
  } elseif ($license_type == 'PREMIUM') {
    $doItYourself .= '<br />';
  } else {
    $doItYourself .= '<input type="submit" data-plan="drupal_2fa_basic_plan" class="mo2f-pricing-button" value="Click here to upgrade" />*';
    $doItYourself .= '<br /><input type="submit" data-plan="otp_recharge_plan" class="mo2f-pricing-button" value="Click here to buy SMS transactions" />*';
  }
  $doItYourself .= '<hr>
    <p>For 1+ users</p><hr>
    <p>Yearly Subscription Fees**<br />
	  <select class="form-control" style="border-radius:5px;width:250px;height:30px;">
		<option > 5 users - $15 per year </option> 
		<option > 10 users - $30 per year </option> 
		<option > 20 users - $45 per year </option> 
		<option > 30 users - $60 per year </option>
		<option > 40 users - $75 per year </option>
		<option > 50 users - $90 per year </option>
		<option > 60 users - $100 per year </option>
		<option > 70 users - $110 per year </option> 
		<option > 80 users - $120 per year </option>
		<option > 90 users - $130 per year </option>
		<option > 100 users - $140 per year </option>
		<option > 150 users - $177.5 per year </option> 
		<option > 200 users - $215 per year </option> 	
		<option > 250 users - $245 per year </option>
		<option > 300 users - $275 per year </option>
		<option > 350 users - $300 per year </option> 
		<option > 400 users - $325 per year </option>
		<option > 450 users - $347.5 per year </option>	
		<option > 500 users - $370 per year </option>			
		<option > 600 users - $395 per year </option>
		<option > 700 users - $420 per year </option>
		<option > 800 users - $445 per year </option>
		<option > 900 users - $470 per year </option>	
		<option > 1000 users - $495 per year </option>
		<option > 2000 users - $549 per year </option>	
		<option > 3000 users - $599 per year </option>
		<option > 4000 users - $649 per year </option>
		<option > 5000 users - $699 per year </option>	
		<option > 10000 users - $799 per year </option>
		<option > 20000 users - $999 per year </option>	
	  </select>
	</p>
    <hr>
    <p style="font-weight:bold">Features:</p>
    <p>
    All Authentication Methods***<br>
  Remember Device<br>
  Enforce 2FA registration for users<br>
  Manage Registered Device Profiles<br>
  Custom Redirection<br>
  Customize Email Templates<br>
  Customize SMS Templates<br><br>
    </p><hr>
    <p style="font-weight:bold;">Backup Method:</p>
    <p>Security Questions(KBA)<br>OTP over EMAIL</p>
    <hr>
    <p>Basic Support by Email</p>
    </div>';

  $premium = '<div class="mo2f-licensing-thumbnail mo2f-licensing-light-tab">
    <h3 class="mo2f-pricing-header">Premium</h3>';
if ($license_type == 'PREMIUM' && strpos($license_plan, 'Premium') !== false) {
    $premium .= '<h4 style="margin: 5px;padding: 12px 12px 2px 0px;">(You are on this plan)</h4>';
  } elseif ($license_type == 'PREMIUM') {
    $premium .= 'Thank you for upgrading.<br /><br />';
  } else {
    $premium .= '<input type="submit" data-plan="drupal_2fa_premium_plan" class="mo2f-pricing-button" value="Click here to upgrade" />*';
	$premium .= '<br /><input type="submit" data-plan="otp_recharge_plan" class="mo2f-pricing-button" value="Click here to buy SMS transactions" />*';
  }

  $premium .= '<hr>
    <p>For 1+ users, Setup and Custom Work</p><hr>
    <p>Yearly Subscription Fees**<br />
	  <select class="form-control" style="border-radius:5px;width:250px;height:30px;">
		<option > 5 users - $15 per year </option> 
		<option > 10 users - $30 per year </option> 
		<option > 20 users - $45 per year </option> 
		<option > 30 users - $60 per year </option>
		<option > 40 users - $75 per year </option>
		<option > 50 users - $90 per year </option>
		<option > 60 users - $100 per year </option>
		<option > 70 users - $110 per year </option> 
		<option > 80 users - $120 per year </option>
		<option > 90 users - $130 per year </option>
		<option > 100 users - $140 per year </option>
		<option > 150 users - $177.5 per year </option> 
		<option > 200 users - $215 per year </option> 	
		<option > 250 users - $245 per year </option>
		<option > 300 users - $275 per year </option>
		<option > 350 users - $300 per year </option> 
		<option > 400 users - $325 per year </option>
		<option > 450 users - $347.5 per year </option>	
		<option > 500 users - $370 per year </option>			
		<option > 600 users - $395 per year </option>
		<option > 700 users - $420 per year </option>
		<option > 800 users - $445 per year </option>
		<option > 900 users - $470 per year </option>	
		<option > 1000 users - $495 per year </option>
		<option > 2000 users - $549 per year </option>	
		<option > 3000 users - $599 per year </option>
		<option > 4000 users - $649 per year </option>
		<option > 5000 users - $699 per year </option>	
		<option > 10000 users - $799 per year </option>
		<option > 20000 users - $999 per year </option>	
	  </select>
	</p>
    <hr>
    <p style="font-weight:bold">Features:</p>
    <p>
    All Authentication Methods***<br>
  Remember Device<br>
  Enforce 2FA registration for users<br>
  Manage Registered Device Profiles<br>
  Custom Redirection<br>
  Customize Email Templates<br>
  Customize SMS Templates<br>
  End to End 2FA Integration****<br>
    </p><hr>
    <p style="font-weight:bold;">Backup Method:</p>
    <p>Security Questions(KBA)<br>OTP over EMAIL</p>
    <hr>
    <p>Premium Support Plans Available</p>
    </div><br><br>';

  $disclaimer = '<h3>* Steps to upgrade to premium module -</h3>
    <p>1. You will be redirected to miniOrange Login Console. Enter your password with which you created an account with us. After that you will be redirected to payment page.</p>
    <p>2. Enter you card details and complete the payment. On successful payment completion, <a href="?q=admin/config/people/mo_auth">Customer Profile</a> tab and click on Check License button.</p>
    <h3>** If you dont find the number of users in the dropdown then check the price for exact number of users by clicking on "Click here to upgrade" button.</h3>
	<p>You can mail us at <a href="mailto:info@miniorange.com"><b>info@miniorange.com</b></a> or submit the support form under Support tab to contact us.</p>
	<p>*** All the authentication methods given under <b>SETUP TWO-FACTOR</b> are supported after upgrading. We also support OTP over SMS, Hardware Token like Yubico and Display Token. Contect us if you want to use these authentication methods.</p>
    <h3>*** End to End 2FA Integration - We will setup a Conference Call / Gotomeeting and do end to end setup for you. We provide services to do the setup on your behalf.';

  $form['header']['#markup'] = $free . $doItYourself . $premium . $disclaimer;
  $customer = new MiniorangeCustomerProfile();
  $form['username'] = array (
    '#type' => 'hidden',
    '#value' => $customer->getRegisteredEmail()
  );
  $form['redirectUrl'] = array (
    '#type' => 'hidden',
    '#value' => MoAuthConstants::$BASE_URL . '/initializepayment'
  );
  $form['requestOrigin'] = array (
    '#type' => 'hidden',
    '#value' => ''
  );
  $form['#action'] = MoAuthConstants::$BASE_URL . '/login';
  $form['#attributes'] = array (
    'target' => '_blank'
  );
  return $form;
}
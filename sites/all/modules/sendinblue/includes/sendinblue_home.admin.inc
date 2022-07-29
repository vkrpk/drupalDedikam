<?php
/**
 * @file
 * Sendinblue module home admin settings.
 */

/**
 * Display Home page of module.
 *
 * @return array
 *   Render array.
 */
function sendinblue_home_page() {
  $page_html = '<div id="wrap" class="box-border-box container-fluid"><div id="wrap-left" class="box-border-box col-md-9">';
  if (SendinblueManager::isLoggedInState()) {
    $page_html .= SendinblueManager::generateHomeLogin();
  }
  else {
    $page_html .= SendinblueManager::generateHomeLogout();
  }

  $page_html .= '</div><div id="wrap-right-side" class="box-border-box  col-md-3">';
  $page_html .= SendinblueManager::generateSidebar();
  $page_html .= '</div></div>';
  $output = array(
    'main_wrapper' => array(
      '#type' => 'markup',
      '#markup' => $page_html,
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'sendinblue') . '/css/admin-setting.css'),
    ),
  );
  return $output;
}

/**
 * Login form of Home page.
 */
function sendinblue_logout_form($form, &$form_state) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Logout'),
  );
  return $form;
}
/**
 * Login form of Home page.
 */
function sendinblue_logout_form_validate($form, &$form_state) {
  $sendInBlueConfigFactory = new SendInBlueConfigFactory();
  $sendInBlueConfigFactory->setSendInBlueOff();
  $sendInBlueConfigFactory->removeAll();
}

/**
 * Login form of Home page.
 */
function sendinblue_login_form($form, &$form_state) {
  $form = array();
  $form['access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => t('API Key'),
    ),
    '#size' => 50,
    '#maxlenght' => 100,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Login'),
  );
  return $form;
}

/**
 * Validation Handler of Login form.
 */
function sendinblue_login_form_validate($form, &$form_state) {
  $accessKey = $form_state['values']['access_key'];

  $sendInBlueConfigFactory = new SendInBlueConfigFactory();
  $sendinblueMailin = new SendinblueMailin();
  $sendinblueMailin = $sendinblueMailin->updateSendinblueMailin($accessKey);
  $sibAccount = $sendinblueMailin->getAccount();

  if ($sibAccount->getEmail() !== NULL) {
    $account_user_name = $sibAccount->getFirstName() . ' ' . $sibAccount->getLastName();
    $sendInBlueConfigFactory->setAccessKey($accessKey);
    $sendInBlueConfigFactory->setAccountEmail($sibAccount->getEmail());
    $sendInBlueConfigFactory->setAccountUsername($account_user_name);
    $sendInBlueConfigFactory->setAccountData($sibAccount);

    SendinblueManager::updateSmtpDetails();
    $sendinblueMailin->partnerDrupal();
  }
  else {
    form_set_error('access_key');
    if (!empty($accessKey)) {
      drupal_set_message(t('API key is invalid'), 'error');
    }
  }
}

/**
 * Form to send email of Home page.
 */
function sendinblue_send_email_form($form, &$form_state) {
  $sendInBlueConfigFactory = new SendInBlueConfigFactory();
  $smtpDetails = $sendInBlueConfigFactory->getSmtpDetails();
  $smtpAvailable = ($smtpDetails !== NULL);
  $sendInBlueTransacEmailOn = $sendInBlueConfigFactory->getSendInBlueOn();

  $form = array();
  if (!$smtpAvailable) {
    $form['sendinblue_alert'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="sendinblue_alert_area" style="padding: 10px;background-color: #fef5f1;color: #8c2e0b;border-color: #ed541d;border-width: 1px;border-style: solid;">',
      '#markup' => t('Current you can not use SendinBlue SMTP. Please confirm at <a href="@smtp-sendinblue" target="_blank">Here</a>', array('@smtp-sendinblue' => 'https://mysmtp.sendinblue.com/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link')),
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );
  }

  $form['sendinblue_on'] = array(
    '#type'          => 'radios',
    '#title'         => t('Send emails through SendinBlue SMTP'),
    '#default_value' => $sendInBlueTransacEmailOn,
    '#description'   => t('Choose "Yes" if you want to use SendinBlue SMTP to send transactional emails.'),
    '#options'       => array(1 => t('Yes'), 0 => t('No')),
    '#disabled' => ($smtpAvailable === TRUE) ? FALSE : TRUE,
  );

  $form['sendinblue_to_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Enter email to send a test'),
    '#description'   => t('Select here the email address you want to send a test email to.'),
    '#disabled' => ($smtpAvailable === TRUE) ? FALSE : TRUE,
    '#states' => array(
      // Hide unless needed.
      'visible' => array(
        ':input[name="sendinblue_on"]' => array('value' => 1),
      ),
      'required' => array(
        ':input[name="sendinblue_on"]' => array('value' => 1),
      ),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
    '#disabled' => ($smtpAvailable === TRUE) ? FALSE : TRUE,
  );
  return $form;
}

/**
 * Validation Handler of Form to send email of Home page.
 */
function sendinblue_send_email_form_validate($form, &$form_state)
{
  $send_email = $form_state['values']['sendinblue_to_email'];

  if (!empty($send_email) && !valid_email_address($send_email)) {
    form_set_error('sendinblue_to_email', t('The email address is invalid.'));
    return;
  }
}

/**
 * Submit Handler of Form to send email of Home page.
 */
function sendinblue_send_email_form_submit($form, &$form_state) {
  $sendInBlueConfigFactory = new SendInBlueConfigFactory();

  $sendinblue_on = $form_state['values']['sendinblue_on'];
  $send_email = $form_state['values']['sendinblue_to_email'];

  if ($sendinblue_on === '1') {
    $sendInBlueConfigFactory->setSendInBlueOn();
  } else {
    $sendInBlueConfigFactory->setSendInBlueOff();
  }

  SendinblueManager::updateSmtpDetails();

  if($send_email){
    SendinblueManager::sendEmail('test', $send_email);
  }
}

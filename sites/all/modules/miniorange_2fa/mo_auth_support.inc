<?php
/**
 * @file
 * Contains support form for miniOrange 2FA Login Module.
 */

 /**
 *  Showing Support form info.
 */
function mo_auth_support($form, &$form_state) {
  $form['markup_1'] = array(
    '#markup' => '<h3>Support</h3><div>Need any help? Just send us a query so we can help you.<br /><br /></div>',
  );
  $email = '';
  $phone = '';
  if (MoAuthUtilities::isCustomerRegistered()) {
  	$customer = new MiniorangeCustomerProfile();
  	$email = $customer->getRegisteredEmail();
  	$phone = $customer->getRegisteredPhone();
  }

  $form['mo_auth_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#default_value' => $email,
    '#attributes' => array('placeholder' => 'Enter your email'),
    '#required' => TRUE,
  );

  $form['mo_auth_phone_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone number'),
    '#default_value' => $phone,
    '#attributes' => array('placeholder' => 'Enter your phone number'),
  );

  $form['mo_auth_support_query'] = array(
    '#type' => 'textarea',
    '#title' => t('Query'),
    '#cols' => '10',
    '#rows' => '5',
    '#attributes' => array('placeholder' => 'Write your query here!'),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Query'),
  );

  return $form;
 }

 /**
  *  Send support query.
  */
function mo_auth_support_submit($form, &$form_state) {
  $email = $form['mo_auth_email_address']['#value'];
  $phone = $form['mo_auth_phone_number']['#value'];
  $query = $form['mo_auth_support_query']['#value'];
  $support = new Miniorange2FASupport($email, $phone, $query);
  $support_response = $support->sendSupportQuery();
  if ($support_response) {
    drupal_set_message(t('Thanks for getting in touch! We shall get back to you shortly.'));
    drupal_goto('admin/config/people/mo_auth');
  }
  else {
    drupal_set_message(t('Error submitting the support query. Please send us your query at <a href="mailto:info@miniorange.com">info@miniorange.com</a>.'), 'error');
    drupal_goto('admin/config/people/mo_auth');
  }
}
<?php

class SendInBlueConfigFactory
{
  const CONFIG_TYPE = 'sendinblue';

  /**
   * Variable name of Sendinblue access key.
   */
  const ACCESS_KEY = 'sendinblue_access_key';

  /**
   * Variable name of Sendinblue account email.
   */
  const ACCOUNT_EMAIL = 'sendinblue_account_email';

  /**
   * Variable name of Sendinblue account user name.
   */
  const ACCOUNT_USERNAME = 'sendinblue_account_username';

  /**
   * Variable name of Sendinblue account data.
   */
  const ACCOUNT_DATA = 'sendinblue_account_data';

  /**
   * Variable name of access_token.
   */
  const ACCESS_TOKEN = 'sendinblue_access_token';

  /**
   * Variable name of attribute lists.
   */
  const ATTRIBUTE_LISTS = 'sendinblue_attribute_lists';

  /**
   * Variable name of smtp details.
   */
  const SMTP_DETAILS = 'sendinblue_smtp_details';

  /**
   * Variable name of smtp details.
   */
  const SENDINBLUE_ON = 'sendinblue_on';
  const DRUPAL_MAIL_SYSTEM = 'mail_system';

  const ALL_VARIABLES_CONFIG = [
    self::ACCESS_KEY,
    self::ACCESS_TOKEN,
    self::ACCOUNT_EMAIL,
    self::ACCOUNT_USERNAME,
    self::ACCOUNT_DATA,
    self::ATTRIBUTE_LISTS,
    self::SMTP_DETAILS,
    self::SENDINBLUE_ON,
  ];

  public function getAccessKey()
  {
    return $this->getVariable(self::ACCESS_KEY);
  }

  public function getAccountEmail()
  {
    return $this->getVariable(self::ACCOUNT_EMAIL);
  }

  public function getAccountUsername()
  {
    return $this->getVariable(self::ACCOUNT_USERNAME);
  }

  public function getAccountData()
  {
    return drupal_json_decode($this->getVariable(self::ACCOUNT_DATA, []));
  }

  public function getSmtpDetails($default = FALSE)
  {
    return $this->getVariable(self::SMTP_DETAILS, $default);
  }

  public function getSendInBlueOn()
  {
    return $this->getVariable(self::SENDINBLUE_ON);
  }

  public function setSendInBlueOn()
  {
    $this->setVariable(self::DRUPAL_MAIL_SYSTEM, array('default-system' => 'SendInBlueMailSystem'));
    $this->setVariable(self::SENDINBLUE_ON, 1);
  }

  public function setSendInBlueOff()
  {
    $this->setVariable(self::DRUPAL_MAIL_SYSTEM, array('default-system' => 'DefaultMailSystem'));
    $this->setVariable(self::SENDINBLUE_ON, 0);
  }

  public function setSmtpDetails($smtp_details)
  {
    $this->setVariable(self::SMTP_DETAILS, $smtp_details);
  }

  public function setAccessToken($access_token)
  {
    $this->setVariable(self::ACCESS_TOKEN, $access_token);
  }

  public function setAccessKey($accessKey)
  {
    $this->setVariable(self::ACCESS_KEY, $accessKey);
  }

  public function setAccountEmail($accountEmail)
  {
    $this->setVariable(self::ACCOUNT_EMAIL, $accountEmail);
  }

  public function setAccountUsername($accountUsername)
  {
    $this->setVariable(self::ACCOUNT_USERNAME, $accountUsername);
  }

  public function setAccountData($accountUserData)
  {
    $this->setVariable(self::ACCOUNT_DATA, drupal_json_encode($accountUserData));
  }

  public function setAttributeLists($attributes)
  {
    $this->setVariable(self::ATTRIBUTE_LISTS, $attributes);
  }

  public function removeAll()
  {
    foreach (self::ALL_VARIABLES_CONFIG as $variable) {
      $this->delVariable($variable);
    }
  }

  private function getVariable($name, $default = NULL){
    return variable_get($name, $default);
  }

  private function setVariable($name, $value){
    variable_set($name, $value);
  }

  private function delVariable($name){
    variable_del($name);
  }
}

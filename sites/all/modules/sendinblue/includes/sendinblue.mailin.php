<?php

use SendinBlue\Client\Configuration;

/**
 * @file
 * Rest class file.
 */

/**
 * Sendinblue REST client.
 */
class SendinblueMailin {
  const SENDINBLUE_API_VERSION_V2 = 'v2';
  const SENDINBLUE_API_VERSION_V3 = 'v3';
  private $sendinblueApiV3;
  private $sendinblueApiV2;
  private $sendInBlueLoggerFactory;

  /**
   * @var SendInBlueApiInterface
   */
  private $sendinblueMailin;

  public function __construct() {
    $this->sendInBlueLoggerFactory = new SendInBlueLoggerFactory();
    $this->sendInBlueConfigFactory = new SendInBlueConfigFactory();

    if (!function_exists('curl_init')) {
      $this->sendInBlueLoggerFactory->error('SendinBlue requires CURL module');
      return;
    }

    if (module_exists('libraries')) {
      libraries_load('sendinblue');
    }

    if (!method_exists(Configuration::class, 'getApiKey')) {
      drupal_set_message('SendInBlue client initialization failed: Unable to load the SendInBlue PHP library.', 'error');
      $this->sendInBlueLoggerFactory->error('SendInBlue client initialization failed: Unable to load the SendInBlue PHP library.');
    }

    $this->sendinblueApiV2 = new SendinblueApiV2();
    $this->sendinblueApiV3 = new SendinblueApiV3();

    $this->updateSendinblueMailin($this->getAccessKey());
  }

  /**
   * Get the access key store in configuration.
   *
   * @return string
   *   The SiB access key
   */
  public function getAccessKey() {
    return $this->sendInBlueConfigFactory->getAccessKey();
  }

  /**
   * Change Class Class in function of API version.
   *
   * @param string $accessKey
   *   The SiB access key.
   *
   * @return SendInBlueApiInterface
   *   SendInBlueApiInterface (V2 or V3)
   */
  public function updateSendinblueMailin($accessKey) {
    if ($this->getApiVersion($accessKey) === self::SENDINBLUE_API_VERSION_V3) {
      $this->sendinblueMailin = $this->sendinblueApiV3;
    }
    else {
      $this->sendinblueMailin = $this->sendinblueApiV2;
    }

    $this->sendinblueMailin->setApiKey($accessKey);

    return $this->sendinblueMailin;
  }

  /**
   * Get the access key store in configuration.
   *
   * @param string $accessKey
   *   The SiB access key.
   *
   * @return string
   *   The SiB API version (V2 or V3)
   */
  public function getApiVersion($accessKey = null) {
    if(empty($accessKey)){
      $accessKey = $this->getAccessKey();
    }

    if (strlen($accessKey) > 20 && strpos($accessKey, 'xkeysib') !== FALSE) {
      return self::SENDINBLUE_API_VERSION_V3;
    }
    return self::SENDINBLUE_API_VERSION_V2;
  }


  /**
   * Get the account email store in configuration.
   *
   * @return string
   *   The SiB account email
   */
  public function getAccountEmail() {
    return $this->sendInBlueConfigFactory->getAccountEmail();
  }

  /**
   * Get the account username store in configuration.
   *
   * @return string
   *   The SiB account username
   */
  public function getAccountUsername() {
    return $this->sendInBlueConfigFactory->getAccountUsername();
  }

  /**
   * Get the data account store in configuration.
   *
   * @return array
   *   The SiB account store
   */
  public function getAccountData() {
    return $this->sendInBlueConfigFactory->getAccountData();
  }

  public function getAccount() {
    return $this->sendinblueMailin->getAccount();
  }

  public function getCampaigns($type) {
    return $this->sendinblueMailin->getTemplates();
  }

  /**
   * @param $id
   * @return GetSmtpTemplateOverview
   */
  public function getCampaign($id) {
    return $this->sendinblueMailin->getTemplate($id);
  }

  /**
   * Get lists of an account.
   *
   * @return array
   *   An array of all lists.
   */
  public function getLists() {
    $lists = $this->sendinblueMailin->getLists();

    if ($lists !== NULL) {
      return $lists->getLists();
    }

    return [];
  }

  public function countUserlists($listIds) {
    return $this->sendinblueMailin->countUserlists($listIds);
  }

  public function getList($id) {
    return $this->sendinblueMailin->getList($id);
  }

  /**
   * Send email via sendinblue.
   *
   * @param array $to
   *   A recipe email address.
   * @param string $subject
   *   A subject of email.
   * @param string $html
   *   A html body of email content.
   * @param string $text
   *   A text body of email content.
   * @param array $from
   *   A sender email address.
   * @param array $replyto
   *   A reply address.
   * @param array $cc
   *   A cc address.
   * @param array $bcc
   *   A bcc address.
   * @param array $attachment
   *   A attachment information.
   * @param array $headers
   *   A header of email.
   *
   * @return CreateSmtpEmail
   *   An array of response code.
   */
  public function sendEmail(
    array $to,
    string $subject,
    string $html,
    string $text,
    array $from = [],
    array $replyto = [],
    array $cc = [],
    array $bcc = [],
    array $attachment = [],
    array $headers = []
  ){
    return $this->sendinblueMailin->sendEmail($to, $subject, $html, $text, $from, $replyto, $cc, $bcc, $attachment, $headers);
  }

  /**
   * @param $email
   * @return GetExtendedContactDetails
   */
  public function getUser($email) {
    return $this->sendinblueMailin->getUser($email);
  }

  /**
   * Create and update user.
   *
   * @param string $email
   *   An email address of user.
   * @param array $attributes
   *   An attributes to update.
   * @param array $blacklisted
   *   An array of black user.
   * @param string $listid
   *   A new listid.
   * @param string $listid_unlink
   *   A link unlink.
   */
  public function createUpdateUser($email, $attributes = [], $blacklisted = [], $listid = '', $listid_unlink = '') {
    $this->sendinblueMailin->createUpdateUser($email, $attributes, $blacklisted, $listid, $listid_unlink);
  }

  public function getAttribute($type) {
    return $this->sendinblueMailin->getAttributes();
  }

  public function getAttributes() {
    return $this->sendinblueMailin->getAttributes();
  }

  /**
   * Get the access token.
   *
   * @return string
   *   An access token information.
   */
  public function getAccessTokens() {
    return $this->sendinblueMailin->getAccessTokens();
  }

  /**
   * @return GetSmtpDetails
   */
  public function getSmtpDetails() {
    return $this->sendinblueMailin->getSmtpDetails();
  }

  /**
   * Add the Partner's name in sendinblue.
   */
  public function partnerDrupal() {
    $this->sendinblueMailin->partnerDrupal();
  }
}

<?php
/**
 * @file
 * Manage class file.
 */

/**
 * Basic manager of module.
 */
class SendinblueManager {

  /**
   * Generate sidebar.
   *
   * @return string
   *   A html of sidebar.
   */
  public static function generateSidebar() {
    $print_html = '<div class="panel panel-default text-left box-border-box  small-content">';
    $print_html .= '<div class="panel-heading"><strong>' . t('About SendinBlue') . '</strong></div>';
    $print_html .= '<div class="panel-body"><p>';
    $print_html .= t('SendinBlue is an online software that allows you to send emails and SMS. Easily manage your Marketing campaigns, transactional emails and SMS.');
    $print_html .= '</p><ul class="sib-widget-menu"><li>';
    $print_html .= t('<a href="@sendinblue-about" target="_blank">> Who we are</a>', array('@sendinblue-about' => 'https://www.sendinblue.com/about/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li><li>';
    $print_html .= t('<a href="@sendinblue-price" target="_blank">> Pricing</a>', array('@sendinblue-price' => 'https://www.sendinblue.com/pricing/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li><li>';
    $print_html .= t('<a href="@sendinblue-feature" target="_blank">> Features</a>', array('@sendinblue-feature' => 'https://www.sendinblue.com/features/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li></ul></div></div>';
    $print_html .= '<div class="panel panel-default text-left box-border-box  small-content">';
    $print_html .= '<div class="panel-heading"><strong>' . t('Need Help ?') . '</strong></div>';
    $print_html .= '<div class="panel-body"><p>' . t('You have a question or need more information ?') . '</p><ul class="sib-widget-menu"><li>';
    $print_html .= t('<a href="@sendinblue-tutorial" target="_blank">> Tutorials</a>', array('@sendinblue-tutorial' => 'https://resources.sendinblue.com/category/tutorials/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li><li>';
    $print_html .= t('<a href="@sendinblue-faq" target="_blank">> FAQ</a>', array('@sendinblue-faq' => 'https://resources.sendinblue.com/category/faq/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li></ul></div></div>';
    return $print_html;
  }

  /**
   * Generate Home layout of Log out.
   *
   * @return string
   *   A html of home page when log out.
   */
  public static function generateHomeLogout() {
    $print_html = '<div id="main-content"><div class="panel panel-default row small-content">';
    $print_html .= '<div class="page-header">';
    $print_html .= t('<span style="color: #777777;">Step1&nbsp;|&nbsp;</span><strong>Create a SendinBlue Account</strong>');
    $print_html .= '</div><div class="panel-body"><div class="col-md-9 row">';
    $print_html .= '<p>' . t('By Creating a free SendinBlue account, you will have access to send a confirmation message.') . '</p>';
    $print_html .= '<ul class="sib-home-feature">';
    $print_html .= '<li>-&nbsp;&nbsp;' . t('Collect your contacts and upload your lists') . '</li>';
    $print_html .= '<li>-&nbsp;&nbsp;' . t('Use SendinBlue SMTP to send your transactional emails') . '</li>';
    $print_html .= '<li class="home-read-more-content">-&nbsp;&nbsp;' . t('Email marketing builders') . '</li>';
    $print_html .= '<li class="home-read-more-content">-&nbsp;&nbsp;' . t('Create and schedule your email marketing campaigns') . '</li>';
    $print_html .= '<li class="home-read-more-content">-&nbsp;&nbsp;' . t('See all of') . '&nbsp;';
    $print_html .= t('<a href="@sendinblue-feature" target="_blank">SendinBlue\'s features</a></li>', array('@sendinblue-feature' => 'https://www.sendinblue.com/features/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</ul>' . t('<a href="@sendinblue-signup" target="_blank" style="margin-top: 10px;display: block;">>&nbsp;Create an account</a>', array('@sendinblue-signup' => 'https://www.sendinblue.com/users/signup/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</div></div></div><div class="panel panel-default row small-content"><div class="page-header">';
    $print_html .= '<span style="color: #777777;">' . t('Step2') . '&nbsp;|&nbsp;</span>';
    $print_html .= '<strong>' . t('Activate your account with your API key') . '</strong></div>';
    $print_html .= '<div class="panel-body"><div class="col-md-9 row"><div id="success-alert" class="alert alert-success" role="alert" style="display: none;">';
    $print_html .= t('You successfully activate your account.');
    $print_html .= '</div>';
    $print_html .= '<div id="failure-alert" class="alert alert-danger" role="alert" style="display: none;">' . t('Please input correct information.') . '</div>';
    $print_html .= '<p>' . t('Once your have created your SendinBlue account, activate this plugin to send all your transactional emails by using SendinBlue SMTP to make sure all of your emails get to your contacts inbox.') . '<br />';
    $print_html .= t('To activate your plugin, enter your API key.') . '<br /></p><p>';
    $print_html .= t('<a href="@sendinblue-apikey" target="_blank">>&nbsp;Get your API key from your account</a>', array('@sendinblue-apikey' => 'https://my.sendinblue.com/advanced/apikey/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</p><p><div class="col-md-7 row">';
    $form = drupal_get_form('sendinblue_login_form');
    $print_html .= drupal_render($form);
    $print_html .= '</div></p></div></div></div></div>';
    return $print_html;
  }

  /**
   * Generate Home layout of Log out.
   *
   * @return string
   *   A html of home page when login.
   */
  public static function generateHomeLogin() {
    $sendinblueMailin = new SendinblueMailin();

    // Calculate total count of subscribers.
    $totalSubscribers = 0;
    $listIds = [];

    $sibLists = $sendinblueMailin->getLists();
    if ($sibLists) {
      foreach ($sibLists as $list) {
        $listIds[] = $list['id'];
      }
      $totalSubscribers = $sendinblueMailin->countUserlists($listIds);
    }

    // Get account details.
    $account_email = $sendinblueMailin->getAccountEmail();
    $account_username = $sendinblueMailin->getAccountUsername();
    $account_data = $sendinblueMailin->getAccountData();

    $print_html = '<div id="main-content">';
    $print_html .= '<div class="panel panel-default row small-content">';
    $print_html .= '<div class="page-header">';
    $print_html .= '<strong>' . t('My Account') . '</strong>';
    $print_html .= '</div><div class="panel-body"><span class="col-md-12"><b>' . t('You are currently logged in as :') . '</b></span>';
    $print_html .= '<div class="col-md-8 row" style="margin-bottom: 10px;"><p class="col-md-12" style="margin-top: 5px;">';
    $print_html .= check_plain($account_username) . ' - ' . check_plain($account_email) . '<br />';
    foreach ($account_data['plan'] as $account_datum) {
      $print_html .= check_plain($account_datum['type']) . ' - ' . check_plain($account_datum['credits']) . ' ' .  t('credits') . '<br />';
    }
    $print_html .= '</p></div>';
    $print_html .= '<div class="col-md-8 row" style="margin-bottom: 10px;"><p class="col-md-12" style="margin-top: 5px;">';
    $print_html .= 'API Version: ' . $sendinblueMailin->getApiVersion() . '<br />';
    $print_html .= '</p></div>';
    $form = drupal_get_form('sendinblue_logout_form');
    $print_html .= drupal_render($form);
    $print_html .= '<span class="col-md-12"><b>' . t('Contacts') . '</b></span><div class="col-md-8 row" style="margin-bottom: 10px;">';
    $print_html .= '<p class="col-md-7" style="margin-top: 5px;">' . t('You have') . ' <span id="sendinblue_total_contacts">';
    $print_html .= check_plain($totalSubscribers) . '</span> ' . t('contacts.') . '<br />';
    $print_html .= t('<a id="sendinblue_list_link" href="@sendinblue_list" target="_blank">>&nbsp;Access to the list of all my contacts</a>', array('@sendinblue_list' => 'https://my.sendinblue.com/users/list/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</p></div></div></div>';
    $print_html .= '<div class="panel panel-default row small-content"><div class="page-header">';
    $print_html .= '<strong>' . t('Transactional emails') . '</strong></div>';
    $print_html .= '<div class="panel-body">';
    $form = drupal_get_form('sendinblue_send_email_form');
    $print_html .= drupal_render($form);
    $print_html .= '</div></div></div>';
    return $print_html;
  }

  /**
   * Generate List page when log in.
   *
   * @return string
   *   A html of list page.
   */
  public static function generateListLogin() {
    $access_token = self::updateAccessToken();
    return '<iframe id="datamain" src="https://my.sendinblue.com/lists/index/access_token/' . check_plain($access_token) . '" width="100%" height="750" scrolling="yes"></iframe>';
  }

  /**
   * Generate Campaign page when log in.
   *
   * @return string
   *   A html of campaign.
   */
  public static function generateCampaignLogin() {
    $access_token = self::updateAccessToken();
    return '<iframe id="datamain" src="https://my.sendinblue.com/camp/listing/access_token/' . check_plain($access_token) . '" width="100%" height="750" scrolling="yes"></iframe>';
  }

  /**
   * Generate Statistic page when log in.
   *
   * @return string
   *   A html of statistic page.
   */
  public static function generateStatisticLogin() {
    $access_token = self::updateAccessToken();
    return '<iframe id="datamain" src="https://my.sendinblue.com/camp/message/access_token/' . check_plain($access_token) . '" width="100%" height="750" scrolling="yes"></iframe>';;
  }

  /**
   * Check if current state is logged in.
   *
   * @return bool
   *   A status of login of user.
   */
  public static function isLoggedInState() {
    $sendInBlueConfigFactory = new SendInBlueConfigFactory();

    if ($sendInBlueConfigFactory->getAccessKey()) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Check if current state is logged in V2 API.
   *
   * @return bool
   *   A status of login of user.
   */
  public static function isLoggedInV2State() {
    $sendinblueMailin = new SendinblueMailin();
    return $sendinblueMailin->getApiVersion() === SendinblueMailin::SENDINBLUE_API_VERSION_V2;
  }

  /**
   * Update access token.
   *
   * @return string
   *   An access token information.
   */
  public static function updateAccessToken() {
    $sendinblueMailin = new SendinblueMailin();
    $sendInBlueConfigFactory = new SendInBlueConfigFactory();

    // Get new access_token.
    $access_token = $sendinblueMailin->getAccessTokens();

    $sendInBlueConfigFactory->setAccessToken($access_token);
    return $access_token;
  }

  /**
   * Get email template by type.
   *
   * @param string $type
   *   A type of email.
   *
   * @return array
   *   An array of email content.
   */
  public static function getEmailTemplate($type = 'test') {
    $file = 'temp';
    $file_path = drupal_get_path('module', 'sendinblue') . '/asset/email-templates/' . $type . '/';
    // Get html content.
    $html_content = file_get_contents($file_path . $file . '.html');
    // Get text content.
    $text_content = file_get_contents($file_path . $file . '.txt');
    $templates = array('html_content' => $html_content, 'text_content' => $text_content);
    return $templates;
  }

  /**
   * Send mail.

   * @param string $type
   *   A type of email.
   * @param string $to_email
   *   A recipe address.
   * @param string $template_id
   *   A template identification.
   */
  public static function sendEmail($type, $to_email, $template_id = '-1') {
    $sendinblueMailin = new SendinblueMailin();

    $account_email = $sendinblueMailin->getAccountEmail();
    $account_username = $sendinblueMailin->getAccountUsername();

    $subjects = [
      'confirm' => t('Subscription confirmed'),
      'test' => t('[SendinBlue SMTP] test email'),
    ];

    // Set subject info.
    $subject = $subjects[$type] ?? '[SendinBlue]';
    $sender_email = !empty($account_email) ? $account_email : t('no-reply@sendinblue.com');
    $sender_name = !empty($account_username) ? $account_username : t('SendinBlue');

    // Get template html and text.
    $template_contents = self::getEmailTemplate($type);
    $html_content = $template_contents['html_content'];
    $text_content = $template_contents['text_content'];

    if ($type === "confirm" && $template_id !== '-1') {
      $template = $sendinblueMailin->getCampaign($template_id);

      if ($template !== NULL) {
        $html_content = $template->getHtmlContent();
        $subject = $template->getSubject();

        if (($template->getFromName() !== '[DEFAULT_FROM_NAME]') && ($template->getFromEmail() !== '[DEFAULT_FROM_EMAIL]')) {
          $sender_name = $template->getFromName();
          $sender_email = $template->getFromEmail();
        }
      }
    }

    // Send mail.
    $replyTo = ['email' => $sender_email, 'name' => $sender_name];
    $from = ['email' => $sender_email, 'name' => $sender_name];
    $to = ['email' => $to_email];

    $base_url = self::getBaseUrl();
    $site_domain = str_replace(['https://', 'http://'], '', $base_url);
    $html_content = str_replace(['{title}', '{site_domain}'], [$subject, $site_domain], $html_content);
    $text_content = str_replace('{site_domain}', $base_url, $text_content);

    $sendinblueMailin->sendEmail($to, $subject, $html_content, $text_content, $from, $replyTo);
  }

  /**
   * Get Base URL.
   *
   * @return string
   *   A base url of the site.
   */
  public static function getBaseUrl() {
    global $base_url;
    return $base_url;
  }

  /**
   * Get Attribute lists.
   *
   * @return array
   *   An array of attributes.
   */
  public static function getAttributeLists() {
    $sendinblueMailin = new SendinblueMailin();
    $sibAttributes = $sendinblueMailin->getAttributes();

    if (!empty($sibAttributes->getAttributes())) {
      $attributes = [];

      foreach ($sibAttributes->getAttributes() as $attribute) {
        if ($attribute->getCategory() === 'normal') {
          $attributes[] = $attribute;
        }
      }

      return $attributes;
    }

    return [];
  }

  /**
   * Get template list.
   *
   * @return array
   *   An array of template.
   */
  public static function getTemplateList() {
    $sendinblueMailin = new SendinblueMailin();
    $sibCampaigns = $sendinblueMailin->getCampaigns('template');
    $templates = array(array(
      'id' => '-1',
      'name' => 'Default',
      ),
    );
    if ($sibCampaigns) {
      foreach ($sibCampaigns->getTemplates() as $template) {
        $templates[] = array(
          'id' => $template->getId(),
          'name' => $template->getName(),
        );
      }
    }
    return $templates;
  }

  /**
   * Get lists.
   *
   * @return array
   *   An array of lists.
   */
  public static function getLists() {
    $sendinblueMailin = new SendinblueMailin();

    return $sendinblueMailin->getLists();
  }

  public static function getListNameById($list_id) {
    $sendinblueMailin = new SendinblueMailin();

    return $sendinblueMailin->getList($list_id);
  }

  /**
   * Check the email address of subscriber.
   *
   * @param string $email
   *   An email address.
   * @param string $list_id
   *   A list id.
   *
   * @return array
   *   A response information.
   */
  public static function validationEmail($email, $list_id) {
    $sendinblueMailin = new SendinblueMailin();
    $contactInfo = $sendinblueMailin->getUser($email);

    if ($contactInfo === NULL) {
      $ret = [
        'code' => 'success',
        'listid' => [],
      ];
      return $ret;
    }

    $listId = $contactInfo->getListIds();

    if ($contactInfo->isEmailBlacklisted()) {
      $ret = [
        'code' => 'update',
        'listid' => $listId,
      ];
    }
    else {
      if (!in_array($list_id, $listId, true)) {
        $ret = [
          'code' => 'success',
          'listid' => $listId,
        ];
      }
      else {
        $ret = [
          'code' => 'already_exist',
          'listid' => $listId,
        ];
      }
    }

    return $ret;
  }

  /**
   * Subscriber user.
   *
   * @param string $email
   *   An email address of subscriber.
   * @param array $info
   *   A data of subscriber.
   * @param array $listids
   *   An array of list id.
   */
  public static function subscribeUser($email, $info = array(), $listids = array()) {
    $sendinblueMailin = new SendinblueMailin();
    $sendinblueMailin->createUpdateUser($email, $info, [], $listids, NULL);
  }

  /**
   * Get subscriber data by email on drupal table.
   *
   * @param string $email
   *   An email address.
   *
   * @return string
   *   A details of subscriber.
   */
  public static function getSubscriberByEmail($email) {
    $record = db_select('sendinblue_contact', 'c')->fields('c')->condition('c.email', $email, '=')->execute()->fetchAssoc();
    return $record;
  }

  /**
   * Add subscriber on drupal table.
   *
   * @param array $data
   *   A data to add in table.
   */
  public static function addSubscriberTable($data = array()) {
    db_insert('sendinblue_contact')->fields(array(
      'email' => $data['email'],
      'info' => $data['info'],
      'code' => $data['code'],
      'is_active' => $data['is_active'],
    ))->execute();
  }

  /**
   * Update smtp details.
   *
   * @return GetSmtpDetails|null
   */
  public static function updateSmtpDetails()
  {
    $sendinblueMailin = new SendinblueMailin();
    $sendInBlueConfigFactory = new SendInBlueConfigFactory();

    $smtpDetails = $sendinblueMailin->getSmtpDetails();
    if ($smtpDetails->isEnabled()) {
      $sendInBlueConfigFactory->setSmtpDetails(drupal_json_encode($smtpDetails));
      return $smtpDetails;
    }

    $sendInBlueConfigFactory->setSendInBlueOff();
    $sendInBlueConfigFactory->setSmtpDetails(null);
    return null;
  }
}

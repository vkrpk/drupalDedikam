<?php

/**
 * Sendinblue REST client.
 */
class SendinblueApiV2 implements SendInBlueApiInterface {

  const API_URL = 'https://api.sendinblue.com/v2.0';

  /**
   * Sib ApiKey.
   *
   * @var string
   */
  public $apiKey;

  /**
   * GuzzleClient to comm with Sib.
   *
   * @var SendinblueHttpClient
   */
  public $sIBHttpClient;

  public function __construct() {
    $this->sIBHttpClient = new SendinblueHttpClient();
    $this->sIBHttpClient->setApiKey($this->apiKey);
    $this->sIBHttpClient->setBaseUrl(self::API_URL);
  }

  /**
   * {@inheritdoc}
   */
  public function setApiKey($apiKey) {
    $this->apiKey = $apiKey;
    $this->sIBHttpClient->setApiKey($this->apiKey);
  }

  /**
   * {@inheritdoc}
   */
  public function getAccount() {
    $account = $this->sIBHttpClient->get("account", "");

    $accountData = $account['data'];

    return new GetAccount(
      [
        'firstName' => $accountData[2]['first_name'],
        'lastName' => $accountData[2]['last_name'],
        'email' => $accountData[2]['email'],
        'companyName' => $accountData[2]['company'],
        'address' => [
          'city' => $accountData[2]['city'],
          'zipCode' => $accountData[2]['zip_code'],
          'country' => $accountData[2]['country'],
          'street' => $accountData[2]['address'],
        ],
        'plan' => [
          [
            'type' => $accountData[0]['plan_type'],
            'credits' => $accountData[0]['credits'],
            'creditsType' => $accountData[0]['credit_type'],
          ],
          [
            'type' => $accountData[1]['plan_type'],
            'credits' => $accountData[1]['credits'],
            'creditsType' => $accountData[1]['credit_type'],
          ],
        ],
      ]
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getTemplates() {
    $response = $this->sIBHttpClient->get("campaign/detailsv2", drupal_json_encode(["type" => 'template']));

    $templates = [];
    if (($response['code'] === 'success') && (is_array($response['data']))) {
      foreach ($response['data']['campaign_records'] as $template) {
        $templates[] = [
          'id' => $template['id'],
          'name' => $template['campaign_name'],
          'subject' => $template['subject'],
          'htmlContent' => $template['html_content'],
          'sender' => [
            'email' => $template['from_email'],
            'name' => $template['from_name'],
          ],
        ];
      }
    }

    return new GetSmtpTemplates(['templates' => $templates, 'count' => count($response['data'])]);
  }

  /**
   * {@inheritdoc}
   */
  public function getTemplate($id) {
    $templates = $this->getTemplates();

    foreach ($templates->getTemplates() as $template) {
      if ($template->getId() === $id) {
        return $template;
      }
    }

    return NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getLists() {
    $lists = $this->sIBHttpClient->get("list", "");

    return new GetLists($lists['data']);
  }

  /**
   * {@inheritdoc}
   */
  public function getList($id) {
    $list = $this->sIBHttpClient->get("list/" . $id, "");

    return new GetExtendedList([
      'id' => $list['data']['id'],
      'name' => $list['data']['name'],
      'totalSubscribers' => $list['data']['total_subscribers'],
      'totalBlacklisted' => $list['data']['total_blacklisted'],
      'createdAt' => $list['data']['entered'],
      'folderId' => $list['data']['list_parent'],
      'dynamicList' => $list['data']['dynamic_list'],
    ]);
  }

  /**
   * {@inheritdoc}
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
  ) {
    $replyto += ['email' => NULL, 'name' => NULL];
    $to += ['email' => NULL, 'name' => NULL];
    $from += ['email' => NULL, 'name' => NULL];

    $emailData = [
      "text" => $text,
      "replyto" => [$replyto['email'] => $replyto['name']],
      "html" => $html,
      "to" => [$to['email'] => $to['name']],
      "attachment" => $attachment,
      "from" => [$from['email'], $from['name']],
      "subject" => $subject,
      "headers" => $headers,
    ];

    if (!empty($cc)) {
      $cc += ['email' => NULL, 'name' => NULL];
      $emailData['cc'] = [$cc['email'] => $cc['name']];
    }

    if (!empty($bcc)) {
      $bcc += ['email' => NULL, 'name' => NULL];
      $emailData['bcc'] = [$bcc['email'] => $bcc['name']];
    }

    $message = $this->sIBHttpClient->post("email", drupal_json_encode($emailData));

    if ($message['code'] === 'success') {
      return new CreateSmtpEmail($message['data']['message-id']);
    }

    return NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getUser($email) {
    $contactInfo = $this->sIBHttpClient->get("user/" . $email, "");

    if (empty($contactInfo['data'])) {
      return NULL;
    }

    return new GetExtendedContactDetails([
      'email' => $contactInfo['data']['email'],
      'emailBlacklisted' => $contactInfo['data']['blacklisted'],
      'smsBlacklisted' => $contactInfo['data']['blacklisted_sms'],
      'createdAt' => $contactInfo['data']['entered'],
      'modifiedAt' => $contactInfo['data']['entered'],
      'listIds' => $contactInfo['data']['listid'],
    ]);
  }

  /**
   * {@inheritdoc}
   */
  public function createUpdateUser($email, array $attributes = [], array $blacklisted = [], $listid = '', $listid_unlink = '') {
    $this->sIBHttpClient->post("user/createdituser", drupal_json_encode(
      [
        "email" => $email,
        "attributes" => $attributes,
        "blacklisted" => $blacklisted,
        "listid" => $listid,
        "listid_unlink" => $listid_unlink,
      ]));
  }

  /**
   * {@inheritdoc}
   */
  public function getAttributes() {
    $sibAttributes = $this->sIBHttpClient->get("attribute/", "");
    $attributes['attributes'] = [];

    foreach ($sibAttributes['data']['normal_attributes'] as $attribute) {
      $attributes['attributes'][] = [
        'name' => $attribute['name'],
        'type' => $attribute['type'],
        'category' => 'normal',
      ];
    }

    return new GetAttributes($attributes);
  }

  /**
   * {@inheritdoc}
   */
  public function getAccessTokens() {
    return $this->sIBHttpClient->get("account/token", "")['data']['access_token'];
  }

  /**
   * {@inheritdoc}
   */
  public function getSmtpDetails() {
    $smtpDetails = $this->sIBHttpClient->get("account/smtpdetail", "");

    $sibSmtpDetails = $smtpDetails['data']['relay_data']['data'];
    $enabled = $smtpDetails['data']['relay_data']['status'] === 'enabled';

    return new GetSmtpDetails($sibSmtpDetails['username'], $sibSmtpDetails['relay'], $sibSmtpDetails['port'], $enabled);
  }

  /**
   * {@inheritdoc}
   */
  public function countUserlists(array $listids = []) {
    $userLists = $this->sIBHttpClient->post("list/display", drupal_json_encode(
      [
        'listids' => $listids,
      ]
    ));

    return $userLists['data']['total_list_records'];
  }

  /**
   * {@inheritdoc}
   */
  public function partnerDrupal() {
    $data = [];
    $data['key'] = $this->apiKey;
    $data['webaction'] = 'MAILIN-PARTNER';
    $data['partner'] = 'DRUPAL';
    $data['source'] = 'Drupal';

    return $this->sIBHttpClient->doRequestDirect($data);
  }

}

<?php

use Drupal\Component\Serialization\Json;
use Drupal\Core\Logger\LoggerChannelFactoryInterface;
use GuzzleHttp\Exception\RequestException;
use GuzzleHttp\ClientInterface;

/**
 * Sendinblue REST client.
 */
class SendinblueHttpClient {
  const WEBHOOK_WS_SIB_URL = 'http://ws.mailin.fr/';

  /**
   * Sib ApiKey.
   *
   * @var string
   */
  public $apiKey;

  /**
   * Sib BaseURL.
   *
   * @var string
   */
  public $baseUrl;

  /**
   * Logger Service.
   *
   * @var \Drupal\Core\Logger\LoggerChannelFactory
   */
  protected $loggerFactory;

  /**
   * SendinblueHttpClient constructor.
   *
   * @param \Drupal\Core\Logger\LoggerChannelFactoryInterface $logger_factory
   *   LoggerChannelFactory.
   * @param \GuzzleHttp\ClientInterface $http_client
   *   ClientInterface.
   */
  public function __construct() {
    if (!function_exists('curl_init')) {
      $msg = 'SendinBlue requires CURL module';
      $logger_factory->get('sendinblue')->error($msg);
      return;
    }
  }

  /**
   * Set the APIKEy for use HTTP cURLs.
   *
   * @param string $apikey
   *   Sendinblue APIKEY.
   */
  public function setApiKey($apikey) {
    $this->apiKey = $apikey;
  }

  /**
   * Set the URL for use HTTP cURLs.
   *
   * @param string $baseUrl
   *   SendInBlue URL.
   */
  public function setBaseUrl($baseUrl) {
    $this->baseUrl = $baseUrl;
  }

  /**
   * Do CURL request with authorization.
   *
   * @param string $resource
   *   A request action of api.
   * @param string $method
   *   A method of curl request.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   An associate array with respond data.
   */
  private function doRequest($resource, $method, $input) {
    if (!function_exists('curl_init')) {
      $msg = 'SendinBlue requires CURL module';
      watchdog('sendinblue', $msg, NULL, WATCHDOG_ERROR);
      return NULL;
    }
    $called_url = $this->baseUrl . "/" . $resource;
    $ch = curl_init($called_url);
    $auth_header = 'api-key:' . $this->apiKey;
    $content_header = "Content-Type:application/json";
    //if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
    // Windows only over-ride because of our api server.
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    //}
    curl_setopt($ch, CURLOPT_HTTPHEADER, array($auth_header, $content_header));
    //curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $method);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $input);
    $data = curl_exec($ch);
    if (curl_errno($ch)) {
      watchdog('sendinblue', 'Curl error: @error', array('@error' => curl_error($ch)), WATCHDOG_ERROR);
    }
    curl_close($ch);
    return drupal_json_decode($data);
  }

  /**
   * Do CURL request directly into sendinblue.
   *
   * @param array $data
   *  A data of curl request.
   * @return array
   *   An associate array with respond data.
   */
  public function doRequestDirect($data) {
    if (!function_exists('curl_init')) {
      $msg = 'SendinBlue requires CURL module';
      watchdog('sendinblue', $msg, NULL, WATCHDOG_ERROR);
      return NULL;
    }
    $url = 'http://ws.mailin.fr/';
    $ch = curl_init();
    $paramData = '';
    $data['source'] = 'Drupal';
    if (is_array($data))
      foreach ($data as $key => $value) {
        $paramData .= $key . '=' . urlencode($value) . '&';
      }
    else {
      $paramData = $data;
    }
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:'));
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $paramData);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_URL, $url);
    $data = curl_exec($ch);
    curl_close($ch);
    return $data;
  }

  /**
   * Get Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function get($resource, $input) {
    return $this->doRequest($resource, 'GET', $input);
  }

  /**
   * Put Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function put($resource, $input) {
    return $this->doRequest($resource, 'PUT', $input);
  }

  /**
   * Post Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function post($resource, $input) {
    return $this->doRequest($resource, 'POST', $input);
  }

  /**
   * Delete Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function delete($resource, $input) {
    return $this->doRequest($resource, 'DELETE', $input);
  }

}

<?php


/**
 *
 */
class GetSmtpDetails {

  /**
   * @var string*/
  public $userName;
  /**
   * @var string*/
  public $relay;
  /**
   * @var int*/
  public $port;
  /**
   * @var bool*/
  private $enabled;

  /**
   * GetSmtpDetails constructor.
   *
   * @param string $userName
   * @param string $relay
   * @param int $port
   * @param bool $enabled
   */
  public function __construct($userName, $relay, $port, $enabled) {
    $this->userName = $userName;
    $this->relay = $relay;
    $this->port = $port;
    $this->enabled = $enabled;
  }

  /**
   * @return string
   */
  public function getUserName() {
    return $this->userName;
  }

  /**
   * @param string $userName
   */
  public function setUserName($userName) {
    $this->userName = $userName;
  }

  /**
   * @return string
   */
  public function getRelay() {
    return $this->relay;
  }

  /**
   * @param string $relay
   */
  public function setRelay($relay) {
    $this->relay = $relay;
  }

  /**
   * @return int
   */
  public function getPort() {
    return $this->port;
  }

  /**
   * @param int $port
   */
  public function setPort($port) {
    $this->port = $port;
  }

  /**
   * @return bool
   */
  public function isEnabled() {
    return $this->enabled;
  }

  /**
   * @param bool $enabled
   */
  public function setEnabled($enabled) {
    $this->enabled = $enabled;
  }

}

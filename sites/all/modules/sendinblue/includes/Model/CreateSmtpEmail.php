<?php

/**
 *
 */
class CreateSmtpEmail {
  /**
   * @var string*/
  public $messageId;

  /**
   * CreateSmtpEmail constructor.
   *
   * @param string $messageId
   */
  public function __construct($messageId) {
    $this->messageId = $messageId;
  }

  /**
   * @return string
   */
  public function getMessageId() {
    return $this->messageId;
  }

  /**
   * @param string $messageId
   */
  public function setMessageId($messageId) {
    $this->messageId = $messageId;
  }

}

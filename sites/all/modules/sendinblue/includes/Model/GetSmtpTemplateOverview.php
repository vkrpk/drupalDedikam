<?php


/**
 *
 */
class GetSmtpTemplateOverview {

  /**
   * @var string*/
  public $id;
  /**
   * @var string*/
  public $name;
  /**
   * @var string*/
  public $subject;
  /**
   * @var string*/
  public $htmlContent;
  /**
   * @var string*/
  public $fromEmail;
  /**
   * @var string*/
  public $fromName;

  /**
   * GetSmtpTemplates construcfromr.
   *
   * @param array $data
   */
  public function __construct($data = []) {
    $this->id = $data['id'];
    $this->subject = $data['subject'];
    $this->htmlContent = $data['htmlContent'];
    $this->name = $data['name'];
    $this->fromEmail = $data['sender']['email'];
    $this->fromName = $data['sender']['name'];
  }

  /**
   * @return string
   */
  public function getId() {
    return $this->id;
  }

  /**
   * @param string $id
   */
  public function setId($id) {
    $this->id = $id;
  }

  /**
   * @return string
   */
  public function getName() {
    return $this->name;
  }

  /**
   * @param string $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * @return string
   */
  public function getSubject() {
    return $this->subject;
  }

  /**
   * @param string $subject
   */
  public function setSubject($subject) {
    $this->subject = $subject;
  }

  /**
   * @return string
   */
  public function getHtmlContent() {
    return $this->htmlContent;
  }

  /**
   * @param string $htmlContent
   */
  public function setHtmlContent($htmlContent) {
    $this->htmlContent = $htmlContent;
  }

  /**
   * @return string
   */
  public function getFromEmail() {
    return $this->fromEmail;
  }

  /**
   * @param string $fromEmail
   */
  public function setFromEmail($fromEmail) {
    $this->fromEmail = $fromEmail;
  }

  /**
   * @return string
   */
  public function getFromName() {
    return $this->fromName;
  }

  /**
   * @param string $fromName
   */
  public function setFromName($fromName) {
    $this->fromName = $fromName;
  }

}

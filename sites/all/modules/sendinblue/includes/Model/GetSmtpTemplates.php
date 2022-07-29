<?php


/**
 *
 */
class GetSmtpTemplates {

  /**
   * @var string*/
  public $count;
  /**
   * @var GetSmtpTemplateOverview[]*/
  public $templates;

  /**
   * GetSmtpTemplates constructor.
   *
   * @param array $data
   */
  public function __construct($data = []) {
    $this->count = $data['count'];

    if (!empty($data['templates'])) {
      foreach ($data['templates'] as $template) {
        $this->templates[] = new GetSmtpTemplateOverview($template);
      }
    }
  }

  /**
   * @return string
   */
  public function getCount() {
    return $this->count;
  }

  /**
   * @param string $count
   */
  public function setCount($count) {
    $this->count = $count;
  }

  /**
   * @return GetSmtpTemplateOverview[]
   */
  public function getTemplates() {
    return $this->templates;
  }

  /**
   * @param GetSmtpTemplateOverview[] $templates
   */
  public function setTemplates($templates) {
    $this->templates = $templates;
  }

}

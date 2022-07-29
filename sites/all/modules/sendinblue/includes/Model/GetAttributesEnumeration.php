<?php

/**
 *
 */
class GetAttributesEnumeration {
  /**
   * @var int*/
  public $value;
  /**
   * @var string*/
  public $label;

  /**
   * GetAttributesEnumeration constructor.
   *
   * @param int $value
   */
  public function __construct($data = []) {
    $this->value = $data['value'];
    $this->label = $data['label'];
  }

  /**
   * @return int
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * @param int $value
   */
  public function setValue($value) {
    $this->value = $value;
  }

  /**
   * @return string
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * @param string $label
   */
  public function setLabel($label) {
    $this->label = $label;
  }

}

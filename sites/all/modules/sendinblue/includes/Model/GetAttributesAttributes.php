<?php

/**
 * Class GetAttributesAttributes.
 *
 * @package Drupal\sendinblue\Tools\Model
 */
class GetAttributesAttributes {

  /**
   * @var string*/
  public $name;
  /**
   * @var string*/
  public $category;
  /**
   * @var string*/
  public $type;
  /**
   * @var string*/
  public $calculatedValue;
  /**
   * @var GetAttributesEnumeration[]*/
  public $enumeration;

  /**
   * GetAttributesAttributes constructor.
   *
   * @param array $data
   */
  public function __construct($data = []) {
    $this->name = $data['name'];
    $this->type = $data['type'] ?? NULL;
    $this->category = $data['category'];

    if (!empty($data['calculatedValue'])) {
      $this->calculatedValue = $data['calculatedValue'];
    }

    if (!empty($data['enumeration'])) {
      foreach ($data['enumeration'] as $enumeration) {
        $this->enumeration[] = new GetAttributesEnumeration($enumeration);
      }
    }
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
  public function getCategory() {
    return $this->category;
  }

  /**
   * @param string $category
   */
  public function setCategory($category) {
    $this->category = $category;
  }

  /**
   * @return string
   */
  public function getType() {
    return $this->type;
  }

  /**
   * @param string $type
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * @return string
   */
  public function getCalculatedValue() {
    return $this->calculatedValue;
  }

  /**
   * @param string $calculatedValue
   */
  public function setCalculatedValue($calculatedValue) {
    $this->calculatedValue = $calculatedValue;
  }

  /**
   * @return GetAttributesEnumeration[]
   */
  public function getEnumeration() {
    return $this->enumeration;
  }

  /**
   * @param GetAttributesEnumeration[] $enumeration
   */
  public function setEnumeration($enumeration) {
    $this->enumeration = $enumeration;
  }

}

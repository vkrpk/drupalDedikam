<?php

/**
 *
 */
class GetAttributes {
  /**
   * @var GetAttributesAttributes[]*/
  public $attributes = [];

  /**
   * GetAttributes constructor.
   *
   * @param GetAttributesAttributes[] $attributes
   */
  public function __construct($attributes) {
    if (!empty($attributes['attributes'])) {
      foreach ($attributes['attributes'] as $attribute) {
        $this->attributes[] = new GetAttributesAttributes($attribute);
      }
    }
  }

  /**
   * @return GetAttributesAttributes[]
   */
  public function getAttributes() {
    return $this->attributes;
  }

  /**
   * @param GetAttributesAttributes[] $attributes
   */
  public function setAttributes($attributes) {
    $this->attributes = $attributes;
  }

}

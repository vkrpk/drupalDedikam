<?php


/**
 *
 */
class GetExtendedClientAddress {

  /**
   * @var string*/
  public $street;
  /**
   * @var string*/
  public $city;
  /**
   * @var string*/
  public $zipCode;
  /**
   * @var string*/
  public $country;

  /**
   * GetAccount constructor.
   */
  public function __construct($data = []) {
    $this->setStreet($data['street']);
    $this->setCity($data['city']);
    $this->setZipCode($data['zipCode']);
    $this->setCountry($data['country']);
  }

  /**
   * @return string
   */
  public function getStreet() {
    return $this->street;
  }

  /**
   * @param string $street
   */
  public function setStreet($street) {
    $this->street = $street;
  }

  /**
   * @return string
   */
  public function getCity() {
    return $this->city;
  }

  /**
   * @param string $city
   */
  public function setCity($city) {
    $this->city = $city;
  }

  /**
   * @return string
   */
  public function getZipCode() {
    return $this->zipCode;
  }

  /**
   * @param string $zipCode
   */
  public function setZipCode($zipCode) {
    $this->zipCode = $zipCode;
  }

  /**
   * @return string
   */
  public function getCountry() {
    return $this->country;
  }

  /**
   * @param string $country
   */
  public function setCountry($country) {
    $this->country = $country;
  }

}

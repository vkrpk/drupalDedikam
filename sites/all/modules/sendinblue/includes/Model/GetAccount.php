<?php

/**
 *
 */
class GetAccount {

  /**
   * @var string*/
  public $email;
  /**
   * @var string*/
  public $firstName;
  /**
   * @var string*/
  public $lastName;
  /**
   * @var string*/
  public $companyName;

  /**
   * @var GetExtendedClientAddress*/
  public $address;

  /**
   * @var GetAccountPlan[]*/
  public $plan = [];

  /**
   * GetAccount constructor.
   */
  public function __construct($data = []) {
    $this->setEmail($data['email']);
    $this->setFirstName($data['firstName']);
    $this->setLastName($data['lastName']);
    $this->setCompanyName($data['companyName']);

    if (!empty($data['address'])) {
      $address = new GetExtendedClientAddress($data['address']);
      $this->setAddress($address);
    }

    if (!empty($data['plan'])) {
      foreach ($data['plan'] as $plan) {
        $accountPlan = new GetAccountPlan($plan);
        $this->addPlan($accountPlan);
      }
    }
  }

  /**
   * @return string
   */
  public function getEmail() {
    return $this->email;
  }

  /**
   * @param string $email
   */
  public function setEmail($email) {
    $this->email = $email;
  }

  /**
   * @return string
   */
  public function getFirstName() {
    return $this->firstName;
  }

  /**
   * @param string $firstName
   */
  public function setFirstName($firstName) {
    $this->firstName = $firstName;
  }

  /**
   * @return string
   */
  public function getLastName() {
    return $this->lastName;
  }

  /**
   * @param string $lastName
   */
  public function setLastName($lastName) {
    $this->lastName = $lastName;
  }

  /**
   * @return string
   */
  public function getCompanyName() {
    return $this->companyName;
  }

  /**
   * @param string $companyName
   */
  public function setCompanyName($companyName) {
    $this->companyName = $companyName;
  }

  /**
   * @return GetExtendedClientAddress
   */
  public function getAddress() {
    return $this->address;
  }

  /**
   * @param GetExtendedClientAddress $address
   */
  public function setAddress($address) {
    $this->address = $address;
  }

  /**
   * @return GetAccountPlan[]
   */
  public function getPlan() {
    return $this->plan;
  }

  /**
   * @param GetAccountPlan $plan
   */
  public function addPlan($plan) {
    $this->plan[] = $plan;
  }

}

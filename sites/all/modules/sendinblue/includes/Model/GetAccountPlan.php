<?php

/**
 *
 */
class GetAccountPlan {

  /**
   * @var string*/
  public $type;
  /**
   * @var string*/
  public $creditsType;
  /**
   * @var int*/
  public $credits;
  /**
   * @var \DateTime|null*/
  public $startDate;
  /**
   * @var \DateTime|null*/
  public $endDate;
  /**
   * @var int|null*/
  public $userLimit;

  /**
   * GetAccount constructor.
   */
  public function __construct($data = []) {
    $this->setType($data['type']);
    $this->setCreditsType($data['creditsType']);
    $this->setCredits($data['credits']);

    if (!empty($data['startDate'])) {
      $this->setStartDate(new \DateTime($data['startDate']));
    }

    if (!empty($data['endDate'])) {
      $this->setEndDate(new \DateTime($data['endDate']));
    }

    if (!empty($data['userLimit'])) {
      $this->setUserLimit($data['userLimit']);
    }
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
  public function getCreditsType() {
    return $this->creditsType;
  }

  /**
   * @param string $creditsType
   */
  public function setCreditsType($creditsType) {
    $this->creditsType = $creditsType;
  }

  /**
   * @return int
   */
  public function getCredits() {
    return $this->credits;
  }

  /**
   * @param int $credits
   */
  public function setCredits($credits) {
    $this->credits = $credits;
  }

  /**
   * @return \DateTime|null
   */
  public function getStartDate() {
    return $this->startDate;
  }

  /**
   * @param \DateTime|null $startDate
   */
  public function setStartDate($startDate) {
    $this->startDate = $startDate;
  }

  /**
   * @return \DateTime|null
   */
  public function getEndDate() {
    return $this->endDate;
  }

  /**
   * @param \DateTime|null $endDate
   */
  public function setEndDate($endDate) {
    $this->endDate = $endDate;
  }

  /**
   * @return int|null
   */
  public function getUserLimit() {
    return $this->userLimit;
  }

  /**
   * @param int|null $userLimit
   */
  public function setUserLimit($userLimit) {
    $this->userLimit = $userLimit;
  }

}

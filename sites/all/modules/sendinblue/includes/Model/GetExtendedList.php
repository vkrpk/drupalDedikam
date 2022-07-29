<?php

/**
 * Class GetExtendedList.
 *
 * @package Drupal\sendinblue\Tools\Model
 *
 *         'id' => 'int',
 * 'name' => 'string',
 * 'totalBlacklisted' => 'int',
 * 'totalSubscribers' => 'int',
 * 'folderId' => 'int',
 * 'createdAt' => '\DateTime',
 * 'campaignStats' => '\SendinBlue\Client\Model\GetExtendedListCampaignStats[]',
 * 'dynamicList' => 'bool'
 */
class GetExtendedList {
  /**
   * @var int*/
  public $id;
  /**
   * @var string*/
  public $name;
  /**
   * @var int*/
  public $totalBlacklisted = 0;
  /**
   * @var int*/
  public $totalSubscribers = 0;
  /**
   * @var int*/
  public $folderId;
  /**
   * @var \DateTime*/
  public $createdAt;
  /**
   * @var int*/
  public $dynamicList;

  /**
   * GetExtendedList constructor.
   */
  public function __construct($data = []) {

    $this->setId($data['id']);
    $this->setName($data['name']);
    $this->setTotalBlacklisted($data['totalBlacklisted']);
    $this->setTotalSubscribers($data['totalSubscribers']);
    $this->setFolderId($data['folderId']);
    $this->setDynamicList($data['dynamicList']);

    if (!empty($data['createdAt'])) {
      $this->setCreatedAt(new \DateTime($data['createdAt']));
    }
  }

  /**
   * @return int
   */
  public function getId() {
    return $this->id;
  }

  /**
   * @param int $id
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
   * @return int
   */
  public function getTotalBlacklisted() {
    return $this->totalBlacklisted;
  }

  /**
   * @param int $totalBlacklisted
   */
  public function setTotalBlacklisted($totalBlacklisted) {
    $this->totalBlacklisted = $totalBlacklisted;
  }

  /**
   * @return int
   */
  public function getTotalSubscribers() {
    return $this->totalSubscribers;
  }

  /**
   * @param int $totalSubscribers
   */
  public function setTotalSubscribers($totalSubscribers) {
    $this->totalSubscribers = $totalSubscribers;
  }

  /**
   * @return int
   */
  public function getFolderId() {
    return $this->folderId;
  }

  /**
   * @param int $folderId
   */
  public function setFolderId($folderId) {
    $this->folderId = $folderId;
  }

  /**
   * @return \DateTime
   */
  public function getCreatedAt() {
    return $this->createdAt;
  }

  /**
   * @param \DateTime $createdAt
   */
  public function setCreatedAt($createdAt) {
    $this->createdAt = $createdAt;
  }

  /**
   * @return int
   */
  public function getDynamicList() {
    return $this->dynamicList;
  }

  /**
   * @param int $dynamicList
   */
  public function setDynamicList($dynamicList) {
    $this->dynamicList = $dynamicList;
  }

}

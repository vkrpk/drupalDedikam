<?php

/**
 *
 */
class GetExtendedContactDetails
{

  /**
   * @var string
   */
  public $email;
  /**
   * @var int
   */
  public $id = 0;
  /**
   * @var bool
   */
  public $emailBlacklisted;
  /**
   * @var bool
   */
  public $smsBlacklisted;
  /**
   * @var \DateTime
   */
  public $createdAt;
  /**
   * @var \DateTime
   */
  public $modifiedAt;
  /**
   * @var array
   */
  public $listIds;
  public $attributes;

  /**
   * GetExtendedContactDetails constructor.
   */
  public function __construct($data = [])
  {
    $this->setEmail($data['email']);
    $this->setSmsBlacklisted($data['smsBlacklisted']);
    $this->setEmailBlacklisted($data['emailBlacklisted']);
    $this->setCreatedAt(new \DateTime($data['createdAt']));
    $this->setModifiedAt(new \DateTime($data['modifiedAt']));
    $this->setListIds($data['listIds']);

    if (isset($data['id'])) {
      $this->setId($data['id']);
    }
    if (isset($data['attributes'])) {
      $this->setAttributes($data['attributes']);
    }
  }

  /**
   * @return string
   */
  public function getEmail()
  {
    return $this->email;
  }

  /**
   * @param string $email
   */
  public function setEmail($email)
  {
    $this->email = $email;
  }

  /**
   * @return int
   */
  public function getId()
  {
    return $this->id;
  }

  /**
   * @param int $id
   */
  public function setId($id)
  {
    $this->id = $id;
  }

  /**
   * @return bool
   */
  public function isEmailBlacklisted()
  {
    return $this->emailBlacklisted;
  }

  /**
   * @param bool $emailBlacklisted
   */
  public function setEmailBlacklisted($emailBlacklisted)
  {
    $this->emailBlacklisted = $emailBlacklisted;
  }

  /**
   * @return bool
   */
  public function isSmsBlacklisted()
  {
    return $this->smsBlacklisted;
  }

  /**
   * @param bool $smsBlacklisted
   */
  public function setSmsBlacklisted($smsBlacklisted)
  {
    $this->smsBlacklisted = $smsBlacklisted;
  }

  /**
   * @return \DateTime
   */
  public function getCreatedAt()
  {
    return $this->createdAt;
  }

  /**
   * @param \DateTime $createdAt
   */
  public function setCreatedAt($createdAt)
  {
    $this->createdAt = $createdAt;
  }

  /**
   * @return \DateTime
   */
  public function getModifiedAt()
  {
    return $this->modifiedAt;
  }

  /**
   * @param \DateTime $modifiedAt
   */
  public function setModifiedAt($modifiedAt)
  {
    $this->modifiedAt = $modifiedAt;
  }

  /**
   * @return array
   */
  public function getListIds()
  {
    return $this->listIds;
  }

  /**
   * @param array $listIds
   */
  public function setListIds($listIds)
  {
    $this->listIds = $listIds;
  }

  /**
   * @return mixed
   */
  public function getAttributes()
  {
    return $this->attributes;
  }

  /**
   * @param mixed $attributes
   */
  public function setAttributes($attributes)
  {
    $this->attributes = $attributes;
  }

}

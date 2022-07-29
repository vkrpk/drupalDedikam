<?php

/**
 *
 */
class GetLists {

  /**
   * @var array*/
  public $lists = [];
  /**
   * @var int*/
  public $count = 0;

  /**
   * GetLists constructor.
   *
   * @param array $lists
   * @param int $count
   */
  public function __construct($lists) {
    foreach ($lists as $list) {
      $this->addList($list);
    }

    $this->setCount(count($this->getLists()));
  }

  /**
   * @return array
   */
  public function getLists() {
    return $this->lists;
  }

  /**
   * @param array $lists
   */
  public function setLists($lists) {
    $this->lists = $lists;
  }

  /**
   * @param array $list
   */
  public function addList($list) {
    $this->lists[] = $list;
  }

  /**
   * @return int
   */
  public function getCount() {
    return $this->count;
  }

  /**
   * @param int $count
   */
  public function setCount($count) {
    $this->count = $count;
  }

}

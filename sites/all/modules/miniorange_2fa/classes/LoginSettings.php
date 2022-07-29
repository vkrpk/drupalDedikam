<?php
class MoAuthLoginSettings {

  private $enable2Factor;

  private $enforceInlineRegistration;

  public function __construct() {
    $this->enable2Factor = variable_get('mo_auth_enable_two_factor', TRUE);
    $this->enforceInlineRegistration = variable_get('mo_auth_enforce_inline_registration', FALSE);
  }

  public static function withValues($enable2Factor, $enforceInlineRegistration) {
    $instance = new MoAuthLoginSettings();
    $instance->setEnable2Factor($enable2Factor);
    $instance->setEnforceInlineRegistration($enforceInlineRegistration);
    return $instance;
  }

  public function updateLoginSettings() {
    variable_set('mo_auth_enable_two_factor', $this->enable2Factor);
    variable_set('mo_auth_enforce_inline_registration', $this->enforceInlineRegistration);
  }

  public function getEnable2Factor() {
    return $this->enable2Factor;
  }

  private function setEnable2Factor($enable2Factor) {
    $this->enable2Factor = $enable2Factor;
  }

  public function getEnforceInlineRegistration() {
    return $this->enforceInlineRegistration;
  }

  private function setEnforceInlineRegistration($enforceInlineRegistration) {
    $this->enforceInlineRegistration = $enforceInlineRegistration;
  }

}
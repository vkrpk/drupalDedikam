<?php

class SendInBlueLoggerFactory
{
  const LOGGER_TYPE = 'sendinblue';

  public function emergency($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_EMERGENCY);
  }

  public function alert($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_ALERT);
  }

  public function critical($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_CRITICAL);
  }

  public function error($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_ERROR);
  }

  public function warning($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_WARNING);
  }

  public function notice($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_NOTICE);
  }

  public function info($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_INFO);
  }

  public function debug($message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, WATCHDOG_DEBUG);
  }

  public function log($level, $message, array $variables = array())
  {
    watchdog(self::LOGGER_TYPE, $message, $variables, $level);
  }
}

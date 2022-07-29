<?php
/**
 * @file
 * Contains constants class.
 */

/**
 * @file
 * This class represents constants used throughout project.
 */
class MoAuthConstants {
  public static $MODULE_NAME = 'mo_auth';
  public static $BASE_URL = 'https://auth.miniorange.com/moas';
  public static $PLUGIN_NAME = 'Drupal Two-Factor Plugin';
  public static $TRANSACTION_NAME = 'Drupal Two-Factor Module';
  public static $DEFAULT_CUSTOMER_ID = '16555';
  public static $DEFAULT_CUSTOMER_API_KEY = 'fFd2XcvTGDemZvbw1bcUesNJWEqKbbUq';
  //public static $DEFAULT_CUSTOMER_ID = '17359';
  //public static $DEFAULT_CUSTOMER_API_KEY = 'mN5Zp0SgPg4PDjm5D2CQwSD0itDbPbB6';

  public static $CUSTOMER_CHECK_API = 'https://auth.miniorange.com/moas/rest/customer/check-if-exists';
  public static $CUSTOMER_CREATE_API = 'https://auth.miniorange.com/moas/rest/customer/add';
  public static $CUSTOMER_GET_API = 'https://auth.miniorange.com/moas/rest/customer/key';
  public static $CUSTOMER_CHECK_LICENSE = 'https://auth.miniorange.com/moas/rest/customer/license';
  public static $SUPPORT_QUERY = 'https://auth.miniorange.com/moas/rest/customer/contact-us';

  public static $USERS_CREATE_API = 'https://auth.miniorange.com/moas/api/admin/users/create';
  public static $USERS_GET_API = 'https://auth.miniorange.com/moas/api/admin/users/get';
  public static $USERS_UPDATE_API = 'https://auth.miniorange.com/moas/api/admin/users/update';
  public static $USERS_SEARCH_API = 'https://auth.miniorange.com/moas/api/admin/users/search';

  public static $AUTH_CHALLENGE_API = 'https://auth.miniorange.com/moas/api/auth/challenge';
  public static $AUTH_VALIDATE_API = 'https://auth.miniorange.com/moas/api/auth/validate';
  public static $AUTH_STATUS_API = 'https://auth.miniorange.com/moas/api/auth/auth-status';
  public static $AUTH_REGISTER_API = 'https://auth.miniorange.com/moas/api/auth/register';
  // public static $AUTH_REGISTER_MOBILE_API = 'https://auth.miniorange.com/moas/api/auth/register-mobile';
  public static $AUTH_REGISTRATION_STATUS_API = 'https://auth.miniorange.com/moas/api/auth/registration-status';
  public static $AUTH_GET_GOOGLE_AUTH_API = 'https://auth.miniorange.com/moas/api/auth/google-auth-secret';
  // public static $AUTH_REGISTER_GOOGLE_AUTH_API = 'https://auth.miniorange.com/moas/api/auth/validate-google-auth-secret';
}

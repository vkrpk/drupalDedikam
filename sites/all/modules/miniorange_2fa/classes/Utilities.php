<?php
/**
 * @file
 * This file is part of miniOrange 2FA module.
 *
 * The miniOrange 2FA module is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 * miniOrange 2FA module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with miniOrange 2FA module.  If not, see <http://www.gnu.org/licenses/>.
 */
class MoAuthUtilities {

  public static function isCurlInstalled() {
    if (in_array('curl', get_loaded_extensions())) {
      return 1;
    } else {
      return 0;
    }
  }

  public static function isCustomerRegistered() {
    if (variable_get('mo_auth_customer_admin_email', NULL) == NULL || variable_get('mo_auth_customer_id', NULL) == NULL || variable_get('mo_auth_customer_token_key', NULL) == NULL || variable_get('mo_auth_customer_api_key', NULL) == NULL) {
      return FALSE;
    }
    return TRUE;
  }

  public static function getHiddenEmail($email) {
    $split = explode("@", $email);
    if (count($split) == 2) {
      $hidden_email = substr($split[0], 0, 1) . 'xxxxxx' . substr($split[0], - 1) . '@' . $split[1];
      return $hidden_email;
    }
    return $email;
  }

  public static function indentSecret($secret) {
    $strlen = strlen($secret);
    $indented = '';
    for ($i = 0; $i <= $strlen; $i = $i + 4) {
      $indented .= substr($secret, $i, 4) . ' ';
    }
    $indented = trim($indented);
    return $indented;
  }

  public static function callService($customer_id, $apiKey, $url, $json) {
    if (! self::isCurlInstalled()) {
      return json_encode(array (
        "status" => 'CURL_ERROR',
        "message" => 'PHP cURL extension is not installed or disabled.'
      ));
    }

    $ch = curl_init($url);

    $current_time_in_millis = round(microtime(TRUE) * 1000);

    $string_to_hash = $customer_id . number_format($current_time_in_millis, 0, '', '') . $apiKey;
    $hash_value = hash("sha512", $string_to_hash);

    $customer_key_header = "Customer-Key: " . $customer_id;
    $timestamp_header = "Timestamp: " . number_format($current_time_in_millis, 0, '', '');
    $authorization_header = "Authorization: " . $hash_value;
	
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_ENCODING, "");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    // Required for https urls.
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array (
      "Content-Type: application/json",
      $customer_key_header,
      $timestamp_header,
      $authorization_header
    ));
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $json);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5);
    curl_setopt($ch, CURLOPT_TIMEOUT, 20);
    $content = curl_exec($ch);
    // $httpcode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    if (curl_errno($ch)) {
      return json_encode(array (
        "status" => 'CURL_ERROR',
        "message" => curl_errno($ch)
      ));
    }
    curl_close($ch);
    return json_decode($content);
  }

}

<?php
class Bluepaid {
	// **************************************
	// PROPERTIES
	// **************************************
	
	#AVAILABLE RETURNED STATUS
	const PAYMENT_STATUS_PAID   = 'ok';
	const PAYMENT_STATUS_REFUSED = 'ko';
	const PAYMENT_STATUS_TEST = 'test';

	# AVAILABLE RETURNED TYPE
	const PAYMENT_TYPE_REFUND   = 'r';
	const PAYMENT_TYPE_TEST = 'test';
	const PAYMENT_TYPE_DEBIT = '';
	
	
	/**
	 * BLUEPAID DOMAIN
	 * @var string
	 */
	const PLATFORM_DOMAIN = 'https://paiement-securise.bluepaid.com/';
	
	/**
	 * Url of the payment page
	 * @var string
	 */
	const PLATFORM_URL = 'in.php';
	
	
	/**
	 * Actual mode for the boutic
	 * @var string
	 */
	var $debug = false;
	
	/**
	 * Total amount to pay
	 * @var float
	 */
	var $amount;
	
	/**
	 * Currency
	 * @var string
	 */
	var $currency;
	
	/**
	 * Order Id
	 * @var Int
	 */
	var $order_id;
	
	
	/**
	 * Boutic Id
	 * @var String
	 */
	var $boutic_id;
	
	
	// **************************************
	// VALIDATION OF ORDER
	// ************************************** 
	/**
	 * Order Id
	 * @var Int
	 */
	var $notify_id_client;
	/**
	 * Trans id - Generated by bluepaid
	 * @var String
	 */
	var $notify_id_trans;
	/**
	 * Transaction mode (''=>normal, 'r'=>refund, test'=>test)
	 * @var String
	 */
	var $notify_mode;
	/**
	 * Transaction Status
	 * @var String
	 */
	var $notify_etat;
	/**
	 * Token generated by uc
	 * @var String
	 */
	var $notify_divers;
	/**
	 * Amount returned by bluepaid
	 * @var Float
	 */
	var $notify_montant;
	/**
	 * Language of payment page
	 * @var String
	 */
	var $notify_langue;
	/**
	 * Transaction validation date
	 * @var Datetime
	 */
	var $notify_date_trans;
	/**
	 * Email of customer returned by bluepaid
	 * @var String
	 */
	var $notify_email_client;
	/**
	 * Boutic id returned by bluepaid
	 * @var String
	 */
	var $notify_id_boutique;
	/**
	 * Currency returned by bluepaid
	 * @var String
	 */
	var $notify_devise;
	
	
	
	
	function Bluepaid(){
		
	}
	
	/**
	 * Returns url to call for payment
	 * @static
	 * @return array[int]string
	 */
	function getPlatformAction(){
		return self::PLATFORM_DOMAIN.self::PLATFORM_URL;
	}
	
	/**
	 * Returns the iso codes of language accepted by the payment page
	 * @static
	 * @return array[int]string
	 */
	function getSupportedLanguages() {
		return array('fr', 'de', 'en', 'es', 'zh', 'it', 'ja', 'pt', 'nl');
	}
	
	/**
	 * Returns the list of ip address of bluepaid
	 * @static
	 * @return array[int]string
	 */
	function getAuthorizedServerForNotification(){
		$authorized_servers = variable_get('uc_bluepaid_ip_servers', false);
		if($authorized_servers)
			$authorized_servers = explode(';', $authorized_servers);
		if(($authorized_servers === false || empty($authorized_servers) || $authorized_servers === NULL) 
		|| ((int)count($authorized_servers) ===1 && isset($authorized_servers['']) && $authorized_servers[''] == '')){
			$authorized_servers = array('193.33.47.34', '193.33.47.35', '193.33.47.39', '87.98.218.80', '163.172.89.73', '195.154.51.98');
		}
		return $authorized_servers;
	}
	
	/**
	 * Returns the iso codes of supported currencies by merchant contract
	 * @static
	 * @return array[int]string
	 */
	function getSupportedCurrencies() {
		$supported_currencies = variable_get('uc_bluepaid_available_currencies', false);
		if($supported_currencies === false || empty($supported_currencies) 
		|| ((int)count($supported_currencies) ===1 && isset($supported_currencies['']) && $supported_currencies[''] == '')){
			$supported_currencies = array('eur', 'usd', 'gbp', 'jpy', 'cad', 'aud', 'chf',);
		}
		return $supported_currencies;
	}
	
	/**
	 * Returns the url to use in case of the user do not validate the payment page
	 * @static
	 * @return string
	 */
	function get_url_failed($keepProtocol=false){	
		$url_retour_stop = false;
		$url_retour_stop = url('uc_bluepaid/cancel', array('absolute' => TRUE));
		if(!$keepProtocol)
		{
			$url_retour_stop = str_replace('http://', '', $url_retour_stop);
			$url_retour_stop = str_replace('https://', '', $url_retour_stop);
		}return $url_retour_stop;
	}
	
	/**
	 * Returns the URL to use when the user returns to the shop after the payment
	 * @static
	 * @return string
	 */
	function get_url_success($keepProtocol=false){
		$url_retour_ok = false;		
		$url_retour_ok = url('uc_bluepaid/complete', array('absolute' => TRUE));
		if(!$keepProtocol)
		{
			$url_retour_ok = str_replace('http://', '', $url_retour_ok);
			$url_retour_ok = str_replace('https://', '', $url_retour_ok);
		}return $url_retour_ok;
	}
	
	/**
	 * Returns the mode to use for the boutic PRODUCTION/TEST
	 * @static
	 * @return boolean
	 */
	function isDebugMode(){
		return $this->debug;
	}
	
	function setDebugMode($debug=false){
		$debugMode = false;
		if($debug === 'TEST')$debugMode = true;
		$this->debug = $debugMode;
		return true;
	}
	
	/**
	 * Available type of transaction 
	 * @return boolean
	 */
	function isAvailableMode($mode){	
		$type_available = array(
			self::PAYMENT_TYPE_REFUND,
			self::PAYMENT_TYPE_DEBIT,
		);
		if (self::isDebugMode())$type_available[] = self::PAYMENT_TYPE_TEST;
		if (in_array($mode, $type_available)) return true;
		return false;
	}
	
	/**
	 * Available status of transaction 
	 * @return boolean
	 */
	function isAvailableStatus($status){
		$status_available = array(
			self::PAYMENT_STATUS_PAID,
			self::PAYMENT_STATUS_REFUSED,
		);
		if (self::isDebugMode())$status_available[] = self::PAYMENT_STATUS_TEST;
		if (in_array($status, $status_available)) return true;
		return false;
	}
	
	function generatePrivateKey(){
		
	}
	
	function convertResultTrans($status){
		switch($status){
			case 'ok':
				return 'completed';
			case 'test':
				return 'test';
			case 'ko':
				return 'failed';
			case 'erreur':
				return 'error';
			case 'attente':
				return 'pending';
			default:return false;	
		}
	} 
	
	// **************************************
	// GETTERS/SETTERS
	// ************************************** 
	public function __set($property,$value) {	
		if (property_exists($this, $property)) {
            $this->$property = $value;
        }
	}
	public function __get($property) {	
		if (property_exists($this, $property)) {
			return $this->$property;
		}
  	}	
}
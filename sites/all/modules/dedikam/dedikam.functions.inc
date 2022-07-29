<?php
/**
 * @file
 * dedikam.functions
 */

 /**
 * convertit un epoch en date selon le format donné
 */
function dedikam_epochtodate($epoch,$format)
{
	$epoch=intval($epoch);
	$dt = new DateTime("@$epoch");
    $dt->setTimezone(new DateTimeZone('Europe/Paris'));
	return $dt->format($format);
//	return $dt->format('Y-m-d H:i:sP');
}	

/**
 * convertit une date au format donnée en epoch
 */
function dedikam_datetoepoch($date,$format)
{
	$dt =  date_create_from_format($format, $date, new DateTimeZone('Europe/Paris'));
	return date_timestamp_get($dt);
}	

/**
  * routine de convertition un nombre d'octets pour affichage
 */
function dedikam_affiche_unite($valeur, $arrondi = TRUE)
{
    if( $valeur >= pow(1000, 4) )
    {
        $valeur = $arrondi ? round( $valeur / pow(1000, 4), 2) : $valeur / pow(1000, 4);
        return $valeur . ' To';
    }
    elseif( $valeur >= pow(1000, 3) )
    {
        $valeur = $arrondi ? round( $valeur / pow(1000, 3), 2) : $valeur / pow(1000, 3);
        return $valeur . ' Go';
    }
    elseif( $valeur >=  pow(1000, 2) )
    {
        $valeur = $arrondi ? round( $valeur / pow(1000, 2), 2) : $valeur / pow(1000, 2);
        return $valeur . ' Mo';
    }
    elseif( $valeur >=  1000 )
    {
        $valeur = $arrondi ? round( $valeur / pow(1000, 1), 2) : $valeur / pow(1000, 1);
        return $valeur . ' Ko';
    }
    else
    {
        return $valeur . ' o';
    }
}

function dedikam_affiche_unite_bi($valeur, $arrondi = TRUE)
{
    if( $valeur >= pow(2, 40) )
    {
        $valeur = $arrondi ? round( $valeur / pow(2, 40), 2) : $valeur / pow(2,40);
        return $valeur . ' Tio';
    }
    elseif( $valeur >= pow(2, 30) )
    {
        $valeur = $arrondi ? round( $valeur / pow(2, 30), 2) : $valeur / pow(2, 30);
        return $valeur . ' Gio';
    }
    elseif( $valeur >=  pow(2, 20) )
    {
        $valeur = $arrondi ? round( $valeur / pow(2, 20), 2) : $valeur / pow(2, 20);
        return $valeur . ' Mio';
    }
    elseif( $valeur >=  1024 )
    {
        $valeur = $arrondi ? round( $valeur / pow(2, 10), 2) : $valeur / pow(2, 10);
        return $valeur . ' Kio';
    }
    else
    {
        return $valeur . ' o';
    }
}
/**
 * calcule le prix d'une offre en fonction du niveau de sécurité et de l'espace disque choisis
 */
function dedikam_prix($level,$space,$eb=0) {
	$space = $space / 1000000000;
    $eb = $eb / 1000000000;
	$prix = $space <= 110 ? $space *  0.20 : (110*0.20)+($space-110) * 0.15;
	switch ($level) {
		case 'N2' : 
			$prix = 2 * $prix;
			break;
		case 'N3' : 
			$prix = 3 * $prix;
			break;
	}
    $prix += $eb <= 110 ? $eb *  0.20 : (110*0.20)+($eb-110) * 0.15;
 	return round($prix,2);
}

/**
 * calcule le prix d'une offre en fonction du niveau de sécurité et de l'espace disque choisis
 */
function dedikam_prix_eb($eb=0) {
    $eb = $eb / 1000000000;
    $prix = $eb <= 110 ? $eb *  0.20 : (110*0.20)+($eb-110) * 0.15;
 	return round($prix,2);
}

/*
 * Normalise le texte :
 *  suppression des espaces et accents. met tout en miniuscules
 */
function dedikam_normalize_text($text) {
 $unwanted_array = array(    'Š'=>'S', 'š'=>'s', 'Ž'=>'Z', 'ž'=>'z', 'À'=>'A', 'Á'=>'A', 'Â'=>'A', 'Ã'=>'A', 'Ä'=>'A', 'Å'=>'A', 'Æ'=>'A', 'Ç'=>'C', 'È'=>'E', 'É'=>'E',
                            'Ê'=>'E', 'Ë'=>'E', 'Ì'=>'I', 'Í'=>'I', 'Î'=>'I', 'Ï'=>'I', 'Ñ'=>'N', 'Ò'=>'O', 'Ó'=>'O', 'Ô'=>'O', 'Õ'=>'O', 'Ö'=>'O', 'Ø'=>'O', 'Ù'=>'U',
                            'Ú'=>'U', 'Û'=>'U', 'Ü'=>'U', 'Ý'=>'Y', 'Þ'=>'B', 'ß'=>'Ss', 'à'=>'a', 'á'=>'a', 'â'=>'a', 'ã'=>'a', 'ä'=>'a', 'å'=>'a', 'æ'=>'a', 'ç'=>'c',
                            'è'=>'e', 'é'=>'e', 'ê'=>'e', 'ë'=>'e', 'ì'=>'i', 'í'=>'i', 'î'=>'i', 'ï'=>'i', 'ð'=>'o', 'ñ'=>'n', 'ò'=>'o', 'ó'=>'o', 'ô'=>'o', 'õ'=>'o',
                            'ö'=>'o', 'ø'=>'o', 'ù'=>'u', 'ú'=>'u', 'û'=>'u', 'ý'=>'y', 'ý'=>'y', 'þ'=>'b', 'ÿ'=>'y' );
  return strtolower(str_replace(' ','',strtr( $text, $unwanted_array )));
}

/**
  * build random string to generate password
  */
function dedikam_random_string($length = 16) {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $randstring = '';
  for ($i = 0; $i < $length; $i++) {
    $randstring .= $characters[rand(0, strlen($characters)-1)];
  }
  return $randstring;
}

function dedikam_md5crypt ($pw, $salt="", $magic="")
    {
        $MAGIC = "$1$";
		
        if ($magic == "") $magic = $MAGIC;
        if ($salt == "") $salt = dedikam_create_salt ();
        $slist = explode ("$", $salt);
        if ($slist[0] == "1") $salt = $slist[1];

        $salt = substr ($salt, 0, 8);
        $ctx = $pw . $magic . $salt;
        $final = dedikam_pahex2bin (md5 ($pw . $salt . $pw));
		
        for ($i=strlen ($pw); $i>0; $i-=16)
        {
            if ($i > 16)
            {
                $ctx .= substr ($final,0,16);
            }
            else
            {
                $ctx .= substr ($final,0,$i);
            }
        }
        $i = strlen ($pw);

        while ($i > 0)
        {
            if ($i & 1) $ctx .= chr (0);
            else $ctx .= $pw[0];
            $i = $i >> 1;
        }
        $final = dedikam_pahex2bin (md5 ($ctx));
		
        for ($i=0;$i<1000;$i++)
        {
            $ctx1 = "";
            if ($i & 1)
            {
                $ctx1 .= $pw;
            }
            else
            {
                $ctx1 .= substr ($final,0,16);
            }
            if ($i % 3) $ctx1 .= $salt;
            if ($i % 7) $ctx1 .= $pw;
            if ($i & 1)
            {
                $ctx1 .= substr ($final,0,16);
            }
            else
            {
                $ctx1 .= $pw;
            }
            $final = dedikam_pahex2bin (md5 ($ctx1));
        }

        $passwd = "";
        $passwd .= dedikam_to64 (((ord ($final[0]) << 16) | (ord ($final[6]) << 8) | (ord ($final[12]))), 4);
        $passwd .= dedikam_to64 (((ord ($final[1]) << 16) | (ord ($final[7]) << 8) | (ord ($final[13]))), 4);
        $passwd .= dedikam_to64 (((ord ($final[2]) << 16) | (ord ($final[8]) << 8) | (ord ($final[14]))), 4);
        $passwd .= dedikam_to64 (((ord ($final[3]) << 16) | (ord ($final[9]) << 8) | (ord ($final[15]))), 4);
        $passwd .= dedikam_to64 (((ord ($final[4]) << 16) | (ord ($final[10]) << 8) | (ord ($final[5]))), 4);
        $passwd .= dedikam_to64 (ord ($final[11]), 2);
        return "$magic$salt\$$passwd";
}

function dedikam_create_salt ()
{
        srand ((double) microtime ()*1000000);
        $salt = substr (md5 (rand (0,9999999)), 0, 8);
        return $salt;
}

function dedikam_pahex2bin ($str)
    {
        if(function_exists('hex2bin'))
        {
            return hex2bin($str);
        }
        else
        {
            $len = strlen ($str);
            $nstr = "";
            for ($i=0;$i<$len;$i+=2)
            {
                $num = sscanf (substr ($str,$i,2), "%x");
                $nstr.=chr ($num[0]);
            }
            return $nstr;
        }
}

function dedikam_to64 ($v, $n)
    {
        $ITOA64 = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        $ret = "";
        while (($n - 1) >= 0)
        {
            $n--;
            $ret .= $ITOA64[$v & 0x3f];
            $v = $v >> 6;
        }
        return $ret;
}

/*
 * retourne l'adresse IP du client
 */
function dedikam_getipaddr()
{
  if (!empty($_SERVER['HTTP_CLIENT_IP']))
  //check ip from share internet
  {
    $ip=$_SERVER['HTTP_CLIENT_IP'];
  }
  elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR']))
  //to check ip is pass from proxy
  {
    $ip=$_SERVER['HTTP_X_FORWARDED_FOR'];
  }
  else
  {
    $ip=$_SERVER['REMOTE_ADDR'];
  }
  return $ip;
}

/*
 * Affiche le libellé correspondant au statut du compte
*/
function dedikam_descstatus($status) {
 $resu = "";
 switch ($status) {
  case "pending" :
   $resu = "en attente de création de l'accès";
   break;
  case "passwd" :
   $resu = "générer un nouveau mot de passe";
   break;
  case "block" :
   $resu = "verrouiller l'accès";
   break;
  case "unblock" :
   $resu = "déverrouiller l'accès";
   break;
  case "delete" :
   $resu = "supprimer l'accès";
   break;
  case "validated" :
   $resu = "accès activé";
   break;
  case "blocked" :
   $resu = "accès verrouillé";
   break;
  case "deleted" :
   $resu = "accès supprimé";
   break;
  case "unblock_delay" :
   $resu = "dévérouiller pendant 24h";
   break;
  case "recreate":
    $resu = "réinitialiser l'accès";
    break;
 }
 return $resu;
}

function dedikam_desc_periodicite($periodicite) {
    $resu = "";
    $periodicite=strtolower($periodicite);
    switch ($periodicite) {
        case "p" :
            $resu = "Déclenchement manuel";
            break;
        case "q" :
            $resu = "Quotidienne";
            break;
        case "h" :
            $resu = "Hebdomadaire";
            break;
        case "m" :
            $resu = "Mensuelle";
            break;
    }
    return $resu;
}

function dedikam_sendmail($key, $subject, $to, &$body) {
    $module = 'dedikam_member_access';
	$message = drupal_mail($module, $key,
				$to, 
				language_default(), 
				array(), 
				'support@dedikam.com', 
				FALSE);
	$message['id'] = $key;
    $message['subject'] = $subject;
	$message['body'] = $body;
	$message['body'][] = "\n---\nCordialement,\n\nL'équipe Dedikam";
	$message['body'][] = "email : ".l("support@dedikam.com","mailto:support@dedikam.com");
	$message['body'][] = "\nVous souhaitez savoir ce qui se passe au sein de la communauté DediKam être au courant des maintenances ou autres informations, Rejoignez nos communautés Facebook et Twitter :";
	$message['body'][] = l("http://www.facebook.com/DediKam","http://www.facebook.com/DediKam");
	$message['body'][] = l("http://twitter.com/DediKam","http://twitter.com/DediKam");

	$system = drupal_mail_system($module,$key);
	$message = $system->format($message);
	return $system->mail($message);
}

function dedikam_affiche_bool($value) {
    $ok = "<span class='ok' style=\"color:#00FF00\"><strong>✔</strong></span>";
    $ko = "<span class='ko' style=\"color:#FF0000\"><strong>✘</strong></span>";
    return ($value == '0' ? $ko : $ok);
}


function dedikam_readfile($filename) {
	return file_get_contents($filename);
}

function dedikam_PYDIO_crypt($password, $data) {
        // Set a random salt
        $salt = openssl_random_pseudo_bytes(8);

        $salted = '';
        $dx = '';
        // Salt the key(32) and iv(16) = 48
        while (strlen($salted) < 48) {
            $dx = md5($dx.$password.$salt, true);
            $salted .= $dx;
        }

        $key = substr($salted, 0, 32);
        $iv  = substr($salted, 32,16);

        $encrypted_data = openssl_encrypt($data, 'aes-256-cbc', $key, true, $iv);
        return base64_encode('Salted__' . $salt . $encrypted_data);
}

function dedikam_PYDIO_getNextInc() {
	$seq = db_query("select nextval('dedikam_pydio_seq') as seq")->fetchAssoc();
	return $seq['seq'];
}

/**
 * BigOrNot_CookieManager
 *
 * Copyleft 2008, Matthieu Huguet
 * All wrongs reserved
 *
 */

class				BigOrNot_CookieManager
{
  /* Server secret key */
  protected			$_secret = '';

  /* Cryptographic algorithm used to encrypt cookies data */
  protected			$_algorithm = MCRYPT_RIJNDAEL_256;
  /* Cryptographic mode (CBC, CFB ...)*/
  protected			$_mode = MCRYPT_MODE_CBC;
  /* mcrypt module resource */
  protected			$_cryptModule = null;

  /* Enable high confidentiality for cookie value (symmetric encryption) */
  protected			$_highConfidentiality = true;

  /* Enable SSL support */
  protected			$_ssl = false;

  /**
   * Constructor
   *
   * Initialize cookie manager and mcrypt module.
   *
   *
   * @param string $secret  server's secret key
   * @param array $config
   */
  public function		__construct($secret, $config = null)
  {
    if (empty($secret))
      throw new Exception('You must provide a secret key');
    $this->_secret = $secret;

    if ($config !== null && !is_array($config))
      throw new Exception('Config must be an array');
    if (is_array($config))
      {
	if (isset($config['high_confidentiality']))
	  $this->_highConfidentiality = $config['high_confidentiality'];
	if (isset($config['mcrypt_algorithm']))
	  $this->_algorithm = $config['mcrypt_algorithm'];
	if (isset($config['mcrypt_mode']))
	  $this->_mode = $config['mcrypt_mode'];
	if (isset($config['enable_ssl']))
	  $this->_ssl = $config['enable_ssl'];
      }

    $this->_cryptModule = mcrypt_module_open($this->_algorithm, '', $this->_mode, '');
    if ($this->_cryptModule === false)
      throw new Exception('Error while loading mcrypt module');
  }

  /**
   * Get the high confidentiality mode
   *
   * @return bool TRUE if cookie data encryption is enabled, or FALSE if it isn't
   */
  public function		getHighConfidentiality()
  {
    return ($this->_highConfidentiality);
  }

  /**
   * Set the high confidentiality mode
   * Enable or disable cookie data encryption
   *
   * @param bool $enable  TRUE to enable, FALSE to disable
   */
  public function		setHighConfidentiality($enable)
  {
    $this->_highConfidentiality = $enable;
    return ($this);
  }


  /**
   * Get the SSL status (enabled or disabled?)
   *
   * @return bool TRUE if SSL support is enabled, or FALSE if it isn't
   */
  public function		getSSL()
  {
    return ($this->_ssl);
  }

  /**
   * Enable SSL support (not enabled by default)
   * pro: protect against replay attack
   * con: cookie's lifetime is limited to SSL session's lifetime
   *
   * @param bool $enable TRUE to enable, FALSE to disable
   */
  public function		setSSL($enable)
  {
    $this->_ssl = $enable;
    return ($this);
  }


  /**
   * Send a secure cookie
   *
   * @param string $name cookie name
   * @param string $value cookie value
   * @param string $username user name (or ID)
   * @param integer $expire expiration time
   * @param string $path cookie path
   * @param string $domain cookie domain
   * @param bool $secure when TRUE, send the cookie only on a secure connection
   * @param bool $httponly when TRUE the cookie will be made accessible only through the HTTP protocol
   */
  public function		setCookie($cookiename, $value, $username, $expire = 0, $path = '', $domain = '', $secure = false, $httponly = null)
  {
    $secureValue = $this->_secureCookieValue($value, $username, $expire);
    $this->setClassicCookie($cookiename, $secureValue, $expire, $path, $domain, $secure, $httponly);
  }

  /**
   * Delete a cookie
   *
   * @param string $name cookie name
   * @param string $path cookie path
   * @param string $domain cookie domain
   * @param bool $secure when TRUE, send the cookie only on a secure connection
   * @param bool $httponly when TRUE the cookie will be made accessible only through the HTTP protocol
   */
  public function		deleteCookie($name, $path = '/', $domain = '', $secure = false, $httponly = null)
  {
    /* 1980-01-01 */
    $expire = 315554400;
    setcookie($name, '', $expire, $path, $domain, $secure, $httponly);
  }


  /**
   * Get a secure cookie value
   *
   * Verify the integrity of cookie data and decrypt it.
   * If the cookie is invalid, it can be automatically destroyed (default behaviour)
   *
   * @param string $cookiename cookie name
   * @param bool $deleteIfInvalid destroy the cookie if invalid
   */
  public function		getCookieValue($cookiename, $deleteIfInvalid = true)
  {
    if ($this->cookieExists($cookiename))
      {
	$cookieValues = explode('|', $_COOKIE[$cookiename]);
	if ((count($cookieValues) === 4) &&
	    ($cookieValues[1] == 0 || $cookieValues[1] >= time()))
	  {
	    $key = hash_hmac('sha1', $cookieValues[0].$cookieValues[1], $this->_secret);
	    $cookieData = base64_decode($cookieValues[2]);
	    if ($this->getHighConfidentiality())
	      $data = $this->_decrypt($cookieData, $key, md5($cookieValues[1]));
	    else
	      $data = $cookieData;

	    if ($this->_ssl && isset($_SERVER['SSL_SESSION_ID']))
	      $verifKey = hash_hmac('sha1', $cookieValues[0].$cookieValues[1].$data.$_SERVER['SSL_SESSION_ID'], $key);
	    else
	      $verifKey = hash_hmac('sha1', $cookieValues[0].$cookieValues[1].$data, $key);

	    if ($verifKey == $cookieValues[3])
	      return ($data);
	  }
      }
    if ($deleteIfInvalid)
      $this->deleteCookie($cookiename);
    return (false);
  }


  /**
   * Send a classic (unsecure) cookie
   *
   * @param string $name cookie name
   * @param string $value cookie value
   * @param integer $expire expiration time
   * @param string $path cookie path
   * @param string $domain cookie domain
   * @param bool $secure when TRUE, send the cookie only on a secure connection
   * @param bool $httponly when TRUE the cookie will be made accessible only through the HTTP protocol
   */
  public function		setClassicCookie($cookiename, $value, $expire = 0, $path = '', $domain = '', $secure = false, $httponly = null)
  {
    /* httponly option is only available for PHP version >= 5.2 */
    if ($httponly === null)
      setcookie($cookiename, $value, $expire, $path, $domain, $secure);
    else
      setcookie($cookiename, $value, $expire, $path, $domain, $secure, $httponly);
  }


  /**
   * Verify if a cookie exists
   *
   * @param string $cookiename
   * @return bool TRUE if cookie exist, or FALSE if not
   */
  public function		cookieExists($cookiename)
  {
    return (isset($_COOKIE[$cookiename]));
  }

  /**
   * Secure a cookie value
   *
   * The initial value is transformed with this protocol :
   *
   *  secureValue = username|expire|base64((value)k,expire)|HMAC(user|expire|value,k)
   *  where k = HMAC(user|expire, sk)
   *  and sk is server's secret key
   *  (value)k,md5(expire) is the result an cryptographic function (ex: AES256) on "value" with key k and initialisation vector = md5(expire)
   *
   * @param string $value unsecure value
   * @param string $username user name (or ID)
   * @param integer $expire expiration time
   * @return string secured value
   */

  protected function		_secureCookieValue($value, $username, $expire)
  {
    $key = hash_hmac('sha1', $username.$expire, $this->_secret);
    if ($this->getHighConfidentiality())
      $encryptedValue = base64_encode($this->_encrypt($value, $key, md5($expire)));
    else
      $encryptedValue = base64_encode($value);


    if ($this->_ssl && isset($_SERVER['SSL_SESSION_ID']))
      $verifKey = hash_hmac('sha1', $username . $expire . $value . $_SERVER['SSL_SESSION_ID'], $key);
    else
      $verifKey = hash_hmac('sha1', $username . $expire . $value, $key);

    $result = array($username, $expire, $encryptedValue, $verifKey);
    return(implode('|', $result));
  }

  /**
   * Encrypt a given data with a given key and a given initialisation vector
   *
   * @param string $data data to crypt
   * @param string $key secret key
   * @param string $iv initialisation vector
   * @return string encrypted data
   */
  protected function		_encrypt($data, $key, $iv)
  {
    $iv = $this->_validateIv($iv);
    $key = $this->_validateKey($key);

    mcrypt_generic_init($this->_cryptModule, $key, $iv);
    $res = mcrypt_generic($this->_cryptModule, $data);
    mcrypt_generic_deinit($this->_cryptModule);

    return ($res);
  }

  /**
   * Decrypt a given data with a given key and a given initialisation vector
   *
   * @param string $data data to crypt
   * @param string $key secret key
   * @param string $iv initialisation vector
   * @return string encrypted data
   */
  protected function		_decrypt($data, $key, $iv)
  {
    $iv = $this->_validateIv($iv);
    $key = $this->_validateKey($key);
    mcrypt_generic_init($this->_cryptModule, $key, $iv);
    $decryptedData = mdecrypt_generic($this->_cryptModule, $data);
    $res = str_replace("\x0", '', $decryptedData);
    mcrypt_generic_deinit($this->_cryptModule);
    return ($res);
  }

  /**
   * Validate Initialization vector
   *
   * If given IV is too long for the selected mcrypt algorithm, it will be truncated
   *
   * @param string $iv Initialization vector
   */
  protected function		_validateIv($iv)
  {
    $ivSize = mcrypt_enc_get_iv_size($this->_cryptModule);
    if (strlen($iv) > $ivSize)
      $iv = substr($iv, 0, $ivSize);
    return ($iv);
  }

  /**
   * Validate key
   *
   * If given key is too long for the selected mcrypt algorithm, it will be truncated
   *
   * @param string $key key
   */
  protected function	_validateKey($key)
  {
    $keySize = mcrypt_enc_get_key_size($this->_cryptModule);
    if (strlen($key) > $keySize)
      $key = substr($key, 0, $keySize);
    return ($key);
  }

}

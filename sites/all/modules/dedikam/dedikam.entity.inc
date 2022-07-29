<?php
/**
 * @file
 * dedikam.entity
 */
 
 function dedikam_entity_info() {
	$dedikam_info = array();
	
	$dedikam_info['member_access'] = array(
		// Basic Info
		'module' => 'dedikam',
		'label' => t('Member Access'),
		'plural label' => t('Member Accesses'),
		'fieldable' => FALSE,
		
		// Database table
		'base table' => 'dedikam_member_access',
		'entity keys' => array(
			'id' => 'id',
			'label' => 'name',
		),
		
		// use classes from the Entity API module
		'entity class' => 'Entity',
		'controller class' => 'EntityAPIController',
		'load hook' => 'member_access_load',
		
	);
  
	$dedikam_info['sauvegarde'] = array(
		// Basic Info
		'module' => 'dedikam',
		'label' => 'Sauvegarde',
		'plural label' => 'Sauvegardes',
		'fieldable' => FALSE,
		
		// Database table
		'base table' => 'dedikam_sauvegarde',
		'entity keys' => array(
			'id' => 'id_sauvegarde',
		),
		
		// use classes from the Entity API module
		'entity class' => 'Entity',
		'controller class' => 'EntityAPIController',
		'load hook' => 'sauvegarde_load',
		
	);

	$dedikam_info['incident'] = array(
		// Basic Info
		'module' => 'dedikam',
		'label' => 'Incident',
		'plural label' => 'Incidents',
		'fieldable' => FALSE,
		
		// Database table
		'base table' => 'dedikam_incident',
		'entity keys' => array(
			'id' => 'id_incident',
		),
		
		// use classes from the Entity API module
		'entity class' => 'Entity',
		'controller class' => 'EntityAPIController',
		'load hook' => 'incident_load',
		
	);

	$dedikam_info['serveur'] = array(
		// Basic Info
		'module' => 'dedikam',
		'label' => 'Serveur',
		'plural label' => 'Serveurs',
		'fieldable' => FALSE,
		
		// Database table
		'base table' => 'dedikam_serveur',
		'entity keys' => array(
			'id' => 'id_serveur',
		),
		
		// use classes from the Entity API module
		'entity class' => 'Entity',
		'controller class' => 'EntityAPIController',
		'load hook' => 'serveur_load',
		
	);

        $dedikam_info['expire_remind'] = array(
                // Basic Info
                'module' => 'dedikam',
                'label' => 'Rappel avant expiration',
                'plural label' => 'Rappels avant expiration',
                'fieldable' => FALSE,
                                                                                                                                                                                
                // Database table
                'base table' => 'dedikam_expire_remind',
                'entity keys' => array(
                        'id' => 'id_remind',
                ),
                                                                                                                                                                                
                // use classes from the Entity API module
                'entity class' => 'Entity',
                'controller class' => 'EntityAPIController',
                'load hook' => 'serveur_load',
                                                                                                                                                                                
        );

        $dedikam_info['iscsi'] = array(
                // Basic Info
                'module' => 'dedikam',
                'label' => 'Disque iSCSI',
                'plural label' => 'Disques iSCSI',
                'fieldable' => FALSE,
 
 
                // Database table
                'base table' => 'dedikam_iscsi',
                'entity keys' => array(
                        'id' => 'id_iscsi',
                ),
 
 
                // use classes from the Entity API module
                'entity class' => 'Entity',
                'controller class' => 'EntityAPIController',
                'load hook' => 'iscsi_load',
 
 
        );

        $dedikam_info['action'] = array(
                // Basic Info
                'module' => 'dedikam',
                'label' => 'Action',
                'plural label' => 'Actions',
                'fieldable' => FALSE,
 
 
                // Database table
                'base table' => 'dedikam_action',
                'entity keys' => array(
                        'id' => 'id_action',
                ),
 
 
                // use classes from the Entity API module
                'entity class' => 'Entity',
                'controller class' => 'EntityAPIController',
                'load hook' => 'action_load',
 
 
        );
	return $dedikam_info;
}

/**
 * Load a single record.
 *
 * @param $id
 *    The id representing the record we want to load.
 */
function member_access_load($id) {
  $access = member_access_load_multiple(array($id));
  if (array_key_exists($id,$access)) {
    return $access[$id];
  } else {
    return NULL;
  }
}

function member_access_load_name($name) {
  $access = member_access_load_multiple_name(array($name));
  if (array_key_exists($name,$access)) {
    return $access[$name];
  } else {
    return NULL;
  }
}

/**
 * Load multiple records.
 */
function member_access_load_multiple($ids = array()) {
	$res = array();
	$query = db_select('dedikam_member_access', 'd');
	$query->join('users','u','d.member_id = u.uid');
	$query->fields('d');
	$query->addField('u','name','member');
	$query->addField('u','mail');
	$query->condition('d.id',$ids,'IN');
	$results = $query->execute()->fetchAllAssoc('id');
	return $results;
}

function member_access_load_multiple_name($names = array()) {
        $res = array();
        $query = db_select('dedikam_member_access', 'd');
	watchdog('my_test', '$query: '.get_class($query), array());
        $query->join('users','u','d.member_id = u.uid');
        $query->fields('d');
        $query->addField('u','name','member');
        $query->addField('u','mail');
        $query->condition('d.name',$names,'IN');
        $results = $query->execute()->fetchAllAssoc('name');
        return $results;
}

/**
 * set user role
 */
function dedikam_set_user_role($member_id) {
	$myuser = module_invoke('user', 'load', $member_id); 
	$query = db_select('dedikam_member_access','m');
	$query->condition('member_id',$member_id);
	$query->groupBy('expire');
	$query->addExpression('max(expire)','expire_date');
	$expire_date = $query->execute()->fetchField();
	
	$user_roles = dedikam_role_get_user_rids($member_id);
	$rid_admin = 3;
	$rid_admin_dedikam = 5;
	$rid_member = 4;
	$rid_auth_user = 2;
	$rid_support = 6;
	
	if ($expire_date != '' && !in_array($rid_admin_dedikam, $user_roles) && !in_array($rid_admin, $user_roles) && !in_array($rid_support, $user_roles)) {
		// la personne n'est pas admin du site, on met à jour la date d'expiration du role
		role_expire_write_record($member_id,$rid_member,$expire_date);
		$myuser = module_invoke('user', 'load', $member_id); 
		$myuserroles = $myuser->roles;
		$myuserroles[$rid_member] = dedikam_role_get_name($rid_member);
		module_invoke('user', 'save', $myuser, array('roles' => $myuserroles)); 		
	}
	
}

/**
 * remove a role from a user
 */
function dedikam_remove_role_from_user($user, $role_name) {
  // For convenience, we'll allow user ids as well as full user objects.
  if (is_numeric($user)) {
    $user = user_load($user);
  }
  if (is_numeric($role_name)) {
    $role = $role_name;
  } else {
    $role = dedikam_role_get_rid($role_name);
  }
  $key = false;
  // Only remove the role if the user already has it.
  if (array_key_exists($role,$user->roles)) {
    $key = $role;
  }
  if ($key) {
    // Make a copy of the roles array, without the deleted one.
    $new_roles = array();
    foreach($user->roles as $id => $name) {
        if ($id != $key) {
            $new_roles[$id] = $name;
        }
    }
    user_save($user, array('roles' => $new_roles));
  }
}

/**
 * Load member access with its name
 */
function dedikam_get_member_access($name) {
	$res = array();
	$query = db_select('dedikam_member_access', 'd');
	$query->fields('d');
	$query->condition('d.name',$name,'=');
	$results = $query->execute()->fetchAll();
	return $results[0];
}

/*
 * return label for entity
 */
function dedikam_label($entity_type, $entity){
	return $entity->name;
}

function dedikam_get_member_name($member_id) {
	$user = db_select('users', 'u')
		->fields('u',array('name'))
		->condition('uid',$member_id,'=')
		->execute()
		->fetchAssoc();
	return $user['name'];
}

function dedikam_get_prorata_restant(&$member_access) {
	if ($member_access == NULL) {
		return 0;
	}
	$year = 365*24*60*60;
	$temps_realise = $member_access->expire - date('U');
	$prorata = $temps_realise / $year;
	return $prorata;
}

function dedikam_get_prorata_realise(&$member_access) {
	return 1 - dedikam_get_prorata_restant($member_access);
}

/*
 * get all member accesses for a user
 */
function dedikam_get_members($userid) {
	$res = array();
	$query = db_select('dedikam_member_access', 'd');
	$query->fields('d');
	$query->condition('d.member_id',$userid,'=');
	$results = $query->execute()->fetchAllAssoc('id');
	return $results;
}

/*
 * retourne l'id de l'attribut d'un produit en fonction de son nom
 */
function product_getattributeid($attribute) {
	$res = array();
	$query = db_select('uc_attributes','u');
	$query->fields('u',array('aid'));
	$query->condition('name',$attribute,'=');
	$results = $query->execute()->fetchAll();
	return $results[0]->aid;
}

/*
 * retourne l'id d'un produit en fonction de sa référence
 */
function product_getproductid($model) {
	$res = array();
	$query = db_select('uc_products','p');
	$query->fields('p',array('nid'));
	$query->condition('model',$model,'=');
	$results = $query->execute()->fetchAll();
	return $results[0]->nid;
}

function dedikam_role_get_rid($name) {
	$res = array();
	$query = db_select('role','r');
	$query->fields('r',array('rid'));
	$query->condition('name',$name,'=');
	$results = $query->execute()->fetchAll();
	return $results[0]->rid;
}

function dedikam_role_get_name($rid) {
	$res = array();
	$query = db_select('role','r');
	$query->fields('r',array('name'));
	$query->condition('rid',$rid,'=');
	$results = $query->execute()->fetchAll();
	return $results[0]->name;
}

function dedikam_role_get_user_rids($uid) {
	$res = array();
	$query = db_select('users_roles','r');
	$query->fields('r',array('rid'));
	$query->condition('uid',$uid,'=');
	$results = $query->execute()->fetchAll();
	$res = array();
	foreach ($results as $result) {
		$res[] = $result->rid;
	}
	return $res;
}

/*
 * L'offre passe au statut à renouveler si la date d'expiration est dans 
 * moins d'un mois
 */
function dedikam_offer_expire($expiration) {
  $diff = $expiration - date('U');
  return ($diff < 31*24*60*60);
}

/*
 * Construit une clé de sécurité pour un accès à owncloud
 */
function dedikam_owncloud_build_link($member_id,$account) {
	$ip = dedikam_getipaddr();
	$pass = dedikam_random_string(48);
	$key = $ip.'$$'.$account.'$$'.$pass;
	
	// nettoyage des anciens accès (date de validité dépassée)
	db_delete('dedikam_owncloud_connect')
		->condition('valid',date('U'),'<')
		->execute();
		
	// insertion d'un nouvel enregistrement	
	$record = array();
	$record['key'] = dedikam_md5crypt($key);
	$record['id'] = $member_id;
	$record['pass'] = $pass;
	$record['valid'] = date('U') + 60*60; // le lien n'est valide que 60 minutes
	if (drupal_write_record('dedikam_owncloud_connect',$record) == SAVED_NEW) {
		return 'dedikam_auth='.urlencode($account.'='.$record['key']);
	} else {
		return '';
	};
}

function dedikam_get_nb_free_access($user) {
  if (is_numeric($user)) {
    $user = user_load($user);
  }
  $field = field_get_items('user', $user, 'field_nb_free_account');
  if ($field) {
    return intval($field[0]['value']);
  } else {
    return 0;
  }
}

function dedikam_increment_free_access($user) {
  if (is_numeric($user)) {
    $user = user_load($user);
  } 
  $field = field_get_items('user', $user, 'field_nb_free_account');
  $value = $field[0]['value'];
  user_save($user, array('field_nb_free_account' => array('und' => array(0 => array('value' => $value + 1)))));
}

/**
 * Return true if option iscsi is enabled
*/ 
function dedikam_is_scsi_enabled($user) {
  if (is_numeric($user)) {
    $user = user_load($user);
  }
  $query = db_select('dedikam_member_access', 'd');
  $query->fields('d');
  $query->condition('d.id',$user->id,'=');
  $results = $query->execute()->fetchAssoc();
  return $result != null && $results['iscsi'] == 1;
}

/**
 * Load a single record.
 *
 * @param $id
 *    The id representing the record we want to load.
 */
function sauvegarde_load($id) {
  return sauvegarde_load_multiple(array($id))[$id];
}

/**
 * Load multiple records.
 */
function sauvegarde_load_multiple($ids = array()) {
	$res = array();
	$query = db_select('dedikam_sauvegarde', 'd');
	$query->fields('d');
	$query->condition('d.id_sauvegarde',$ids,'IN');
	$results = $query->execute()->fetchAllAssoc('id_sauvegarde');
	return $results;
}

/**
 * Load a single record.
 *
 * @param $id
 *    The id representing the record we want to load.
 */
function expire_remind_load($id) {
  return expire_remind_load_multiple(array($id))[$id];
}
                                                                                                                                                                                
/**
 * Load multiple records.
 */
function expire_remind_load_multiple($ids = array()) {
        $res = array();
        $query = db_select('dedikam_expire_remind', 'd');
        $query->fields('d');
        $query->condition('d.id_remind',$ids,'IN');
        $results = $query->execute()->fetchAllAssoc('id_remind');
        return $results;
}
       
/**
 * retourne le nombre du SI pour un compte donné
 */
function get_nb_sauvegarde_incrementale($account) {
	$res = array();
	$query = db_select('dedikam_sauvegarde', 'd');
	$query->addExpression('count(d.id_sauvegarde)','nb');
	$query->condition('d.access_id',$account);
	$query->condition('d.type_sauvegarde','I');
	$results = $query->execute()->fetchAll();
	return $results[0]->nb;
}

function get_sauvegarde_externe_for_member($member_id) {
	if (! variable_get('use_eb')) {
		return FALSE;
	}
  	$query = db_select('dedikam_sauvegarde', 'd');
	$query->fields('d');
	$query->condition('d.access_id',$member_id);
	$query->condition('d.type_sauvegarde','E');
	$results = $query->execute()->fetchAll();
	if ($results) {
        return $results[0];
    } else {
        return FALSE;
    }
}

function get_userid_for_sauvegarde($id_sauvegarde) {
	$res = array();
	$query = db_select('dedikam_sauvegarde', 's');
	$query->join('dedikam_member_access','a','s.access_id = a.id');
	$query->addField('a','member_id');
	$query->condition('s.id_sauvegarde',$id_sauvegarde);
	$results = $query->execute()->fetchAll();
	return $results[0]->member_id;
}

function get_owncloud_eb($account) {
    $resu = db_query("select count(*) as nb from oc_group_user where gid like 'EB%' and uid = :userid",
		array(':userid'=>$account));
    if ($resu) {
        $r = $resu->fetchAll();
        return ($r[0]->nb > 0);
    } else {
        return FALSE;
    }
}

function set_owncloud_eb($account,$serveur) {
/*
	ddl('set_owncloud_eb');
    db_query("delete from oc_group_user where gid like 'EB%' and uid = :userid",
		array(':userid'=>$account));
	db_query("insert into oc_group_user (gid,uid) values(:gid,:userid)",
		array(':gid' => 'EB'.$serveur, ':userid'=>$account));
*/
}

function unset_owncloud_eb($account) {
/*    ddl('unset_owncloud_eb');
	db_query("delete from oc_group_user where gid like 'EB%' and uid = :userid",
		array(':userid'=>$account));
*/
}

/**
 * Load a single record.
 *
 * @param $id
 *    The id representing the record we want to load.
 */
function incident_load($id) {
  return incident_load_multiple(array($id))[$id];
}

/**
 * Load multiple records.
 */
function incident_load_multiple($ids = array()) {
	$res = array();
	$query = db_select('dedikam_incident', 'd');
	$query->fields('d');
	$query->condition('d.id_incident',$ids,'IN');
	$results = $query->execute()->fetchAllAssoc('id_incident');
	return $results;
}

function incident_tableau_etat_services() {
	$query  = '(SELECT \'Nextcloud\' as service, min(owncloud) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
        $query .= '(SELECT \'Pydio\' as service, min(pydio) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'SSH\' as service, min(ssh) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'RSYNC\' as service, min(rsync) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'SFTP\' as service, min(sftp) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'FTPS\' as service, min(ftp) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'CIFS\' as service, min(cifs) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'WEBDAV\' as service, min(webdav) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'iSCSI\' as service, min(iscsi) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'FTP\' as service, min(si) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    if (variable_get('use_eb')) {
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'Sauv. ext.\' as service, min(eb) as etat FROM {dedikam_incident} WHERE '.date('U').' > date_debut AND (date_fin IS NULL OR '.date('U').' < date_fin))';
    }
    $results = db_query($query)->fetchAll();
    $rows = array();
    $header = array();
    foreach ($results as $result) {
       $header[$result->service] = $result->service;
       $rows[0][$result->service] = array('data' => dedikam_affiche_bool($result->etat),);
    }
    return theme('table',array(
            'header' => $header,
            'rows' => $rows,
            'attributes' => array(
                'align' => 'center',
                'border' => 0,
                'cellpadding' => 1,
                'cellspacing' => 1,
                'class' => array("table-service"),
                //'style' => 'width:140px',
            )
        )
    );
}

function incident_en_cours() {
	$res = array();
	$query = db_select('dedikam_incident', 'd');
	$query->fields('d');
	$query->where('d.date_fin is null OR '.date('U').' < d.date_fin');
    $query->orderBy('d.date_debut','DESC');
	$results = $query->execute()->fetchAll();
    $resu = '';
    foreach ($results as $result) {
        $resu .= '<H4>Incident '.(date('U')<$result->date_debut ? 'planifié au ' : ' du ').dedikam_epochtodate($result->date_debut,'d/m/Y à H:i').' :</H4>';
        // $resu .= '<ul>';
        // $resu .= '<li>Service Owncloud : '.dedikam_affiche_bool($result->owncloud).'</li>';
        // $resu .= '<li>Service SSH : '.dedikam_affiche_bool($result->ssh).'</li>';
        // $resu .= '<li>Service RSYNC : '.dedikam_affiche_bool($result->rsync).'</li>';
        // $resu .= '<li>Service SFTP : '.dedikam_affiche_bool($result->sftp).'</li>';
        // $resu .= '<li>Service sauv. inc. : '.dedikam_affiche_bool($result->si).'</li>';
        // $resu .= '<li>Service sauv. ext. : '.dedikam_affiche_bool($result->eb).'</li>';
        // $resu .= '</ul>';
        $resu .= '<p>'.$result->description.'</p>';
        if ($result->date_fin) {
            $resu .= 'Date prévisionnelle de retour à la normale : '.dedikam_epochtodate($result->date_fin,'d/m/Y à H:i');
        }
    }
    
    return ($resu == '' ? 'Tous les services fonctionnent correctement.' : $resu);
}

function incident_historique() {
	$res = array();
	$query = db_select('dedikam_incident', 'd');
	$query->fields('d');
	$query->where('d.date_fin is not null AND '.date('U').' >= d.date_fin');
    $query->orderBy('d.date_debut','DESC');
	$results = $query->execute()->fetchAll();
    $rows = array();
    foreach ($results as $result) {
        $rows[] = array(
                'data' => array(
                    array(
                        'data' => dedikam_epochtodate($result->date_debut,'d/m/Y à H:i'),
                        'style' => 'text-align:center; vertical-align:middle',
                    ),
                    array(
                        'data' => dedikam_epochtodate($result->date_fin,'d/m/Y à H:i'),
                        'style' => 'text-align:center; vertical-align:middle',
                    ),
                    array(
                        'data' =>   ($result->owncloud == 0 ? 'Owncloud<br/>' : '').
                                    ($result->pydio == 0 ? 'Pydio<br/>' : '').
                                    ($result->ssh == 0 ? 'SSH<br/>' : '').
                                    ($result->rsync == 0 ? 'RSYNC<br/>' : '').
                                    ($result->sftp == 0 ? 'SFTP<br/>' : '').
                                    ($result->ftp == 0 ? 'FTP/FTPS<br/>' : '').
                                    ($result->cifs == 0 ? 'cifs<br/>' : '').
                                    ($result->iscsi == 0 ? 'iscsi<br/>' : '').
                                    ($result->si == 0 ? 'Sauv. inc.<br/>' : '').
                                    ($result->eb == 0 ? 'Sauv. ext.<br/>' : ''),
                        'style' => 'text-align:center; vertical-align:middle',
                    ),
                    array(
                        'data' => $result->description,
                        'style' => 'text-align:left; vertical-align:middle',
                    ),
                ),
                'no_striping' => TRUE
        );
    }
    
    if ($results) {
        return theme('table',array(
            'header' => array(
                array( 
                    'data' => 'Date de début',
                    'scope' => 'col',
                    'style' => 'text-align:center; vertical-align:middle; width:150px',
                ),
                array( 
                    'data' => 'Date de fin',
                    'scope' => 'col',
                    'style' => 'text-align:center; vertical-align:middle; width:150px',
                ),
                array( 
                    'data' => 'Service impactés',
                    'scope' => 'col',
                    'style' => 'text-align:center; vertical-align:middle; width:100px',
                ),
                array( 
                    'data' => 'Description',
                    'scope' => 'col',
                    'style' => 'text-align:center; vertical-align:middle; width:300px',
                ),
            ),
            'rows' => $rows,
            'attributes' => array(
                'align' => 'center',
                'border' => 1,
                'cellpadding' => 1,
                'cellspacing' => 1,
                'style' => 'width:100%',
            )
            )
        );
    } else {
        return '<p>Néant.</p>';
    }
}

function incident_flag() {
	$query  = 'select min(etat) as min from ';
    $query .= '((SELECT \'Owncloud\' as service, min(owncloud) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
        $query .= '(SELECT \'Pydio\' as service, min(pydio) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'SSH\' as service, min(ssh) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'RSYNC\' as service, min(rsync) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'SFTP\' as service, min(sftp) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'FTP/FTPS\' as service, min(ftp) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'CIFS\' as service, min(cifs) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
    $query .= '(SELECT \'iSCSI\' as service, min(iscsi) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'Sauv. inc.\' as service, min(si) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    if (variable_get('use_eb')) {
    $query .= ' UNION ALL ';
	$query .= '(SELECT \'Sauv. ext.\' as service, min(eb) as etat FROM {dedikam_incident} WHERE date_fin IS NULL OR '.date('U').' < date_fin)';
    }
    $query .= ' ) as etat';
    $result = db_query($query)->fetchField();
    return (! is_null($result) && $result == '0') ? 0 : 1;
}

/**
 * Load a single record.
 *
 * @param $id
 *    The id representing the record we want to load.
 */
function serveur_load($id) {
  return serveur_load_multiple(array($id))[$id];
}

/**
 * Load multiple records.
 */
function serveur_load_multiple($ids = array()) {
	$res = array();
	$query = db_select('dedikam_serveur', 'd');
	$query->fields('d');
	$query->condition('d.id_serveur',$ids,'IN');
	$results = $query->execute()->fetchAllAssoc('id_serveur');
	return $results;
}

/*
 * retourne le 1er serveur capable de fournir $espace de stockage et ayant le moins d'espace disque disponible
 */
function serveur_choose($space = 0) {
    $query = "select s.id_serveur as id_serveur, s.espace as total, coalesce (s.espace - b.quota,s.espace) as restant from drupal_dedikam_serveur as s left outer join 
(select id_serveur, sum(quota) as quota from drupal_dedikam_sauvegarde group by id_serveur) as b on (s.id_serveur = b.id_serveur) where s.actif = 1 and coalesce (s.espace - b.quota,s.espace) > $space order by 3 limit 1";
    $result = db_query($query)->fetchAssoc();
    return $result;
}

function serveur_espace_restant($id_serveur) {
    $query = "select s.id_serveur, s.espace as total, coalesce (s.espace - b.quota,s.espace) as restant from drupal_dedikam_serveur as s left outer join 
(select id_serveur, sum(quota) as quota from drupal_dedikam_sauvegarde group by id_serveur) as b on (s.id_serveur = b.id_serveur) where s.id_serveur = $id_serveur";
    $result = db_query($query)->fetchAssoc();
    return ($result ? $result['restant'] : null);
}

/*
 * Mise à jour du quota pour owncloud
 */
/*
function dedikam_set_owncloud_quota($account, $quota) {
	$quota_str = strval($quota / 1000000000).' GB';
	$result = db_query("select '1' as line_exists from oc_preferences where userid = :userid and appid = 'files' and configkey='quota'", array(':userid' => $account))->fetchField();
	if ($result) {
		//update
		//ddl($result);
		db_query("update oc_preferences set configvalue=:value where userid=:userid and appid='files' and configkey='quota'",
			array(':value'=>$quota_str,':userid'=>$account));
	} else {
		//insert
		//ddl('insert '.$account);
		db_query("insert into oc_preferences (userid,appid,configkey,configvalue) values(:userid,'files','quota',:value)",
			array(':userid'=>$account,':value'=>$quota_str));

	}
}
*/

/**
 * Load a single record.
 *
 * @param $id
 *    The id representing the record we want to load.
 */
function action_load($id) {
  return action_load_multiple(array($id))[$id];
}
 
/**
 * Load multiple records.
 */
function action_load_multiple($ids = array()) {
        $res = array();
        $query = db_select('dedikam_action', 'd');
        $query->fields('d');
        $query->condition('d.id_action',$ids,'IN');
        $results = $query->execute()->fetchAllAssoc('id_action');
        return $results;
}

/**
 * Ajoute une action à réaliser en asynchrone par un script shell
 */
function ajouter_action($destination, $key, $operation) {
        $record['action_destination'] = $destination;
        $record['action_key'] = $key;
        $record['action_name'] = $operation;
        $resu = drupal_write_record('dedikam_action',$record);
	return $resu;
}
 
/**
 * Retourne l'état de la dernière action
 */
function etat_action($destination, $key) {
        $query = db_select('dedikam_action', 'd');
        $query->fields('d');
        $query->condition('d.action_destination',$destination);
        $query->condition('d.action_key',$key);
        $query->condition('d.action_status',0);
	$query->orderBy('d.id_action','DESC');
	$query->range(0,1);
        $result = $query->execute()->fetchAssoc();
		ddl($result);
        return ($result ? $result['action_name'] : 'validated');
}

/**
 * Load a single record.
 *
 * @param $id
 *    The id representing the record we want to load.
 */
function iscsi_load($id) {
  return iscsi_load_multiple(array($id))[$id];
}
 
/**
 * Load multiple records.
 */
function iscsi_load_multiple($ids = array()) {
        $res = array();
        $query = db_select('dedikam_iscsi', 'd');
        $query->fields('d');
        $query->condition('d.id_iscsi',$ids,'IN');
        $results = $query->execute()->fetchAllAssoc('id_iscsi');
        return $results;
}

/**
 * retourne le prochain disque iscsi disponible pour un accès
*/
function iscsi_next_lun_number($access) {
        if (is_numeric($access)) {
                $access = member_access_load($access);
        }
        $query = db_select('dedikam_iscsi', 'd');
        $query->addExpression('min(disk_lun)', 'min');
        $query->addExpression('max(disk_lun) + 1', 'next');
        $query->groupBy('d.access_id');
        $query->condition('d.access_id',$access->id,'=');
        $result = $query->execute()->fetchAssoc();
	$next = ($result ? $result['next'] : 0);
	if ($result && $result['min'] > 0) {
		$next = 0;
	}
	return $next;
}

/**
 * retourne la taille occupée par tous les disques iscsi d'un accès
 */
function iscsi_get_global_size($access) {
        if (is_numeric($access)) {
                $access = member_access_load($access);
        }
        $query = db_select('dedikam_iscsi', 'd');
        $query->addExpression('sum(disk_size)', 'size');
        $query->groupBy('d.access_id');
        $query->condition('d.access_id',$access->id,'=');
        $query->condition('d.disk_status','deleted','<>');
        $result = $query->execute()->fetchAssoc();
        return ($result ? $result['size'] : '0');
}

/**
 * retourne le nombre de disques iscsi d'un accès
*/
function iscsi_get_nb_disks($access) {
        if (is_numeric($access)) {
                $access = member_access_load($access);
        }
        $query = db_select('dedikam_iscsi', 'd');
        $query->addExpression('count(*)', 'nb');
        $query->groupBy('d.access_id');
        $query->condition('d.access_id',$access->id,'=');
        $result = $query->execute()->fetchAssoc();
        return ($result ? $result['nb'] : '0');
}

function dedikam_maj_quota($access) {
	if (is_numeric($access)) {
		$access = member_access_load($access);
	}
    $sizes = explode(';',trim(shell_exec('/mnt/mfs/quotaCheck.sh '.$access->name)));
dd($sizes);
    $size = $sizes[0];
    $files = $sizes[1];
    $files = is_numeric($files) ? $files : -1;
    $dirs = $sizes[2];
    $dirs = is_numeric($dirs) ? $dirs : -1;
    if (is_numeric($size)) {
        $date =date('U');
        // mise à jour de la bdd
        db_update('dedikam_member_access')
            ->condition('id', $access->id)
            ->fields(array(
                'lastquotacheck' => $size,
                'quotacheckdatetime' => $date,
                'nb_files' => $files,
                'nb_dirs' => $dirs,
                )
            )
            ->execute();
 
        // vérification du quota
        if (($size / $access->quota) > ($access->alert_quota_threshold / 100.0))
        {
            $key = 'overquota_'.$access->id;
            $subject = 'Dépassement du quota de l\'accès '.$access->name.' ( '.$access->payee.' )';
            $body = array();
            $body[] = "Bonjour ".$access->member.",\n";
            $body[] = 'L\'espace disque utilisé par l\'accès '.$access->name.' ('.$access->payee.') a
 dépassé les '.$access->alert_quota_threshold.'% des '.dedikam_affiche_unite($access->quota).' autorisés.';
			if ($access->quota > $size) {
                $body[] = '';
            }
            dedikam_sendmail($key, $subject, $access->mail, $body);
		}
	}
}


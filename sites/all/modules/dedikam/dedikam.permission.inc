<?php
/**
 * @file
 * dedikam.permission
 */

 function dedikam_permission() {
	$permissions = array(
		'admin member_access' => array(
			'title' => t('Administer Member Access'),
			'description' => t('Allows users to administer Member Access.'),
			'restrict access' => TRUE,
		),
		'edit member_access' => array(
			'title' => t('Edit Member Access'),
			'description' => t('Allows users to edit Member Access.'),
			'restrict access' => FALSE,
		),
		'view offer' => array(
			'title' => t('Buy Dedikam Offer'),
			'description' => t('Allows users to buy any Dedikam offer.'),
			'restrict access' => FALSE,
		),
		'admin incident' => array(
			'title' => 'Administrer les incidents',
			'description' => t('Autorise les utilisateurs à gérer les incidents.'),
			'restrict access' => TRUE,
		),
		'admin serveur' => array(
			'title' => 'Administrer les serveurs de sauvegardes externalisées',
			'description' => t('Autorise les utilisateurs à gérer les serveurs de sauvegardes externalisées.'),
			'restrict access' => TRUE,
		),
	);
	
	return $permissions;
}

function dedikam_member_access_access($op, $entity = NULL, $account = NULL, $entity_type = NULL) {
    global $user;
	if (!isset($account)) {
		$account = $user;
	}
	switch ($op) {
		case 'admin' :
			return user_access('admin member_access', $account);
			break;
		case 'edit' :
			return ($entity != NULL && $user->uid == $entity->member_id);
			break;
		default :
			return user_access('admin member_access', $account);
	}
}

function dedikam_offer_access($op, $account = NULL, $entity_type = NULL) {
	global $user;
	if (!isset($account)) {
		$account = $user;
	}
	switch ($op) {
		case 'view' :
			return user_access('view offer', $account);
			break;
		default :
			return user_access('view offer', $account);
	}
}

function dedikam_incident_access($op, $account = NULL, $entity_type = NULL) {
	global $user;
	if (!isset($account)) {
		$account = $user;
	}
    return user_access('admin incident', $account);
}

function dedikam_serveur_access($op, $account = NULL, $entity_type = NULL) {
	global $user;
	if (!isset($account)) {
		$account = $user;
	}
    return user_access('admin serveur', $account);
}

function dedikam_eb_access($type_sauvegarde = NULL, $eb = NULL) {
    global $user;
    if ($user->uid == 1) {
	return TRUE;
    }
    $res = array();
    if ($eb->type_sauvegarde == $type_sauvegarde) {
        $query = db_select('dedikam_member_access', 'd');
        $query->join('users','u','d.member_id = u.uid');
        $query->fields('d');
        $query->condition('d.id',$eb->access_id);
        $query->condition('u.uid',$user->uid);
        $results = $query->execute()->fetchAll();
        return sizeof($results) == 1;
    } else {
        return FALSE;
    }
}

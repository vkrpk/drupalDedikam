<?php
/**
 * Set of functions that implement dedikam cron jobs
**/

/*
 * Add a new cron queue
 */
function dedikam_cron_queue_info() {
  $queues = array();
  
  // préparation d'un nouvel accès
  $queues['dedikam_new_account'] = array(
    'worker callback' => 'dedikam_send_newaccess_callback',
  );

  // mise à jour d'un accès
  $queues['dedikam_upd_account'] = array(
    'worker callback' => 'dedikam_send_updaccess_callback',
  );

  // activation d'un accès
  $queues['dedikam_enable_account'] = array(
    'worker callback' => 'dedikam_enable_account_callback',
  );

  // désactivation d'un accès
  $queues['dedikam_disable_account'] = array(
    'worker callback' => 'dedikam_disable_account_callback',
  );

  // suppression d'un accès de la table des access
  $queues['dedikam_remove_account'] = array(
    'worker callback' => 'dedikam_remove_account_callback',
  );

  // suppression d'un accès suite à résiliation
  $queues['dedikam_termination'] = array(
    'worker callback' => 'dedikam_termination_callback',
  );

  // désactivation des si quand le quota > 95%
  $queues['dedikam_disable_si'] = array(
    'worker callback' => 'dedikam_disable_si_callback',
  );

  // nettoyage des accès supprimés
  $queues['dedikam_delete_account'] = array(
    'worker callback' => 'dedikam_delete_account_callback',
  );
  
  // relance à 1 jour de l'expiration de l'accès
  $queues['dedikam_remind_1'] = array(
    'worker callback' => 'dedikam_remind_1_callback',
  );
  
  // relance à 3 jours de l'expiration de l'accès
  $queues['dedikam_remind_3'] = array(
    'worker callback' => 'dedikam_remind_3_callback',
  );
  
  // relance à 7 jours de l'expiration de l'accès
  $queues['dedikam_remind_7'] = array(
    'worker callback' => 'dedikam_remind_7_callback',
  );
  
  // relance à 15 jours de l'expiration de l'accès
  $queues['dedikam_remind_15'] = array(
    'worker callback' => 'dedikam_remind_15_callback',
  );
  
  // relance à 31 jours de l'expiration de l'accès
  $queues['dedikam_remind_31'] = array(
    'worker callback' => 'dedikam_remind_31_callback',
  );

  return $queues;
}

/*
 *  add new dedikam accesses to cron remind queue
 */
function dedikam_add_remind_queue($remind, $previous_remind) {
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','member_id','payee','name','expire'))
				->condition('expire',date('U')+$remind*24*60*60,'<')
				->condition('expire_remind',$previous_remind,'=')
				->condition('status','deleted','!=')
				->execute()
				->fetchAll();
  $queue = DrupalQueue::get('dedikam_remind_'.strval($remind));
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }
}

/*
 * Init cron : add new dedikam accesses to cron queue
 */
function dedikam_cron() {  
  // préparation d'un nouvel accès
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','name'))
				->condition('status','newaccess')
				->execute()
				->fetchAll();
  $queue = DrupalQueue::get('dedikam_new_account');
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }

  // mise à jour d'un accès
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','name'))
				->condition('status','updaccess')
				->execute()
				->fetchAll();
  $queue = DrupalQueue::get('dedikam_upd_account');
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }

  // suppression d'un accès expiré depuis plus de 15 jours
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','name'))
                ->where('expire < '.date('U').'-15*24*60*60 AND status=\'blocked\'')
				->execute()
				->fetchAll();
  $queue = DrupalQueue::get('dedikam_remove_account');
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }

  // suppression d'un accès expiré depuis plus de 15 jours
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','name'))
                ->where(date('U').' - termination > 7*24*60*60 and a.status not in (\'delete\',\'deleted\')')
				->execute()
				->fetchAll();
  $queue = DrupalQueue::get('dedikam_termination');
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }

  // nettoyage des accès supprimés
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','name','member_id'))
				->condition('status','deleted')
				->execute()
				->fetchAll();
  $queue = DrupalQueue::get('dedikam_delete_account');
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }

  // désactivation d'un accès expiré ou dont le quota a été dépassé
/*
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','name'))
                ->where("status = 'validated' AND (expire < ".date('U')." OR (quota >= 0 AND 100 * lastquotacheck / quota >= 105 AND unblock_until is null) OR (unblock_until is not null AND unblock_until < ".date('U')."))")
  				->execute()
				->fetchAll();
*/
   $accesses = db_select('dedikam_member_access', 'a')
                                ->fields('a',array('id','name'))
                ->where("status = 'validated' AND (expire < ".date('U')." OR (unblock_until is not null AND unblock_until < ".date('U')."))")
                                ->execute()
                                ->fetchAll();

  $queue = DrupalQueue::get('dedikam_disable_account');
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }
  
  // réactivation d'un accès expiré
  $accesses = db_select('dedikam_member_access', 'a')
				->fields('a',array('id','name'))
                ->where("status = 'blocked' AND expire >= ".date('U')." AND (quota = -1 OR lastquotacheck < quota)")
				->execute()
				->fetchAll();
  $queue = DrupalQueue::get('dedikam_enable_account');
  foreach ($accesses as $access) {
    $queue->createItem($access);
  }
  
  $interval = 60*60*24;  // 1 day
  if ((REQUEST_TIME - variable_get('dedikam_dedikam_disable_si_last_check', 0)) > $interval) {
    // désactivation des si quand le quota > 95%
    $query = db_select('dedikam_member_access', 'a');
    $query->join('dedikam_sauvegarde', 'b', 'a.id = b.access_id');
    $query->fields('a',array('id','member_id'))
          ->where("b.type_sauvegarde = 'I' and b.active = 1 and a.quota > 0 and 100 * a.lastquotacheck / a.quota > 95");
    $result = $query->execute()
		    ->fetchAll();
    $queue = DrupalQueue::get('dedikam_disable_si');
    foreach ($result as $access) {
        $queue->createItem($access);
    }
    variable_set('dedikam_dedikam_disable_si_last_check', REQUEST_TIME);
  }
  
  
  // relance à 31 jours de l'expiration de l'accès
  dedikam_add_remind_queue(31,-1);

  // relance à 15 jours de l'expiration de l'accès
  dedikam_add_remind_queue(15,31);
  
  // relance à 7 jours de l'expiration de l'accès
  dedikam_add_remind_queue(7,15);

  // relance à 3 jours de l'expiration de l'accès
  dedikam_add_remind_queue(3,7);

  // relance à 0 jour de l'expiration de l'accès
  dedikam_add_remind_queue(1,3);
}

/*
 * Action callback :
 *  update database
 */
function dedikam_send_newaccess_callback($access) {
	// ddl($access, 'dedikam_send_newaccess_callback - access');
    $member_access = member_access_load($access->id);
    if (is_null($member_access)) {
        return;
    }
	// ddl($member_access, 'dedikam_send_newaccess_callback - member_access');
	// Mise à jour du rôle
	dedikam_set_user_role($access->id);
		
	// Mise à jour du quota pour owncloud
	//dedikam_set_owncloud_quota($member_access->name,$member_access->quota);
		
	// Mise à jour du mot du status
	db_update('dedikam_member_access')
			->condition('id', intval($member_access->id))
			->fields(array(
				'status' => 'pending',
				)
			)
			->execute();
}

/*
 * Action callback :
 *  update database
 */
function dedikam_send_updaccess_callback($access) {
	$member_access = member_access_load($access->id);
    if (is_null($member_access)) {
        return;
    }

	// Mise à jour du rôle
	dedikam_set_user_role($member_access->member_id);
		
	// Mise à jour du quota pour owncloud
	//dedikam_set_owncloud_quota($member_access->name,$member_access->quota);
		
	// Mise à jour du mot du status
	db_update('dedikam_member_access')
			->condition('id', intval($member_access->id))
			->fields(array(
				'status' => 'validated',
                'termination' => null,
				)
			)
			->execute();
}

/*
 * Action callback :
 *  1. update database so that access has to be deleted
 */
function dedikam_remove_account_callback($access) {
	db_update('dedikam_member_access')
		->condition('id', intval($access->id))
		->fields(array(
			'status' => 'delete',
			)
		)
		->execute();
}

/*
 * Action callback :
 *  1. update database so that access has to be deleted after account termination
 */
function dedikam_termination_callback($access) {
    $key = '_termination_'.$access->id;
    $member = member_access_load($access->id);
    if (is_null($member)) {
        return;
    }
	$member_nb_disks = iscsi_get_nb_disks($member);
	if ($member_nb_disks > 0) {
        db_update('dedikam_iscsi')
                ->condition('access_id', intval($access->id))
                ->fields(array(
                        'disk_status' => 'termination'
                        )
                )
                ->execute();
	} else {
	db_update('dedikam_member_access')
		->condition('id', intval($access->id))
		->fields(array(
			'status' => 'delete',
			)
		)
		->execute();
	}
	db_update('dedikam_sauvegarde')
		->condition('access_id', intval($access->id))
		->condition('type_sauvegarde', 'E')
		->fields(array(
			'status' => 'delete',
			)
		)
		->execute();
	db_update('dedikam_sauvegarde')
		->condition('access_id', intval($access->id))
		->condition('type_sauvegarde', 'I')
		->fields(array(
			'status' => 'deleted',
			)
		)
		->execute();
    // remboursement au prorata temporis
    /*
    $prorata= round(floor(12.0 * ($member->expire - $member->termination) / (366.0*24*60*60)) * $member->paid / 12.0,2);
    $subject = 'Demande de remboursement suite à résiliation de l\'accès '.$member->name.' ( '.$member->payee.' )';
    $body = array();
    $body[] = 'Bonjour,';
    $body[] = 'L\'adhérent '.$member->member.' a fait une demande de résiliation depuis plus de 7 jours :';
    $body[] = 'Accès : '.$member->name;
    $body[] = 'A rembourser : '.sprintf('%01.2f €',$prorata);
    $result = dedikam_sendmail($key, $subject, 'support@dedikam.com', $body);
    */
}

/*
 * Action callback :
 *  1. update database so that access has not to be deleted
 */
function dedikam_undelete_account_callback($access) {
	db_update('dedikam_member_access')
		->condition('id', intval($access->id))
		->fields(array(
			'status' => 'pending',
			)
		)
		->execute();
}

/*
 * Action callback :
 *  1. update database so that access is disabled
select u.uid, u.name as membername, count(a.id) as nb, ro.name
 from drupal_users as u left outer join drupal_dedikam_member_access as a on (a.member_id = u.uid) inner join drupal_users_roles as r on (r.uid = u.uid) inner join drupal_role as ro on (r.rid = ro.rid) group by u.uid, u.name, ro.name

select a2.*
 from drupal_dedikam_member_access as a1 inner join drupal_dedikam_member_access as a2 on (a1.member_id = a2.member_id) where a1.id = 1046 and a2.status <> 'deleted'

 */
function dedikam_delete_account_callback($access) {
	$user = module_invoke('user', 'load', $access->member_id); 
    if ($user === false) {
        // l'utilisateur n'existe plus, on efface les lignes de la table
        db_delete('dedikam_member_access')
            ->condition('member_id',$access->member_id)
            ->execute();
            return;
    }
    $query = db_select('dedikam_member_access', 'a1');
	$query->join('dedikam_member_access','a2','a1.member_id = a2.member_id');
	$query->fields('a2');
	$query->condition('a1.id',$access->id);
	$query->condition('a2.status','deleted','<>');

    $accesses = $query->execute()->fetchAll();
  	$rid_admin = 3;
	$rid_admin_dedikam = 5;
	$rid_member = 4;
	$rid_auth_user = 2;
	$rid_support = 6;

    if (! $accesses) {
        $user_roles = dedikam_role_get_user_rids($access->member_id);
        if (!in_array($rid_admin_dedikam, $user_roles) && !in_array($rid_admin, $user_roles) && !in_array($rid_support, $user_roles)) {
            dedikam_remove_role_from_user($access->member_id,$rid_member);
        }
    }
}


/*
 * Action callback :
 *  1. disable all si
 */
function dedikam_disable_si_callback($access) {
	$key = 'disable_si_'.$access->id;
    
    db_update('dedikam_sauvegarde')
		->condition('access_id', intval($access->id))
		->condition('type_sauvegarde', 'I')
		->fields(array(
			'active' => 0,
			)
		)
		->execute();
	$member = db_select('users', 'u')
				->fields('u',array('name','mail'))
				->condition('uid',$access->member_id,'=')
				->execute()
				->fetchAssoc();
    $maccess = member_access_load($access->id);
    if (is_null($maccess)) {
        return;
    }
    
    $subject = 'Desactivation des Sauvegardes Incrémentale sur votre accès Dedikam - '.$maccess->payee;
    
	$body = array();
	$body[] = "Bonjour ".$member['name'].",\n";
	$body[] = "Les sauvegardes incrémentales de votre espace de stockage sur Dedikam.com, pour l'accès ".$maccess->payee.", ont été désactivées car l'espace utilisé dépasse 95% de votre quota disque.\n\n";
	$body[] = "Vous êtes invités à supprimer vos fichiers inutiles ou à étendre votre espace de stockage.\n\n";
    
    dedikam_sendmail($key, $subject, $member['mail'], $body);
}

/*
 * Action callback :
 *  1. update database so that access is disabled
 */
function dedikam_disable_account_callback($access) {
	db_update('dedikam_member_access')
		->condition('id', intval($access->id))
		->fields(array(
			'status' => 'block',
            'unblock_until' => NULL,
			)
		)
		->execute();
}

/*
 * Action callback :
 *  1. update database so that access is enabled
 */
function dedikam_enable_account_callback($access) {
	db_update('dedikam_member_access')
		->condition('id', intval($access->id))
		->fields(array(
			'status' => 'unblock',
			)
		)
		->execute();
}

/*
 * Action callback rooted to dedikam_remind with correct parameters
 */
function dedikam_remind_1_callback($access) {
	dedikam_remind($access,1);
}

/*
 * Action callback rooted to dedikam_remind with correct parameters
 */
function dedikam_remind_3_callback($access) {
	dedikam_remind($access,3);
}

/*
 * Action callback rooted to dedikam_remind with correct parameters
 */
function dedikam_remind_7_callback($access) {
	dedikam_remind($access,7);
}

/*
 * Action callback rooted to dedikam_remind with correct parameters
 */
function dedikam_remind_15_callback($access) {
	dedikam_remind($access,15);
}

/*
 * Action callback rooted to dedikam_remind with correct parameters
 */
function dedikam_remind_31_callback($access) {
	dedikam_remind($access,31);
}

/*
 * Action callback :
 *  1. send remind mail
 *  2. update database
 */
function dedikam_remind(&$access, $days) {
    $key = '_remind_'.$access->id;
    $member = member_access_load($access->id);
    if (is_null($member)) {
        return;
    }

    $record_remind['access_name'] = $access->name;
    $record_remind['sent_date'] = date('U');
    $record_remind['expire_date'] = $access->expire;
    $record_remind['expire_remind'] = $days;
    $record_remind['mail'] = $member->mail;
    
    if ($member->quota == 2000000000 && $days > 7) {
        // accès gratuit
        $days = 7;
        $result = true;
    } else {
        $subject = 'Expiration de vos services de stockage sur Dedikam - '.$access->payee;
        $body = array();
        $body[] = "Bonjour ".$member->member.",\n";
        $body[] = "Votre espace de stockage sur Dedikam.com, pour l'accès ".$access->payee.", va arriver à expiration le ".dedikam_epochtodate($access->expire,'d/m/Y')."\n\n";
        $body[] = "Cliquez ici pour vous connecter a votre espace, et renouveler votre service : ".l("https://dedispace.dedikam.com/user/","https://dedispace.dedikam.com/user/")."\nATTENTION APRES LA DATE D'EXPIRATION DE VOTRE ACCES, VOS DONNEES SERONT SUPPRIMEES DEFINITIVEMENT\n";

        $result = dedikam_sendmail($key, $subject, $member->mail, $body);
    }
    
	$next_remind = 1;
	switch ($days) {
		case 31 :
			$next_remind = 15;
			break;
		case 15 :
			$next_remind = 7;
			break;
		case 7 :
			$next_remind = 3;
			break;
		case 3 :
			$next_remind = 1;
			break;
	}

        $record_remind['message'] = implode('',$body);
        $record_remind['status'] = $result ? 1 : 0;
	drupal_write_record('dedikam_expire_remind',$record_remind);

	if ($result) {
		db_update('dedikam_member_access')
			->condition('id', intval($access->id))
			->fields(array(
				'expire_remind' => $next_remind,
				)
			)
			->execute();
	}
}

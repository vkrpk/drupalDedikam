<?php

function dedikam_rules_action_info() {
  $actions = array(
    'dedikam_action_save_entity' => array(
      'label' => t('Save entity Member Access'),
      'group' => t('Dedikam'),
      'parameter' => array(
         'order' => array(
           'label' => t('Order'),
           'type' => 'uc_order',
         ),
      ),
	),
    'dedikam_action_delete_user' => array(
      'label' => t('Delete user\'s Member Access'),
      'group' => t('Dedikam'),
      'parameter' => array(
         'account' => array(
           'label' => 'Utilisateur supprimé',
           'type' => 'user',
         ),
      ),
	),
  );
  
  return $actions;
}

function dedikam_action_save_entity_sendmail($member_id,$new) {
    $member = member_access_load($member_id);
    $key = 'dedikam_action_save_entity_sendmail_'.$member->id;
    $subject = 'Commande acceptée pour l\'accès '.$member->name.' ( '.$member->payee.' )';
    $body = array();
    $body[] = 'Bonjour,';
    $body[] = 'Votre commande pour l\'accès '.$member->member.' ( '.$member->payee.' ) a été acceptée.';
$body[] = 'Si vous souhaitez obtenir une facture de type formel, veuillez en faire la demande au support DediKam par mail ou ticket de support.';
    $body[] = 'L\'équipe Dedikam vous remercie.';
    if ($new) {
        $body[] = 'Vous allez prochainement recevoir un mail avec vos identifiants.';
    }
    $result = dedikam_sendmail($key, $subject, $member->mail, $body);
}

/*
 * Création de l'accès dedikam une fois la commande validée
 */ 
function dedikam_action_save_entity($order) {
  $products = $order->products;
  foreach ($products as $product) {
	$record = array();
	$record_eb = array();
    	$has_eb = FALSE;
	$account = '';
	$record['member_id'] = $product->order_uid;
	$record['update_date'] = time();
	$record['payee'] = '';
	foreach($product->data['attributes'] as $key => $value) {
		switch (dedikam_normalize_text($key)) {
			case 'nomducompte' :
				$account = $value[0];
				break;
			case 'aliasducompte' :
				$record['payee'] = $value[0];
				break;
			case 'espacedisque' :
				$record['quota'] = intval($value[0])*1000000000;
				break;
			case 'niveau' :
				$record['level'] = $value[0];
				break;
			case 'sauvegardeexternalisee' :
				if (intval($value[0]) > 0) {
                    $record_eb['quota'] = intval($value[0])*1000000000;
                    $record_eb['type_sauvegarde'] = 'E';
                    $record_eb['status'] = 'create';                
                    $has_eb = TRUE;
                }
				break;
		}
	}
	if ($product->model == 'ESSAI')
	{
		$nb_essais=dedikam_get_nb_free_access($product->order_uid);
		if ($nb_essais == 4) {
			// on n'ajoute pas d'offre d'essai si 4 ont déjà été commandées
			continue;
		}
		$record['offre_essai'] = 1;
		$expire = 30;
	} else {
		$expire = 366;
	}

	if ($account != '') {
		// mise à jour du compte;
		$member = dedikam_get_member_access($account);
		if (dedikam_offer_expire($member->expire)) {
			$paid = $product->price;
			$edit_access = false;
		} else {
			$paid = $member->paid + $product->price;
			$edit_access = true;
		}
		if (drupal_write_record('dedikam_member_access',$record,strval($member->id)) == SAVED_UPDATED) {
  			if ($edit_access) {
                            $num = db_update('dedikam_member_access')
                                ->condition('id', intval($member->id))
                                ->fields(array(
                                                'paid' => $paid,
                                                'level' => $record['level'],
                                                'quota' => $record['quota'],
                                                'status' => 'updaccess',
						'update_date' => date('U'),
						'expire_remind' => -1,
                                        )
                                )
                                ->execute();			
			} else {
			    $num = db_update('dedikam_member_access')
				->condition('id', intval($member->id))
				->fields(array(
						'paid' => $paid,
						'level' => $record['level'],
						'quota' => $record['quota'],
						//'expire' => date('U') + $expire*24*60*60,
						'expire' => $member->expire + $expire*24*60*60,
                        			'date_created' => date('U'),
						'update_date' => date('U'),
						'status' => 'updaccess',
						'expire_remind' => -1,
					)
				)
				->execute();
			}
			if ($num == 0) {
				drupal_set_message(t('An error occurred and processing did not complete. Please contact this adminstrator.'),'error');
			} else {
				drupal_set_message('Compte mis à jour');
                dedikam_action_save_entity_sendmail($member->id,false);
			}
            if ($has_eb) {
                $eb = get_sauvegarde_externe_for_member($member->id);
                $record_eb['access_id'] = intval($member->id);
                //set_owncloud_eb($member->name,$eb->id_serveur);
                if ($eb) {
                    $espace_restant = serveur_espace_restant($eb->id_serveur);
                    if ($eb->quota > $record_eb['quota'] && $espace_restant + $eb->quota - $record_eb['quota'] < 0) {
                        // plus assez d'espace pour le nouveau quota
                        drupal_set_message('Erreur à la mise à jour de l\'EB : le serveur n\'a plus assez d\'espace disque disponible', 'error');
                        $record_eb['lasterror'] = 'Le serveur de sauvegardes externalisées n°'.$eb_id_serveur.' n\'a plus assez d\'espace disque disponible. Merci de contacter l\'administrateur.';
                        $record_eb['status'] = 'error';
                    }
                    $eb_id = $eb->id_sauvegarde;
                    if (drupal_write_record('dedikam_sauvegarde',$record_eb, $eb_id)) {
                            drupal_set_message('Sauvegarde externe mise à jour');
                    } else {
                        drupal_set_message('Erreur de mise à jour de la sauvegarde externe','error');
                    }
                } else {
                    $record_eb['dossier'] = '/';
                    $record_eb['periodicite'] = 'P';
                    $record_eb['active'] = 0;
                    $serveur = serveur_choose($record_eb['quota']);
                    if ($serveur) {
                        $record_eb['id_serveur'] = $serveur['id_serveur'];
                    } else {
                        drupal_set_message('Erreur à la création de l\'EB : plus aucun serveur n\'est disponible avec suffisament d\'espace disque', 'error');
                        $record_eb['lasterror'] = 'Aucun serveur de sauvegarde externalisée disponible. Merci de contacter l\'administrateur.';
                        $record_eb['status'] = 'error';
                    }
                    if (drupal_write_record('dedikam_sauvegarde',$record_eb)) {
                            drupal_set_message('Sauvegarde externe ajoutée');
                    } else {
                        drupal_set_message('Erreur à la création de la sauvegarde externe','error');
                    }
                }
            } else {
                unset_owncloud_eb($member->name);
            }
		} else {
			drupal_set_message(t('An error occurred and processing did not complete. Please contact this adminstrator.'),'error');
		};
		$account_name = $account;
		$account_quota = $record['quota'];
	} else {		
		// nouveau compte
		if (drupal_write_record('dedikam_member_access',$record) == SAVED_NEW) {
            $access_id = intval($record['id']);
			$name = sprintf('dk%1$06d',$access_id);
			$num = db_update('dedikam_member_access')
				->condition('id', $access_id)
				->fields(array(
						'name' => $name,
						'expire' => date('U') + $expire*24*60*60,
                        'date_created' => date('U'),
						'paid' => $product->price,
						'lastquotacheck' => 0,
						'status' => 'newaccess',
					)
				)
				->execute();
			if ($num == 0) {
				drupal_set_message(t('An error occurred and processing did not complete. Please contact this adminstrator.'),'error');
			} else {
				drupal_set_message('Compte créé');
                dedikam_action_save_entity_sendmail($access_id,true);
			}
			$account_name = $name;
			$account_quota = $record['quota'];
            if ($has_eb) {
                $record_eb['access_id'] = $access_id;
                $record_eb['dossier'] = '/';
                $record_eb['type_sauvegarde'] = 'E';
                $record_eb['periodicite'] = 'P';
                $record_eb['active'] = '0';
                $serveur = serveur_choose($record_eb['quota']);
                if ($serveur) {
                    $record_eb['id_serveur'] = $serveur['id_serveur'];
                    // ($member->name, $serveur['id_serveur']);
                } else {
                    drupal_set_message('Erreur à la création de l\'EB : plus aucun serveur n\'est disponible avec suffisament d\'espace disque', 'error');
                    $record_eb['lasterror'] = 'Aucun serveur de sauvegarde externalisée disponible. Merci de contacter l\'administrateur.';
                    $record_eb['status'] = 'error';
                }
                if (drupal_write_record('dedikam_sauvegarde',$record_eb)) {
                    drupal_set_message('Sauvegarde externe ajoutée');
                } else {
                    drupal_set_message('Erreur à la création de la sauvegarde externe','error');
                }
            }
		} else {
			drupal_set_message(t('An error occurred and processing did not complete. Please contact this adminstrator.'),'error');
		};
	}
	
    if ($product->model == 'ESSAI') {
        // on incrémente le compteur du nombre de comptes gratuits
        dedikam_increment_free_access($product->order_uid);
    }

    // Mise à jour du rôle
	dedikam_set_user_role($product->order_uid);
	
	// Mise à jour du quota pour owncloud
	//dedikam_set_owncloud_quota($account_name,$account_quota);
		
  }
}

function dedikam_action_delete_user($account) {
	db_update('dedikam_member_access')
			->condition('member_id', intval($account->uid))
			->fields(array(
				'status' => 'delete',
				)
			)
			->execute();
}

<?php

// Form-generating function for the editing form.
function dedikam_iscsi_form($form, $form_state, $member_access) {
	
	$iscsi_enabled = ($member_access->iscsi == '1' ? true : false);
	$afficher_actions = true;
	$member_nb_disks = iscsi_get_nb_disks($member_access);
	$status = etat_action('iscsi',$member_access->id);

        $form['iscsi']['access_id'] = array(
                '#type' => 'hidden',
                '#value' => $member_access->id,
        );

	if ($status == 'enable' ) {
		$message = 'en cours d\'activation, veuillez patienter';
		$afficher_actions = false;
	} else if ($status == 'disable' ) {
		$message = 'en cours de désactivation, veuillez patienter';
		$afficher_actions = false;
        } else if ($status == 'password' ) {
                $message = 'modification du mot de passe en cours, veuillez patienter';
                $afficher_actions = false;
	} else if ($status == 'enadisks' ) {
                $message = 'disques en cours d\'activation, veuillez patienter';
                $afficher_actions = false;
        } else if ($status == 'disdisks' ) {
                $message = 'disques en cours de désactivation, veuillez patienter';
                $afficher_actions = false;
        } else {
		$message = ($iscsi_enabled ? 'activée' : 'désactivée');
	}

	// activation / désactivation de l'option
	$form['iscsi']['etat_iscsi'] = array(
		'#markup' => "<h4>Etat de l'option iSCSI : <b>" . $message . "</b></h4>",
	);

	$nb_sessions = exec('sudo /mnt/mfs/www/scripts/iscsi/etat_montage.sh '.$member_access->name);
	$link = l('afficher', 'iscsi/ajax/nojs/'.$member_access->id, array('attributes' => array('class' => 'use-ajax')));

	if ($iscsi_enabled) {
            $table = <<<__END__
Informations de connexion :
<table>
<tr><td width="125">Serveur</td><td>iscsi.dedikam.com</td></tr>
<tr><td width="125">Target</td><td>iqn.2015.dedikam.com:{$member_access->name}</td></tr>
<tr><td width="125">Identifiant CHAP</td><td>{$member_access->name}</td></tr>
<tr><td width="125">Mot de passe</td><td><div id="password">{$link}</div></td></tr>
</table>
__END__;
            $form['iscsi']['param_connexion'] = array(
                '#markup' => $table,
            );	
	}

	if ($afficher_actions) {
	    if ($iscsi_enabled) {
		if ($member_nb_disks == 0) {
		$form['iscsi']['iscsi_disable'] = array(
                	'#type' => 'submit',
                	'#value' => "Désactiver l'option",
			'#submit' => array("dedikam_iscsi_form_submit_iscsi"),
        	);
		}
		$form['iscsi']['iscsi_reset_password'] = array(
                        '#type' => 'submit',
                        '#value' => "Réinitialiser le mot de passe",
                        '#submit' => array("dedikam_iscsi_form_submit_reset_password"),
                );
		if ($member_nb_disks > 0) {
			if ($member_access->iscsi_disks_enabled == 1) {
        		        $form['iscsi']['iscsi_disable_disks'] = array(
        		        '#type' => 'submit',
                     		'#value' => "Désactiver tous les disques",
				'#submit' => array("dedikam_iscsi_form_submit_iscsi"),
         	       		);
			} else {
                		$form['iscsi']['iscsi_enable_disks'] = array(
                        	'#type' => 'submit',
                        	'#value' => "Activer tous les disques",
				'#submit' => array("dedikam_iscsi_form_submit_iscsi"),
                		);
			}
		}
	    } else {
                $form['iscsi']['iscsi_enable'] = array(
                        '#type' => 'submit',
                        '#value' => "Activer l'option",
			'#submit' => array("dedikam_iscsi_form_submit_iscsi"),
                );
		return $form;
	    }
	} else {
		return $form;
	}

	$form['iscsi']['new_disque'] = array(
             '#markup' => "<hr/><h4>Ajout d'un nouveau disque</h4>",
        );

	// ajouter un disque
	dedikam_maj_quota($member_access);
	$member_access = member_access_load($member_access->id);
	$taillemaxi = floor((0.9*$member_access->quota - $member_access->lastquotacheck) / (1024*1024*1024)); // 90% du quota - espace occupé
	if ($member_access->quota < 0) {
		$taillemaxi = 50;
	} else if ($taillemaxi < 5) {
		$taillemaxi = 0;
	}	
	
	if ($taillemaxi == 0) {
		$form['iscsi_espace_disque']['taille_info'] = array(
			'#markup' => '<div id="console" class="clearfix"><div class="messages warning">Vous ne disposez pas d\'assez d\'espace disque libre</div></div>',
		);
	} else {
		$form['iscsi_espace_disque'] = array(
                	'#title' => "",
                	'#type' => 'fieldset',
                	'#collapsible' => FALSE,
        	);
		$form['iscsi_espace_disque']['disk_space'] = array(
	        '#title' => 'Taille du disque',
	        '#type' => 'jslider',
        	'#min' => 5,
        	'#max' => $taillemaxi,
        	'#step' => 1,
	        '#value' => 5,
	        '#orientation' => 'horizontal',
       	 	'#display_inputs' => FALSE,
       	 	'#display_values' => FALSE,
       	 	'#display_bubble' => TRUE,
       	 	'#display_values_format' => '%{value}% Go',
        	'#display_bubble_format' => '<span class="slider-value">%{value}%</br>Go</span>',
        	'#slider_length' => '500px',
            	);
		$form['iscsi_add']['iscsi_ajouter'] = array(
                        '#type' => 'submit',
                        '#value' => "Ajouter le disque",
                        '#submit' => array("dedikam_iscsi_form_submit_add_iscsi"),
                );
        }

	return $form;
}

// Activation / désactivation de l'option iSCSI
function dedikam_iscsi_form_submit_iscsi($form, &$form_state) {
	$id = $form_state['values']['access_id'];
	if ($form_state['values']['op'] == 'Activer l\'option') {
		ajouter_action('iscsi', $id, 'enable');
		dedikam_iscsi_change_password($form_state['values']['access_id'], true);
	} else if ($form_state['values']['op'] == 'Désactiver l\'option') {
		ajouter_action('iscsi', $id, 'disable');
        } else if ($form_state['values']['op'] == 'Activer tous les disques') {
		ajouter_action('iscsi', $id, 'enadisks');
        } else if ($form_state['values']['op'] == 'Désactiver tous les disques') {
		ajouter_action('iscsi', $id, 'disdisks');
        }
}

function dedikam_iscsi_form_submit_add_iscsi($form, &$form_state) {
	$access_id = $form_state['values']['access_id'];
	$next_lun = iscsi_next_lun_number($access_id);
	$record['access_id'] = $access_id;
	$record['disk_name'] = 'disk'.($next_lun+1);
	$record['disk_lun'] = $next_lun;
	$record['disk_size'] = $form_state['values']['disk_space']['value']*1000000000;
	$record['disk_status'] = 'create';
	$resu=drupal_write_record('dedikam_iscsi',$record);
	ajouter_action('iscsi', $record['id_iscsi'], 'create');
	if ($resu) {
		drupal_set_message('Disque iscsi ajouté avec succès.<br/>Vous allez recevoir un message de confirmation par mail.', 'status');
	} else {
		drupal_set_message('Erreur à la création du nouveau disque, Veuillez contacter le support.','error');
	}
}

function dedikam_iscsi_change_password($access_id, $ifisnull=true) {
	$query = db_update('dedikam_member_access')
	->condition('id', $access_id);
	if ($ifisnull) {
		$query->isNull('iscsi_password');
	}
	$nbre = $query->fields(array(
		'iscsi_password' => dedikam_random_string(16),
		)
	)
	->execute();
	if ($nbre == 1) {
		ajouter_action('iscsi', $access_id, 'password');
	}
}

function dedikam_iscsi_form_submit_reset_password($form, &$form_state) {
	$access_id = $form_state['values']['access_id'];
	dedikam_iscsi_change_password($access_id, false);
}

function ajax_iscsi_response($type = 'ajax', $member_access) {
  global $user;
  if ($type == 'ajax') {
        if ($user->uid == $member_access->member_id) {
                $output = $member_access->iscsi_password;
        } else {
                $output = 'affichage interdit';
        }
        $commands = array();
        $commands[] = ajax_command_replace('#password', $output);
        $page = array('#type' => 'ajax', '#commands' => $commands);
        ajax_deliver($page);
  }
  else {
    $output =  'Accès interdit.';
    return $output;
  }
}


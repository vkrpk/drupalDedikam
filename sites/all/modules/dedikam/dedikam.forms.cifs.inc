<?php


// Form-generating function for the editing form.
function dedikam_cifs_form($form, $form_state, $member_access) {
	
	drupal_add_library('system', 'drupal.ajax');

	$cifs_enabled = ($member_access->cifs == '1' ? true : false);
	$afficher_actions = true;

        $form['cifs']['access_id'] = array(
                '#type' => 'hidden',
                '#value' => $member_access->id,
        );

	$status = etat_action('cifs', $member_access->id);

	if ($status == 'enable' ) {
		$message = 'en cours d\'activation, veuillez patienter';
		$afficher_actions = false;
	} else if ($status == 'disable' ) {
		$message = 'en cours de désactivation, veuillez patienter';
		$afficher_actions = false;
        } else if ($status == 'password' ) {
                $message = 'modification du mot de passe en cours, veuillez patienter';
                $afficher_actions = false;
        } else if ($status == 'create' ) {
                $message = 'en cours de création, veuillez patienter';
                $afficher_actions = false;
        } else if ($status == 'delete' ) {
                $message = 'en cours de suppression, veuillez patienter';
                $afficher_actions = false;
        } else {
		$message = ($cifs_enabled ? 'activée' : 'désactivée');
	}

	// activation / désactivation de l'option
	$form['cifs']['etat_cifs'] = array(
		'#markup' => "<h4>Etat de l'option CIFS : <b>" . $message . "</b></h4>",
	);

	$nb_sessions = sizeof($nb_sessions);
	$command = 'sudo -S /mnt/mfs/www/scripts/cifs/connections.sh '.$member_access->name.' 2>&1';
	exec($command, $nb_sessions, $return);
	$user_exists = false;
	if ($return == 0) {
		$nb_sessions = sizeof($nb_sessions);
		$user_exists = true;
	} else {
		$nb_sessions = "";
	}
	if ($status == 'validated') {
		$link = '<div id="password">'.l('afficher', 'cifs/ajax/nojs/'.$member_access->id, array('attributes' => array('class' => 'use-ajax'))).'</div>';
	} else {
		$link = "";
	}

	if ($cifs_enabled) {
            $table = <<<__END__
Informations de connexion :
<table>
<tr><td width="125">Serveur</td><td>cifs.dedikam.com</td></tr>
<tr><td width="125">Dossier</td><td>{$member_access->name}_share</td></tr>
<tr><td width="125">Partage</td><td>\\\\cifs.dedikam.com\\{$member_access->name}_share</td></tr>
<tr><td width="125">Identifiant</td><td>{$member_access->name}</td></tr>
<tr><td width="125">Mot de passe</td><td>{$link}</td></tr>
</table>
__END__;
            $form['cifs']['param_connexion'] = array(
                '#markup' => $table,
            );	
	}

	if ($afficher_actions) {
	    if ($cifs_enabled) {
		$form['cifs']['cifs_disable'] = array(
                	'#type' => 'submit',
                	'#value' => "Désactiver l'option",
			'#submit' => array("dedikam_cifs_form_submit_cifs"),
        	);
		$form['cifs']['cifs_reset_password'] = array(
                        '#type' => 'submit',
                        '#value' => "Réinitialiser le mot de passe",
                        '#submit' => array("dedikam_cifs_form_submit_reset_password"),
                );
		return $form;
	    } else {
		$form['cifs']['cifs_enable'] = array(
                        '#type' => 'submit',
                        '#value' => "Activer l'option",
                        '#submit' => array("dedikam_cifs_form_submit_cifs"),
                );
		if ($user_exists) {
                $form['cifs']['cifs_delete'] = array(
                        '#type' => 'submit',
                        '#value' => "Supprimer le partage",
                        '#submit' => array("dedikam_cifs_form_submit_cifs"),
			'#attributes' => array('onclick' => 'if(!confirm("Êtes-vous sûr de vouloir supprimer le partage ? L\'opération est irrémédiable.")){return false;}'),
                );
		}
		return $form;
	    }
	}

	return $form;
}

// Activation / désactivation de l'option iSCSI
function dedikam_cifs_form_submit_cifs($form, &$form_state) {
	$id = $form_state['values']['access_id'];
	if ($form_state['values']['op'] == 'Activer l\'option') {
		dedikam_cifs_change_password($id, true);
		ajouter_action('cifs',$id,'enable');
	} else if ($form_state['values']['op'] == 'Désactiver l\'option') {
		ajouter_action('cifs',$id,'disable');
	} else if ($form_state['values']['op'] == 'Supprimer le partage') {
                ajouter_action('cifs',$id,'delete');
        }
}

function dedikam_cifs_change_password($access_id, $ifisnull=true) {
	$query = db_update('dedikam_member_access')
	->condition('id', $access_id);
	if ($ifisnull) {
		$query->isNull('cifs_password');
	}
	$nbres = $query->fields(array(
		'cifs_password' => dedikam_random_string(16),
		)
	)
	->execute();
	if ($nbres == 1) {
		ajouter_action('cifs',$access_id,'password');
	}
}

function dedikam_cifs_form_submit_reset_password($form, &$form_state) {
	$access_id = $form_state['values']['access_id'];
	dedikam_cifs_change_password($access_id, false);
}

function ajax_cifs_response($type = 'ajax', $member_access) {
  global $user;
  if ($type == 'ajax') {
  	if ($user->uid == $member_access->member_id) {
    		$output = $member_access->cifs_password;
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

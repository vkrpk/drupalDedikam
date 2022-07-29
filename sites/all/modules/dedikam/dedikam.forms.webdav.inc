<?php


// Form-generating function for the editing form.
function dedikam_webdav_form($form, $form_state, $member_access) {
	
	drupal_add_library('system', 'drupal.ajax');

	$webdav_enabled = ($member_access->webdav == '1' ? true : false);
	$afficher_actions = true;

        $form['webdav']['access_id'] = array(
                '#type' => 'hidden',
                '#value' => $member_access->id,
        );

	$status = etat_action('webdav', $member_access->id);

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
        } else {
		$message = ($webdav_enabled ? 'activée' : 'désactivée');
	}

	// activation / désactivation de l'option
	$form['webdav']['etat_webdav'] = array(
		'#markup' => "<h4>Etat de l'option WEBDAV : <b>" . $message . "</b></h4>",
	);

	if ($status == 'validated') {
		$link = '<div id="password">'.l('afficher', 'webdav/ajax/nojs/'.$member_access->id, array('attributes' => array('class' => 'use-ajax'))).'</div>';
	} else {
		$link = "";
	}

	if ($webdav_enabled) {
	    $lwebdav=l("https://webdav.dedikam.com/{$member_access->name}","https://webdav.dedikam.com/{$member_access->name}");
            $table = <<<__END__
Informations de connexion :
<table>
<tr><td width="125">Serveur</td><td>{$lwebdav}</td></tr>
<tr><td width="125">Identifiant</td><td>{$member_access->name}</td></tr>
<tr><td width="125">Mot de passe</td><td>{$link}</td></tr>
</table>
__END__;
            $form['webdav']['param_connexion'] = array(
                '#markup' => $table,
            );	
	}

	if ($afficher_actions) {
	    if ($webdav_enabled) {
		$form['webdav']['webdav_disable'] = array(
                	'#type' => 'submit',
                	'#value' => "Désactiver l'option",
			'#submit' => array("dedikam_webdav_form_submit_webdav"),
        	);
		$form['webdav']['webdav_reset_password'] = array(
                        '#type' => 'submit',
                        '#value' => "Réinitialiser le mot de passe",
                        '#submit' => array("dedikam_webdav_form_submit_reset_password"),
                );
		return $form;
	    } else {
		$form['webdav']['webdav_enable'] = array(
                        '#type' => 'submit',
                        '#value' => "Activer l'option",
                        '#submit' => array("dedikam_webdav_form_submit_webdav"),
                );
		return $form;
	    }
	}

	return $form;
}

// Activation / désactivation de l'option iSCSI
function dedikam_webdav_form_submit_webdav($form, &$form_state) {
	$id = $form_state['values']['access_id'];
	if ($form_state['values']['op'] == 'Activer l\'option') {
		dedikam_webdav_change_password($id, true, false);
		ajouter_action('webdav',$id,'enable');
	} else if ($form_state['values']['op'] == 'Désactiver l\'option') {
		ajouter_action('webdav',$id,'disable');
        }
}

function dedikam_webdav_change_password($access_id, $ifisnull=true, $addaction=true) {
	$query = db_update('dedikam_member_access')
	->condition('id', $access_id);
	if ($ifisnull) {
		$query->isNull('webdav_password');
	}
	$nbres = $query->fields(array(
		'webdav_password' => dedikam_random_string(16),
		)
	)
	->execute();
	if ($nbres == 1 && $addaction) {
		ajouter_action('webdav',$access_id,'password');
	}
}

function dedikam_webdav_form_submit_reset_password($form, &$form_state) {
	$access_id = $form_state['values']['access_id'];
	dedikam_webdav_change_password($access_id, false);
}

function ajax_webdav_response($type = 'ajax', $member_access) {
  global $user;
  if ($type == 'ajax') {
  	if ($user->uid == $member_access->member_id) {
    		$output = $member_access->webdav_password;
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

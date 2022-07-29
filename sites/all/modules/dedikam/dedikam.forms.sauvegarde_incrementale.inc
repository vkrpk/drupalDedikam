<?php

// Form-generating function for the editing form.
function dedikam_sauvegarde_incrementale_form($form, $form_state, $member_access) {
	
	$form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->id,
	);

	$form['new_sauvegarde'] = array(
		'#title' => 'Création d\'une sauvegarde incrémentale',
		'#type' => 'fieldset',
		'#collapsible' => FALSE,
	);

	$form['new_sauvegarde']['dossier'] = array(
		'#title' => 'Dossier à sauvegarder',
		'#type' => 'textfield',
        '#required' => TRUE,
        '#rules' => array (
            array(
                'rule' => 'regexp[/^[a-zA-Z0-9_.-]+$/]',
                'error' => 'Le nom du dossier doit être composé des caractères suivants : a-zA-Z0-9._-',
            )
        ),
    );
    
	$form['new_sauvegarde']['periodicite'] = array(
		'#title' => 'Type de sauvegarde',
		'#type' => 'select',
		'#options' => array(
			'Q' => 'Quotidienne',
			'H' => 'Hebdomadaire',
			'M' => 'Mensuelle',
		),
		'#default_value' => 'Q',
	);
	$form['new_sauvegarde']['duree_roulement'] = array(
		'#title' => 'Durée de roulement',
		'#size' => 10,
		'#maxlength' => 10,
		'#type' => 'textfield',
        '#required' => TRUE,
        '#rules' => array (
            array(
                'rule' => 'regexp[/^[1-9][0-9]*$/]',
                'error' => 'La durée de roulement doit être un nombre.',
            )
        ),
    );

	$form['new_sauvegarde']['actions'] = array('#type' => 'actions');
	$form['new_sauvegarde']['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Ajouter',
		'#weight' => 10,
	);
	
	return $form;
}

function dedikam_sauvegarde_incrementale_form_submit($form, &$form_state) {
	if (isset($form_state['values']['access_id'])) {
		$member=member_access_load($form_state['values']['access_id']);
	} else {
		drupal_set_message('Ajout interdit : cet accès ne vous appartient pas.','error');
		return;
	}
	
	global $user;
	if ($member->member_id != $user->uid) {
		drupal_set_message('Ajout interdit : cet accès ne vous appartient pas.','error');
		return;
	}
	
	$record['access_id'] = $form_state['values']['access_id'];
	$record['dossier'] = $form_state['values']['dossier'];
	$record['type_sauvegarde'] = 'I';
	$record['periodicite'] = $form_state['values']['periodicite'];
	$record['roulement'] = $form_state['values']['duree_roulement'];
	$record['active'] = 1;

	if (drupal_write_record('dedikam_sauvegarde',$record) == SAVED_NEW) {
		drupal_set_message('Sauvegarde incrémentale ajoutée avec succès','status');
	} else {
		drupal_set_message('Une erreur interne s\'est produite','error');
	}
	
	$form_state['redirect'] = 'sauvegardes_incrementales/'.$record['access_id'].'/edit';
}

<?php

function dedikam_sauvegarde_notify_form($form,$form_state,$member_access) {
	$form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->id,
	);
    $form['notify'] = array (
        '#title' => 'Envoyer un mail après les sauvegardes',
        '#type' => 'checkbox',
        '#default_value' => $member_access->notifybackup,
		'#ajax' => array(
			'callback' => 'ajax_set_sauvegarde_notify_callback',
			'effect' => 'none',
			'progress' => array(
				'type' => 'throbber',
				'message' => '',
			),
		),
    );
    return $form;
}

function ajax_set_sauvegarde_notify_callback($form, &$form_state) {
	if (isset($form_state['values']['access_id'])) {
		$id = $form_state['values']['access_id'];
        $member=member_access_load($id);
	} else {
		drupal_set_message('Mise à jour interdite : cet accès ne vous appartient pas.','error');
		return;
	}
	
	global $user;
	if ($member->member_id != $user->uid) {
		drupal_set_message('Mise à jour interdite : cet accès ne vous appartient pas.','error');
		return;
	}
	
    $check = intval($form_state['values']['notify']);
    
	$nbupdates = db_update('dedikam_member_access')
			->condition('id', intval($form_state['values']['access_id']))
				->fields(array(
						'notifybackup' => $check,
                ))
			->execute();
}

<?php

 // Form-generating function for the editing form.
function dedikam_quota_alert_threshold_form($form, $form_state, $member_access) {
	
	$form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->id,
	);

	$form['seuil'] = array(
		'#title' => '',
        '#prefix' => 'Alerter à ',
		'#type' => 'select',
		'#options' => array(
			'50' => '50%',
			'60' => '60%',
			'70' => '70%',
			'80' => '80%',
			'90' => '90%',
			'95' => '95%',
			'99' => '99%',
		),
		'#default_value' => is_null($member_access->alert_quota_threshold) ? '90' : $member_access->alert_quota_threshold,
		'#ajax' => array(
			'callback' => 'ajax_set_threshold_callback',
			'effect' => 'none',
			'progress' => array(
				'type' => 'throbber',
				'message' => NULL,
			),
		),
	);

	return $form;
}

function ajax_set_threshold_callback($form, &$form_state) {
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
	
    $fields = array();
	$alert_quota_threshold = intval($form_state['values']['seuil']);
    $quota = intval($member->quota);
    $size = intval($member->lastquotacheck);
    $fields['alert_quota_threshold'] = $alert_quota_threshold;
	$nbupdates = db_update('dedikam_member_access')
			->condition('id', intval($form_state['values']['access_id']))
			->fields($fields)
			->execute();
    
    if (($size / $quota) > ($alert_quota_threshold / 100.0))
    {
        $key = 'overquota_'.$id;
        $subject = 'Dépassement du quota de l\'accès '.$member->name.' ( '.$member->payee.' )';
        $subject = 'Dépassement du quota';
        $body = array();
        $body[] = "Bonjour ".$member->member.",\n";
        $body[] = 'L\'espace disque utilisé par l\'accès '.$member->name.' ('.$member->payee.') a dépassé les '.$alert_quota_threshold.'% des '.dedikam_affiche_unite($quota).' autorisés.';
        if ($quota > $size) {
            $body[] = '';
        }
        dedikam_sendmail($key, $subject, $member->mail, $body);
    }
    
}

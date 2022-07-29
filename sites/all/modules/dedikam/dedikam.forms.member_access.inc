<?php

// Form-generating function for the editing form.
function dedikam_member_access_form($form, $form_state, $profile, $member_access) {
	$admin = ($profile == 'admin');
	
	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->id,
	);
	$form['profile'] = array(
		'#type' => 'hidden',
		'#value' => $profile,
	);
	$form['name'] = array(
		'#title' => 'Accès client',
		'#type' => 'textfield',
		'#default_value' => $member_access->name,
		'#disabled' => TRUE,
	);
	$form['member_name'] = array(
		'#title' => t('User name'),
		'#type' => 'textfield',
		'#default_value' => isset($member_access->member_id) ? dedikam_get_member_name($member_access->member_id) : '',
		'#disabled' => TRUE,
	);
	$form['payee'] = array(
		'#title' => 'Alias de l\'accès',
		'#type' => 'textfield',
		'#default_value' => $member_access->payee,
		'#maxlength' => '20',
	);
	$form['ssh_active'] = array (
		'#type' => 'radios',
		'#title' => 'Etat de l\'accès SSH/SFTP',
		'#options' => array(
				1 => 'Activé',
				0 => 'Désactivé',
				),
		'#default_value' => $member_access->ssh_active,
	);
	if ($admin) {
		$form['expire'] = array(
			'#title' => t('Expire'),
			'#type' => 'date_popup',
			'#default_value' => dedikam_epochtodate($member_access->expire,'Y-m-d H:i:s'),
			'#date_year_range' => '-1:+2',
		);
	}
	if ($admin) {
		$form['quota'] = array(
			'#title' => t('Quota (en Go)'),
			'#type' => 'jslider',
			'#min' => 10,
			'#max' => 5000,
			'#step' => 10,
			'#value' => intval($member_access->quota/1000000000),
			'#orientation' => 'horizontal',
			'#display_inputs' => FALSE,
			'#display_values' => TRUE,
			'#slider_length' => '500px'
		);
		$form['quota_illimite'] = array(
			'#title' => 'Quota illimité',
			'#type' => 'checkbox',
			'#default_value' => ($member_access->quota == -1 ? 1 : 0)
		);
	}
	if ($admin) {
		$form['level'] = array(
			'#title' => 'Niveau de sécurité',
			'#type' => 'select',
			'#options' => array(
				'N1' => 'N1',
				'N2' => 'N2',
				'N3' => 'N3',
			),
			'#default_value' => $member_access->level,
		);
	}
	
   if ($admin) {
		switch ($member_access->status) {
			case 'validated' :
				$form['status'] = array(
					'#title' => 'Statut de l\'accès',
					'#type' => 'select',
					'#options' => array(
						'validated' => dedikam_descstatus('validated'),
						'passwd' => dedikam_descstatus('passwd'),
						'block' => dedikam_descstatus('block'),
						'delete' => dedikam_descstatus('delete'),
						'pending' => dedikam_descstatus('pending'),
					),
					'#default_value' => 'validated',
				);
				break;
			case 'blocked' :
				$form['status'] = array(
					'#title' => 'Statut de l\'accès',
					'#type' => 'select',
					'#options' => array(
						'blocked' => dedikam_descstatus('blocked'),
						'unblock' => dedikam_descstatus('unblock'),
						'delete' => dedikam_descstatus('delete'),
                        'unblock_delay' => dedikam_descstatus('unblock_delay'),
					),
					'#default_value' => 'blocked',
				);
				break;
			case 'deleted' :
				$form['status'] = array(
					'#title' => 'Statut de l\'accès',
					'#type' => 'select',
					'#options' => array(
						'blocked' => dedikam_descstatus('blocked'),
						'unblock' => dedikam_descstatus('unblock'),
						'delete' => dedikam_descstatus('delete'),
						'pending' => dedikam_descstatus('pending'),
					),
					'#default_value' => 'blocked',
				);
				break;
			default :
				$form['status'] = array(
					'#title' => 'Statut de l\'accès',
					'#type' => 'select',
					'#options' => array(
						$member_access->status => dedikam_descstatus($member_access->status),
					),
					'#default_value' => $member_access->status,
				);
		}
		if ($member_access->lasterror) {
			$form['lasterror'] = array(
				'#title' => 'Dernière erreur d\'exécution',
				'#type' => 'textarea',
				'#default_value' => $member_access->lasterror,
				'#disabled' => TRUE,
				'#rows' => 3,
			);
		}
		$form['reset_passwd'] = array(
			'#type' => 'hidden',
			'#value' => '0',
		);
	} else {
		if ($member_access->status == 'validated') {
			$form['reset_passwd'] = array(
				'#title' => 'Réinitialiser le mot de passe',
				'#type' => 'checkbox',
			);
		} else {
			$form['reset_passwd'] = array(
				'#type' => 'hidden',
				'#value' => '0',
			);
		}
	}
	
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Enregistrer',
		'#weight' => 10,
	);
    $form['actions']['cancel'] = array(
        '#type' => 'submit',
        '#value' => 'Retour ou annuler',
        '#weight' => 20,
	);	
	
	field_attach_form('member_access', $member_access, $form, $form_state, '');
	
	return $form;
}

// Form submission handler for editing form.
function dedikam_member_access_form_submit($form, &$form_state) {
	$admin = ($form_state['values']['profile'] == 'admin');

	$member = member_access_load($form_state['values']['id']);

    if ($form_state['values']['op'] == $form_state['values']['cancel']) {
        $form_state['redirect'] = $admin ? 'admin/member_access' : 'user/member_access';
        return;
    }

	$fields = array();
	if ($admin) {
		$fields['expire'] = dedikam_datetoepoch($form_state['values']['expire'],'Y-m-d H:i');
		if ($form_state['values']['quota_illimite'] == '1') {
			$fields['quota'] = -1;
		} else {
			$fields['quota'] = intval($form_state['values']['quota']['value'])*1000000000;
		}
		$fields['level'] = $form_state['values']['level'];
		if ($form_state['values']['status'] == 'unblock_delay')
        {
            $fields['status'] = 'unblock';
            $fields['unblock_until'] = date('U')+24*60*60;
        } else {
            $fields['status'] = $form_state['values']['status'];
        }
	} else {
		if ($form_state['values']['reset_passwd'] && intval($form_state['values']['reset_passwd']) == 1) {
			$fields['status'] = 'passwd';
			drupal_set_message('Mot de passe changé avec succès. Vous allez recevoir prochainement un courriel avec votre nouveau mot de passe.','status');
		}
	}
	
	$fields['payee'] = $form_state['values']['payee'];
	
	if ($member->ssh_active !=  $form_state['values']['ssh_active']) {
		$fields['ssh_active'] =  $form_state['values']['ssh_active'];
		ajouter_action('sshactivation',$form_state['values']['id'], ($form_state['values']['ssh_active'] == 1 ? 'enable' : 'disable') );
	}

	$nbupdates = db_update('dedikam_member_access')
			->condition('id', intval($form_state['values']['id']))
			->fields($fields)
			->execute();
	
	if ($nbupdates == 1) {
		drupal_set_message(t('Account updated'));
	}
	if ($admin) {
		drupal_goto('admin/member_access/'.$form_state['values']['id'].'/edit');
	} else {
		drupal_goto('user/member_access/'.$form_state['values']['id'].'/edit');
	}
}

<?php

// Form-generating function for the editing form.
function dedikam_owncloud_activation_form($form, $form_state, $member_access) {

	$member_access = member_access_load($member_access->id);

	$oc_active = $member_access->oc_active == 1;

	$form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->id,
	);

	$form['access_name'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->name,
	);

/*
	$form['access_owncloud'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->owncloud,
	);
*/
		$form['oc_active'] = array(
				'#type' => 'hidden',
				'#value' => $member_access->oc_active,
		);

	$action = etat_action('ocactivation', $member_access->id);

	if ($action == 'validated') {
		if ($oc_active) {
			$form['btactivate'] = array(
				'#title' => '',
				'#value' => 'Désactiver',
				'#type' => 'button',
				'#attributes' => array('style' => 'margin-right:0px;'),
				'#ajax' => array(
					'callback' => 'ajax_set_ocactivation_callback',
					'wrapper' => 'btocenable'.$member_access->id,
					'effect' => 'none',
					'method' => 'replace',
					'progress' => array(
						'type' => 'none',
					),
				),
			);
		} else {
			$form['btactivate'] = array(
				'#title' => '',
				'#value' => 'Activer',
				'#type' => 'button',
				'#attributes' => array('style' => 'margin-right:0px;'),
				'#ajax' => array(
					'callback' => 'ajax_set_ocactivation_callback',
					'wrapper' => 'btocenable_'.$member_access->id,
					'effect' => 'none',
					'method' => 'replace',
					'progress' => array(
						'type' => 'none',
					),
				),
			);
		}
	} else {
		$form['btactivate'] = array(
			'#title' => '',
			'#value' => $oc_active ? 'Activation en cours' : 'Désactivation en cours',
			'#type' => 'button',
			'#attributes' => array('style' => 'margin-right:0px;'),
			'#disabled' => 'true',
		);
	}

	return $form;
}

function ajax_set_ocactivation_callback($form, &$form_state) {
		$id = $form_state['values']['access_id'];
	db_update('dedikam_member_access')
				->condition('id', intval($id))
				->fields(array(
						'oc_active' => 1 -  $form_state['values']['oc_active'],
						)
				)
				->execute();
	$member = member_access_load($id);
		ajouter_action('ocactivation',$id, $member->oc_active == 1 ? 'enable' : 'disable');
	$form1 = drupal_get_form('dedikam_owncloud_activation_form_'.$id,$member);

		return array(
			'#type' => 'ajax',
			'#commands' => array(
				ajax_command_replace("#ajax_bt_ocenable_$id", "<div id=\"ajax_bt_ocenable_$id\">".drupal_render($form1).'</div>'),
			),
		);
}

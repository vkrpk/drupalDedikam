<?php

// Form-generating function for the editing form.
function dedikam_pydio_activation_form($form, $form_state, $member_access) {
	
	$member_access = member_access_load($member_access->id);

	$pydio_active = $member_access->pydio_active == 1;

	$form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->id,
	);

	$form['access_name'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->name,
	);

		$form['pydio_active'] = array(
				'#type' => 'hidden',
				'#value' => $member_access->pydio_active,
		);

	$action = etat_action('pydioactivation', $member_access->id);

	if ($action == 'validated') {
		if ($pydio_active) {
			$form['btactivate'] = array(
				'#title' => '',
				'#value' => 'Désactiver',
				'#type' => 'button',
				'#attributes' => array('style' => 'margin-right:0px;'),
				'#ajax' => array(
					'callback' => 'ajax_set_pydioactivation_callback',
					'wrapper' => 'btpydioenable'.$member_access->id,
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
					'callback' => 'ajax_set_pydioactivation_callback',
					'wrapper' => 'btpydioenable_'.$member_access->id,
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
			'#value' => $pydio_active ? 'Activation en cours' : 'Désactivation en cours',
			'#type' => 'button',
			'#attributes' => array('style' => 'margin-right:0px;'),
			'#disabled' => 'true',
		);
	}

	return $form;
}

function ajax_set_pydioactivation_callback($form, &$form_state) {
	$id = $form_state['values']['access_id'];
	db_update('dedikam_member_access')
		->condition('id', intval($id))
		->fields(array(
				'pydio_active' => 1 -  $form_state['values']['pydio_active'],
			)
		)
		->execute();
	$member = member_access_load($id);
	ajouter_action('pydioactivation',$id, $member->pydio_active == 1 ? 'enable' : 'disable');
	$form1 = drupal_get_form('dedikam_pydio_activation_form_'.$id,$member);

	return array(
		'#type' => 'ajax',
		'#commands' => array(
			ajax_command_replace("#ajax_bt_pydioenable_$id", "<div id=\"ajax_bt_pydioenable_$id\">".drupal_render($form1).'</div>'),
		),
	);
}

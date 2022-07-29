<?php

// Form-generating function for the editing form.
function dedikam_owncloud_refresh_form($form, $form_state, $member_access) {
	
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

	$action = etat_action('ocrefresh', $member_access->id);

	if ($action == 'validated') {
		$form['btrefresh'] = array(
			'#title' => '',
			'#value' => 'Rescan',
			'#type' => 'button',
			'#attributes' => array('style' => 'margin-right:0px;'),
			'#ajax' => array(
				'callback' => 'ajax_set_ocrefresh_callback',
				'wrapper' => 'btocrefresh'.$member_access->id,
				'effect' => 'none',
				'method' => 'replace',
				'progress' => array(
					'type' => 'none',
				),
			),
		);
	} else {
		$form['btrefresh'] = array(
			'#title' => '',
			'#value' => 'Rescan en cours',
			'#type' => 'button',
			'#attributes' => array('style' => 'margin-right:0px;'),
			'#disabled' => 'true',
		);
	}

	return $form;
}

function ajax_set_ocrefresh_callback($form, &$form_state) {
    $id = $form_state['values']['access_id'];
	ajouter_action('ocrefresh',$id,'refresh');
	$member = member_access_load($id);
	$form1 = drupal_get_form('dedikam_owncloud_refresh_form_'.$id,$member);

    return array(
            '#type' => 'ajax',
            '#commands' => array(
                ajax_command_replace("#ajax_bt_ocrefresh_$id", "<div id=\"ajax_bt_ocrefresh$id\">".drupal_render($form1).'</div>'),
            ),
        );
}

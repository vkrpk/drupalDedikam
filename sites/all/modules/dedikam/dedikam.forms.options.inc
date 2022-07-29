<?php

// Form-generating function for the editing form.
function dedikam_options_form($form, $form_state, $access) {
	
    $form['access_id'] = array(
        '#type' => 'hidden',
        '#value' => ($access ? $access->id : null),
    );
    
	$form['options'] = array(
		'#title' => 'Choix des options pour l\'accès '.$access->name.' ('.$access->payee.') :',
		'#type' => 'fieldset',
		'#collapsible' => FALSE,
	);
        $form['options']['cifs'] = array(
                '#title' => 'CIFS : Partages windows (SMB)',
                '#type' => 'checkbox',
        '#default_value' => ($access ? $access->cifs : 0),
        );

	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Valider',
		'#weight' => 10,
 	);
        $form['actions']['back'] = array(
                '#type' => 'submit',
                '#value' => 'Retour',
		'#weight' => 20,
		'#submit' => array('dedikam_options_back_submit'),
        );
 

	return $form;
}

function dedikam_options_form_submit($form, &$form_state) {
	$record['action_destination'] = 'cifs';
	$record['action_key'] = $form_state['values']['access_id'];
	$record['action_name'] = intval($form_state['values']['cifs']) == 1 ? "enable" : "disable";
        $resu = drupal_write_record('dedikam_action',$record);
	
	$form_state['redirect'] = 'user/member_access/'.$form_state['values']['access_id'].'/options';
}

function dedikam_options_back_submit($form, &$form_state) {
	$form_state['redirect'] = 'user/member_access';
}

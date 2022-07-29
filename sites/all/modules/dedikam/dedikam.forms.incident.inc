<?php

// Form-generating function for the editing form.
function dedikam_incident_form($form, $form_state, $incident) {
	
    $form['incident_id'] = array(
        '#type' => 'hidden',
        '#value' => ($incident ? $incident->id_incident : null),
    );
    
	$form['services'] = array(
		'#title' => 'Services impactés',
		'#type' => 'fieldset',
		'#collapsible' => FALSE,
	);
	$form['services']['owncloud'] = array(
		'#title' => 'Owncloud',
		'#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->owncloud : 0),
	);
        $form['services']['pydio'] = array(
                '#title' => 'Pydio',
                '#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->pydio : 0),
        );
	$form['services']['ssh'] = array(
		'#title' => 'SSH',
		'#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->ssh : 0),
	);
	$form['services']['rsync'] = array(
		'#title' => 'RSync',
		'#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->rsync : 0),
	);
	$form['services']['sftp'] = array(
		'#title' => 'SFTP',
		'#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->sftp : 0),
	);
        $form['services']['ftp'] = array(
                '#title' => 'FTP/FTPS',
                '#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->ftp : 0),
        );
	$form['services']['si'] = array(
		'#title' => 'Sauvegardes incrémentales',
		'#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->si : 0),
	);
	if (variable_get('use_eb')) {
	$form['services']['eb'] = array(
		'#title' => 'Sauvegardes externalisées',
		'#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->eb : 0),
	);
	}
        $form['services']['cifs'] = array(
                '#title' => 'CIFS',
                '#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->cifs : 0),
        );
        $form['services']['webdav'] = array(
                '#title' => 'WEBDAV',
                '#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->webdav : 0),
        );
        $form['services']['iscsi'] = array(
                '#title' => 'iSCSI',
                '#type' => 'checkbox',
        '#default_value' => ($incident ? 1 - $incident->iscsi : 0),
        );
    $form['dates'] = array(
        '#title' => 'Période impactée',
		'#type' => 'fieldset',
		'#collapsible' => FALSE,
    );
    $form['dates']['date_debut'] = array(
        '#title' => 'Date de début',
        '#type' => 'date_popup',
		'#default_value' => ($incident && $incident->date_debut ? dedikam_epochtodate($incident->date_debut,'Y-m-d H:i:s') : dedikam_epochtodate(date('U'),'Y-m-d H:i:s')),
		'#date_year_range' => '-1:+2',
        '#required' => TRUE,
    );
    $form['dates']['date_fin'] = array(
        '#title' => 'Date de fin',
        '#type' => 'date_popup',
		'#default_value' => ($incident && $incident->date_fin ? dedikam_epochtodate($incident->date_fin,'Y-m-d H:i:s') : null),
		'#date_year_range' => '-1:+2',
    );
    
    $form['description'] = array(
        '#title' => 'Description',
		'#type' => 'fieldset',
		'#collapsible' => FALSE,
    );
    $form['description']['text'] = array(
        '#title' => '',
        '#type' => 'text_format',
        '#format' => 'filtered_html',
		'#default_value' => ($incident ? $incident->description : null),
    );

	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Enregistrer',
		'#weight' => 10,
	);
	
	return $form;
}

function dedikam_incident_form_validate($form, &$form_state) {
    if ( $form_state['values']['date_fin'] && $form_state['values']['date_debut'] > $form_state['values']['date_fin']) {
		form_set_error('date_fin', 'La date de fin doit être postérieure à la date de début.');
	}
}

function dedikam_incident_form_submit($form, &$form_state) {
	$record['id_incident'] = $form_state['values']['incident_id'];
	$record['owncloud'] = 1-intval($form_state['values']['owncloud']);
	$record['pydio'] = 1-intval($form_state['values']['pydio']);
	$record['ssh'] = 1-intval($form_state['values']['ssh']);
	$record['rsync'] = 1-intval($form_state['values']['rsync']);
	$record['sftp'] = 1-intval($form_state['values']['sftp']);
	$record['ftp'] = 1-intval($form_state['values']['ftp']);
	$record['cifs'] = 1-intval($form_state['values']['cifs']);
	$record['webdav'] = 1-intval($form_state['values']['webdav']);
	$record['si'] = 1-intval($form_state['values']['si']);
	$record['iscsi'] = 1-intval($form_state['values']['iscsi']);
	if (variable_get('use_eb')) {
		$record['eb'] = 1-intval($form_state['values']['eb']);
	}
	$record['date_debut'] = ($form_state['values']['date_debut'] ? dedikam_datetoepoch($form_state['values']['date_debut'],'Y-m-d H:i') : null);
	$record['date_fin'] = ($form_state['values']['date_fin'] ? dedikam_datetoepoch($form_state['values']['date_fin'],'Y-m-d H:i') : null);
	$record['description'] = $form_state['values']['text']['value'];

	if ($form_state['values']['incident_id']) {
        $resu = drupal_write_record('dedikam_incident',$record, 'id_incident');
    } else {
        $resu = drupal_write_record('dedikam_incident',$record);
    }
    if ($resu == SAVED_NEW) {
		drupal_set_message('Incident ajouté avec succès','status');
	} else {
        if ($resu == SAVED_UPDATED) {
            drupal_set_message('Incident mis à jour avec succès','status');
        } else {
		    drupal_set_message('Une erreur interne s\'est produite','error');
        }
	}
	
	$form_state['redirect'] = 'admin/incidents/'.$record['id_incident'].'/edit';
}

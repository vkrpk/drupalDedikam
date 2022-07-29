<?php

// Form-generating function for the editing form.
function dedikam_quota_refresh_form($form, $form_state, $member_access) {
	
	$form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->id,
	);

	$form['access_name'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->name,
	);

	$form['access_quota'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->quota,
	);

	$form['btrefresh'] = array(
        '#title' => '',
        '#value' => 'Rafraichir',
        '#type' => 'button',
		'#ajax' => array(
			'callback' => 'ajax_set_refresh_callback',
            'wrapper' => 'btrefresh'.$member_access->id,
			'effect' => 'none',
            'method' => 'replace',
            'progress' => array(
				'type' => 'throbber',
				'message' => NULL,
			),
		),
	);

	return $form;
}

function ajax_set_refresh_callback($form, &$form_state) {
    $id = $form_state['values']['access_id'];
    dedikam_maj_quota($id);
    $member = member_access_load($id);
    $files = $member->nb_files;
    $dirs = $member->nb_dirs;

    // mise à jour de l'interface
    $quota = $member->quota;
    $size = $member->lastquotacheck;
    $date = $member->quotacheckdatetime;
    if ($size > $quota) {
        if ($quota == -1) {
            $lastquotacheck = 0;
            $quota = 1;
        } else {
            $lastquotacheck = $quota;
        }
    } else {
        $lastquotacheck = $size;
    }
    $libre = round(100 * ($quota - $lastquotacheck)/$quota,1);
    $occupe = 100 - $libre;
	
    $div_files  = '<i>Consommé :</i> <span class="text-bold-content">'.($quota == 1 ? '0' : round(100 * $size /$quota,0)).' %</span><br>';
    $div_files .= '<i>Fichiers :</i> <span class="text-bold-content">'.$files.'</span><br>';
    $div_files .= '<i>Dossiers :</i> <span class="text-bold-content">'.$dirs.'</span>';

    return array(
            '#type' => 'ajax',
            '#commands' => array(
                ajax_command_replace("#ajax_datemaj_$id", "<div id=\"ajax_datemaj_$id\"><i>Dernière vérification du quota :</i> ".dedikam_epochtodate($date,'d/m/Y à H:i').'</div>'),
		ajax_command_replace("#ajax_quota_$id", "<div id=\"ajax_quota_$id\">".dedikam_affiche_unite_bi($size).'</div>'),
		ajax_command_replace("#ajax_files_$id", "<div id=\"ajax_files_$id\">".$div_files.'</div>'),
                ajax_command_invoke(NULL, "maj_data", array("#container_espace_$id",$libre,$occupe)),
            ),
        );
}

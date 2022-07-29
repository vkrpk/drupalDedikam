<?php

function dedikam_sauvegarde_externalisee_edit_form($form, $form_state, $eb) {
	global $user;
    $msg_restore = 'Êtes-vous sûr de vouloir restaurer la sauvegarde externalisée ?\nToutes les données seront écrasées.';
    $msg_purge = 'Êtes-vous sûr de vouloir purger les sauvegardes externalisées ?\nToutes les sauvegardes seront effacées.';
	$jscript  = '(function ($) {';
	$jscript .= '	Drupal.behaviors.dedikam'.$user->uid.' = {';
	$jscript .= '		attach: function(context, settings) {';
	$jscript .= '			{$("#restore-submit").click(function() {';
	$jscript .= '					return confirm("' . $msg_restore . '");';
	$jscript .= '				});';
	$jscript .= '			}';
	$jscript .= '			{$("#purge-submit").click(function() {';
	$jscript .= '					return confirm("' . $msg_purge . '");';
	$jscript .= '				});';
	$jscript .= '			}';
	$jscript .= '		}';
	$jscript .= '	}';
	$jscript .= '})(jQuery);';
    drupal_add_js($jscript, 'inline');

	$member = member_access_load($eb->access_id);
    $title = 'pour l\'accès '.$member->name;
	if ($member->payee != '') {
		$title .= ' - '.$member->payee;
	}

	$form['title'] = array (
        '#markup' => '<h3>'.$title.'</h3>',
    );
    $form['id'] = array(
		'#type' => 'hidden',
		'#value' => $eb->id_sauvegarde,
	);
    $form['access_name'] = array(
		'#type' => 'hidden',
		'#value' => $member->name,
	);
    $form['prev_type'] = array(
		'#type' => 'hidden',
		'#value' => $eb->periodicite,
	);
    $form['prev_dossier'] = array(
		'#type' => 'hidden',
		'#value' => $eb->dossier,
	);

    $disabled = ($eb->status != 'scheduled' ? TRUE : FALSE);
	$form['status'] = array(
        '#title' => 'Etat de l\'environnement de sauvegarde',
		'#type' => 'fieldset',
		'#collapsible' => FALSE,        
    );
	switch ($eb->status) {
        case ('scheduled') :
            $texte = '<span style="color:#00FF00"><strong>Prêt</strong></span>';
            break;
        case ('create') :
            $texte = '<i>En cours de création ...</i>';
            break;
        case ('recreate') :
            $texte = '<i>En cours de création ...</i>';
            break;
        case ('running') :
            $texte = '<i>Sauvegarde en cours ...</i>';
            break;
        case ('run') :
            $texte = '<i>Démarrage prévu dans les prochaines minutes ...</i>';
            break;
        case ('restore') :
            $texte = '<i>Restauration prévue dans les prochaines minutes ...</i>';
            break;
        case ('restoring') :
            $texte = '<i>Restauration en cours ...</i>';
            break;
        case ('cancel') :
            $texte = '<i>Abandon de la sauvegarde dans moins d\'une minute ...</i>';
            break;
        default :
            $texte = '<span style="color:#FF0000"><strong>En erreur, veuillez contacter l\'administrateur du site</strong></span>';
            break;
    }

	if ($eb->status == 'scheduled') {
        $display_purge = '';
    } else {
        $display_purge = 'style="DISPLAY:none"';
    }
	if ($eb->status == 'scheduled' && $eb->derniere) {
        $display_restore = '';
    } else {
        $display_restore = 'style="DISPLAY:none"';
    }
	if ($eb->status == 'scheduled' && $eb->periodicite == 'P') {
        $display_launch = '';
    } else {
        $display_launch = 'style="DISPLAY:none"';
    }
	if ($eb->status == 'running' || $eb->status == 'restoring' ) {
        $display_kill = '';
    } else {
        $display_kill = 'style="DISPLAY:none"';
    }
    $display_refresh = '';

    if ($eb->lasterror) {
        $tr_error = '<tr><td width="150">Erreur :</td><td><div style="width:100%; max-height:300px; overflow:auto"><pre>'.$eb->lasterror.'</pre></div></td></tr>';
    } else {
        $tr_error = '';
    }
    
    if ($eb->bilan) {
        //$tr_bilan = '<tr><td width="150">Bilan :</td><td><div style="width:600px; max-height:300px; overflow:auto"><pre>'.dedikam_readfile($eb->bilan).'</pre></div></td></tr>';
	$tr_bilan = '<tr><td width="150">Bilan :</td><td>'.l('Télécharger le fichier bilan',file_create_url(file_build_uri(str_replace('/mnt/mfs/www','',$eb->bilan)))).'</td></tr>';
    } else {
        $tr_bilan = '';
    }

    $pourcent = $eb->quota > 0 ? ' ('.round(100 * $eb->utilise / $eb->quota,2).' %)' : '';
    $form['status']['tab'] = array(
		'#markup' => '<table>'
                        .'<tr><td width="150">Dossier</td><td>'.$eb->dossier.'</td></tr>'
                        .'<tr><td>Type de sauvegarde</td><td>'.dedikam_desc_periodicite($eb->periodicite).'</td></tr>'
                        .'<tr><td>Espace disque</td><td>'.dedikam_affiche_unite_bi($eb->utilise).' sur '.dedikam_affiche_unite($eb->quota).$pourcent.($eb->dateverif ? '<br/>le '.dedikam_epochtodate($eb->dateverif,'d/m/Y à H:i') : '').'</td></tr>'
                        .'<tr><td width="150">Etat</td><td>'.$texte.'</td></tr>'
                        .'<tr><td width="150">Dernière sauvegarde</td><td>'.($eb->derniere ? dedikam_epochtodate($eb->derniere,'d/m/Y à H:i') : 'Aucune').'</td></tr>'
                        .$tr_error
                        .$tr_bilan
                    .'</table>',
	);
   
    $form['status']['actions'] = array('#type' => 'actions');
    $form['status']['actions']['lancer'] = array(
        '#type' => 'submit',
        '#value' => 'Déclencher',
        '#prefix' => '<span id="div-lancer" '.$display_launch.'>',
        '#suffix' => '</span>',
    );
    $form['status']['actions']['restaurer'] = array(
        '#type' => 'submit',
        '#value' => 'Restaurer',
        '#prefix' => '<span id="div-lancer" '.$display_restore.'>',
        '#suffix' => '</span>',
        '#id' => 'restore-submit',
    );
    $form['status']['actions']['purger'] = array(
        '#type' => 'submit',
        '#value' => 'Purger',
        '#prefix' => '<span id="div-purger" '.$display_purge.'>',
        '#suffix' => '</span>',
        '#id' => 'purge-submit',
    );
    $form['status']['actions']['abandonner'] = array(
        '#type' => 'submit',
        '#value' => 'Abandonner',
        '#prefix' => '<span id="divkill" '.$display_kill.'>',
        '#suffix' => '</span>',
        '#id' => 'purge-submit',
    );
    $form['status']['actions']['rafraichir'] = array(
        '#type' => 'submit',
        '#value' => 'Rafraîchir',
        '#prefix' => '<span id="divrefresh" '.$display_refresh.'>',
        '#suffix' => '</span>',
        '#id' => 'refresh-submit',
    );

    $form['param'] = array(
        '#title' => 'Paramétrage',
		'#type' => 'fieldset',
		'#collapsible' => FALSE,        
    );

    $form['param']['dossier'] = array(
		'#title' => 'Dossier à sauvegarder',
		'#type' => 'textfield',
        '#default_value' => $eb->dossier,
        '#autocomplete_path' => 'storage/'.$member->id.'/autocomplete',
        '#attributes' => array('autocomplete' =>'off'),
        '#disabled' => $disabled,
        '#rules' => array (
            array(
                'rule' => 'regexp[/^[a-zA-Z0-9_.\/-]+$/]',
                'error' => 'Le nom du dossier doit être composé des caractères suivants : /a-zA-Z0-9._-',
            )
        ),        
	);
    $form['param']['dossier_info'] = array(
		'#markup' => 'La saisie du dossier est obligatoire. Mettre <tt>/</tt> pour sauvegarder tout l\'accès.',
	);
    /*
    $form['param']['oc_eb'] = array(
        '#title' => 'Créer le dossier EB dans Owncloud pour accéder à la sauvegarde externalisée',
        '#type' => 'checkbox',
        '#default_value' => get_owncloud_eb($member->name) ? 1 : 0,
    );
    */
    $form['param']['periodicite'] = array(
		'#title' => 'Type de sauvegarde',
		'#type' => 'radios',
		'#options' => array(
			'Q' => 'Quotidienne',
			'H' => 'Hebdomadaire',
			'M' => 'Mensuelle',
			'P' => 'Manuelle',
		),
		'#default_value' => $eb->periodicite,
        '#ajax' => array(
            'callback' => 'ajax_show_active',
            'progress' => array('type' => 'none'),
        ),
        '#disabled' => $disabled,
	);

	if ($eb->periodicite == 'P') {
        $display_auto = 'style="DISPLAY:none"';      
    } else {
        $display_auto = '';
    }

    $form['param']['active'] = array(
		'#title' => 'Démarrage automatique',
		'#type' => 'radios',
        '#prefix' => '<div id="div-active" '.$display_auto.'>',
        '#suffix' => '</div>',
                '#options' => array(
            '1' => 'Oui',
            '0' => 'Non',
        ),
        '#default_value' => $eb->active,
        '#disabled' => $disabled,
	);

    $form['param']['actions'] = array('#type' => 'actions');
    if (! $disabled) {
        $form['param']['actions']['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Mettre à jour',
            '#weight' => 10,
        );
    }
    $form['param']['actions']['retour'] = array(
        '#type' => 'submit',
        '#value' => 'Retour',
        '#weight' => 12,
    );
    
    return $form;
}

function ajax_show_active($form, &$form_state) {
    if ($form_state['values']['periodicite'] == 'P') {
        $ajax_commands[] =  ajax_command_invoke('#div-active', 'hide',array('slow'));
    } else {
        $ajax_commands[] =  ajax_command_invoke('#div-active', 'show',array('slow'));
    }
    return array('#type' => 'ajax', '#commands' => $ajax_commands);
}

function dedikam_sauvegarde_externalisee_edit_form_submit($form, &$form_state) {
    if (isset($form_state['values']['id'])) {
		$eb=sauvegarde_load($form_state['values']['id']);
	} else {
		drupal_set_message('Mise à jour interdite','error');
		return;
	}
    
    $maj = FALSE;
    switch ($form_state['values']['op']) {
        case $form_state['values']['lancer'] :
            drupal_set_message('La sauvegarde externalisée va être lancée prochainement.','status');
            $eb->active = 1;
            $eb->status = 'run';
            $eb->bilan = null;
            break;
        case $form_state['values']['restaurer'] :
            $eb->status = 'restore';
            $eb->bilan = null;
            break;
        case $form_state['values']['purger'] :
            $eb->status='recreate';
            $eb->derniere=null;
            $eb->bilan = null;
            break;
        case $form_state['values']['abandonner'] :
            $eb->status='cancel';
            break;
        case $form_state['values']['rafraichir'] :
            $form_state['redirect'] = 'sauvegardes_externes/'.$form_state['values']['id'].'/edit';
            return;
            break;
        case $form_state['values']['retour'] :
            $form_state['redirect'] = 'user/member_access';
            return;
            break;
        default :
            if ($form_state['values']['dossier'] != $form_state['values']['prev_dossier']) {
                $eb->status='recreate';
                $eb->derniere=null;
                $eb->bilan = null;
            }
            $eb->dossier = $form_state['values']['dossier'];
            $eb->periodicite = $form_state['values']['periodicite'];
            $eb->active = ($form_state['values']['periodicite'] == 'P' ? 0 : $form_state['values']['active']);
            /*
            if ($form_state['values']['oc_eb'] == 1) {
                set_owncloud_eb($form_state['values']['access_name'],$eb->id_serveur);
            } else {
                unset_owncloud_eb($form_state['values']['access_name']);
            }
            */
            $maj = TRUE;
            break;
    }

	if (drupal_write_record('dedikam_sauvegarde',$eb,array('id_sauvegarde')) == SAVED_UPDATED) {
		if ($maj) { drupal_set_message('Sauvegarde externalisée mise à jour.','status'); }
	} else {
		drupal_set_message('Une erreur interne s\'est produite','error');
	}
	$form_state['redirect'] = 'sauvegardes_externes/'.$form_state['values']['id'].'/edit';
}

function dedikam_member_files_callback($member, $string) {
    $files=array();
    $dir = '/mnt/mfs/'.$member->name.'/files';
    $param_dirs = explode('/',$string);
    $rel_path = '';
    foreach ($param_dirs as $pdir) {
        if (! empty($pdir) && $pdir != '..') {
            $dir .= '/'.$pdir;
            $rel_path .= '/'.$pdir;
        }
    }
    $string=$pdir;
    $dh  = opendir($dir);
    while (false !== ($filename = @readdir($dh))) {
        if ($filename != '.' && $filename != '..' && (empty($string) || stripos($filename, $string) !== FALSE)) {
            if (is_dir($dir.'/'.$filename)) {
                $files[$rel_path.'/'.$filename] = $rel_path.'/'.$filename;
            }
        }
    }
    drupal_json_output($files);
    exit();
}

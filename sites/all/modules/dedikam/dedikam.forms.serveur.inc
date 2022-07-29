<?php

// Form-generating function for the editing form.
function dedikam_serveur_form($form, $form_state, $serveur) {
	$msg = 'Êtes-vous sûr de vouloir supprimer ce serveur ?';
	$jscript  = '(function ($) {';
	$jscript .= '	Drupal.behaviors.dedikam = {';
	$jscript .= '		attach: function(context, settings) {';
	$jscript .= '			{$("#delete-submit").click(function() {';
	$jscript .= '					return confirm("' . $msg . '");';
	$jscript .= '				});';
	$jscript .= '			}';
	$jscript .= '		}';
	$jscript .= '	}';
	$jscript .= '})(jQuery);';

	drupal_add_js($jscript, 'inline');
	
    $form['id_serveur'] = array(
        '#type' => 'hidden',
        '#value' => ($serveur ? $serveur->id_serveur : null),
    );
    
	$form['nom'] = array(
		'#title' => 'Nom',
		'#size' => 20,
		'#maxlength' => 50,
		'#type' => 'textfield',
        '#default_value' => ($serveur ? $serveur->nom : ''),
        '#required' => TRUE,
    );
	$form['adresse'] = array(
		'#title' => 'Adresse IP ou FQDN',
		'#size' => 40,
		'#maxlength' => 255,
		'#type' => 'textfield',
        '#default_value' => ($serveur ? $serveur->adresse : ''),
        '#required' => TRUE,
        '#rules' => array (
            array(
                'rule' => 'regexp[/^((?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))|(([a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?\.)+[a-zA-Z]{2,6})$/]',
                'error' => 'Le format du champ %field n\'est pas correct.',
            )
        ),
    );
	$form['racine'] = array(
		'#title' => 'Répertoire racine',
		'#size' => 40,
		'#maxlength' => 255,
		'#type' => 'textfield',
        '#default_value' => ($serveur ? $serveur->racine : ''),
        '#required' => TRUE,
        '#rules' => array (
            array(
                'rule' => 'regexp[/^\/?([^\/]+\/?)+$/]',
                'error' => 'Le format du champ %field n\'est pas correct.',
            )
        ),
    );
	$form['espace'] = array(
		'#title' => 'Espace total de stockage',
		'#size' => 20,
		'#maxlength' => 20,
		'#type' => 'textfield',
        '#default_value' => ($serveur ? dedikam_affiche_unite($serveur->espace,FALSE) : '0'),
        '#required' => TRUE,
        '#rules' => array (
            array(
                'rule' => 'regexp[/^(\d+([\.,]\d+)?)([ ]?([KMGT]o?)?)$/]',
                'error' => 'Le format du champ %field n\'est pas correct.',
            )
        ),
    );
	$form['actif'] = array(
		'#title' => 'Actif',
		'#type' => 'checkbox',
        '#default_value' => ($serveur ? $serveur->actif : 1),
	);

	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Enregistrer',
		'#weight' => 10,
	);
	$form['actions']['delete'] = array(
		'#type' => 'submit',
		'#value' => 'Supprimer',
		'#weight' => 11,
        '#id' => 'delete-submit',
	);
	
	return $form;
}

function dedikam_serveur_form_submit($form, &$form_state) {
	$record['id_serveur'] = $form_state['values']['id_serveur'];
    if ($form_state['values']['op'] == $form_state['values']['delete']) {
        db_delete('dedikam_serveur')
            ->condition('id_serveur',$record['id_serveur'])
            ->execute();
      	$form_state['redirect'] = 'admin/serveurs/list';
        return;
    }

    $espace = $form_state['values']['espace'];
    preg_match('/^(\d+([\.,]\d+)?)([ ]?([KMGT]o?)?)$/', $espace, $matches);
    $espace = floatval(str_replace(',','.', $matches[1]));
    if (isset($matches[4])) {
        switch ($matches[4]) {
            case 'Ko' :
                $espace *= 1000;
                break;
            case 'Mo' :
                $espace *= pow(1000, 2);
                break;
            case 'Go' :
                $espace *= pow(1000, 3);
                break;
            case 'To' :
                $espace *= pow(1000, 4);
                break;
        }
    }
	$record['nom'] = $form_state['values']['nom'];
	$record['adresse'] = $form_state['values']['adresse'];
	$record['racine'] = $form_state['values']['racine'];
	$record['espace'] = $espace;
	$record['actif'] = intval($form_state['values']['actif']);
    
	if ($form_state['values']['id_serveur']) {
        $resu = drupal_write_record('dedikam_serveur',$record, 'id_serveur');
    } else {
        $resu = drupal_write_record('dedikam_serveur',$record);
    }
    if ($resu == SAVED_NEW) {
		drupal_set_message('Serveur ajouté avec succès','status');
	} else {
        if ($resu == SAVED_UPDATED) {
            drupal_set_message('Serveur mis à jour avec succès','status');
        } else {
		    drupal_set_message('Une erreur interne s\'est produite','error');
        }
	}
	$form_state['redirect'] = 'admin/serveurs/'.$record['id_serveur'].'/edit';
}
<?php

function dedikam_member_access_reset_form($form, &$form_state, $member_access_id) {
    $member_access = member_access_load($member_access_id);

	if (variable_get('use_eb')) {
		$msg = 'Toutes les données seront supprimées.\nVous devrez passer par le bouton \"Purger\" de la sauvegarde externalisée si vous souhaitez également la réinitialiser.\nUn nouveau de passe sera généré et envoyé par mail.\n\nÊtes-vous sûr de vouloir formater cet accès ?';
	} else {
		$msg = 'Toutes les données seront supprimées.\nUn nouveau de passe sera généré et envoyé par mail.\n\nÊtes-vous sûr de vouloir formater cet accès ?';
	}
	$jscript  = '(function ($) {';
	$jscript .= '	Drupal.behaviors.dedikam'.$member_access_id.' = {';
	$jscript .= '		attach: function(context, settings) {';
	$jscript .= '			{$("#reset-submit_'.$member_access_id.'").click(function() {';
	$jscript .= '					return confirm("' . $msg . '");';
	$jscript .= '				});';
	$jscript .= '			}';
	$jscript .= '		}';
	$jscript .= '	}';
	$jscript .= '})(jQuery);';

	drupal_add_js($jscript, 'inline');
    
    $form['id'] = array(
		'#type' => 'hidden',
		'#value' => $member_access_id,
	);

    $form['name'] = array(
		'#type' => 'hidden',
		'#value' => $member_access->name,
	);

	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Formater',
		'#weight' => 10,
		'#id' => 'reset-submit_'.$member_access_id,
	);

	return $form;
 }

function dedikam_member_access_reset_form_submit($form, &$form_state) {
	$id=$form_state['values']['id'];
 	$member_nb_disks = iscsi_get_nb_disks($id);
        if ($member_nb_disks > 0) {
        db_update('dedikam_iscsi')
                ->condition('access_id', intval($id))
                ->fields(array(
                        'disk_status' => 'format'
                        )
                )
                ->execute();
        } 
    db_delete('dedikam_sauvegarde')
            ->condition('access_id',$form_state['values']['id'],'=')
            ->condition('type_sauvegarde','I','=')
            ->execute();
        ajouter_action('cifs', $id, 'delete');
        ajouter_action('webdav', $id, 'disable');
        ajouter_action('user', $id, 'recreate');
    drupal_set_message('La demande de réinitialisation de l\'accès '.$form_state['values']['name'].' a été enregistrée.','status');
	$form_state['redirect'] = 'user/member_access';
 }

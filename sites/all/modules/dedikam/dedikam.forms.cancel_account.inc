<?php

function dedikam_cancel_account_form($form, $form_state, $user) {
	$msg = 'Êtes-vous sûr de vouloir supprimer votre accès ?\nToutes les données seront perdues.';
	$jscript  = '(function ($) {';
	$jscript .= '	Drupal.behaviors.dedikam'.$user->uid.' = {';
	$jscript .= '		attach: function(context, settings) {';
	$jscript .= '			{$("#delete-submit").click(function() {';
	$jscript .= '					return confirm("' . $msg . '");';
	$jscript .= '				});';
	$jscript .= '			}';
	$jscript .= '		}';
	$jscript .= '	}';
	$jscript .= '})(jQuery);';

	drupal_add_js($jscript, 'inline');

	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $user->uid,
	);
	$form['name'] = array(
		'#type' => 'hidden',
		'#value' => $user->name,
	);
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Supprimer l\'accès',
        '#id' => 'delete-submit',
	);
	
	return $form;
}

function dedikam_cancel_account_form_submit($form, &$form_state) {
    // suppression de toutes les sauvegardes de l'accès
    db_delete('dedikam_sauvegarde')
        ->where('status = \'deleted\' and access_id in (select id from {dedikam_member_access} where member_id = :id)',array(':id' => $form_state['values']['id']))
        ->execute();
    // suppression de tous les accès de l'accès
    db_delete('dedikam_member_access')
        ->condition('member_id',$form_state['values']['id'],'=')
        ->condition('status','deleted')
        ->execute();
    // suppression du user
    user_delete($form_state['values']['id']);
    drupal_set_message('L\'accès '.$form_state['values']['name'].' et toutes ses données ont été supprimés.','status');
	$form_state['redirect'] = 'accueil';
}

<?php

function dedikam_sauvegarde_incrementale_delete_form($form, &$form_state, $sauvegarde) {
	$msg = 'Êtes-vous sûr de vouloir supprimer cette sauvegarde incrémentale ?';
	$jscript  = '(function ($) {';
	$jscript .= '	Drupal.behaviors.dedikam'.$sauvegarde->id_sauvegarde.' = {';
	$jscript .= '		attach: function(context, settings) {';
	$jscript .= '			{$("#delete-submit_'.$sauvegarde->id_sauvegarde.'").click(function() {';
	$jscript .= '					return confirm("' . $msg . '");';
	$jscript .= '				});';
	$jscript .= '			}';
	$jscript .= '		}';
	$jscript .= '	}';
	$jscript .= '})(jQuery);';

	drupal_add_js($jscript, 'inline');
    $member_access = member_access_load($sauvegarde->access_id);
    
    $form['id_sauvegarde'] = array(
		'#type' => 'hidden',
		'#value' => $sauvegarde->id_sauvegarde,
	);
	$form['active'] = array(
		'#type' => 'hidden',
		'#value' => $sauvegarde->active,
	);
	$form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $sauvegarde->access_id,
	);

	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Supprimer',
		'#weight' => 10,
		'#id' => 'delete-submit_'.strval($sauvegarde->id_sauvegarde),
	);
    if ($sauvegarde->active == 1) {
        $form['actions']['toggleactive'] = array(
            '#type' => 'submit',
            '#value' => 'Désactiver',
            '#weight' => 15,
            '#id' => 'toggleactive-submit_'.strval($sauvegarde->id_sauvegarde),
        );
    } else {
        if (intval($member_access->quota) == -1 || ( intval($member_access->quota) > 0 && 100 * intval($member_access->lastquotacheck) / intval($member_access->quota) <= 95)) {
            $form['actions']['toggleactive'] = array(
                '#type' => 'submit',
                '#value' => 'Activer',
                '#weight' => 15,
                '#id' => 'toggleactive-submit_'.strval($sauvegarde->id_sauvegarde),
            );
        }
    }

	return $form;
 }

  function dedikam_sauvegarde_incrementale_delete_form_submit($form, &$form_state) {
    if ($form_state['values']['op'] == 'Supprimer') {
        $nb = db_delete('dedikam_sauvegarde')
                ->condition('id_sauvegarde',$form_state['values']['id_sauvegarde'],'=')
                ->execute();
        if ($nb==1) {
            drupal_set_message('La sauvegarde incrémentale a été supprimée','status');
        }
    } else {
        db_update('dedikam_sauvegarde')
                ->condition('id_sauvegarde',$form_state['values']['id_sauvegarde'],'=')
                ->fields(array(
                    'active' => $form_state['values']['active'] == 0 ? 1 : 0,
                    )
                )
                ->execute();
        drupal_set_message('La sauvegarde incrémentale a été modifiée','status');
    }
	$form_state['redirect'] = 'sauvegardes_incrementales/'.$form_state['values']['access_id'].'/edit';
 }

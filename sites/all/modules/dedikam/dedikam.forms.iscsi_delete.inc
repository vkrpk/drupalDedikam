<?php

function dedikam_iscsi_delete_form($form, &$form_state, $disk) {
	$msg = 'Êtes-vous sûr de vouloir supprimer ce disque iScsi ?';
	$jscript  = '(function ($) {';
	$jscript .= '	Drupal.behaviors.dedikam'.$disk->id_iscsi.' = {';
	$jscript .= '		attach: function(context, settings) {';
	$jscript .= '			{$("#delete-submit_'.$disk->id_iscsi.'").click(function() {';
	$jscript .= '					return confirm("' . $msg . '");';
	$jscript .= '				});';
	$jscript .= '			}';
	$jscript .= '		}';
	$jscript .= '	}';
	$jscript .= '})(jQuery);';

	drupal_add_js($jscript, 'inline');
    $member_access = member_access_load_name($disk->access_name);
    
    $form['id_iscsi'] = array(
		'#type' => 'hidden',
		'#value' => $disk->id_iscsi,
	);

	$form['actions'] = array('#type' => 'actions');
	if ($disk->disk_status == 'deleted') {
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Recréer '.$disk->disk_name,
		'#weight' => 10,
		'#id' => 'recreate-submit_'.strval($disk->id_iscsi),
	);
        $form['actions']['submit1'] = array(
                '#type' => 'submit',
                '#value' => 'Purger '.$disk->disk_name,
                '#weight' => 10,
                '#id' => 'purge-submit_'.strval($disk->id_iscsi),
        );
	} else if ($disk->disk_status == 'validated') {
        $form['actions']['submit'] = array(
                '#type' => 'submit',
                '#value' => 'Supprimer '.$disk->disk_name,
                '#weight' => 10,
                '#id' => 'delete-submit_'.strval($disk->id_iscsi),
        );
	}

	return $form;
 }

  function dedikam_iscsi_delete_form_submit($form, &$form_state) {
    if (strpos($form_state['values']['op'],'Supprimer') !== FALSE) {
        $nb = db_update('dedikam_iscsi')
		->fields(array('disk_status'=>'delete'))
                ->condition('id_iscsi',$form_state['values']['id_iscsi'],'=')
                ->execute();
	ajouter_action('iscsi', $form_state['values']['id_iscsi'], 'delete');
        if ($nb==1) {
            drupal_set_message('Demande de suppression du disque iscsi prise en compte','status');
        }
    } else if (strpos($form_state['values']['op'],'Recréer') !== FALSE) {
        $nb = db_update('dedikam_iscsi')
                ->fields(array('disk_status'=>'create'))
                ->condition('id_iscsi',$form_state['values']['id_iscsi'],'=')
                ->execute();
	ajouter_action('iscsi', $form_state['values']['id_iscsi'], 'create');
    } else if (strpos($form_state['values']['op'],'Purger') !== FALSE) {
        $nb = db_delete('dedikam_iscsi')
                ->condition('id_iscsi',$form_state['values']['id_iscsi'],'=')
                ->execute();
    }
 }

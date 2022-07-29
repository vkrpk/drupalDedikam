<?php

function dedikam_termination_page() {
    return views_embed_view('resilier','default');
}

 // Form-generating function for the editing form.
function dedikam_termination_form($form, $form_state, $member_id) {
	
	$member_access=member_access_load($member_id);
    
    $form['access_id'] = array(
		'#type' => 'hidden',
		'#value' => $member_id,
	);

	if ($member_access->termination == null) {
        // on peut demander la résiliation de l'accès
        $form['btaction'] = array(
            '#type' => 'submit',
            '#value' => 'Résilier l\'accès',
            '#weight' => 15,
            '#id' => 'btaction_'.strval($member_id),
        );
    } else {
        $form['termination_date'] = array(
            '#markup' => 'Demande faite le '.dedikam_epochtodate($member_access->termination,'d/m/Y à H:i'),
        );
        if ( date('U') - $member_access->termination <= 7*24*60*60) {
            // demande de résiliation < 7 jours : on peut annuler
            $form['btaction'] = array(
                '#type' => 'submit',
                '#value' => 'Annuler la demande',
                '#weight' => 15,
                '#id' => 'btaction_'.strval($member_id),
            );
        } else {
            // trop tard pour annuler
            $form['termination_date_1'] = array(
                '#markup' => '<br/>L\'accès va bientôt être supprimé',
            );
        }
    }

	return $form;
}

function dedikam_termination_form_submit($form, $form_state) {
    $access_id = $form_state['values']['access_id'];
    $member = member_access_load($access_id);

    if ($form_state['values']['btaction'] == 'Résilier l\'accès') {
        $key = 'termination_'.$access_id;
        $termination = date('U');
        // demande de résiliation
        db_update('dedikam_member_access')
                ->condition('id',$access_id,'=')
                ->fields(array(
                    'termination' => $termination,
                    )
                )
                ->execute();
        $termination = intval($termination);
        $expire = $member->expire;
        $prorata= round(floor(12.0 * ($expire - $termination) / (366.0*24*60*60)) * $member->paid / 12.0,2);
        $subject = 'Demande de résiliation de l\'accès '.$member->name.' ( '.$member->payee.' )';
        $body = array();
        $body[] = "Bonjour ".$member->member.",\n";
        $body[] = "Votre demande de résiliation pour l'accès ".$member->name." ( ".$member->payee." ) a été prise en compte.";
        $body[] = "Vous avez la possibilité d'annuler cette demande dans les 7 prochains jours.";
        $body[] = "Toutes les données seront supprimées de manière définitive après ce délai.";
//        $body[] = "Une demande de remboursement interviendra après la suppression de l\'accès.";
//        $body[] = 'Tout mois commencé étant dû, le montant remboursé estimé est le suivant : '.sprintf('%0.2f €',$prorata);
        $result = dedikam_sendmail($key, $subject, $member->mail, $body);
        if ($result) {
            drupal_set_message('La demande de résiliation a été prise en compte','status');
        } else {
            drupal_set_message('La demande de résiliation a été prise en compte mais un problème est survenu à l\'envoi du mail.','status');
        }
    } else {
        $key = 'untermination_'.$access_id;
        if ($form_state['values']['btaction'] == 'Annuler la demande') {
            db_update('dedikam_member_access')
                    ->condition('id',$access_id,'=')
                    ->fields(array(
                        'termination' => null,
                        )
                    )
                    ->execute();
            $subject = 'Annulation de la demande de résiliation de l\'accès '.$member->name.' ( '.$member->payee.' )';
            $body = array();
            $body[] = 'Bonjour '.$member->member.",";
            $body[] = 'Votre demande de résiliation pour l\'accès '.$member->name.' ( '.$member->payee." ) a été annulée.";
            $result = dedikam_sendmail($key, $subject, $member->mail, $body);
            if ($result) {
                drupal_set_message('La demande de résiliation a été annulée','status');
            } else {
                drupal_set_message('La demande de résiliation a été annulée mais un problème est survenu à l\'envoi du mail.','status');
            }
        }
    }
}

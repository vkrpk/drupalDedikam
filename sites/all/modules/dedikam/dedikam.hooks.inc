<?php
/**
 * @file
 * dedikam.hooks
 */

 
// Suppression de la colonne quantité dans le panier
function dedikam_form_alter(&$form, &$form_state, $form_id) {
	switch ($form_id) {
		case 'uc_cart_view_form' :
			foreach ($form['items'] as $k => $item) {
				if (is_array($item) && isset($item['qty'])) {
					$form['items'][$k]['qty']['#type'] = 'value';
					$form['items'][$k]['qty']['#value'] = 1;
				}
			}
			unset($form['items']['#columns']['qty']);
			unset($form['actions']['update']);
			break;
		case 'uc_cart_checkout_form' :
			break;
/*
		case 'contact-site-form' :
			$form['#submit'][] = 'dedikam_contact_form_submit';
			break;
*/
                case 'user_register_form' :
                        $form['cutomtext'] = array(
                                '#type' => 'item',
                                '#markup' => '<div><b><font color=#FF0000>En cliquant sur "Créer un nouveau compte", vous allez recevoir un message avec des instructions supplémentaires  à votre adresse de courriel.</font></b></div>',
                                '#weight' => 1,
                        );
                        break;
	}
}

// Mise à jour du prix en fonction des attributs
function dedikam_uc_product_alter(&$node) {
	if ($node->type == 'product' && ($node->model == 'ESSAI' || $node->model == 'ABO')) {
		$offre_essai = $node->model == 'ESSAI';
		$diskspace_aid = product_getattributeid('diskspace');
		$username_aid = product_getattributeid('username');
		$account_aid = product_getattributeid('account');
		$level_aid = product_getattributeid('level');
		$prorata_aid = product_getattributeid('prorata');
		$eb_aid = product_getattributeid('eb');

		if ($offre_essai) {
			$price = 0;
		} else {
			if (variable_get('use_eb')) {
				$eb = isset($node->data['attributes'][$eb_aid]) ?  $node->data['attributes'][$eb_aid]*1000000000 : 0;
			} else {
				$eb = 0;
			}
            $price = dedikam_prix($node->data['attributes'][$level_aid],$node->data['attributes'][$diskspace_aid]*1000000000,$eb);
		
			if (isset($node->data['attributes'][$account_aid]) && $node->data['attributes'][$account_aid] != '') {
				$member = dedikam_get_member_access($node->data['attributes'][$account_aid]);
				$expire = dedikam_offer_expire($member->expire);
				if (! $expire) {
					// c'est une modification de l'abonnement en cours
					$price = max(0,round(($price-$member->paid)*dedikam_get_prorata_restant($member),2));
				}
			}
		}
		
		$node->price = $node->sell_price = $price;
	}
}

function dedikam_views_pre_render(&$view) {
	//dargs();
}

/*
function dedikam_contact_form_submit(&$form, $form_state) {
	drupal_set_message('Votre message a été envoyé avec succès.<br/>L\'équipe Dedikam vous répondra dans les meilleurs délais.');
}
*/


/**
 * Implements hook_variable_info().
 */
function dedikam_variable_info($options) {
  $variable['use_eb'] = array(
    'title' => 'Activer les sauvegardes externalisées',
    'type' => 'boolean',
    'default' => 0,
  );
  return $variable;
}


function dedikam_user_login(&$edit, $account) {
  user_save($account, array('field_logout_at' => array('und' => array(0 => array('value' => 0)))));
}

<?php

// Form-generating function for the editing form.
// profile = new / renew / edit
function dedikam_offers_form($form, $form_state,$profile = 'new', $member_access = NULL) {
    $options = array(
        'cache' => FALSE,
    );
    drupal_add_js(drupal_get_path('module', 'dedikam'). '/js/offers_form.js', $options);
    drupal_add_css('#edit-disk-space {margin-left: 10px;}',$option['type'] = 'inline');
    drupal_add_css('#edit-eb-quota {margin-left: 30px;}',$option['type'] = 'inline');
    
    global $user;

    if ($profile == 'edit') {
        $title = 'pour l\'accès '.$member_access->name;
        if ($member_access->payee != '') {
            $title .= ' - '.$member_access->payee;
        }

        $form['title'] = array (
            '#markup' => '<h3>'.$title.'</h3>',
        );
    }
    
    $form['#validate'][] = 'dedikam_offers_form_validate';
    $form['id'] = array(
        '#type' => 'hidden',
        '#value' => $user->uid,
    );
    $form['action'] = array(
        '#type' => 'hidden',
        '#value' => $profile,
    );
    $eb = NULL;
    if ($member_access != NULL) {
        $form['access_id'] = array(
            '#type' => 'hidden',
            '#value' => $member_access->id,
        );
        $eb = get_sauvegarde_externe_for_member($member_access->id);
    }

    if ($profile == 'new') {
        $nb_gratuits = dedikam_get_nb_free_access($user->uid);
        $form['fs_offre_essai'] = array(
            '#title' => 'Offre d\'essai : 10Go gratuits',
            '#type' => 'fieldset',
            '#collapsible' => FALSE,
        );
        $form['fs_offre_essai']['info1'] = array(
            '#markup' => 'Testez toutes les fonctionnalités Dedikam gratuitement pendant 30 jours.', 
        );
        $form['fs_offre_essai']['info2'] = array(
            '#markup' => '<br/>Vous pouvez commander jusqu\'à 4 accès gratuits.',
        );
        if ($nb_gratuits >= 4) {
            $form['fs_offre_essai']['info2'] = array(
                '#markup' => '<br/>Vous pouvez commander jusqu\'à 4 accès gratuits. Vous les avez tous utilisés.',
            );
        } else {
            $form['fs_offre_essai']['info2'] = array(
                '#markup' => '<br/>Vous pouvez commander jusqu\'à 4 accès gratuits. Il vous en reste '.(4-$nb_gratuits).'.',
            );
            $form['fs_offre_essai']['offre_essai'] = array(
                '#type' => 'checkboxes',
                '#options' => array(
                    '1' => 'Profiter de l\'offre',
                ),
                '#ajax' => array(
                    'callback' => 'ajax_set_summary_callback',
                    'wrapper' => 'div-summary',
                    'method' => 'replace',
                    'effect' => 'none',
                    'progress' => array(
                        'type' => 'throbber',
                        'message' => NULL,
                    ),
                ),
            );
        }
    }
	
	 $form['fs_options_show']['acces_show'] = array(
        '#title' => '<h4><a href="https://www.dedikam.com/lexique/">Nos options</a></h4>', 
        '#type' => 'checkboxes',
        '#options' => array(
            'oc' => 'Applications web PYDIO et NEXTCLOUD',
            'ssh' => 'SSH',
            'rsync' => 'RSYNC sur SSH',
            'sftp' => 'SFTP',
            'ftp' => 'FTPS/FTP',
            'iscsi' => 'ISCSI',
	    'cifs' => 'CIFS',
	    'webdav' => 'WEBDAV',
	 
        ),
        '#disabled' => TRUE,
        '#default_value' => array('oc','ssh','rsync','sftp','si','cifs','iscsi'),
    );
	
    
    $form['fs_espace_disque'] = array(
        '#title' => 'Choisissez votre espace disque en déplaçant le curseur orange vers la droite<br><br><FONT color="#ff0000"><div align="left"><font size="2"><b>Tarif affiché<br>en bas de page</b></font></div>',
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
    );
    
    $min = 10;
    $default_quota = 10;
    switch ($profile) {
        case 'edit' :
            $min = max(10,$member_access->quota/1000000000);
            $default_quota = $min;
            break;
        case 'renew' :
            $min = max(10,10*ceil($member_access->lastquotacheck/10000000000));
            $default_quota = max(10,$member_access->quota/1000000000);
            break;
    }
    $form['fs_espace_disque']['disk_space'] = array(
        '#title' => 'Espace disque',
        '#type' => 'jslider',
        '#min' => $min,
        '#max' => 5000,
        '#step' => 10,
        '#value' => $default_quota,
        '#orientation' => 'horizontal',
        '#display_inputs' => FALSE,
        '#display_values' => FALSE,
        '#display_bubble' => TRUE,
        '#display_values_format' => '%{value}% Go',
        '#display_bubble_format' => '<span class="slider-value">%{value}%</br>Go</span>',
        '#slider_length' => '500px',
        '#ajax' => array(
            'callback' => 'ajax_set_summary_callback',
            'wrapper' => 'div-summary',
            'method' => 'replace',
            'effect' => 'none',
            'progress' => array(
                'type' => 'throbber',
                'message' => NULL,
            ),
            'event' => 'change',
        ),
    );
    $form['fs_espace_disque']['comment'] = array(
	'#markup' => '<div class="contact-limit" style="font-style: italic;text-align:center;"><br><FONT color="#ff0000">Trafic illimité - Bande passante : 500 Mbit/s à 1Gbit/s</FONT><br>Au dessus de 5000Go, veuillez nous '.l('contacter','https://www.dedikam.com/contact/').'.</div>',
    );

    $form['fs_payee'] = array(
        '#title' => 'Choisissez un alias pour l\'accès',
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
    );
    $form['fs_payee']['username'] = array(
        '#title' => 'Nom de l\'accès',
        '#type' => 'textfield',
        '#default_value' => ($member_access != NULL ? $member_access->payee : ''),
        '#disabled' => ($member_access != NULL ? TRUE : FALSE),
        '#maxlength' => '20',
        '#required' => TRUE,
    );
    $form['fs_payee']['alert'] = array(
        '#markup' => '<span id="edit-username-alert"></span>',
    );
    $form['fs_options'] = array(
        '#title' => 'Choisissez vos options',
        '#type' => 'fieldset',
        //'#suffix'=>'lala',
        '#collapsible' => FALSE,
    );
    
    $options = array();
    $default_level = 'N1';
    if ($member_access != NULL && $profile == 'edit') {
        switch ($member_access->level) {
            case 'N1' : 
                $options = array(
                    'N1' => '<div class="border-checked">Réplication simple<p>Données réparties<br/>sur 1 serveur</p><span><strong>Niveau 1</strong>disponibilité normale</span></div>',
                    'N2' => '<div class="border-checked">Réplication double<p>Données réparties<br/>sur 2 serveurs</p><span><strong>Niveau 2</strong>disponibilité accrue</span></div>',
                    'N3' => '<div class="border-checked">Réplication triple<p>Données réparties<br/>sur 3 serveurs</p><span><strong>Niveau 3</strong>disponibilité critique</span></div>',
                );
                break;
            case 'N2' :
                $options = array(
                    'N2' => '<div class="border-checked">Réplication double<p>Données réparties<br/>sur 2 serveurs</p><span><strong>Niveau 2</strong>disponibilité accrue</span></div>',
                    'N3' => '<div class="border-checked">Réplication triple<p>Données réparties<br/>sur 3 serveurs</p><span><strong>Niveau 3</strong>disponibilité critique</span></div>',
                );
                break;
            case 'N3' :
                $options = array(
                    'N3' => '<div class="border-checked">Réplication triple<p>Données réparties<br/>sur 3 serveurs</p><span><strong>Niveau 3</strong>disponibilité critique</span></div>',
                );
                break;
        }
        $default_level = $member_access->level;
    } else {
        $options = array(
            'N1' => '<div class="border-checked">Réplication simple<p>Données réparties<br/>sur 1 serveur</p><span><strong>Niveau 1</strong>disponibilité normale</span></div>',
            'N2' => '<div class="border-checked">Réplication double<p>Données réparties<br/>sur 2 serveurs</p><span><strong>Niveau 2</strong>disponibilité accrue</span></div>',
            'N3' => '<div class="border-checked">Réplication triple<p>Données réparties<br/>sur 3 serveurs</p><span><strong>Niveau 3</strong>disponibilité critique</span></div>',
        );
    }
    $form['fs_options']['level'] = array(
        '#title' => '<h4>Votre niveau de disponibilité</h4><p>Toutes vos données fragmentées puis copiées et réparties sur différents serveurs et Datacenter selon le niveau de disponibilité choisi. <a href="https://www.dedikam.com/lexique/#niveaux">En savoir plus</a></p>',
        '#type' => 'radios',
        '#default_value' => $default_level,
        '#options' => $options,
        '#ajax' => array(
            'callback' => 'ajax_set_summary_callback',
            'wrapper' => 'div-summary',
            'method' => 'replace',
            'effect' => 'none',
            'progress' => array(
                'type' => 'throbber',
                'message' => NULL,
            ),
        ),
    );

    $form['fs_options']['acces'] = array(
        '#title' => '<h4>Accès (compris dans l\'offre)</h4>',
        '#type' => 'checkboxes',
        '#options' => array(
            'oc' => 'Accès WEB avec PYDIO et OWNCLOUD',
            'ssh' => 'SSH',
            'rsync' => 'RSYNC sur SSH',
            'sftp' => 'SFTP',
            'cifs' => 'CIFS',
            'iscsi' => 'ISCSI',
	    'si' => '<a href="/content/lexique#sauvegardes">Sauvegardes incrémentales</a>',
        ),
        '#disabled' => TRUE,
        '#default_value' => array('oc','ssh','rsync','sftp','si','cifs','iscsi'),
    );

    if(variable_get('use_eb')) {
    $form['fs_options']['si'] = array(
//        '#title' => '<h4>Sauvegardes</h4>',
        '#type' => 'checkboxes',
        '#options' => array(
            'si' => 'Sauvegardes incrémentales (compris dans l\'offre) <a href="https://www.dedikam.com/lexique/#niveaux">En savoir plus</a>',
        ),
        '#disabled' => TRUE,
        '#default_value' => array('si'),
    );
    $form['fs_options']['eb'] = array(
        '#type' => 'checkboxes',
        '#options' => array(
            'eb' => 'Sauvegardes externalisées sur un autre Data Center.&nbsp;&nbsp;&nbsp;&nbsp;<i>(Taille limitée à l\'espace disque commandé)</i><p>Cette option permettra d\'envoyer des données dans un autre DataCenter et sur un site géographique différent et sur un serveur indépendant du cloud Dedikam. <a href="/content/lexique#sauvegardes">En savoir plus sur cette option</a></p>',
        ),
        '#ajax' => array(
            'callback' => 'ajax_show_eb_block',
            'wrapper' => 'div-eb',
            'method' => 'replace',
            'effect' => 'none',
            'progress' => array('type' => 'none'),
        ),
        '#default_value' => array($eb ? 'eb' : null),
        '#disabled' => ($profile == 'edit' &&  $eb !== FALSE),
    );
    $form['fs_options']['eb_quota'] = array(
        '#type' => 'jslider',
        '#min' => ($eb && $profile == 'edit' ? $eb->quota / 1000000000 : 10),
        '#max' => $default_quota,
        '#step' => 10,
        '#value' => ($eb ? max(10,$eb->quota / 1000000000) : 10),
        '#element_validate' => null,
        '#orientation' => 'horizontal',
        '#display_inputs' => FALSE,
        '#display_values' => FALSE,
        '#display_bubble' => TRUE,
        '#display_values_format' => '%{value}% Go',
        '#display_bubble_format' => '<span class="slider-value">%{value}%</br>Go</span>',
        '#slider_length' => '500px',
        '#prefix' => '<div id="div-eb" style="'.($eb ? '' : 'DISPLAY:none').'">',
        '#suffix' => '</div>',
        '#ajax' => array(
            'callback' => 'ajax_set_summary_callback',
            'wrapper' => 'div-summary',
            'method' => 'replace',
            'effect' => 'none',
            'progress' => array(
                'type' => 'throbber',
                'message' => NULL,
            ),
            'event' => 'change',
        ),
    );
    }
    $form['fs_summary'] = array(
        '#title' => 'Récapitulatif de votre commande<FONT color="#ff0000"> (Le tarif est annuel)',
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
    );
            
    switch ($profile) {
        case 'new' :
            $form['fs_summary']['s_summary'] = 
                ajax_set_html(
                    'N1',
                    10000000000,
                    0,
                    dedikam_prix('N1',10000000000)
                );
            break;
        case 'edit' :
            $form['fs_summary']['s_summary'] = 
                ajax_set_html($default_level,
                    $default_quota*1000000000,
                    ($eb ? $eb->quota : 0),
                    dedikam_prix($default_level,$default_quota*1000000000),
                    $member_access,
                    $profile
                );
            break;
        case 'renew' :
            $form['fs_summary']['s_summary'] = 
                ajax_set_html($default_level,
                    $default_quota*1000000000,
                    ($eb ? max(10*1000000000,$eb->quota) : 0),
                    dedikam_prix($default_level,$default_quota*1000000000, ($eb ? max(10*1000000000,$eb->quota) : 0)),
                    $member_access,
                    $profile
                );
            break;
    }

    if ( $user-> uid ) {
        $form['actions'] = array('#type' => 'actions');
        $form['actions']['submit'] = array(
            '#id' => 'add-submit',
            '#type' => 'submit',
            '#value' => t('Add to cart'),
            '#weight' => 10,
        );
    } else {
        $form['log_user'] = array (
            '#markup' => '<p class="must-log">* Vous devez <a href="https://dedispace.dedikam.com/user/register">CREER UN COMPTE</a> ou vous connecter sur votre <a href="https://dedispace.dedikam.com/user">DEDISPACE</a> avant de passer une commande.<br><i><font size="2">(Les tarifs sont annuels, pour un paiement échelonné veuillez nous contacter)</font></i></p>',
        );
    }
    if ($profile == 'edit') {
        $form['actions']['cancel'] = array(
            '#type' => 'button',
            '#prefix' => '<br/>',
            '#value' => 'Annuler',
            '#weight' => 12,
        );
    }

    return $form;
}

function dedikam_offers_form_validate($form, &$form_state) {
    global $user;
    if ($form_state['values']['action'] == 'new')
    {
        $accesses = dedikam_get_members($user->uid);
        foreach ($accesses as $id => $access) {
            if ((strcasecmp($access->payee,$form_state['values']['username']) == 0) && ($access->expire>date('U'))) {
                form_set_error('username', 'un accès actif existe déjà avec ce nom, choisissez un autre nom');
            }
        }
    }
}

function ajax_set_summary_callback($form, $form_state) {
    $member = NULL;
    if (isset($form_state['values']['access_id'])) {
        $member=member_access_load($form_state['values']['access_id']);
    }
    $offre_essai = isset($form_state['values']['offre_essai']) && $form_state['values']['offre_essai']['1'] == '1';
    if ($offre_essai) {
        $resu = ajax_set_html(
            'N1',
            10*1000000000,
            variable_get('use_eb') ? 10*1000000000 : 0,
            0,
            $member,
            'essai'
        );
    } else {
		if(isset($form_state['values']['eb']['eb'])){
			if($form_state['values']['eb']['eb'] == '0'){
				$eb = 0;
			}else{
				if(isset($form_state['values']['eb_quota']['value'])){
					$eb = intval($form_state['values']['eb_quota']['value'])*1000000000;
				}
			}
		}else{
			$eb = 0;
		}
		
      //  $eb = ($form_state['values']['eb']['eb'] == '0') ? 0 : intval($form_state['values']['eb_quota']['value'])*1000000000;
        $quota = intval($form_state['values']['disk_space']['value'])*1000000000;
        $eb = $eb > $quota ? $quota : $eb;
        $resu = ajax_set_html(
            strval($form_state['values']['level']),
            $quota,
            variable_get('use_eb') ? $eb : 0,
            dedikam_prix($form_state['values']['level'],$quota,$eb),
            $member,
            $form_state['values']['action']
        )
        ;
    }
    if (variable_get('use_eb')) {
    return array(
        '#type' => 'ajax',
        '#commands' => array(
            ajax_command_replace("#div-summary", drupal_render($resu)),
            ajax_command_invoke('#edit-eb-quota-container', 'slider',array('option', array('max' => intval($form_state['values']['disk_space']['value'])))),
            ajax_command_invoke('#edit-eb-quota-container', 'slider',array('value', $form_state['values']['eb_quota']['value'] > $form_state['values']['disk_space']['value'] ? $form_state['values']['disk_space']['value'] : $form_state['values']['eb_quota']['value'])),
            ajax_command_invoke('#edit-eb-quota', 'slider',array('refresh')),
        ),
    );
    } else {    
    return array(
        '#type' => 'ajax',
        '#commands' => array(
            ajax_command_replace("#div-summary", drupal_render($resu)),
        ),
    );
    }
}

function ajax_show_eb_block($form, &$form_state) {
    $ajax_commands = array();
    $member = NULL;
    if (isset($form_state['values']['access_id'])) {
        $member=member_access_load($form_state['values']['access_id']);
    }
    $offre_essai = isset($form_state['values']['offre_essai']) && $form_state['values']['offre_essai']['1'] == '1';
    if ($form_state['values']['eb']['eb'] == '0') {
        $ajax_commands[] =  ajax_command_invoke('#div-eb', 'hide',array('slow'));
        if ($offre_essai) {
            $ajax_html = ajax_set_html(
                    'N1',
                    10*1000000000,
                    10*1000000000,
                    0,
                    $member,
                    'essai'
            );
        } else {
            $ajax_html = ajax_set_html(
                    strval($form_state['values']['level']),
                    intval($form_state['values']['disk_space']['value'])*1000000000,
                    0,
                    dedikam_prix($form_state['values']['level'],intval($form_state['values']['disk_space']['value'])*1000000000),
                    $member,
                    $form_state['values']['action']
            );
        }
    } else {
        $ajax_commands[] =  ajax_command_invoke('#div-eb', 'show',array('slow'));
        if ($offre_essai) {
            $ajax_html = ajax_set_html(
                    'N1',
                    10*1000000000,
                    10*1000000000,
                    0,
                    $member,
                    'essai'
            );
        } else {
            $eb = intval($form_state['values']['eb_quota']['value'])*1000000000;
            $quota = intval($form_state['values']['disk_space']['value'])*1000000000; 
            $eb = $eb > $quota ? $quota : $eb;
            $ajax_html = ajax_set_html(
                    strval($form_state['values']['level']),
                    $quota,
                    $eb,
                    dedikam_prix($form_state['values']['level'],$quota,$eb),
                    $member,
                    $form_state['values']['action']
            );
        }
    }
    $ajax_commands[] =  ajax_command_replace('#div-summary', drupal_render($ajax_html));
    return array('#type' => 'ajax', '#commands' => $ajax_commands);
}


function ajax_set_html($level,$diskspace,$eb,$prix, &$member = NULL, $action = 'new') {
    $header = array(
        'Niveau',
        'Espace disque',
     /*   'Accès',
        'Sauvegardes',*/
        'Total',
    );
    $rows = array();
    
    switch ($action) {
        case 'edit' :
            $prix_data = '<strong class="price">'. max(0,round(($prix-$member->paid)*dedikam_get_prorata_restant($member),2)).' &euro;</strong><br/>jusqu\'au '.dedikam_epochtodate($member->expire,'d/m/Y');
            break;
        case 'renew' :
            $prix_data = '<strong class="price">'.$prix.' &euro;</strong><br/>jusqu\'au '.dedikam_epochtodate($member->expire + 366*24*60*60,'d/m/Y');
            break;
        case 'essai' :
            $prix_data = '<strong class="price">'.$prix.' &euro;</strong><br/>jusqu\'au '.dedikam_epochtodate(date('U') + 30*24*60*60,'d/m/Y');
            break;
        default :
            $prix_data = '<strong class="price">'.$prix.' &euro;</strong><br/>jusqu\'au '.dedikam_epochtodate(date('U')+366*24*60*60,'d/m/Y');
    }
    
    $rows[] = array (
        'data' => array(
            array('data' => $level,'align' => 'center'),
            array('data' => '<strong class="diskspace">'.dedikam_affiche_unite($diskspace).'</strong><br/>('.dedikam_affiche_unite_bi($diskspace).')','align' => 'center'),
          //  array('data' => 'PYDIO<br/>OWNCLOUD<br/>SFTP<br/>RSYNC/SSH','align' => 'center'),
          //  array('data' => 'S. Inc.'.($eb > 0 ? '<br/>S. Ext. : '.dedikam_affiche_unite($eb).'<br/>Prix : '.dedikam_prix_eb($action == 'essai' ? 0 : $eb).' &euro;' : ''),'align' => 'center'),
            array('data' => $prix_data),
        ),
    );
    
    $resu = array(
        '#theme' => 'table',
        '#header' =>$header,
        '#rows' => $rows,
        '#prefix' => '<div id="div-summary">',
        '#suffix' => '</div>',
    );
    return $resu;
}


/*
 * Ajout de l'abonnement au panier
 * attributs :
 *  4 : diskspace
 *  5 : username
 *  7 : level
 *  8 : prorata
 *  11: eb (external backup)
 */
function dedikam_offers_form_submit($form, &$form_state) {
    $profile = $form_state['values']['action'];
    $offre_essai = isset($form_state['values']['offre_essai']) && $form_state['values']['offre_essai']['1'] == '1';
    $diskspace_aid = product_getattributeid('diskspace');
    $username_aid = product_getattributeid('username');
    $account_aid = product_getattributeid('account');
    $level_aid = product_getattributeid('level');
    $prorata_aid = product_getattributeid('prorata');
    $eb_aid = product_getattributeid('eb');
    $cid = uc_cart_get_id();
    $essai_nid = product_getproductid('ESSAI');
    $abo_nid = product_getproductid('ABO');

    if ($profile == 'edit' && $form_state['values']['op'] == $form_state['values']['cancel']) {
        $form_state['redirect'] = 'user/member_access';
        return;
    }
    
    $member = NULL;
    $account = '';
    $expire = NULL;
    $eb = FALSE;
    
    if (isset($form_state['values']['access_id'])) {
        $member=member_access_load($form_state['values']['access_id']);
        $account = $member->name;
        $eb = get_sauvegarde_externe_for_member($member->id);
    }
    
    if ($form_state['values']['action'] == 'renew' && $eb && $form_state['values']['eb']['eb'] == 0) {
        // suppression des anciennes eb
        $record_eb['status'] = 'delete';
        drupal_write_record('dedikam_sauvegarde',$record_eb, $eb->id_sauvegarde);
    }
    
    if ($member) {
        $expire = dedikam_offer_expire($member->expire);
    }
    
    if ($expire || ! $member) {
        // c'est un renouvel de l'abonnement
        $prorata = '';
    } else {
        // c'est une modification de l'abonnement en cours
        $prorata = strval($member->paid).'€ jusqu\'au '.dedikam_epochtodate($member->expire,'d/m/Y');
    }
    
    if ($offre_essai) {
        $nid = $essai_nid; // Offres Dedikam
        $data = array( 
            'attributes' => array( 
                $account_aid => $account,
                $username_aid => $form_state['values']['username'],
                $diskspace_aid => 10,
                $level_aid => 'N1',
                $eb_aid => variable_get('use_eb') ? 10 : 0,
                $prorata_aid => NULL,
            ),
            'module' => 'uc_product',
        );
    } else {
        $nid = $abo_nid; // Offres Dedikam
        if ($form_state['values']['eb']['eb'] == '0') {
            $eb = null;
        } else {
            $eb = intval($form_state['values']['eb_quota']['value']);
            $quota = intval($form_state['values']['disk_space']['value']); 
            $eb = $eb > $quota ? $quota : $eb;
        }
        $data = array( 
            'attributes' => array( 
                $account_aid => $account,
                $username_aid => $form_state['values']['username'],
                $diskspace_aid => $form_state['values']['disk_space']['value'],
                $level_aid => $form_state['values']['level'],
                $eb_aid => $eb,
                $prorata_aid => $prorata,
            ),
            'module' => 'uc_product',
        );
    }

    $items_now=uc_cart_get_contents($cid = NULL, $action = NULL);
    $abos = array();
    $essais= array();
    foreach ($items_now as $item) {
        $itemdata = $item->data;

        // Si un article avec le même username existe, on le supprime avant de l'ajouter ne nouveau
        // pour mettre à jour les autres attributs
        if(($item->nid != $essai_nid) && ($item->nid==strval($nid)) && ($itemdata['attributes'][$username_aid] == $data['attributes'][$username_aid])){
            drupal_set_message('Mise à jour de l\'accès Dedikam'.($itemdata['attributes'][$username_aid] != '' ? ' pour l\'alias '.$itemdata['attributes'][$username_aid] : ''));
            uc_cart_remove_item($item->nid, $cid, $itemdata);
        }
        
        if ($item->nid == $essai_nid) {
            $essais[] = $itemdata;
        } else {
            $abos[] = $itemdata;
        }
    }
    
    $do_insert = TRUE;
    if ($nid == $essai_nid && count($essais) > 0) {
        drupal_set_message('Une offre d\'essai est déjà dans le panier','warning');
        $do_insert = FALSE;
    }
    if ($nid == $essai_nid && count($abos) > 0) {
        drupal_set_message('Vous ne pouvez pas commander une offre d\'essai avec une autre offre, l\'offre d\'essai n\'a pas été ajoutée du panier','warning');
        $do_insert = FALSE;
    }
    if ($nid == $abo_nid && count($essais) > 0) {
        foreach ($essais as $essai) {
            uc_cart_remove_item($essai_nid, $cid, $essai);
        }
        drupal_set_message('Vous ne pouvez pas commander une offre d\'essai avec une autre offre, l\'offre d\'essai a été retirée du panier','warning');
    }
    
    if ($do_insert) {
        $quantity = 1;
        uc_cart_add_item($nid, $quantity, $data, $cid);
    }
    
    $form_state['redirect'] = 'cart';
}

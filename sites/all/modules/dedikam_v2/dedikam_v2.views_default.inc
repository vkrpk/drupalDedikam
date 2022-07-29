<?php
/**
 * @file
 * dedikam_v2.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function dedikam_v2_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'slider';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Slider';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'slideshow';
  $handler->display->display_options['style_options']['slideshow_type'] = 'views_slideshow_cycle';
  $handler->display->display_options['style_options']['slideshow_skin'] = 'default';
  $handler->display->display_options['style_options']['skin_info'] = array(
    'class' => 'default',
    'name' => 'Par défaut',
    'module' => 'views_slideshow',
    'path' => '',
    'stylesheets' => array(),
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'title' => 0,
    'field_slider_image' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['enable'] = 1;
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'title' => 0,
    'field_slider_image' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['timeout'] = '5000';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['speed'] = '700';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['delay'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['start_paused'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['remember_slide_days'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['items_per_slide'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['wait_for_image_load_timeout'] = '3000';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartype'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartypenobg'] = 0;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_slider_image']['id'] = 'field_slider_image';
  $handler->display->display_options['fields']['field_slider_image']['table'] = 'field_data_field_slider_image';
  $handler->display->display_options['fields']['field_slider_image']['field'] = 'field_slider_image';
  $handler->display->display_options['fields']['field_slider_image']['label'] = '';
  $handler->display->display_options['fields']['field_slider_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_slider_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_slider_image']['settings'] = array(
    'image_style' => '1920x280',
    'image_link' => '',
  );
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'slider' => 'slider',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['slider'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Block'),
  );
  $export['slider'] = $view;

  $view = new view();
  $view->name = 'user_member_access';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'dedikam_member_access';
  $view->human_name = 'user_member_access';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Mes accès';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    5 => '5',
    3 => '3',
    4 => '4',
    6 => '6',
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'php_2' => 'php_2',
    'id' => 'id',
    'lastquotacheck' => 'lastquotacheck',
    'quota' => 'quota',
    'php' => 'php',
    'payee' => 'payee',
    'php_9' => 'php_9',
    'name_1' => 'name_1',
    'member_id' => 'member_id',
    'expire' => 'expire',
    'php_4' => 'php_4',
    'level' => 'level',
    'php_1' => 'php_1',
    'quotacheckdatetime' => 'quotacheckdatetime',
    'php_5' => 'php_5',
    'status' => 'status',
    'php_6' => 'php_6',
    'php_8' => 'php_8',
    'php_3' => 'php_3',
  );
  $handler->display->display_options['style_options']['default'] = 'id';
  $handler->display->display_options['style_options']['info'] = array(
    'php_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'lastquotacheck' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'quota' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'payee' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php_9' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'member_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '<br/>',
      'empty_column' => 0,
    ),
    'expire' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php_4' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'level' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php_1' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'quotacheckdatetime' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php_5' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php_6' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php_8' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php_3' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: PHP */
  $handler->display->display_options['header']['php']['id'] = 'php';
  $handler->display->display_options['header']['php']['table'] = 'views';
  $handler->display->display_options['header']['php']['field'] = 'php';
  $handler->display->display_options['header']['php']['php_output'] = '<?php
drupal_add_js(drupal_get_path(\'module\', \'dedikam\').\'/js/highcharts.js\');
drupal_add_js(drupal_get_path(\'module\', \'dedikam\').\'/js/maj_data.js\');
?>
<?php
 print l(\'+ Ajouter un accès\',\'dedikam/offres/form\');
?>
';
  /* Footer: Global: PHP */
  $handler->display->display_options['footer']['php']['id'] = 'php';
  $handler->display->display_options['footer']['php']['table'] = 'views';
  $handler->display->display_options['footer']['php']['field'] = 'php';
  $handler->display->display_options['footer']['php']['php_output'] = '<?php
 print l(\'+ Ajouter un accès\',\'dedikam/offres/form\');
?>
';
  /* No results behavior: Global: PHP */
  $handler->display->display_options['empty']['php']['id'] = 'php';
  $handler->display->display_options['empty']['php']['table'] = 'views';
  $handler->display->display_options['empty']['php']['field'] = 'php';
  $handler->display->display_options['empty']['php']['empty'] = TRUE;
  $handler->display->display_options['empty']['php']['php_output'] = '<h4>Vous ne possédez aucun accès pour l\'instant, ou votre commande est en cours de traitement</h4>
<?php print l(\'+ Ajouter un accès\',\'dedikam/offres/form\'); ?>
';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_2']['id'] = 'php_2';
  $handler->display->display_options['fields']['php_2']['table'] = 'views';
  $handler->display->display_options['fields']['php_2']['field'] = 'php';
  $handler->display->display_options['fields']['php_2']['label'] = 'init';
  $handler->display->display_options['fields']['php_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php_2']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php_2']['php_setup'] = 'function inttocheck($value) {
  return ($value == 1 ? \'✔\' : \'✘\' );
}

function userdir_exists($access) {
 return file_exists("/mnt/mfs/$access");
}';
  $handler->display->display_options['fields']['php_2']['php_value'] = 'return \'\';';
  $handler->display->display_options['fields']['php_2']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_2']['php_click_sortable'] = '';
  /* Field: Member Access: Member access ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Member Access: Lastquotacheck */
  $handler->display->display_options['fields']['lastquotacheck']['id'] = 'lastquotacheck';
  $handler->display->display_options['fields']['lastquotacheck']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['lastquotacheck']['field'] = 'lastquotacheck';
  $handler->display->display_options['fields']['lastquotacheck']['label'] = '';
  $handler->display->display_options['fields']['lastquotacheck']['exclude'] = TRUE;
  $handler->display->display_options['fields']['lastquotacheck']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['lastquotacheck']['separator'] = ' ';
  /* Field: Member Access: Quota */
  $handler->display->display_options['fields']['quota']['id'] = 'quota';
  $handler->display->display_options['fields']['quota']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['quota']['field'] = 'quota';
  $handler->display->display_options['fields']['quota']['label'] = '';
  $handler->display->display_options['fields']['quota']['exclude'] = TRUE;
  $handler->display->display_options['fields']['quota']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['quota']['separator'] = ' ';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Applications Web';
  $handler->display->display_options['fields']['php']['alter']['path'] = 'user/member_access/[id]/edit';
  $handler->display->display_options['fields']['php']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
$link = dedikam_owncloud_build_link($row->id,$row->name_1);
/*
if (userdir_exists($row->name_1) && $row->status == \'validated\' && $link != \'\' && (intval($row->quota) == -1 || intval($row->lastquotacheck) < intval($row->quota))) {
*/
if (userdir_exists($row->name_1) && ($row->status == \'validated\' || strpos($row->status,\'iscsi_\' )  !== FALSE ) && $link != \'\' ) {
  echo \'<div class="logo_appli_content"><span class="logo_appli">Owncloud</span><br/>\'.l(\'<img src="/sites/default/files/owncloud.png" alt="OwnCloud" title="OwnCloud" height="20px" style="margin:auto;">\',\'https://owncloud.dedikam.com/index.php?\'.$link,array(\'html\'=>TRUE));
    echo \'</div><div class="logo_appli_content row_2"><span class="logo_appli">Pydio</span><br/>\'.l(\'<img src="/sites/default/files/pydio.png" alt="Pydio" title="Pydio" height="20px" style="margin:auto;">\',\'https://pydio.dedikam.com/dedikam.php?user=\'.$row->name_1,array(\'html\'=>TRUE));
echo \'</div>\';
} else {
  if ($row->status == \'validated\' && $link != \'\' && intval($row->quota) > 0 && intval($row->lastquotacheck) > intval($row->quota)) {
    echo \'Quota dépassé.<br/>Les applications web sont désactivées.\';
  } else {
    echo \'Accès indisponible.\';
  }
}
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Member Access: Cifs */
  $handler->display->display_options['fields']['cifs']['id'] = 'cifs';
  $handler->display->display_options['fields']['cifs']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['cifs']['field'] = 'cifs';
  $handler->display->display_options['fields']['cifs']['exclude'] = TRUE;
  /* Field: Member Access: Payee */
  $handler->display->display_options['fields']['payee']['id'] = 'payee';
  $handler->display->display_options['fields']['payee']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['payee']['field'] = 'payee';
  $handler->display->display_options['fields']['payee']['label'] = '';
  $handler->display->display_options['fields']['payee']['exclude'] = TRUE;
  $handler->display->display_options['fields']['payee']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_9']['id'] = 'php_9';
  $handler->display->display_options['fields']['php_9']['table'] = 'views';
  $handler->display->display_options['fields']['php_9']['field'] = 'php';
  $handler->display->display_options['fields']['php_9']['label'] = 'Alias du client';
  $handler->display->display_options['fields']['php_9']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php_9']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_9']['php_output'] = '<?php
echo $row->payee;if ($row->status == \'validated\') { ?>
</br>
<a href="<?php echo \'/user/member_access/\'.$row->id.\'/edit\'; ?>" class="modifier" alt="modifier" title="modifier">edit</a>
<?php } ?>';
  $handler->display->display_options['fields']['php_9']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_9']['php_click_sortable'] = '';
  /* Field: Member Access: Label */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = '';
  $handler->display->display_options['fields']['name_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name_1']['alter']['path'] = 'admin/member_access/manage/[id]';
  $handler->display->display_options['fields']['name_1']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['name_1']['alter']['alt'] = 'Modifier';
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
  /* Field: Member Access: Member_id */
  $handler->display->display_options['fields']['member_id']['id'] = 'member_id';
  $handler->display->display_options['fields']['member_id']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['member_id']['field'] = 'member_id';
  $handler->display->display_options['fields']['member_id']['label'] = '';
  $handler->display->display_options['fields']['member_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['member_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['member_id']['separator'] = '';
  /* Field: Member Access: Expire */
  $handler->display->display_options['fields']['expire']['id'] = 'expire';
  $handler->display->display_options['fields']['expire']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['expire']['field'] = 'expire';
  $handler->display->display_options['fields']['expire']['label'] = '';
  $handler->display->display_options['fields']['expire']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expire']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expire']['separator'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_4']['id'] = 'php_4';
  $handler->display->display_options['fields']['php_4']['table'] = 'views';
  $handler->display->display_options['fields']['php_4']['field'] = 'php';
  $handler->display->display_options['fields']['php_4']['label'] = 'Expiration du compte';
  $handler->display->display_options['fields']['php_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_4']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_4']['php_value'] = 'return dedikam_epochtodate($row->expire,\'d/m/Y\');';
  $handler->display->display_options['fields']['php_4']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_4']['php_click_sortable'] = '';
  /* Field: Member Access: Level */
  $handler->display->display_options['fields']['level']['id'] = 'level';
  $handler->display->display_options['fields']['level']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['level']['field'] = 'level';
  $handler->display->display_options['fields']['level']['label'] = '';
  $handler->display->display_options['fields']['level']['exclude'] = TRUE;
  $handler->display->display_options['fields']['level']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'Espace commandé';
  $handler->display->display_options['fields']['php_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_output'] = '<?php
$retour = dedikam_affiche_unite($row->quota);
$retour .= \'<br/>\';
$retour .= \'Niveau : \'.$row->level;
print $retour;
?>';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Field: Member Access: Quotacheckdatetime */
  $handler->display->display_options['fields']['quotacheckdatetime']['id'] = 'quotacheckdatetime';
  $handler->display->display_options['fields']['quotacheckdatetime']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['quotacheckdatetime']['field'] = 'quotacheckdatetime';
  $handler->display->display_options['fields']['quotacheckdatetime']['label'] = '';
  $handler->display->display_options['fields']['quotacheckdatetime']['exclude'] = TRUE;
  $handler->display->display_options['fields']['quotacheckdatetime']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_5']['id'] = 'php_5';
  $handler->display->display_options['fields']['php_5']['table'] = 'views';
  $handler->display->display_options['fields']['php_5']['field'] = 'php';
  $handler->display->display_options['fields']['php_5']['label'] = 'Dernière vérification du quota';
  $handler->display->display_options['fields']['php_5']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_5']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_5']['php_value'] = 'return ($row->quotacheckdatetime > 0 ? dedikam_epochtodate($row->quotacheckdatetime,\'d/m/Y\') : \'\');';
  $handler->display->display_options['fields']['php_5']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_5']['php_click_sortable'] = '';
  /* Field: Member Access: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = '';
  $handler->display->display_options['fields']['status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  /* Field: Member Access: Nb_dirs */
  $handler->display->display_options['fields']['nb_dirs']['id'] = 'nb_dirs';
  $handler->display->display_options['fields']['nb_dirs']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['nb_dirs']['field'] = 'nb_dirs';
  $handler->display->display_options['fields']['nb_dirs']['label'] = '';
  $handler->display->display_options['fields']['nb_dirs']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nb_dirs']['element_label_colon'] = FALSE;
  /* Field: Member Access: Nb_files */
  $handler->display->display_options['fields']['nb_files']['id'] = 'nb_files';
  $handler->display->display_options['fields']['nb_files']['table'] = 'dedikam_member_access';
  $handler->display->display_options['fields']['nb_files']['field'] = 'nb_files';
  $handler->display->display_options['fields']['nb_files']['label'] = '';
  $handler->display->display_options['fields']['nb_files']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nb_files']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_6']['id'] = 'php_6';
  $handler->display->display_options['fields']['php_6']['table'] = 'views';
  $handler->display->display_options['fields']['php_6']['field'] = 'php';
  $handler->display->display_options['fields']['php_6']['label'] = 'Accès client';
  $handler->display->display_options['fields']['php_6']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_6']['php_output'] = '<?php echo $row->name_1; ?>
<br/>
<?php
if ($row->status != \'recreate\') {
if (dedikam_offer_expire($row->expire)) {
?>
<input type="submit" onClick="location.href=\'<?php echo \'/user/offres/\'.$row->id.\'/renew\'; ?>\'" value=\'Renouveler\' class="form-submit views-ajax-processed-processed">
<?php
} else {
?>
<input type="submit" onClick="location.href=\'<?php echo \'/user/offres/\'.$row->id.\'/edit\'; ?>\'" value=\'Modifier\' class="form-submit views-ajax-processed-processed">
<?php
}
$form = drupal_get_form(\'dedikam_member_access_reset_form_\'.$row->id,$row->id);
print drupal_render($form);
}
?>';
  $handler->display->display_options['fields']['php_6']['use_php_click_sortable'] = '2';
  $handler->display->display_options['fields']['php_6']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_8']['id'] = 'php_8';
  $handler->display->display_options['fields']['php_8']['table'] = 'views';
  $handler->display->display_options['fields']['php_8']['field'] = 'php';
  $handler->display->display_options['fields']['php_8']['label'] = 'Détail de l\'accès client';
  $handler->display->display_options['fields']['php_8']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_8']['php_output'] = '<div class="item-row"><i>Exp. du compte :</i> <span class="text-bold-content" style="color:red"><?php echo dedikam_epochtodate($row->expire,\'d/m/Y\'); ?></span><br>
<i>Statut :</i> <span class="text-bold-content"><?php echo ($row->status == \'newaccess\' || $row->status == \'pending\' || userdir_exists($row->name_1) ? dedikam_descstatus($row->status) : \'Problème d\\\'accès au dossier\');  ?></span>

</div>
<div class="item-row">
<i>Niveau :</i><span class="text-bold-content"> <?php echo $row->level; ?>&nbsp;</span>
</div>
<div class="item-row">
<i>Esp. commandé :</i> <span class="text-bold-content"><?php echo $row->quota == -1 ? \'illimité\' : dedikam_affiche_unite($row->quota).\' ( \'.dedikam_affiche_unite_bi($row->quota).\' ) \'; ?>&nbsp;</span>
</div>
<div class="item-row">
<div id="ajax_datemaj_<?php echo $row->id; ?>"><i>Dernière vérification du quota :</i><span class="text-bold-content"> <?php echo ($row->quotacheckdatetime > 0 ? dedikam_epochtodate($row->quotacheckdatetime,\'d/m/Y à H:i\') : \'\'); ?></span></div>
</div>
<div class="item-row">
<i class="edit">Option SI :

<?php 
// Affichage SI
if ( $row->status == \'validated\' ) { ?>
&nbsp;<a href="<?php echo \'/sauvegardes_incrementales/\'.$row->id.\'/edit\'?>" class="modifier" alt="modifier" title="modifier">edit</a>
<?php } ?>

</i><br>


<i>Nombre de tache(s) : </i> <span class="text-bold-content"><?php echo get_nb_sauvegarde_incrementale($row->id); ?></span>
<?php
if ($row->status == \'validated\' || strpos($row->status,\'iscsi_\') !== FALSE) {
$member = member_access_load($row->id);
$form = drupal_get_form(\'dedikam_sauvegarde_notify_form_\'.$row->id,$member);
print drupal_render($form);
}
?>
</div>



<?php if (user_has_role(8)) { // rôle betatest ?>

<div class="item-row">
<i class="edit">Option iSCSI :
<?php
// Affichage de l\'option iSCSI
if ( $row->status == \'validated\' || strpos($row->status,\'iscsi_\') !== FALSE ) { ?>
&nbsp;<a href="<?php echo \'/iscsi/\'.$row->id.\'/edit\'?>" class="modifier" alt="modifier" title="modifier">edit</a>
<?php } ?>
</i><br>
<i>Nbre de disque(s) :</i> <span class="text-bold-content"><?php echo iscsi_get_nb_disks($row->id); ?></span>

</div>


<div class="item-row item-row-last">
<i class="edit">Option CIFS :
<?php
// Affichage de l\'option CIFS ?>
&nbsp;<a href="<?php echo \'/cifs/\'.$row->id.\'/edit\'?>" class="modifier" alt="modifier" title="modifier">edit</a>
<?php } ?>
<?php
$eb = get_sauvegarde_externe_for_member($row->id);
if ($eb) {
?>
<i>Sauv. Ext :</i> <?php echo dedikam_affiche_unite($eb->quota); 
if ($row->status == \'validated\') { ?>
&nbsp;<a href="<?php echo \'/sauvegardes_externes/\'.$eb->id_sauvegarde.\'/edit\'?>" class="modifier" alt="modifier" title="modifier">edit</a>
<?php } ?>
<?php } ?>
</i><br>
<i>Statut :</i> <span class="text-bold-content"><?php echo $row->cifs == 1 ? "activée" : "désactivée"; ?></span>
</div>';
  $handler->display->display_options['fields']['php_8']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_8']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_3']['id'] = 'php_3';
  $handler->display->display_options['fields']['php_3']['table'] = 'views';
  $handler->display->display_options['fields']['php_3']['field'] = 'php';
  $handler->display->display_options['fields']['php_3']['label'] = 'Espace occupé';
  $handler->display->display_options['fields']['php_3']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_3']['php_output'] = '<?php
if ($row->lastquotacheck > $row->quota) {
  if ($row->quota == -1) {
    $lastquotacheck = 0;
    $quota = 1;
  } else {
    $lastquotacheck = $row->quota;
    $quota = $row->quota;
  }
} else {
  $lastquotacheck = $row->lastquotacheck;
  $quota = $row->quota;
}
$libre = round(100 * ($quota - $lastquotacheck)/$quota,1);
$occupe = 100 - $libre;
echo \'<div class="espace-occupe">\';
echo \'<div id="ajax_espace_\'.$row->id.\'" align="center">\';
?>
<script type="text/javascript">
(function ($) { 
$(document).ready(function(){
    $(\'#container_espace_<?php echo $row->id; ?>\').highcharts({
        chart: {
            type: \'column\',
            backgroundColor: \'rgba(255,255,255,0)\',
        },
        credits: {
           enabled: false,
        },
        title: {
           text: \'\',
        },
        plotOptions: {
           column: {
              stacking: \'normal\',
           }
        },
        xAxis: {
           lineWidth: 0,
           labels: {
              enabled: false,
           },
           minorTickLength: 0,
           tickLength: 0,
           minPadding: 0,
        },
        yAxis: {
           max: 100,
           gridLineWidth: 0,
           minorGridLineWidth: 0,
           labels: {
              enabled: false,
           },
           minorTickLength: 0,
           tickLength: 0,
           title: \'\',
        },
        tooltip: {
           headerFormat : \'<span style="font-size: 10px">{series.name} :</span><br/>\',
           pointFormat: \'<span style="font-size: 10px">{point.y} %</span>\',
        },
        series: [
           {
            name: \'libre\',
            data: [<?php echo $libre; ?>],
            showInLegend: false,
           },
           {
            name: \'utilisé\',
            data: [<?php echo $occupe; ?>],
            showInLegend: false,
           },
        ],
        colors: [\'#66FF66\',\'#660000\'],
    });
});
})(jQuery);
</script>
<?php 

$member = member_access_load($row->id);
$form1 = drupal_get_form(\'dedikam_quota_refresh_form_\'.$row->id,$member);
print drupal_render($form1);
?>
<div class="item-row row-quota">
<div id="container_espace_<?php echo $row->id; ?>" style="width:60px; height:90px;"></div>
</div>
<?php
echo \'<div id="ajax_quota_\'.$row->id.\'">\'.dedikam_affiche_unite_bi($member->lastquotacheck).\'</div></div>\';
$form2 = drupal_get_form(\'dedikam_quota_alert_threshold_form_\'.$row->id,$member);
print drupal_render($form2);
echo \'</div>\';
?>
<div class="item-row">

<i>Esp. consommé :</i> <span class="text-bold-content"><?php echo $row->quota == -1 ? \'0\' : round(100 * $row->lastquotacheck /$row->quota,0); ?> %</span><br>
<i>Fichiers :</i> <span class="text-bold-content"><?php if($row->nb_files > 0) { echo $row->nb_files; } ?></span><br>
<i>Dossiers :</i> <span class="text-bold-content"><?php if($row->nb_dirs > 0) { echo $row->nb_dirs; } ?></span>
</div>

';
  $handler->display->display_options['fields']['php_3']['use_php_click_sortable'] = '6';
  $handler->display->display_options['fields']['php_3']['php_click_sortable'] = 'return $row1->lastquotacheck - $row2->lastquotacheck;';
  /* Sort criterion: Member Access: Payee */
  $handler->display->display_options['sorts']['payee']['id'] = 'payee';
  $handler->display->display_options['sorts']['payee']['table'] = 'dedikam_member_access';
  $handler->display->display_options['sorts']['payee']['field'] = 'payee';
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'if ($GLOBALS[\'user\']->uid == $row->member_id) {
  if ($row->status == \'delete\' || $row->status == \'deleted\') {
    return TRUE;
  }
  return FALSE;
} else {
  return TRUE;
}
';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/member_access';
  $handler->display->display_options['menu']['title'] = 'Accès';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['user_member_access'] = array(
    t('Master'),
    t('Mes accès'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('init'),
    t('.'),
    t(','),
    t(' '),
    t('Applications Web'),
    t('Cifs'),
    t('Alias du client'),
    t('Modifier'),
    t('Expiration du compte'),
    t('Espace commandé'),
    t('Dernière vérification du quota'),
    t('Accès client'),
    t('Détail de l\'accès client'),
    t('Espace occupé'),
    t('Page'),
  );
  $export['user_member_access'] = $view;

  return $export;
}

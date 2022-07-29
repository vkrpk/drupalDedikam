<?php
/**
 * @file
 * dedikam.forms
 */

module_load_include('inc','dedikam','dedikam.forms.offres');
module_load_include('inc','dedikam','dedikam.forms.member_access');
module_load_include('inc','dedikam','dedikam.forms.sauvegarde_notify');
module_load_include('inc','dedikam','dedikam.forms.sauvegarde_incrementale');
module_load_include('inc','dedikam','dedikam.forms.sauvegarde_incrementale_delete');
module_load_include('inc','dedikam','dedikam.forms.quota_alert_threshold');
module_load_include('inc','dedikam','dedikam.forms.owncloud_refresh');
module_load_include('inc','dedikam','dedikam.forms.quota_refresh');
module_load_include('inc','dedikam','dedikam.forms.termination');
module_load_include('inc','dedikam','dedikam.forms.incident');
module_load_include('inc','dedikam','dedikam.forms.cancel_account');
module_load_include('inc','dedikam','dedikam.forms.sauvegarde_externalisee_edit');
module_load_include('inc','dedikam','dedikam.forms.serveur');
module_load_include('inc','dedikam','dedikam.forms.member_access_reset');
module_load_include('inc','dedikam','dedikam.forms.options');
module_load_include('inc','dedikam','dedikam.forms.iscsi');
module_load_include('inc','dedikam','dedikam.forms.iscsi_delete');
module_load_include('inc','dedikam','dedikam.forms.cifs');
module_load_include('inc','dedikam','dedikam.forms.webdav');
module_load_include('inc','dedikam','dedikam.forms.owncloud_activation');
module_load_include('inc','dedikam','dedikam.forms.pydio_activation');

function dedikam_forms($form_id) {
    $forms = array();
    if (strpos($form_id, 'dedikam_sauvegarde_incrementale_delete_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_sauvegarde_incrementale_delete_form',
        );
    };
    if (strpos($form_id, 'dedikam_quota_alert_threshold_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_quota_alert_threshold_form',
        );
    };
    if (strpos($form_id, 'dedikam_quota_refresh_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_quota_refresh_form',
        );
    };
    if (strpos($form_id, 'dedikam_owncloud_refresh_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_owncloud_refresh_form',
        );
    };
    if (strpos($form_id, 'dedikam_owncloud_activation_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_owncloud_activation_form',
        );
    };
    if (strpos($form_id, 'dedikam_pydio_activation_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_pydio_activation_form',
        );
    };
    if (strpos($form_id, 'dedikam_termination_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_termination_form',
        );
    };
    if (strpos($form_id, 'dedikam_sauvegarde_notify_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_sauvegarde_notify_form',
        );
    };
    if (strpos($form_id, 'dedikam_member_access_reset_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_member_access_reset_form',
        );
    };
    if (strpos($form_id, 'dedikam_iscsi_delete_form_') === 0) {
        $forms[$form_id] = array(
            'callback' => 'dedikam_iscsi_delete_form',
        );
    };

    return $forms;    
}

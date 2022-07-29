<?php

/**
 * @file
 * Role Expire Views hooks
 *
 * Views module integration with the role expire module. Exposes the following
 * to Views:
 *  Fields:
 *    "Role expiration all data" - all Role expire data
 *    "Role expiration date/time" - the date/time that a role expires
 *    "Role expiration role" - the role that expires at the given Role expiration date/time
 *  Filters:
 *    "Role expiration date/time"
 *    "Role expiration role"
 *  Arguments (aka "Contextual Filters"):
 *    "Role expiration role" - on the querystring as a role ID, not as a role name.
 *
 * NOTE: The Views API hook hook_views_api must be defined in the main module.
 * @see role_expire_views_api() in role_expire.module.
 */


/**
 * Implementation of hook_views_data().
 *
 * The purpose of this hook is to tell Views what data we make available.
 */
function role_expire_views_data() {
  $data['role_expire']['table']['group']  = t('User');

  $data['role_expire']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Expose all role expire information.
  $data['role_expire']['all'] = array(
    'title' => t('Role expiration all data'),
    'help' => t('Shows roles that expire with their expiration date'),
    // Information for displaying the rid
    'field' => array(
      'handler' => 'role_expire_handler_field_all',
      'click sortable' => TRUE,
    ),
  );

  // Expose the role expiration date
  $data['role_expire']['expiry_timestamp'] = array(
    'title' => t('Role expiration date/time'),
    'help' => t('Date and time the role will expire. (See also Role expiration role.)'),
    'field' => array(
      'handler' => 'role_expire_handler_field_expiry_timestamp',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Expose the role id from role_expire
  $data['role_expire']['rid'] = array(
    'title' => t('Role expiration role'),
    'help' => t('The Role that corresponds with the Role expiration date/time'),
    // Information for displaying the rid
    'field' => array(
      'handler' => 'role_expire_handler_field_rid',
      'click sortable' => TRUE,
    ),
    // Information for accepting a rid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_users_roles_rid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'rid',
    ),
    // Information for accepting a uid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_user_roles',
    ),
    // Information for sorting on a uid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Class role_expire_handler_field_all.
 *
 * This class is extended by role_expire_handler_field_expiry_timestamp.
 *
 * @see views_handler_field_user_roles.
 * @see views_handler_field_date.
 */
class role_expire_handler_field_all extends views_handler_field_prerender_list {

  /**
   * {@inheritdoc}
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['uid'];
  }

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['date_format'] = array('default' => 'small');
    $options['custom_date_format'] = array('default' => '');
    $options['timezone'] = array('default' => '');

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    $date_formats = array();
    $date_types = system_get_date_types();
    foreach ($date_types as $key => $value) {
      $date_formats[$value['type']] = t('@date_format format', array('@date_format' => $value['title'])) . ': ' . format_date(REQUEST_TIME, $value['type']);
    }

    $form['date_format'] = array(
      '#type' => 'select',
      '#title' => t('Date format'),
      '#options' => $date_formats + array(
          'custom' => t('Custom'),
        ),
      '#default_value' => isset($this->options['date_format']) ? $this->options['date_format'] : 'small',
    );
    $form['custom_date_format'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom date format'),
      '#description' => t('If "Custom", see the <a href="@url" target="_blank">PHP manual</a> for date formats. Otherwise, enter the number of different time units to display, which defaults to 2.', array('@url' => 'http://php.net/manual/function.date.php')),
      '#default_value' => isset($this->options['custom_date_format']) ? $this->options['custom_date_format'] : '',
      '#dependency' => array(
        'edit-options-date-format' => $this->supported_date_types(),
      ),
    );

    $form['timezone'] = array(
      '#type' => 'select',
      '#title' => t('Timezone'),
      '#description' => t('Timezone to be used for date output.'),
      '#options' => array(
          '' => t('- Default site/user timezone -'),
        ) + system_time_zones(FALSE),
      '#default_value' => $this->options['timezone'],
      '#dependency' => array(
        'edit-options-date-format' => array_merge(array('custom'), array_keys($date_formats)),
      ),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Provide a list of all of the supported standard date types.
   *
   * @return array
   *   The list of supported formats.
   */
  protected function supported_date_types() {
    return array(
      'custom',
    );
  }

  function pre_render(&$values) {
    $uids = array();
    $this->items = array();

    foreach ($values as $result) {
      $uids[] = $this->get_value($result, NULL, TRUE);
    }

    if ($uids) {

      $format = $this->options['date_format'];
      if (in_array($format, $this->supported_date_types())) {
        $custom_format = $this->options['custom_date_format'];
      }
      else {
        $custom_format = '';
      }

      $result = db_query("SELECT e.uid, e.rid, e.expiry_timestamp, r.name FROM {role_expire} e INNER JOIN {role} r ON r.rid = e.rid WHERE e.uid IN (:uids) ORDER BY r.name",
        array(':uids' => $uids));
      foreach ($result as $role) {

        $timezone = !empty($this->options['timezone']) ? $this->options['timezone'] : NULL;
        $expirationDate = format_date(check_plain($role->expiry_timestamp), $format, $custom_format, $timezone);

        $this->items[$role->uid][$role->rid]['role'] = check_plain($role->name) . ' (' . $expirationDate . ')';
        $this->items[$role->uid][$role->rid]['rid'] = $role->rid;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function render_item($count, $item) {
    return $item['role'];
  }
}

/**
 * Class role_expire_handler_field_expiry_timestamp.
 */
class role_expire_handler_field_expiry_timestamp extends role_expire_handler_field_all {
  function pre_render(&$values) {
    $uids = array();
    $this->items = array();

    foreach ($values as $result) {
      $uids[] = $this->get_value($result, NULL, TRUE);
    }

    if ($uids) {

      $format = $this->options['date_format'];
      if (in_array($format, $this->supported_date_types())) {
        $custom_format = $this->options['custom_date_format'];
      }
      else {
        $custom_format = '';
      }

      $result = db_query("SELECT e.uid, e.rid, e.expiry_timestamp, r.name FROM {role_expire} e INNER JOIN {role} r ON r.rid = e.rid WHERE e.uid IN (:uids) ORDER BY r.name",
        array(':uids' => $uids));
      foreach ($result as $role) {

        $timezone = !empty($this->options['timezone']) ? $this->options['timezone'] : NULL;
        $expirationDate = format_date(check_plain($role->expiry_timestamp), $format, $custom_format, $timezone);

        $this->items[$role->uid][$role->rid]['role'] = $expirationDate;
        $this->items[$role->uid][$role->rid]['rid'] = $role->rid;
      }
    }
  }
}

/**
 * Class role_expire_handler_field_rid.
 *
 * @see views_handler_field_user_roles.
 */
class role_expire_handler_field_rid extends views_handler_field_prerender_list {
  /**
   * {@inheritdoc}
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['uid'];
  }

  function pre_render(&$values) {
    $uids = array();
    $this->items = array();

    foreach ($values as $result) {
      $uids[] = $this->get_value($result, NULL, TRUE);
    }

    if ($uids) {
      $result = db_query("SELECT e.uid, e.rid, e.expiry_timestamp, r.name FROM {role_expire} e INNER JOIN {role} r ON r.rid = e.rid WHERE e.uid IN (:uids) ORDER BY r.name",
        array(':uids' => $uids));
      foreach ($result as $role) {
        $this->items[$role->uid][$role->rid]['role'] = check_plain($role->name);
        $this->items[$role->uid][$role->rid]['rid'] = $role->rid;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function render_item($count, $item) {
    return $item['role'];
  }
}

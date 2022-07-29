/**
 * @file JS file to perform authentication and registration for miniOrange
 *       Authentication service.
 */
(function($) {
  var form_names = [ 'mo_auth_authenticate_user',
      'mo_auth_test_email_verification',
      'mo_auth_configure_qrcode_authentication',
      'mo_auth_test_qrcode_authentication', 'mo_auth_inline_registration' ];
  $(document).ready(function() {
    var formIds = document.getElementsByName("form_id");
    for (var i = 0; i < formIds.length; i++) {
      if ($.inArray(formIds[i].value, form_names) != -1) {
        var str = formIds[i].value;
        str = str.replace(/_/g, "-");
        getAuthStatus(str);
      }
    }
  });
  function getAuthStatus(formId) {
    var txId = Drupal.settings.mo_authentication.txId;
    var jsonString = "{\"txId\":\"" + txId + "\"}";
    var url = Drupal.settings.mo_authentication.url;
    $.ajax({
      url : url,
      type : "POST",
      dataType : "json",
      data : jsonString,
      contentType : "application/json; charset=utf-8",
      success : function(result) {
        var response = JSON.parse(JSON.stringify(result));
        console.log(response);
        if (response.status != 'IN_PROGRESS') {
          document.getElementById(formId).submit();
        } else {
          setTimeout(getAuthStatus, 3000, formId);
        }
      }
    });
  }
}(jQuery));
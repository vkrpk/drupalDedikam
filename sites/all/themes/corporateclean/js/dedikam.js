jQuery(document).ready(function($) {
 	$("<label class='btn-refresh' for='edit-btrefresh'>Rafraichir</label>").insertBefore('input[name=btrefresh]');
 	$('.btn-refresh').parent().parent().addClass('form-refresh');
})
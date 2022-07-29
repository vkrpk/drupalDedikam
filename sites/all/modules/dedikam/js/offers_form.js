(function ($) {
  Drupal.behaviors.dedikam = {
    attach: function(context, settings) {
   	$("form#dedikam-offers-form").submit(function(event) {
            if ($("#edit-username").val() == '') {
             	$( "#edit-username-alert" ).html( "<strong style=\"color:#FF0000\">* Le nom de l'accès est obligatoire.</string>" );
	        $( "#edit-username" ).addClass("error");
                $( "#edit-username" ).focus();
            	event.preventDefault();
       	    }
       	});
            
        $("#edit-offre-essai").click(function(){
			if ($("#edit-offre-essai-1").prop('checked')) {
				$("#edit-disk-space-container").slider("disable");
				$("#edit-fs-options").attr('disabled','disabled');
 		               //$("#edit-eb-quota-container").slider("disable");
                		$('#edit-level').addClass('offre-essai');
			} else {
				$("#edit-disk-space-container").slider("enable");
				$("#edit-fs-options").removeAttr('disabled');
                		//$("#edit-eb-quota-container").slider("enable");
                		$('#edit-level').removeClass('offre-essai');
			}
		});

		$("#edit-level input").each(function(){
            if($(this).is(':checked')){
            	$('#votre-choix').remove();
                $(this).parent(".form-item-level").addClass("check");
                $(this).parent(".form-item-level").append("<span id='votre-choix'>Votre choix</span>");

            } else {
            	$(this).parent(".form-item-level").removeClass("check");

            }
        });

         //$('#edit-eb-eb').each(function(){
         //   if($(this).is(':checked')){
         //       $(this).parent().parent().addClass('check');
         //   }else{
         //       $(this).parent().parent().removeClass('check')
         //   }
         //})
    }
  }
})(jQuery);

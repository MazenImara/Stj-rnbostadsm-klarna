(function($) {
	if(Drupal.admin !== undefined) {
		Drupal.admin.behaviors.add_body_margin = function (context, settings, $adminMenu) {
			$('body').attr('style', function(i,s) {
				if(s === undefined) s = '';
				return s + 'margin-top: '+$adminMenu.height()+'px !important;'
			});
		}
	}
})(jQuery);
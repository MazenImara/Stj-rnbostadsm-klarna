(function($) {
	Drupal.behaviors.language_switcher_select_block = {
    attach: function (context, settings) {
      if(settings.language_switcher_select_block !== undefined) {
        var url_mapping = settings.language_switcher_select_block;
        var $language_switcher = $('.language-switcher-select');
        if($language_switcher.length > 0) {
          $language_switcher.change(function() {
            var new_value = $(this).val();
            window.location.href = url_mapping[new_value];
          });
        }
      }
		}
	}
})(jQuery);
(function($) {
	Drupal.behaviors.hero_webform = {
    attach: function (context, settings) {
      var $webform = $('.webform-client-form');
      if($webform.length > 0) {
        $webform.each(function() {
          var $components = $(this).find('.webform-component-textfield input, .webform-component-email input, .webform-component-textarea textarea');
          var $submit = $(this).find('.form-submit');
          if($components.length > 0) {
            $components.each(function() {
              var default_value = $(this).val();
              var $component = $(this);
              $component.focus(function() {
                if($component.val() === default_value) {
                  $component.val('');
                }
              });
              $component.blur(function() {
                if(!$component.val()) {
                  $component.val(default_value);
                }
              });
              $submit.mousedown(function(e) {
                if($component.val() === default_value) {
                  $component.val('');
                }
              }, $component);
            });
          }
        });
      }
		}
	}
})(jQuery);

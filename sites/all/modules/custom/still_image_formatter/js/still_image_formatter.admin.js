(function($) {
	Drupal.behaviors.still_image_formatter_admin = {
    attach: function (context, settings) {
      var field_name = settings.still_image_formatter.field_name;
      var ids = settings.still_image_formatter.ids;
      //Caption      
      var $caption_type = $('#field-ui-display-overview-form #' + ids['caption_type']);
      if($caption_type.length > 0) {
        //Property
        var $caption_property = $('#field-ui-display-overview-form #' + ids['caption_property']);
        if($caption_property.length > 0) {
          if($caption_type.val() !== 'property') {
            $caption_property.hide();
          }
          $caption_type.change(function() {
            if($(this).val() === 'property') {
              $caption_property.show();
            }
            else {
              $caption_property.hide();
            }
          });
        }
        //Custom
        var $caption_custom = $('#field-ui-display-overview-form #' + ids['caption_custom']);
        if($caption_custom.length > 0) {
          if($caption_type.val() !== 'custom') {
            $caption_custom.hide();
          }
          $caption_type.change(function() {
            if($(this).val() === 'custom') {
              $caption_custom.show();
            }
            else {
              $caption_custom.hide();
            }
          });
        }
      }
      //Image link      
      var $link_type = $('#field-ui-display-overview-form #' + ids['link_type']);
      if($link_type.length > 0) {
        //Property
        var $link_property = $('#field-ui-display-overview-form #' + ids['link_property']);
        if($link_property.length > 0) {
          if($link_type.val() !== 'property') {
            $link_property.hide();
          }
          $link_type.change(function() {
            if($(this).val() === 'property') {
              $link_property.show();
            }
            else {
              $link_property.hide();
            }
          });
        }
        //Custom
        var $link_custom = $('#field-ui-display-overview-form #' + ids['link_custom']);
        if($link_custom.length > 0) {
          if($link_type.val() !== 'custom') {
            $link_custom.hide();
          }
          $link_type.change(function() {
            if($(this).val() === 'custom') {
              $link_custom.show();
            }
            else {
              $link_custom.hide();
            }
          });
        }
      }
		}
	}
})(jQuery);
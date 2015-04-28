(function($) {
  update_item_settings = function(){
    var data = {};
    var serialized_data = $('#item-settings-form *').serializeArray();
    for(var i in serialized_data) {
      var name = serialized_data[i].name.split('[').pop().replace(/\]+$/,"");
      var value = serialized_data[i].value;
      data[name] = value;
    }
    $('#item-settings').val(JSON.stringify(data));
  };
  
	Drupal.behaviors.bildspel_admin = {
    attach: function (context, settings) {
      $('#item-settings-form *').change(function() {
        update_item_settings();
      });
      $('#item-settings-form').bind('DOMNodeInserted', function(e) {
        update_item_settings();
      });
		}
	}
})(jQuery);
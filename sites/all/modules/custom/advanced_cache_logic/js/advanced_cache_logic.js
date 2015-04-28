(function($) {
	$(document).ready(function() {
    this.advlogchecked = false;
    
    var $cache = $('#edit-general-cache-toggle');

    if($cache.length>0) {
      var $advlogic = $('.form-item-general-advanced-cache-logic');
      if($advlogic.length>0) {
        if($cache.attr('checked')) {
          $advlogic.show();
        }
        else {
          $advlogic.hide();
          $advlogic.find('input').attr('checked', false);
        }
        this.advlogchecked = $advlogic.find('input').attr('checked');

        $cache.click(function() {
          if($cache.attr('checked')) {
            $advlogic.find('input').attr('checked', this.advlogchecked);
            $advlogic.show();
          }
          else {
            //$advlogic.hide();
            this.advlogchecked = $advlogic.find('input').attr('checked');
            $advlogic.find('input').attr('checked', false);
          }
        });
      }
    }
	});
})(jQuery);
(function($) {
	$(document).ready(function(){
    $sida = $('.form-item-adminpanel-node-types div.form-item-adminpanel-node-types-page');
    if($sida.length > 0) {
      $sidtyper = $('.form-item-adminpanel-sidtyper');
      if($sidtyper.length > 0) {
        $sida.after($sidtyper);
        var sida_selected = $sida.children('.form-checkbox').attr('checked');
        if(!sida_selected) {
          $sidtyper.hide();
        }
        $sida.click(function() {
          sida_selected = $sida.children('.form-checkbox').attr('checked');
          if(sida_selected) {
            $sidtyper.show();
          }
          else {
            $sidtyper.hide();
          }
        });
      }
    }
  });
})(jQuery);
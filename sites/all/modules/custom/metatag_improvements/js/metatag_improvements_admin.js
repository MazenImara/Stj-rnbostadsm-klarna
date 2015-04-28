(function($) {
	$(document).ready(function(){
    $sida = $('.form-item-metatag-improvements-allowed-types div.form-item-metatag-improvements-allowed-types-page');
    if($sida.length > 0) {
      $sidtyper = $('.form-item-metatag-improvements-allowed-sidtyp');
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
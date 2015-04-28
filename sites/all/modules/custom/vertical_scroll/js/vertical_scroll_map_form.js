(function($) {
	$(document).ready(function(){
    var $wrapper = $('#edit-data > .fieldset-content');
    if($wrapper.length>0) {
      var tabledrag;
      var sticky;
      var maintable;
      $wrapper.children().each(function() {
        if($(this).hasClass('tabledrag-toggle-weight-wrapper')) {
          tabledrag = this;
        }
        else if($(this).hasClass('sticky-header')) {
          sticky = this;
        }
        else if($(this).hasClass('tabledrag-processed')) {
          $(tabledrag).addClass($(this).attr('id'));
          $(sticky).addClass($(this).attr('id'));
          if($(this).hasClass('hidden')) {
            $(tabledrag).addClass('hidden');
            $(sticky).addClass('hidden');
          }
        }
      });
    }
    $('#edit-destinations a.destination').click(function() {
      if(!$(this).hasClass('active')) {
        $(this).addClass('active');
        $(this).siblings().removeClass('active');
        var classes = $(this).attr('class').split(' ');
        for(var i in classes) {
          var parts = classes[i].split('-');
          if(parts[1]) {
            if($wrapper.length>0) {
              $wrapper.find('#sources-'+parts[1]+', .sources-'+parts[1]).removeClass('hidden');
              $wrapper.children().not('#sources-'+parts[1]+', .sources-'+parts[1]).addClass('hidden');
            }
          }
        }
      }
      return false;
    });
  });
})(jQuery);
(function($) {
  $.fn.getAttributes = function() {
      var attributes = {}; 

      if( this.length ) {
          $.each( this[0].attributes, function( index, attr ) {
              attributes[ attr.name ] = attr.value;
          } ); 
      }

      return attributes;
  };
	$(document).ready(function() {
    $('.node-galleribild .gallery-bildtext').each(function() {
      var image = $(this).siblings('a').children('img');
      if($(image).length > 0) {
        if($(image).attr('width')) {
          $(this).css('width', $(image).attr('width'));
        }
        else {
          $(this).css('width', $(image).attr('data-wdith'));
        }
      }
    });
	});
})(jQuery);

(function($) {
  $.fn.limitMaxlength = function(options){
    this.after($('<div class="charsRemaining">'));
    var settings = jQuery.extend({
        attribute: "maxlength",
        onLimit: function(){},
        onEdit: function(){}
    }, options);
    if($.isFunction( metatag_improvements_get_defaults )) {
      var defaults = metatag_improvements_get_defaults(settings.type);
      var maxlength = defaults['maxlength'];
      this.attr('maxlength', defaults['maxlength']);
      this.attr('default', defaults['value']);
      if(this.val()==defaults['value']) {
        this.siblings('.charsRemaining').css('visibility', 'hidden');
      }
    }
    // Event handler to limit the textarea
    var onEdit = function(){
        var textarea = jQuery(this);
        //var maxlength = parseInt(textarea.attr(settings.attribute));

        if(textarea.val().length > maxlength){
            textarea.val(textarea.val().substr(0, maxlength));

            // Call the onlimit handler within the scope of the textarea
            jQuery.proxy(settings.onLimit, this)();
        }

        // Call the onEdit handler within the scope of the textarea
        jQuery.proxy(settings.onEdit, this)(maxlength - textarea.val().length);
    }

    this.each(onEdit);

    return this.keyup(onEdit)
                .keydown(onEdit)
                .focus(onEdit)
                .live('input paste', onEdit);
  }
	$(document).ready(function(){
    var onEditCallback = function(remaining){
        $(this).siblings('.charsRemaining').text("Tecken kvar: " + remaining);
        if(remaining > 0){
            //$(this).removeClass('border-red');
        }
    }
    var onLimitCallback = function(){
        //$(this).addClass('border-red');
    }
    $('.metatags-form #edit-metatags-description-value').limitMaxlength({
        onEdit: onEditCallback,
        onLimit: onLimitCallback,
        type: "description"
    });
    $('.metatags-form #edit-metatags-title-value').limitMaxlength({
        onEdit: onEditCallback,
        onLimit: onLimitCallback,
        type: "title"
    });
    $('.metatags-form #edit-metatags-description-value').click(function() {
      if($(this).val()==$(this).attr('default')) {
        $(this).val('');
        $(this).siblings('.charsRemaining').text("Tecken kvar: " + $(this).attr('maxlength'));
        $(this).siblings('.charsRemaining').css('visibility', 'visible');
      }
    });
    $('.metatags-form #edit-metatags-description-value').blur(function() {
			if($(this).val()=='') {
				$(this).val($(this).attr('default'));
				$(this).siblings('.charsRemaining').css('visibility', 'hidden');
			}
		});
		$('.metatags-form #edit-metatags-title-value').click(function() {
      if($(this).val()==$(this).attr('default')) {
        $(this).val('');
        $(this).siblings('.charsRemaining').text("Tecken kvar: " + $(this).attr('maxlength'));
        $(this).siblings('.charsRemaining').css('visibility', 'visible');
      }
    });
    $('.metatags-form #edit-metatags-title-value').blur(function() {
			if($(this).val()=='') {
				$(this).val($(this).attr('default'));
				$(this).siblings('.charsRemaining').css('visibility', 'hidden');
			}
		});
  });
})(jQuery);
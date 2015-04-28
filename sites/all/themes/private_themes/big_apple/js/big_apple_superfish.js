(function($) {
	$(document).ready(function(){
		$('#block-superfish-2 ul ul:visible').each(function() {
			var this_height = $(this).height();
			var this_parent = $(this).parent().parent();
			if($(this_parent).length > 0) {
				$(this_parent).css('margin-bottom', this_height);
			}
		});
		$('#block-superfish-2 ul ul:visible').livequery(function() {
			var this_height = $(this).height();
			var this_parent = $(this).parent().parent();
			if($(this_parent).length > 0) {
				$(this_parent).css('margin-bottom', this_height);
			}
		});
		/*
		$('#block-superfish-2 ul ul:hidden').livequery(function() {
			var this_parent = $(this).parent().parent();
			if($(this_parent).length > 0) {
				$(this_parent).css('margin-bottom', '0');
			}
		});
		*/
  });
})(jQuery);
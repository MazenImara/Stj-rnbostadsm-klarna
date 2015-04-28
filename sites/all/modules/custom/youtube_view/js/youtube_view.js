(function($) {
	Drupal.behaviors.youtube_view = {
    attach: function (context, settings) {
			var container = $('#youtube-view');
			if($(container).length > 0) {
				$.ajax({
				    url: 'ajax/pull-youtube-videos',
				    type: 'GET',
				    success: function(result) {
							$(container).html(result);
				    }
				});
			}
		}
	}
})(jQuery);
(function($) {
  $.fn.linkimagefield_nivoslider_bindswipe = function() {
    $(this).swipe( {
      swipeLeft:function(event, direction, distance, duration, fingerCount) {
        $(this).find('.nivo-directionNav .nivo-nextNav').click();
      },
      swipeRight:function(event, direction, distance, duration, fingerCount) {
        $(this).find('.nivo-directionNav .nivo-prevNav').click();
      },
      threshold:50
    });
  }
})(jQuery);
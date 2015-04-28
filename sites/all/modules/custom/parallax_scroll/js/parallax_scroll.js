(function($) {
  var last_scroll = 0;
  parallax_init = function() {
    var total_height = $('#page').height();
    for(var i in Drupal.settings.parallax_scroll.jqlayers) {
      if(Drupal.settings.parallax_scroll.jqlayers[i].data('type') === 'background') {
        Drupal.settings.parallax_scroll.jqlayers[i].css('max-height', total_height);
      }
    }
  };
  $.fn.check_top_limits = function(top) {
    var top_limit_min = $(this).data('top-min');
    if (typeof top_limit_min !== 'undefined' && top_limit_min !== false) {
      var top_limit_max = $(this).data('top-max');
      if(top < top_limit_min || (top - 0 + $(this).data('height')) > top_limit_max) {
        if($(this).is(":visible")) {
          $(this).hide();
        }
      }
      else {
        if(!$(this).is(":visible")) {
          $(this).show();
        }
      }
    }
  };
	Drupal.behaviors.parallax_scroll = {
    attach: function (context, settings) {
      if(Drupal.settings.parallax_scroll.layers !== undefined) {
        last_scroll = $(window).scrollTop();
        var $html = $('<div id="parallax-wrapper"></div>');
        Drupal.settings.parallax_scroll.jqlayers = [];
        for(var i in Drupal.settings.parallax_scroll.layers) {
          var layer = Drupal.settings.parallax_scroll.layers[i];
          var id = 'id="parallax-layer-'+i+'" ';
          var type = 'data-type="background"';
          var height = '';
          if(layer['picture'] === '_none') {
            id = '';
            type = 'data-type="sprite"';
            height = 'data-height="'+layer.height+'"';
          }
          var $layer = $('<div class="parallax-layer parallax-layer-'+i+'" '+id+type+height+'data-speed="'+layer.speed+'"></div>');
          if($layer.data('type') === 'background' && $layer.data('speed') == 1) {
            $layer.css({
              'position': 'absolute'
            });
          }
          $html.append($layer);
          Drupal.settings.parallax_scroll.jqlayers.push($layer);
        }
        $('body').prepend($html);
        $(window).load(function() {
          parallax_init();
        });
        $(window).resize(function() {
          parallax_init();
        });
        
        // add a smooth mousewheel
        $(window).mousewheel(function(event, delta) {    
          $.scrollTo.window().queue([]).stop();
          if (delta < 0) {
            $('body').stop().scrollTo('+=100', 500);
          } else
            $('body').stop().scrollTo('-=100', 500);

          return false;
        });
        
        $(window).scroll(function() {
          var new_scroll = $(window).scrollTop();
          var scroll_diff = new_scroll - last_scroll;
          var scroll_diff_abs = Math.abs(scroll_diff);
          last_scroll = new_scroll;
          for(var i in Drupal.settings.parallax_scroll.jqlayers) {
            var $layer = Drupal.settings.parallax_scroll.jqlayers[i];
            if($layer.data('type') === 'background') {
              if($layer.data('speed') != 1) {
                var yPos = -($(window).scrollTop() * $layer.data('speed'));
                // Put together our final background position
                var coords = '50% '+ yPos + 'px';
                // Move the background
                $layer.css({ backgroundPosition: coords });
              }
            }
            else if($layer.data('type') === 'sprite') {
              var yPos = $layer.position().top;
              yPos += (scroll_diff * $layer.data('speed'));
              $layer.css({ top: yPos });
              $layer.check_top_limits(yPos);
            }
          }
        });
      }
		}
	}
})(jQuery);
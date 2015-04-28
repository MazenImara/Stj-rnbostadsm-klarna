(function($) {
  var vertical_scroll_sources;
  var vertical_scroll_settings;
  var donothing;
  $(window).hashchange(function() {
    if(!donothing) {
      var vertical_scroll_immaname;
      if(window.location.hash) {
        vertical_scroll_immaname = window.location.hash.substr(1);
      }
      else {
        vertical_scroll_immaname = '';
      }
/*
      if(vertical_scroll_immaname) {
        $.scrollTo('a[id="vertical-scroll-anchor-'+vertical_scroll_immaname+'"]', {
          duration: parseInt(vertical_scroll_settings['duration']),
          margin: parseInt(vertical_scroll_settings['margin']),
          offset: parseInt(vertical_scroll_settings['offset'])
        });
      }
      else {
        $.scrollTo({top:0, left:0}, {
          duration: parseInt(vertical_scroll_settings['duration']),
          margin: parseInt(vertical_scroll_settings['margin']),
          offset: parseInt(vertical_scroll_settings['offset'])
        });
      }
*/
      $('.menu li a').each(function() {
        if($(this).attr('href') == '#'+vertical_scroll_immaname) {
          $(this).addClass('active');
        }
        else {
          $(this).removeClass('active');
        }
      });
    }
    donothing = false;
  });

	$(document).ready(function(){
    vertical_scroll_sources = Drupal.settings.vertical_scroll_sources;
    vertical_scroll_settings = Drupal.settings.vertical_scroll_settings;

    for (fragment in vertical_scroll_sources) {
      for (i in vertical_scroll_sources[fragment]) {
        vertical_scroll_sources[fragment][i] = 'a[href = "'+vertical_scroll_sources[fragment][i]+'"]';
      }
      var selectors = vertical_scroll_sources[fragment].join(", ");
      $(selectors).each(function() {
        $(this).attr('href', fragment);
        $(this).click(function(e) {
          donothing = true;
          var vertical_scroll_aname = $(this).attr('href').substr(1);
          var link_title = '';
          var menu_parent = $(this).parents('.menu');
          if($(menu_parent).length>0) { //Den här länken hör till en meny, gör den active och alla andra inactive
            $(menu_parent).find('li a').removeClass('active');
            $(this).addClass('active');
            link_title = $(this).html();
          }

          if(vertical_scroll_aname) {
            $.scrollTo('a[id="vertical-scroll-anchor-'+vertical_scroll_aname+'"]', {
							axis: 'y',
              duration: parseInt(vertical_scroll_settings['duration']),
              margin: parseInt(vertical_scroll_settings['margin']),
              offset: {left: 0, top: parseInt(vertical_scroll_settings['offset'])}
            });
          }
          else {
            $.scrollTo({top:0, left:0}, {
							axis: 'y',
              duration: parseInt(vertical_scroll_settings['duration']),
              margin: parseInt(vertical_scroll_settings['margin']),
              offset: {left: 0, top: parseInt(vertical_scroll_settings['offset'])}
            });
          }

          window.location.hash = $(this).attr('href');
          e.preventDefault();
        });
      });
    }
    if(window.location.hash) {
      var vertical_scroll_immaname = window.location.hash.substr(1);
      if(vertical_scroll_immaname) {
        $.scrollTo('a[id="vertical-scroll-anchor-'+vertical_scroll_immaname+'"]', {
					axis: 'y',
          duration: parseInt(vertical_scroll_settings['duration']),
          margin: parseInt(vertical_scroll_settings['margin']),
          offset: {left: 0, top: parseInt(vertical_scroll_settings['offset'])}
        });
      }
      $('.menu li a').each(function() {
        if($(this).attr('href') === '#'+vertical_scroll_immaname) {
          $(this).addClass('active');
        }
        else {
          $(this).removeClass('active');
        }
      });
    }
  });
})(jQuery);
(function($) {
  var bildspel_list = [];
  
  $.fn.nextOrFirst = function () {
    var next = this.next.apply(this, arguments);
    if (!next.length) {
      var siblings = this.siblings();
      next = siblings.first();
    }
    return next;
  };
  $.fn.previousOrLast = function () {
    var previous = this.prev.apply(this, arguments);
    if (!previous.length) {
      var siblings = this.siblings();
      previous = siblings.last();
    }
    return previous;
  };
  $.fn.isAfter = function(sel){
    return this.prevAll(sel).length !== 0;
  };
  
  bildspel = function(element, settings) {
    this.settings = settings;
    this.locked = false;
    this.$bildspel = $(element);
    this.$active_slide = this.$bildspel.children('li.active');
    this.$next_slide = null;
    this.items = [];
    this.$wrapper = this.$bildspel.parent();
    this.init();
  };
  bildspel.prototype = {
    init: function() {
      var self = this;
      self.$bildspel.children('li').each(function(i) {
        $(this).attr('data-index', i);
        self.items.push({element: $(this)});
      });
      self.$wrapper.addClass('bildspel-wrapper');
      //Initialize list
      self.update_size();
      for(var i in self.items) {
        $(self.items[i].element).css({
          'position': 'absolute',
          'top': 0,
          'left': 0,
          'width': '100%'
        });
      }

      //Lägg bara till slide options om det finns mer än ett element
      if(self.items.length > 1) {
        //Add navigation
        if(self.settings.navigation) { 
          var $navigation = $('<div class="bildspel-navigation"></div>');
          var $previous = $('<span class="previous"></span>');
          $previous.click(function() {
            self.$next_slide = self.$active_slide.previousOrLast();
            self.slide(0);
          });
          var $next = $('<span class="next"></span>');
          $next.click(function() {
            self.$next_slide = self.$active_slide.nextOrFirst();
            self.slide(1);
          });
          $navigation.append($previous, $next);
          self.$wrapper.append($navigation);
        }
        //Add pager
        if(self.settings.pager) {
          var $pager_wrapper = $('<div class="bildspel-pager-wrapper"></div>');
          var $pager = $('<div class="bildspel-pager"></div>');
          for(var i in self.items) {
            var $item = self.items[i].element;
            var $dot = $('<span class="pager-item">');
            $dot.attr('data-index', i);
            $dot.addClass($item.attr('class'));
            $dot.click(function() {
              if(!$(this).hasClass('active')) {
                self.$next_slide = self.items[$(this).data('index')].element;
                self.slide();
              }
            });
            self.items[i].dot = $dot;
            $pager.append($dot);
          }
          $pager_wrapper.append($pager);
          self.$wrapper.append($pager_wrapper);
        }
        //Initiate autoslide
        if(self.settings.frame_length > 0) {
          self.$wrapper.bind('mouseenter, mouseover', function() {
            self.$wrapper.addClass('mouseover');
          });
          self.$wrapper.bind('mouseout', function() {
            self.$wrapper.removeClass('mouseover');
          });
          setInterval(function() {
            self.autoslide();
          }, self.settings.frame_length * 1000);
        }
      }
    },
    update_size: function() {
      var self = this;
      //Get height of active li
      var height = self.$active_slide.height();
      self.$bildspel.css({
        'height': height
      });
    },
    autoslide: function() {
      var self = this;
      if(!self.$wrapper.hasClass('mouseover')) {
        self.$next_slide = self.$active_slide.nextOrFirst();
        self.slide(1);
      }
    },
    slide: function(direction) {
      var self = this;      
      if(!self.locked) {
        self.$active_slide.finished = false;
        self.$next_slide.finished = false;
        self.locked = true;
        var $pager = $(this).parent().children('.bildspel-pager-wrapper').children('.bildspel-pager');
        if($pager.length > 0) {
          $pager.children('span').removeClass('active');
          self.$next_slide.dot.addClass('active');
        }
        switch(this.settings.transition) {
          case 'slide_horizontal':
            if(direction === undefined) {
              direction = 1;
              if(self.$active_slide.isAfter(self.$next_slide)) {
                direction = 0;
              }
            }
            var width = self.$active_slide.width();
            if(direction == 1) { //Kom in från höger
              var active_target_left = 0 - width;
              self.$next_slide.css('left', width);
            }
            else { //Kom in från vänster
              var active_target_left = width;
              self.$next_slide.css('left', 0 - width);
            }
            var active_target = {left: active_target_left};
            var next_target = {left: 0};
            break;
          case 'fade':
          default:
            self.$next_slide.css('opacity', 0);
            var active_target = {opacity: 0};
            var next_target = {opacity: 1};
        }
        self.$next_slide.css('display', 'list-item');
        self.$active_slide.animate(
          active_target, 
          {
            duration: 500,
            queue: false,
            complete: function() {
              self.$active_slide.finished = true;
              if(self.$next_slide.finished) {
                self.slide_completed();
              }
            }
          }
        );
        self.$next_slide.animate(
          next_target, 
          {
            duration: 500,
            queue: false,
            complete: function() {
              self.$next_slide.finished = true;
              if(self.$active_slide.finished) {
                self.slide_completed();
              }
            }
          }
        );
      }
    },
    slide_completed: function() {
      var self = this;
      self.$active_slide.removeClass('active');
      self.$active_slide.css({
        'display': 'none'
      });
      self.$next_slide.addClass('active');
      self.$active_slide = self.$next_slide;
      self.$next_slide = null;
      var $pager = self.$wrapper.children('.bildspel-pager-wrapper').children('.bildspel-pager');
      if($pager.length > 0) {
        $pager.children('span').removeClass('active');
        self.items[self.$active_slide.data('index')].dot.addClass('active');
      }
      self.locked = false;
    }
  };
    
	Drupal.behaviors.bildspel = {
    attach: function (context, settings) {
      $(window).load(function() {
        var settings = Drupal.settings.bildspel;
        $bildspel = $('.bildspel');
        $bildspel.each(function() {
          var this_bildspel = new bildspel(this, settings);
          bildspel_list.push(this_bildspel);
        });
      });
      $(window).resize(function() {
        for(var i in bildspel_list) {
          bildspel_list[i].update_size();
        }
      });
		}
	}
})(jQuery);
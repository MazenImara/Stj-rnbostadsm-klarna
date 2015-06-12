(function($) {
	Drupal.behaviors.hero_hero = {
    height: 0,
    locked: false,
    movement: [0],
    offsets: [0],
    offsetY: 0,
    init: function() {
      var self = this;
      self.height = 0;
      self.locked = false;
      self.movement = [0];
      self.offsets = [0];
      self.offsetY = 0;
            
      if(!$('#block-superfish-3').is(':visible')) {
        self.reset();
        return;
      }
      
      var $block = $('#zone-branding #block-views-bildspel-block');
      var $herobilder = $('#zone-branding #block-views-bildspel-block .field-name-field-bild img');
      var $title = $('#zone-branding .field-name-title-field');
      var $body = $('#views_slideshow_cycle_teaser_section_bildspel-block .field-name-body');
      
      var window_height = $(window).height();
      var header_height = $('#zone-user').height();
      var old_height = $block.height();
      var new_height = window_height - header_height;
      var height_diff = new_height - old_height;
      var old_body_top = parseInt($body.css('top'), 10);
      var new_body_top = 226;
      if(new_height > 550) {
        new_body_top = old_body_top + height_diff;
      }
      var new_title_bottom = new_height - new_body_top + 30;
      
      $block.css('height', new_height);
      $title.css('bottom', new_title_bottom);
      $body.css('top', new_body_top);
      $herobilder.css('height', new_height);
      
      self.offsetY = $(document).scrollTop();
    },
    reset: function() {
      var $block = $('#zone-branding #block-views-bildspel-block');
      var $herobilder = $('#zone-branding #block-views-bildspel-block .field-name-field-bild img');
      var $title = $('#zone-branding .field-name-title-field');
      var $body = $('#views_slideshow_cycle_teaser_section_bildspel-block .field-name-body');
      $block.css('height', '');
      $title.css('bottom', '');
      $body.css('top', '');
      $herobilder.css('height', '');
    },
    aniscroll: function(direction) {
      var self = this;
      var destination = 0;
      if(direction == 1) {
        destination = self.height;
      }
      $("html, body").animate(
        {scrollTop: destination}, 
        {
          duration: 150,
          queue: false,
          complete: function() {
            self.offsetY = $(document).scrollTop();
            if(self.offsetY > destination) {
              $("html, body").scrollTop(destination - 1);
              self.offsetY = $(document).scrollTop();
            }
            else if(self.offsetY < destination) {
              $("html, body").scrollTop(destination + 1);
              self.offsetY = $(document).scrollTop();
            }
            self.movement = [0];
            self.offsets = [0];
            self.locked = false;
          }
        }
      );
    },
    attach: function (context, settings) {
      var self = this;
      var $herobilder = $('#zone-branding #block-views-bildspel-block .field-name-field-bild img');
      if($herobilder.length > 0) {
        self.init();
        $(window).resize(function() {
          self.init();
        });
        $herobild = $herobilder.first();
        //Hantera hjulscrollning
        $(document).bind('mousewheel DOMMouseScroll', function(event){
          $block = $('#zone-branding #block-views-bildspel-block');
          self.height = parseInt($herobild.css('height'), 10) + $block.offset().top;
          if(self.height && !self.locked) {
            var offsetY_current = $(document).scrollTop();
            if (event.originalEvent.wheelDelta > 0 || event.originalEvent.detail < 0) { //Scrolla upp
              if(offsetY_current <= self.height) {
                event.preventDefault();
                self.locked = true;
                self.aniscroll(0);
              }
            }
            else { //Scrolla ner
              if(offsetY_current >= 0 && offsetY_current < self.height) {
                event.preventDefault();
                self.locked = true;
                self.aniscroll(1);
              }
            }
          }
          else {
            event.preventDefault();
          }
        });
      }
		}
	}
})(jQuery);
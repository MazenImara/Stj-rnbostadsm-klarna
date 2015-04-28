(function($) {
	$(document).ready(function() {
    //---------Tabbar
    var tabs = new Array();
    //Allmänt
    tabs[0] = new Array();
    tabs[0]['name'] = 'general';
    tabs[0]['title'] = 'Allm&auml;nt';
    tabs[0]['elements'] = new Array();
    tabs[0]['elements'].push($('#edit-general-admin-variables'));
    tabs[0]['elements'].push($('.form-item-general-admin-open-website'));
    //Funktioner
    tabs[1] = new Array();
    tabs[1]['name'] = 'functions';
    tabs[1]['title'] = 'Funktioner';
    tabs[1]['elements'] = new Array();
    tabs[1]['elements'].push($('#edit-reseller'));
    tabs[1]['elements'].push($('#edit-functions'));
    tabs[1]['elements'].push($('.form-item-general-admin-theme'));
    tabs[1]['elements'].push($('.form-item-files-general-admin-install-theme'));
    tabs[1]['elements'].push($('#edit-general-admin-language'));
    tabs[1]['elements'].push($('.form-item-general-admin-responsive'));
    //Övrigt
    tabs[2] = new Array();
    tabs[2]['name'] = 'other';
    tabs[2]['title'] = '&Ouml;vrigt';
    tabs[2]['elements'] = new Array();
    tabs[2]['elements'].push($('#edit-general-admin-production'));
    tabs[2]['elements'].push($('#edit-general-admin-production'));
    tabs[2]['elements'].push($('.form-item-general-admin-reset-passwords'));
		tabs[2]['elements'].push($('.form-item-general-admin-reset-flood'));
    tabs[2]['elements'].push($('.form-item-general-admin-clear-test-nodes'));
    //Skapar tabbar
    var tabs_el = $('<div class="document-tabs">');
    var ul = $('<ul>');
    for(var i in tabs) {
      var tab = $('<li class="document-tab '+tabs[i]['name']+'"</li>');
      $(tab).append('<span>'+tabs[i]['title']+'</span>');
      //Sätter defaulttab till "Allmänt"
      if(i == 0) {
        $(tab).addClass('active');
      }
      else {
        for(var j in tabs[i]['elements']) {
					if($(tabs[i]['elements'][j]).length > 0) {
          	$(tabs[i]['elements'][j]).hide();
					}
        }
      }
      $(ul).append(tab);
    }
    $(tabs_el).append(ul);
    $('.column-main').prepend(tabs_el);
    $('.document-tabs ul li span').click(function() {
      var this_parent = $(this).parent();
      if(!$(this_parent).hasClass('active')) {
        $(this_parent).siblings().removeClass('active');
        $(this_parent).addClass('active');
        for(var i in tabs) {
          for(var j in tabs[i]['elements']) {
						if($(tabs[i]['elements'][j]).length > 0) {
	            if($(this_parent).hasClass(tabs[i]['name'])) {
	              $(tabs[i]['elements'][j]).show();
	            }
	            else {
	              $(tabs[i]['elements'][j]).hide();
	            }
						}
          }
        }
      }
    });

    //----Cachning
    this.cssjschecked = false;
    $cache = $('#edit-general-admin-cache-toggle');
    if($cache.length>0) {
      $cssjs = $('.form-item-general-admin-cache-css-js');
      $backend = $('.form-type-radios.form-item-general-admin-pagecache-backend');
      if($cssjs.length>0) {
        if($cache.attr('checked')) {
          $cssjs.show();
          $backend.show();
        }
        else {
          $cssjs.hide();
          $cssjs.find('input').attr('checked', false);
          $backend.hide();
        }
        this.cssjschecked = $cssjs.find('input').attr('checked');

        $cache.click(function() {
          if($cache.attr('checked')) {
            $cssjs.find('input').attr('checked', this.cssjschecked);
            $cssjs.show();
            $backend.show();
          }
          else {
            $cssjs.hide();
            this.cssjschecked = $cssjs.find('input').attr('checked');
            $cssjs.find('input').attr('checked', false);
            $backend.hide();
          }
        });
      }
    }
    //---------Facebook like button
    $facebook = $('#edit-facebook-like-button');
    $custom_address = $('.form-item-facebook-like-button-address');
    if($facebook.length>0) {
      if($custom_address.length>0) {
        if($facebook.attr('checked')) {
          $custom_address.show();
        }
        else {
          $custom_address.hide();
        }
      }
      $facebook.click(function() {
        if($facebook.attr('checked')) {
          if($custom_address.length>0) {
            $custom_address.show();
          }
        }
        else {
          if($custom_address.length>0) {
            $custom_address.hide();
          }
        }
      });
    }
    //----------Sociala medier
    $social = $('#edit-social');
    if($social.length>0) {
      $facebook_id = $('.form-item-social-facebook-id');
      $facebook_current = $('.social-current-facebook-icon');
      $facebook_path = $('.form-item-social-facebook-icon-path');
      $facebook_upload = $('.form-item-files-social-facebook-icon-upload');
      $twitter_name = $('.form-item-social-twitter-name');
      $twitter_current = $('.social-current-twitter-icon');
      $twitter_path = $('.form-item-social-twitter-icon-path');
      $twitter_upload = $('.form-item-files-social-twitter-icon-upload');
      if($social.attr('checked')) {
        if($facebook_current.length>0) $facebook_current.show();
        if($facebook_path.length>0) $facebook_path.show();
        if($facebook_upload.length>0) $facebook_upload.show();
        if($twitter_current.length>0) $twitter_current.show();
        if($twitter_path.length>0) $twitter_path.show();
        if($twitter_upload.length>0) $twitter_upload.show();
      }
      else {
        if($facebook_current.length>0) $facebook_current.hide();
        if($facebook_path.length>0) $facebook_path.hide();
        if($facebook_upload.length>0) $facebook_upload.hide();
        if($twitter_current.length>0) $twitter_current.hide();
        if($twitter_path.length>0) $twitter_path.hide();
        if($twitter_upload.length>0) $twitter_upload.hide();
      }
      $social.click(function() {
        if($social.attr('checked')) {
          if($facebook_current.length>0) $facebook_current.show();
          if($facebook_path.length>0) $facebook_path.show();
          if($facebook_upload.length>0) $facebook_upload.show();
          if($twitter_current.length>0) $twitter_current.show();
          if($twitter_path.length>0) $twitter_path.show();
          if($twitter_upload.length>0) $twitter_upload.show();
        }
        else {
          if($facebook_current.length>0) $facebook_current.hide();
          if($facebook_path.length>0) $facebook_path.hide();
          if($facebook_upload.length>0) $facebook_upload.hide();
          if($twitter_current.length>0) $twitter_current.hide();
          if($twitter_path.length>0) $twitter_path.hide();
          if($twitter_upload.length>0) $twitter_upload.hide();
        }
      });
    }
	});
})(jQuery);
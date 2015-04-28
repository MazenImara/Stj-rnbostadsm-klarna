var adminpanel_menus = {};
var adminpanel_translations = {};
var adminpanel_var_oldtime = 0;
var adminpanel_var_timer = "";
var adminpanel_var_running = 0;
var adminpanel_var_ajaxobj = null;
if (!Array.prototype.indexOf) {
    Array.prototype.indexOf = function (searchElement /*, fromIndex */ ) {
        "use strict";

        if (this === void 0 || this === null) throw new TypeError();

        var t = Object(this);
        var len = t.length >>> 0;
        if (len === 0) return -1;

        var n = 0;
        if (arguments.length > 0) {
            n = Number(arguments[1]);
            if (n !== n) // shortcut for verifying if it's NaN
            n = 0;
            else if (n !== 0 && n !== (1 / 0) && n !== -(1 / 0)) n = (n > 0 || -1) * Math.floor(Math.abs(n));
        }

        if (n >= len) return -1;

        var k = n >= 0 ? n : Math.max(len - Math.abs(n), 0);

        for (; k < len; k++) {
            if (k in t && t[k] === searchElement) return k;
        }
        return -1;
    };
}
(function ($) {
  $.fn.update_sublink_count_recursive = function() {
    if($(this).length > 0) {
      var plid = $(this).attr('data-plid');
      if(plid == 0) {
        $(this).init_sublink_count_recursive(new Array());
      }
      else {
        $(this).siblings('[data-mlid='+plid+']').update_sublink_count_recursive();
      }
    }
  }
  $.fn.init_sublink_count_recursive = function(completed_plids) {
    var count_total = 0;
    var count_published = 0;
    var count_translated = 0;
    var this_menu = $(this).attr('data-menu');
    var this_mlid = $(this).attr('data-mlid');
    var this_lang = $(this).attr('data-lang');
    if (adminpanel_menus[this_menu][this_lang].hasOwnProperty(this_mlid)) { //Kollar att posten är en förälder
      if(completed_plids.indexOf(this_mlid) == -1) { //Kollar att posten inte har gåtts igenom redan
        completed_plids.push(this_mlid);
        for(var i in adminpanel_menus[this_menu][this_lang][this_mlid]) {
          count_total++;
          if($(adminpanel_menus[this_menu][this_lang][this_mlid][i]).find('.menu-enabled .form-item input').attr('checked')) {
            count_published++;
          }
          var tsource = 0;
          if($(adminpanel_menus[this_menu][this_lang][this_mlid][i]).attr('data-tsid') !== undefined) {
            tsource = $(adminpanel_menus[this_menu][this_lang][this_mlid][i]).attr('data-tsid');
          }
          if(tsource > 0) {
            var count = 0;
            for(var prop in adminpanel_translations[tsource]) {
            	if(adminpanel_translations[tsource].hasOwnProperty(prop)) {
            		++count;
              }
            }
            if(count == Drupal.settings.adminpanel.language_count) {
              count_translated++;
            }
          }
          var data = $(adminpanel_menus[this_menu][this_lang][this_mlid][i]).init_sublink_count_recursive(completed_plids);
          count_total += data.count_total;
          count_published += data.count_published;
          count_translated += data.count_translated;
        }
      }
    }
    if(count_total) {
      if(!$(this).hasClass('link-parent')) {
        $(this).addClass('link-parent');
      }
      $(this).find('.menu-sublinks').addClass('cursor-pointer');
      $(this).find('.menu-sublinks-total').html(count_total);
      $(this).find('.menu-sublinks-published').html(count_published);
      $(this).find('.menu-sublinks-translated').html(count_translated);
    }
    else {
      if($(this).hasClass('link-parent')) {
        $(this).removeClass('link-parent');
      }
      $(this).find('.menu-sublinks').removeClass('cursor-pointer');
      $(this).find('.menu-sublinks-total').html('');
      $(this).find('.menu-sublinks-published').html('');
      $(this).find('.menu-sublinks-translated').html('');
    }
    var returned = {
      count_total: count_total,
      count_published: count_published,
      count_translated: count_translated
    };
    return returned;
  }
  $.fn.update_weight = function() {
    var plid = $(this).attr('data-plid');
    var previous_plid = $(this).attr('data-previous-plid');
    var menu_weight = $(this).find('.menu-weight');
    if($(menu_weight).length > 0) {
      var all_elements = new Array();
      var option_values = new Array();
      if ($(menu_weight).is('select')) {
        // Get a list of acceptable values.
        $('option', menu_weight).each(function () {
          option_values.push(this.value);
        });
      }
      var option_copy = $.extend(true, new Array(), option_values);
      all_elements = $(this).siblings('[data-plid='+previous_plid+']').find('.menu-weight');
      $(this).update_weight_include_siblings(all_elements, option_copy);
      option_copy = $.extend(true, new Array(), option_values);
      all_elements = $(this).siblings('[data-plid='+plid+']').andSelf().find('.menu-weight');
      $(this).update_weight_include_siblings(all_elements, option_copy);
    }
  }
  $.fn.update_weight_include_siblings = function(all_elements, option_values) {
    if (option_values) { //Select list
      var maxVal = option_values[option_values.length - 1];
      // If there are more items than possible values, assign the maximum value to the row.
      $(all_elements).each(function() {
        if (option_values.length > 0) {
          this.value = option_values.shift();
        }
        else {
          this.value = maxVal;
        }
      });
    }
    else {
      // Assume a numeric input field.
      var weight = parseInt($(all_elements[0]).val(), 10) || 0;
      $(all_elements).each(function() {
        this.value = weight;
        weight++;
      });
    }
  }
  $.fn.change_parent = function() {
    var this_menu = $(this).attr('data-menu');
    var mlid = $(this).attr('data-mlid');
    var lang = $(this).attr('data-lang');
    var previous_plid = $(this).attr('data-previous-plid');
    var plid = $(this).attr('data-plid');
    //1. Ta bort mlid från förra plid-trädet
    for(var i in adminpanel_menus[this_menu][lang][previous_plid]) {
      if($(adminpanel_menus[this_menu][lang][previous_plid][i]).attr('data-mlid') === mlid) {
        adminpanel_menus[this_menu][lang][previous_plid].splice(i, 1);
        break;
      }
    }
    if($(adminpanel_menus[this_menu][lang][previous_plid]).length < 1) {
      delete adminpanel_menus[this_menu][lang][previous_plid];
    }
    //2. Ta bort hela det nya plid-trädet
    delete adminpanel_menus[this_menu][lang][plid];
    //3. Lägg till alla som matchar det nya plid-trädet, de lägger sig i rätt ordning tack vare att vi använder array i stället för object som datastruktur för sista nivån.
    if(!adminpanel_menus[this_menu][lang]) adminpanel_menus[this_menu][lang] = {};
    if(!adminpanel_menus[this_menu][lang][plid]) adminpanel_menus[this_menu][lang][plid] = new Array();
    $(this).siblings('[data-plid='+plid+']').andSelf().each(function() {
      $(this).add_item();
    });
    //4. Sätt parentelementet till open
    $(this).siblings('[data-mlid='+plid+']').addClass('open');
    //5. Ändra plid i inputelementet
    $(this).find('input.menu-plid').attr('value', plid);
    //6. Uppdatera underlänkarfältet för både förra parent och nya
    $(this).siblings('[data-mlid='+previous_plid+']').update_sublink_count_recursive();
    $(this).siblings('[data-mlid='+plid+']').update_sublink_count_recursive();
  }
  $.fn.add_item = function() {
    var this_menu = $(this).attr('data-menu');
    var lang = $(this).attr('data-lang');
    var plid = $(this).attr('data-plid');
    adminpanel_menus[this_menu][lang][plid].push(this);
  }
  $.fn.click_to_toggle_children = function(this_menu) {
    var this_parent = $(this).parents('tr')[0];
    var this_lang = $(this_parent).attr('data-lang');
    var tsource = 0;
    if($(this_parent).attr('data-tsid') !== undefined) {
      tsource = $(this_parent).attr('data-tsid');
    }
    if($(this_parent).hasClass('open')) {
      $(this_parent).close_children_recursive($(this_parent).attr('data-mlid'));
      if(Drupal.settings.adminpanel.sync_languages) {
        if(tsource > 0) { //Stöd för översättningar finns
          //Uppdatera alla översättningar
          for(var lang in adminpanel_translations[tsource]) {
            if(lang !== this_lang) {
              $(adminpanel_translations[tsource][lang]).close_children_recursive($(adminpanel_translations[tsource][lang]).attr('data-mlid'));
            }
          }
        }
      }
    }
    else {
      if(adminpanel_menus[this_menu][this_lang][$(this_parent).attr('data-mlid')]) {
        $(this_parent).open_children();
      }
      if(Drupal.settings.adminpanel.sync_languages) {
        if(tsource > 0) { //Stöd för översättningar finns
          //Uppdatera alla översättningar
          for(var lang in adminpanel_translations[tsource]) {
            if(lang !== this_lang) {
              if(adminpanel_menus[this_menu][lang][$(adminpanel_translations[tsource][lang]).attr('data-mlid')]) {
                $(adminpanel_translations[tsource][lang]).open_children();
              }
            }
          }
        }
      }
    }
  }
  $.fn.close_children_recursive = function(plid) {
    $(this).removeClass('open');
    var arrow = $(this).find('.col-title .open-children');
    $(arrow).removeClass('open');
    $(this).siblings().each(function() {
      var lang = $(this).attr('data-lang');
      if($(this).attr('data-plid') == plid) {
        $(this).close_children_recursive($(this).attr('data-mlid'));
        $(this).detach();
      }
    });
  }
  $.fn.open_children = function() {
    $(this).addClass('open');
    var arrow = $(this).find('.col-title .open-children');
    $(arrow).addClass('open');
    var this_menu = $(this).attr('data-menu');
    var lang = $(this).attr('data-lang');
    var plid = $(this).attr('data-mlid');
    var prev_item = this;
    if(adminpanel_menus[this_menu][lang][plid]) {
      for(var i in adminpanel_menus[this_menu][lang][plid]) {
        $(prev_item).after(adminpanel_menus[this_menu][lang][plid][i]);
        prev_item = adminpanel_menus[this_menu][lang][plid][i];
      }
    }
  }
  $.fn.open_visible_children_recursive = function() {
    //Jag behöver inte lägga till klassen 'open' eftersom elementet redan har det.
    var plid = $(this).attr('data-mlid');
    var prev_item = this;
    $(this).siblings('[data-plid='+plid+']').each(function() {
      $(prev_item).after(this);
      $(this).open_visible_children_recursive();
    });
  }
  $.fn.open_all_children_recursive = function() {
    var this_menu = $(this).attr('data-menu');
    var lang = $(this).attr('data-lang');
    var plid = $(this).attr('data-mlid');
    var prev_item = this;
    if(adminpanel_menus[this_menu][lang][plid]) {
      if(!$(this).hasClass('open')) {
        $(this).addClass('open');
        for(var i in adminpanel_menus[this_menu][lang][plid]) {
          $(adminpanel_menus[this_menu][lang][plid][i]).addClass('hidden');
          $(prev_item).after(adminpanel_menus[this_menu][lang][plid][i]);
          prev_item = $(adminpanel_menus[this_menu][lang][plid][i]).open_all_children_recursive();
        }
      }
    }
    return prev_item;
  }
  $.fn.update_indentations_recursive = function(indentation_count) {
    var this_menu = $(this).attr('data-menu');
    var lang = $(this).attr('data-lang');
    var plid = $(this).attr('data-mlid');
    if(adminpanel_menus[this_menu][lang][plid]) {
      for(var i in adminpanel_menus[this_menu][lang][plid]) {
        $(adminpanel_menus[this_menu][lang][plid][i]).update_indentations_recursive(indentation_count + 1);
      }
    }
    var indentations = $(this).find('td:first-child .indentation');
    $(indentations).remove();
    for(var i = 0; i < indentation_count + 1; i++) {
      $(this).find('td:first-child').prepend($('<div class="indentation">'));
    }
  }
  $.fn.sync_translations_position = function() {
    var this_menu = $(this).attr('data-menu');
    var this_lang = $(this).attr('data-lang');
    var tsource = 0;
    if($(this).attr('data-tsid') !== undefined) {
      tsource = $(this).attr('data-tsid');
    }
    if(tsource > 0) { //Elementet har stöd för översättningar
      var parent_tsource = 0;
      if($(this).attr('data-plid') > 0) { //Har parent
        var this_parent = $(this).siblings('[data-mlid='+$(this).attr('data-plid')+']');
        if($(this_parent).attr('data-tsid') !== undefined) {
          parent_tsource = $(this_parent).attr('data-tsid');
        }
      }
      //Loopa översättningar som inte är på sourcespråket
      for(var lang in adminpanel_translations[tsource]) {
        if(lang !== this_lang) {
          var translation = adminpanel_translations[tsource][lang];
          //-----Uppdatera position--//
          //1. Hitta originalets närmast ovanför liggande element som har en översättning i det här språket
          var prev_item = this;
          var running = true;
          while(running) {
            var cur_item = $(prev_item).prev();
            if($(cur_item).length > 0) {
              var prev_tsource = 0;
              if($(cur_item).attr('data-tsid') !== undefined) {
                prev_tsource = $(cur_item).attr('data-tsid');
              }
              if(prev_tsource > 0) { //Elementet har översättningar
                if(adminpanel_translations[prev_tsource][lang]) { //En översättning finns på det här språket
                  //2. Hitta översättningen till detta element
                  var prev_translation = adminpanel_translations[prev_tsource][lang];
                  
                  //3. Flytta det här elementet och dess barn så att de ligger efter det elementet.
                  $(prev_translation).after(translation);
                  $(translation).open_visible_children_recursive();
                  //Avbryt slutligen loopen
                  running = false;
                }
                else { //Elementet har översättningar men inte på det här språket, fortsätt med nästa element
                  prev_item = cur_item;
                }
              }
              else { //Elementet har inga översättningar, fortsätt med nästa element
                prev_item = cur_item;
              }
            }
            else { //Slut på element
              //Lägg elementet allra överst
              $(translation).parent().prepend(translation);
              //Flytta barnen
              $(translation).open_visible_children_recursive();
              //Avbryt loopen
              running = false;
            }
          }
          //------------------------/
          //Uppdatera indentations
          var indentation_count = $(this).find('td:first-child .indentation').size();
          var plid = $(translation).attr('data-mlid');
          if(adminpanel_menus[this_menu][lang][plid]) {
            for(var i in adminpanel_menus[this_menu][lang][plid]) {
              $(adminpanel_menus[this_menu][lang][plid][i]).update_indentations_recursive(indentation_count);
            }
          }
          var indentations = $(translation).find('td:first-child .indentation');
          $(indentations).remove();
          for(var i = 0; i < indentation_count; i++) {
            $(translation).find('td:first-child').prepend($('<div class="indentation">'));
          }
          //Uppdatera plid
          if(parent_tsource > 0) { //Parent har stöd för översättningar
            //Hitta parent för det aktuella språket
            var translated_parent = adminpanel_translations[parent_tsource][lang];
            if($(translated_parent).length > 0) { //Hittat parent
              //Uppdatera plid
              $(translation).attr('data-previous-plid', $(translation).attr('data-plid'));
              $(translation).attr('data-plid', $(translated_parent).attr('data-mlid'));
              //För att det ska bli lättöverskådligt bör parentelementet öppnas upp om det är stängt. Dessutom måste det faktiskt öppnas för att gömda element inte ska tas bort!
              if(!$(translated_parent).hasClass('open')) {
                $(translated_parent).open_children();
              }
              //Uppdatera adminpanel_menus
              $(translation).change_parent();
            }
          }
          else { //Antingen har inte elementet någon parent eller så har parent inte stöd för översättningar.
            //Kolla om elementet hade en parent förut
            if($(this).attr('data-previous-plid') > 0) {
              //Elementet har haft en parent. Uppdatera översättningen så att plid sätts till 0.
              $(translation).attr('data-previous-plid', $(translation).attr('data-plid'));
              $(translation).attr('data-plid', 0);
              //Uppdatera adminpanel_menus
              $(translation).change_parent();
            }
          }
          //Uppdatera vikt
          $(translation).update_weight();
        }
      }
    }
  }
  $.fn.adminpanel_search_check_time = function(abort) {
  	if(abort) {
  		window.clearInterval(adminpanel_var_timer);
  		adminpanel_var_timer = "";
  		adminpanel_var_running = 0;
  		if(adminpanel_var_ajaxobj) adminpanel_var_ajaxobj.abort();
  		$(this).removeClass('throbbing');
  		return;
  	}
  	var newtime = new Date().getTime();
  	if(newtime - adminpanel_var_oldtime >= 600) {
  		window.clearInterval(adminpanel_var_timer);
  		adminpanel_var_timer = "";
  		$(this).adminpanel_search_launch_search();
  	}
  }
  $.fn.adminpanel_search_launch_search = function() {
    //alert($(this).attr('class'));
  	search_string = $(this).val();
  	if(search_string) {
  		$(this).addClass('throbbing');
  		var this_obj = this;
  		adminpanel_var_running = 1;
  		adminpanel_var_ajaxobj = $.ajax({
    		cache: false,
    		url: Drupal.settings.basePath + 'ajax/dashboard-search',
    		data: {search: search_string, destination: Drupal.settings.adminpanel.destination},
    		dataType: 'json',
    		error: function(request, status, error) {
          if(status !== 'abort') {
    			 alert(status);
          }
    		},
    		success: function(data, status, request) {
          $(this_obj).removeClass('throbbing');
    			var html = data;
    			$(this_obj).parent().siblings('#adminpanel-search-result').html(html);
    		}
    	});
  	}
  	else {
      $(this).parent().siblings('#adminpanel-search-result').html('');
    }
  }
  $(document).ready(function() {
    var pass_parent = $('#adminpanel-edit-your-account-form .form-type-password-confirm .password-parent');
    var textfield = $(pass_parent).children('input');
    var pass = $(pass_parent).children('.password-strength');
    pass.detach();
    $(textfield).after(pass);
    var form_wrapper = $('.menu-overview-form > div');

    var page_lang = Drupal.settings.adminpanel.lang;
    //Menyvyn
    $('.menu-overview-form .menu-overview-table').each(function() {
      var this_id = $(this).attr('id').split('-');
      var this_lang = this_id.pop();
      var this_menu = this_id.pop();

      wrapper = $('<div class="form-wrapper menu-overview-table-wrapper '+this_lang+'"></div>');
      $(this).prevUntil('.menu-overview-table').appendTo(wrapper);
      $(wrapper).append($(this));
      if(page_lang != this_lang) {
        $(wrapper).hide();
      }
      $(form_wrapper).each(function() {
        var wrapper_parent = $(this).parent();
        if(this_menu === $(wrapper_parent).attr('data-menu')) {
          $(this).append($(wrapper));
        }
      });
      //Arrow click event
      $(this).find('tbody tr td.col-title .open-children').click(function() {
        $(this).click_to_toggle_children(this_menu);
      });
      //Sublink summary click event
      $(this).find('tbody tr td.menu-sublinks').click(function() {
        $(this).click_to_toggle_children(this_menu);
      });
      //Toggling av publiceringsstatus
      $(this).find('tbody tr .menu-enabled .form-item input').click(function() {
        var this_parent = $(this).parents('tr');
        var this_lang = $(this_parent).attr('data-lang');
        var this_plid = $(this_parent).attr('data-plid');
        //Uppdatera språkfältet för den här tabellen
        var this_langlink = $(this_parent).find('td.col-languages a[data-lang='+this_lang+']');
        if($(this).attr('checked')) {
          $(this_langlink).removeClass('unpublished').addClass('published');
        }
        else {
          $(this_langlink).removeClass('published').addClass('unpublished');
        }
        //Uppdatera översättningar
        var tsource = 0;
        if($(this_parent).attr('data-tsid') !== undefined) {
          tsource = $(this_parent).attr('data-tsid');
        }
        //Uppdatera underlänkarfältet för parents
        $(this_parent).siblings('[data-mlid='+this_plid+']').update_sublink_count_recursive();
        if(tsource > 0) { //Stöd för översättningar finns
          for(var lang in adminpanel_translations[tsource]) {
            if(lang !== this_lang) {
              //Uppdatera språkfältet
              var translation_langlink = $(adminpanel_translations[tsource][lang]).find('td.col-languages a[data-lang='+this_lang+']');
              if($(this).attr('checked')) {
                $(translation_langlink).removeClass('unpublished').addClass('published');
              }
              else {
                $(translation_langlink).removeClass('published').addClass('unpublished');
              }
              if(Drupal.settings.adminpanel.sync_status) {
                //Synka språkfältet för originalet
                this_langlink = $(this_parent).find('td.col-languages a[data-lang='+lang+']');
                var translation_plid = $(adminpanel_translations[tsource][lang]).attr('data-plid');
                if($(this).attr('checked')) {
                  $(this_langlink).removeClass('unpublished').addClass('published');
                }
                else {
                  $(this_langlink).removeClass('published').addClass('unpublished');
                }
                //Synka språkfältet för översättning
                translation_langlink = $(adminpanel_translations[tsource][lang]).find('td.col-languages a[data-lang='+lang+']');
                if($(this).attr('checked')) {
                  $(translation_langlink).removeClass('unpublished').addClass('published');
                }
                else {
                  $(translation_langlink).removeClass('published').addClass('unpublished');
                }
                //Synka publiceringsstatus
                $(adminpanel_translations[tsource][lang]).find('.menu-enabled .form-item input').attr('checked', $(this).attr('checked'));
                //Uppdatera underlänkarfältet för parents
                $(adminpanel_translations[tsource][lang]).siblings('[data-mlid='+translation_plid+']').update_sublink_count_recursive();
              }
            }
          }
        }
        var this_table = $(this).parents('table');
        if($(this_table).siblings('.tabledrag-changed-warning').length == 0) {
          $(Drupal.theme('tableDragChangedWarning')).insertBefore(this_table).hide().fadeIn('slow');
        }
      });
      //Skapa underlänk changed event
      $(this).find('tbody tr .col-create-sublink select').add($(this).siblings('.menu-add-link')).change(function() {
        var this_value = $(this).val();
        if(this_value !== '0') {
          this_value = this_value.split('_');
          if(this_value[1] === 'menulink') { //Skapa menylänk
            var menu_name = this_value[2];
            var plid = this_value[3];
            var language = this_value[4];
            var address = Drupal.settings.basePath+'admin/structure/menu/manage/'+menu_name+'/add?plid='+plid+'&lang='+language+'&destination='+Drupal.settings.adminpanel.destination;
            window.location.href = address;
          }
          else if(this_value[1] === 'node') { //Skapa nod
            var node_type = this_value[2];
            var menu_name = this_value[3];
            var plid = this_value[4];
            var language = this_value[5];
            var destination_string = '';
            if(node_type !== 'panel') {
              destination_string = '&destination='+Drupal.settings.adminpanel.destination;
            }
            var address = Drupal.settings.basePath+'node/add/'+node_type+'?menu-name='+menu_name+'&plid='+plid+'&lang='+language+destination_string;
            window.location.href = address;
          }
        }
      });
      //Init
      if(!adminpanel_menus[this_menu]) adminpanel_menus[this_menu] = {};
      if(!adminpanel_menus[this_menu][this_lang]) adminpanel_menus[this_menu][this_lang] = {};
      $(this).find('tbody tr').each(function() {
        if($(this).attr('data-plid') >= 0) {
          //Initierar adminpanel_menus
          if(!adminpanel_menus[this_menu][this_lang][$(this).attr('data-plid')]) adminpanel_menus[this_menu][this_lang][$(this).attr('data-plid')] = new Array();
          adminpanel_menus[this_menu][this_lang][$(this).attr('data-plid')].push(this);
          //Initierar adminpanel_translations
          var tsource = 0;
          if($(this).attr('data-tsid') !== undefined) {
            tsource = $(this).attr('data-tsid');
          }
          if(tsource > 0) {
            if(!adminpanel_translations[tsource]) adminpanel_translations[tsource] = {};
            if(!adminpanel_translations[tsource][this_lang]) adminpanel_translations[tsource][this_lang] = {};
            adminpanel_translations[tsource][this_lang] = this;
          }
          //Gömmer barn
          if($(this).attr('data-plid') > 0) {
            $(this).detach();
          }
        }
      });
    });
    //Sökvyn
    $('.search-view .form-item .form-text').keypress(function(e) {
  		if(!(adminpanel_var_timer=="" && adminpanel_var_running==0)) {
  			$(this).adminpanel_search_check_time(true);
  		}
  		adminpanel_var_oldtime = new Date().getTime();
  		adminpanel_var_textfield = this;
  		if(e.which==13) { //Man har tryckt enter
  			$(this).adminpanel_search_launch_search();
  			return false;
  		}
  		else {
        var this_obj = this;
  			adminpanel_var_timer = window.setInterval(function() {
          $(this_obj).adminpanel_search_check_time();
        }, 200);
  		}
    });
    //Innehållsvyn
    $('.content-type-view').each(function() {
      //Filtrering av innehållstyper
      var bullet_tabs = $(this).find('.bullet-tabs');
      $(bullet_tabs).find('li span').click(function() {
        $(this).parent().siblings().removeClass('active');
        $(this).parent().addClass('active');
        var this_type = $(this).parent().attr('data-type');
        $(bullet_tabs).siblings('form').hide();
        $(bullet_tabs).siblings('form#adminpanel-generate-node-list-'+this_type).show();
      });
    });

    //Initiera Underlänkar-fältet
    var completed_plids = new Array();
    for(var menu in adminpanel_menus) {
      for(var lang in adminpanel_menus[menu]) {
        for(var plid in adminpanel_menus[menu][lang]) {
          if(completed_plids.indexOf(plid) == -1) {
            for(var i in adminpanel_menus[menu][lang][plid]) {
              var data = $(adminpanel_menus[menu][lang][plid][i]).init_sublink_count_recursive(completed_plids);
            }
          }
        }
      }
    }
    completed_plids = null;
    $(form_wrapper).each(function() {
      $(this).prepend($(this).find('.menu-overview-table-wrapper'));
    });
    $('.menu-overview-form .form-actions .button-yes').click(function() {
      //Lägg in alla element igen
      $(this).parents('.menu-overview-form').find('.menu-overview-table tbody tr').each(function() {
        $(this).open_all_children_recursive();
      });
    });
    
    $('.menu-tabs li a').click(function() {
      $(this).parent().siblings().removeClass('active');
      $(this).parent().addClass('active');
      var this_lang = $(this).attr('class');
      var this_form = $(this).parents('.menu-tabs').siblings('.menu-overview-form');
      $(this_form).find('.menu-overview-table-wrapper').each(function() {
        if($(this).hasClass(this_lang)) {
          $(this).show();
        }
        else {
          $(this).hide();
        }
      });
      return false;
    });
    if ($.cookie('Drupal.tableDrag.showWeight') == 1) {
      adminpanel_showColumns();
    }
    else {
      adminpanel_hideColumns();
    }
    //Initiera AJAX för innehållspanelen
    $('.content-type-view').delegate('.node-list-form .node-list-table th a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});
    $('.content-type-view').delegate('.node-list-form .pager-item a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});
    $('.content-type-view').delegate('.node-list-form .pager-first a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});
    $('.content-type-view').delegate('.node-list-form .pager-item a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});
    $('.content-type-view').delegate('.node-list-form .pager-previous a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});
    $('.content-type-view').delegate('.node-list-form .pager-next a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});
    $('.content-type-view').delegate('.node-list-form .pager-next a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});
    $('.content-type-view').delegate('.node-list-form .pager-last a', 'click', function(el, a, b, c) {$(this).adminpanel_clickajax(el, a, b, c);return (false);});

    //E-postinställningar för kontaktformulär
    $('.webform-email-view .webform-email-view-table').each(function() {
      var this_id = $(this).attr('id').split('-');
      var this_lang = this_id.pop();
      if(this_lang !== page_lang) {
        $(this).hide();
      }
    });
    $('.webform-tabs li a').click(function() {
      $(this).parent().siblings().removeClass('active');
      $(this).parent().addClass('active');
      var this_lang = $(this).attr('class');
      $(this).parents('.webform-tabs').siblings('.webform-email-view-table').each(function() {
        if($(this).hasClass(this_lang)) {
          $(this).show();
        }
        else {
          $(this).hide();
        }
      });
      return false;
    });
  });
  $.fn.adminpanel_clickajax = function(el, a, b, c) {
		var url = jQuery.url(el.currentTarget.getAttribute('href'));
		var this_form = $(this).parents('.node-list-form');
		var node_type = $(this_form).attr('id').replace('adminpanel-generate-node-list-', '').replace('-', '_');
		var page = url.param('page');
		var sort = url.param('sort');
		var order = url.param('order');
		if(!page) page = 0;
  	if(!sort) sort = '';
  	if(!order) order = '';
  	//Visa throbber
  	var this_parent = $(this).parent();
    if($(this_parent).is('th')) {
      //Om man har klickat på th, sätt throbbern direkt vid länken.
      $(this).addClass('adminpanel-th-throbbing');
    }
    else if($(this_parent).is('li')) {
      // Om man har klickat på en pagerlänk, sätt throbbern i början av pagerraden.
      var pager = $(this_parent).parents('div.pager');
      if($(pager).length > 0) {
        $(pager).addClass('adminpanel-pager-throbbing');
      }
    }
  	$.ajax({
  		cache: false,
  		url: Drupal.settings.basePath + 'ajax/dashboard-refresh',
  		data: {node_type: node_type, page: page, sort: sort, order: order, destination: Drupal.settings.adminpanel.destination},
  		dataType: 'json',
  		error: function(request, status, error) {
  			alert(status);
  		},
  		success: function(data, status, request) {
  			var html = $(data);
  			$(html).css('display', 'block');
  			$(this_form).replaceWith(html);
  		}
  	});
		//$(form).scrollTo();
  }
  /**
 * After the row is dropped, update a single table field according to specific
 * settings.
 *
 * @param changedRow
 *   DOM object for the row that was just dropped.
 * @param group
 *   The settings group on which field updates will occur.
 */
  Drupal.tableDrag.prototype.adminpanel_updateField = function (changedRow, group) {
    var rowSettings = this.rowSettings(group, changedRow);
    // Set the row as its own target.
    if (rowSettings.relationship == 'self' || rowSettings.relationship == 'group') {
      var sourceRow = changedRow;
      //alert('self or group');
    }
    // Siblings are easy, check previous and next rows.
    else if (rowSettings.relationship == 'sibling') {
      var previousRow = $(changedRow).prev('tr').get(0);
      var nextRow = $(changedRow).next('tr').get(0);
      var sourceRow = changedRow;
      if ($(previousRow).is('.draggable') && $('.' + group, previousRow).length) {
        if (this.indentEnabled) {
          if ($('.indentations', previousRow).length == $('.indentations', changedRow)) {
            sourceRow = previousRow;
          }
        }
        else {
          sourceRow = previousRow;
        }
      }
      else if ($(nextRow).is('.draggable') && $('.' + group, nextRow).length) {
        if (this.indentEnabled) {
          if ($('.indentations', nextRow).length == $('.indentations', changedRow)) {
            sourceRow = nextRow;
          }
        }
        else {
          sourceRow = nextRow;
        }
      }
      //Uppdatera vikter
      $(changedRow).update_weight();
      //Uppdatera översättningar. Det är här uppdateringen måste ske eftersom siblingkontrollen sker sist.
      if(Drupal.settings.adminpanel.sync_languages) {
        $(changedRow).sync_translations_position();
      }
    }
    // Parents, look up the tree until we find a field not in this group.
    // Go up as many parents as indentations in the changed row.
    else if (rowSettings.relationship == 'parent') {
      var previousRow = $(changedRow).prev('tr');
      while (previousRow.length && $('.indentation', previousRow).length >= this.rowObject.indents) {
        previousRow = previousRow.prev('tr');
      }
      // If we found a row.
      if (previousRow.length) {
        sourceRow = previousRow[0];
        $(changedRow).attr('data-previous-plid', $(changedRow).attr('data-plid'));
        $(changedRow).attr('data-plid', $(sourceRow).attr('data-mlid'));
        //alert('parent row: '+$(sourceRow).attr('data-mlid'));
      }
      // Otherwise we went all the way to the left of the table without finding
      // a parent, meaning this item has been placed at the root level.
      else {
        $(changedRow).attr('data-previous-plid', $(changedRow).attr('data-plid'));
        $(changedRow).attr('data-plid', '0');
        // Use the first row in the table as source, because it's guaranteed to
        // be at the root level. Find the first item, then compare this row
        // against it as a sibling.
        sourceRow = $(this.table).find('tr.draggable:first').get(0);
        if (sourceRow == this.rowObject.element) {
          sourceRow = $(this.rowObject.group[this.rowObject.group.length - 1]).next('tr.draggable').get(0);
        }
        var useSibling = true;
      }
      //För att det ska bli lättöverskådligt bör parentelementet öppnas upp om det är stängt. Dessutom måste det faktiskt öppnas för att gömda element inte ska tas bort!
      if($(changedRow).attr('data-plid') > 0) {
        var parentRow = $(changedRow).siblings('[data-mlid='+$(changedRow).attr('data-plid')+']');
        if(!$(parentRow).hasClass('open')) {
          $(parentRow).open_children();
        }
      }
      //Uppdatera adminpanel_menus
      $(changedRow).change_parent();
      //--Uppdatera indentations för elementets barn
      var indentation_count = $(changedRow).find('td:first-child .indentation').size();
      var this_menu = $(changedRow).attr('data-menu');
      var lang = $(changedRow).attr('data-lang');
      var plid = $(changedRow).attr('data-mlid');
      if(adminpanel_menus[this_menu][lang][plid]) {
        for(var i in adminpanel_menus[this_menu][lang][plid]) {
          $(adminpanel_menus[this_menu][lang][plid][i]).update_indentations_recursive(indentation_count);
        }
      }
    }
  };
  Drupal.tableDrag.prototype.updateField = Drupal.tableDrag.prototype.adminpanel_updateField;

  var adminpanel_hideColumns = function () {
    var count = 0;
    for(var prop in adminpanel_menus) {
    	if(adminpanel_menus.hasOwnProperty(prop)) {
    		++count;
    		break;
      }
    }
    if(count) {
      for(var menu in adminpanel_menus) {
        for(var lang in adminpanel_menus[menu]) {
          for(var plid in adminpanel_menus[menu][lang]) {
            for(var i in adminpanel_menus[menu][lang][plid]) {
              // Hide weight/parent cells and headers.
              $(adminpanel_menus[menu][lang][plid][i]).find('.tabledrag-hide', 'table.tabledrag-processed').css('display', 'none');
              // Show TableDrag handles.
              $(adminpanel_menus[menu][lang][plid][i]).find('.tabledrag-handle', 'table.tabledrag-processed').css('display', '');
              // Reduce the colspan of any effected multi-span columns.
              $(adminpanel_menus[menu][lang][plid][i]).find('.tabledrag-has-colspan', 'table.tabledrag-processed').each(function () {
                this.colSpan = this.colSpan - 1;
              });
              // Change link text.
              $(adminpanel_menus[menu][lang][plid][i]).find('.tabledrag-toggle-weight').text(Drupal.t('Show row weights'));
            }
          }
        }
      }
      $('.menu-overview-form th.tabledrag-hide').css('display', 'none');
      // Change cookie.
      $.cookie('Drupal.tableDrag.showWeight', 0, {
        path: Drupal.settings.basePath,
        // The cookie expires in one year.
        expires: 365
      });
      // Trigger an event to allow other scripts to react to this display change.
      $('table.tabledrag-processed').trigger('columnschange', 'hide');
    }
  };
  Drupal.tableDrag.prototype.hideColumns = adminpanel_hideColumns;
  
  /**
 * Show the columns containing weight/parent form elements
 * Undo hideColumns().
 */
  var adminpanel_showColumns = function () {
    var count = 0;
    for(var prop in adminpanel_menus) {
    	if(adminpanel_menus.hasOwnProperty(prop)) {
    		++count;
    		break;
      }
    }
    if(count) {
      for(var this_menu in adminpanel_menus) {
        for(var lang in adminpanel_menus[this_menu]) {
          for(var plid in adminpanel_menus[this_menu][lang]) {
            for(var i in adminpanel_menus[this_menu][lang][plid]) {
              // Show weight/parent cells and headers.
              $(adminpanel_menus[this_menu][lang][plid][i]).find('.tabledrag-hide', 'table.tabledrag-processed').css('display', '');
              // Hide TableDrag handles.
              $(adminpanel_menus[this_menu][lang][plid][i]).find('.tabledrag-handle', 'table.tabledrag-processed').css('display', 'none');
              // Increase the colspan for any columns where it was previously reduced.
              $(adminpanel_menus[this_menu][lang][plid][i]).find('.tabledrag-has-colspan', 'table.tabledrag-processed').each(function () {
                this.colSpan = this.colSpan + 1;
              });
              // Change link text.
              $(adminpanel_menus[this_menu][lang][plid][i]).find('.tabledrag-toggle-weight').text(Drupal.t('Hide row weights'));
            }
          }
        }
      }
      $('.menu-overview-form th.tabledrag-hide').css('display', '');
      // Change cookie.
      $.cookie('Drupal.tableDrag.showWeight', 1, {
        path: Drupal.settings.basePath,
        // The cookie expires in one year.
        expires: 365
      });
      // Trigger an event to allow other scripts to react to this display change.
      $('table.tabledrag-processed').trigger('columnschange', 'show');
    }
  };
  Drupal.tableDrag.prototype.showColumns = adminpanel_showColumns;
})(jQuery);
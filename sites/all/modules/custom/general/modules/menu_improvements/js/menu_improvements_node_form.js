var menu_improvements_menu_options = {};
(function($) {
	$.fn.menu_improvements_update_summary = function(context, new_value) {
		$('fieldset.menu-link-form', context).drupalSetSummary(function (context) {
			if(new_value) {
	      return Drupal.checkPlain(new_value);
			}
			else {
				return Drupal.t('Not in menu');
			}
    });
	}
  $.fn.menu_improvements_filter_parent_links = function(current_language) {
    for(var lang in Drupal.settings.menu_improvements_flatlist) {
      for(var mlid in Drupal.settings.menu_improvements_flatlist[lang]) {
        var item = Drupal.settings.menu_improvements_flatlist[lang][mlid];
        if(lang === current_language) {
          $(item).show();
        }
        else {
          $(item).hide();
        }
      }
    }
  }
  $.fn.menu_improvements_find_closest_translated_parent_recursive = function(current_language) {
    var this_plid = $(this).attr('data-plid');
    if (typeof this_plid !== 'undefined' && this_plid !== false) { //Det är en riktig menylänk
      var this_lang = $(this).attr('data-lang');
      //Hitta närmaste parent
      var this_parent = Drupal.settings.menu_improvements_flatlist[this_lang][this_plid];
      if($(this_parent).length > 0) {
        //Hitta en översättning av parent
        var parent_t_source = $(this_parent).attr('data-t_source');
        if (typeof parent_t_source !== 'undefined' && parent_t_source !== false) { //Elementet har översättningar, kolla om det finns en på det aktuella språket.
          var parent_t_type = $(this_parent).attr('data-t_type');
          var translated_parent = Drupal.settings.menu_improvements_translations[parent_t_type][parent_t_source][current_language];
          if($(translated_parent).length > 0) { //Hittat närmaste översatta parent! returnera denna.
            return translated_parent;
          }
          else { //Det finns ingen översättning på det aktuella språket, fortsätt att leta.
            return $(this_parent).menu_improvements_find_closest_translated_parent_recursive(current_language);
          }
        }
        else { //Elementet har inga översättningar, fortsätt att leta.
          return $(this_parent).menu_improvements_find_closest_translated_parent_recursive(current_language);
        }
      }
      else {
        //Det finns ingen parent, välj menynamnet.
        var this_menu_name = $(this).attr('data-menu_name');
        return menu_improvements_menu_options[this_menu_name];
      }
    }
    else { //Man har valt menynamnet, då behöver ingen översättning hittas.
      return this;
    }
    //Om jag har glömt någon möjlighet så returneras det här elementet.
    return this;
  }

	Drupal.behaviors.menu_improvements = {
    attach: function (context, settings) {
	    if(Drupal.settings.menu_improvements_hierarchy) {
				var number_of_links = 0;
				var menu_link_summary = '';
				//Loopa genom alla menylänkar i formuläret
				$('#menu-improvements-current-links tr .menu-improvements-current-links-parent, .form-item-menu-improvements-add-link-parent').each(function() {
					if($(this).hasClass('menu-improvements-current-links-parent')) {
						var this_type = 'current_link';
					}
					else {
						var this_type = 'add_link';
					}
		      //Init
		      var parent_selector = $(this).find('select');
		      var lang_selector = $('#edit-language');
		      var current_language = $(lang_selector).val();
		      var current_parent = $(parent_selector).val();
		      //Init hierarchy
		      for(var lang in Drupal.settings.menu_improvements_hierarchy) {
		        for(var plid in Drupal.settings.menu_improvements_hierarchy[lang]) {
		          for(var i in Drupal.settings.menu_improvements_hierarchy[lang][plid]) {
		            var mlid = Drupal.settings.menu_improvements_hierarchy[lang][plid][i];
		            $(parent_selector).find('option').each(function() {
		              var this_value = $(this).val().split(':');
		              var this_menu_name = this_value[0];
		              var this_mlid = this_value[1];
		              if(this_mlid === mlid) {
		                $(this).attr('data-lang', lang);
		                $(this).attr('data-plid', plid);
		                $(this).attr('data-menu_name', this_menu_name);
		                Drupal.settings.menu_improvements_flatlist[lang][mlid] = this;
		              }
		              else if(this_mlid === '0') { //Menyelement
		                $(this).attr('data-menu_name', this_menu_name);
		                menu_improvements_menu_options[this_menu_name] = this;
		              }
		            });
		          }
		        }
		      }
		      //Init translations
		      for(var t_type in Drupal.settings.menu_improvements_translations) {
		        for(var t_source in Drupal.settings.menu_improvements_translations[t_type]) {
		          for(var lang in Drupal.settings.menu_improvements_translations[t_type][t_source]) {
		            var mlid = Drupal.settings.menu_improvements_translations[t_type][t_source][lang];
		            $(parent_selector).find('option').each(function() {
		              var this_value = $(this).val();
		              var this_mlid = this_value.split(':').pop();
		              if(this_mlid === mlid) {
		                $(this).attr('data-t_type', t_type);
		                $(this).attr('data-t_source', t_source);
		                if($(this).val() === current_parent) {
		                  current_parent = this;
		                }
		                Drupal.settings.menu_improvements_translations[t_type][t_source][lang] = this;
		              }
		            });
		          }
		        }
		      }
		
		      //Initial filtering
		      $(parent_selector).menu_improvements_filter_parent_links(current_language);
		      //Language change
		      $(lang_selector).change(function() {
		        current_language = $(this).val();
		        $(parent_selector).menu_improvements_filter_parent_links(current_language);
		        var this_t_source = $(current_parent).attr('data-t_source');
		        if (typeof this_t_source !== 'undefined' && this_t_source !== false) { //Elementet har översättningar
		          var this_t_type = $(current_parent).attr('data-t_type');
		          if(Drupal.settings.menu_improvements_translations[this_t_type][this_t_source][current_language]) { //Hittat en översättning på det nya språket
		            $(Drupal.settings.menu_improvements_translations[this_t_type][this_t_source][current_language]).attr('selected', true);
		            current_parent = Drupal.settings.menu_improvements_translations[this_t_type][this_t_source][current_language];
		          }
		          else { //Hittade ingen översättning på det nya språket, hitta närmaste översatta parent och sätt den som selected
		            current_parent = $(current_parent).menu_improvements_find_closest_translated_parent_recursive(current_language);
		          }
		        }
		        else { //Elementet har inga översättningar, hitta närmaste översatta parent och sätt den som selected
		          current_parent = $(current_parent).menu_improvements_find_closest_translated_parent_recursive(current_language);
		        }
		        $(current_parent).attr('selected', true);
		      });
		      //Parent change
		      $(parent_selector).change(function() {
		        $(parent_selector).find('option').each(function() {
		          if($(this).attr('selected') == true) {
		            current_parent = this;
		          }
		        });
		      });
		      /*--------Om nodtiteln och menylänken är lika vid sidladdning och man ändrar nodtiteln, ska menylänken också ändras automatiskt. Om man däremot skriver i menylänken ska automatisk uppdatering avbrytas direkt. */
			    var node_title_input = $('.form-item-title input');

			    if(this_type === 'add_link') {
						var menu_title_input = $(this).prevAll('.form-item-menu-improvements-add-link-link-title').find('#edit-menu-improvements-add-link-link-title');
					}
					else {
						var menu_title_input = $(this).prevAll('.menu-improvements-current-links-title').find('.form-text');
						number_of_links++;
						if(!menu_link_summary) {
							menu_link_summary = $(menu_title_input).val();
						}
					}
			    if($(node_title_input).length > 0 && $(menu_title_input).length > 0) {
			      if($(node_title_input).val() === $(menu_title_input).val()) {
			        var synchronize_titles = true;
			        $(node_title_input).change(function() {
			          if(synchronize_titles) {
			            $(menu_title_input).val($(node_title_input).val());
			            //Uppdaterar sammanfattningen i vertical scroll
									if(this_type === 'add_link') {
										if(number_of_links == 0) {
											$(this).menu_improvements_update_summary(context, $(menu_title_input).val());
										}
									}
									else {
										if(number_of_links > 1) {
											$(this).menu_improvements_update_summary(context, Drupal.t('Multiple menu links'));
										}
										else {
											$(this).menu_improvements_update_summary(context, $(menu_title_input).val());
										}
									}
			          }
			        });
			      }
			      $(menu_title_input).change(function() {
							synchronize_titles = false;
							//Uppdaterar sammanfattningen i vertical scroll
							if(this_type === 'add_link') {
								if(number_of_links == 0) {
									$(this).menu_improvements_update_summary(context, $(menu_title_input).val());
								}
							}
							else {
								if(number_of_links > 1) {
									$(this).menu_improvements_update_summary(context, Drupal.t('Multiple menu links'));
								}
								else {
									$(this).menu_improvements_update_summary(context, $(menu_title_input).val());
								}
							}
						});
			    }
				});
				//Uppdaterar sammanfattningen i vertical scroll
				if(number_of_links > 1) {
					menu_link_summary = Drupal.t('Multiple menu links');
				}
				$(this).menu_improvements_update_summary(context, menu_link_summary);
	    }
		}
	}
})(jQuery);
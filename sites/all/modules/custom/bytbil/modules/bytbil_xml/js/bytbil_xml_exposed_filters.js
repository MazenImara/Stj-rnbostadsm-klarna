Object.size = function(obj) {
  var size = 0, key;
  for (key in obj) {
    if (obj.hasOwnProperty(key)) size++;
  }
  return size;
};
var bytbil_marke_modell = new Array();
(function($) {
	Drupal.behaviors.bytbil_xml = {
    attach: function (context, settings) {
		  /*-----------------------Hanterar Märke/modell-----------------------*/
		  if($('#edit-field-bytbil-bilmodell-tid').css('display') !== 'none') {
		    $('#edit-field-bytbil-bilmodell-tid').hide();
		    //Populera arrayen
		    $('#edit-field-bytbil-bilmodell-tid option').each(function() {
		      if($(this).val()!='All') {
		        option_array = new Array();
		        option_array['value'] = $(this).val();
		        option_array['text'] = $(this).html();
		        if(option_array['text'].substr(0, 1)!='-') { //Huvudkategori
		          bytbil_marke_modell.push(option_array);
		        }
		        else { //Underkategori
		          option_array['text'] = option_array['text'].substr(1);
		          i = bytbil_marke_modell.length - 1;
		          if(!('children' in bytbil_marke_modell[i])) {
		            bytbil_marke_modell[i]['children'] = new Array();
		          }
		          bytbil_marke_modell[i]['children'].push(option_array);
		        }
		      }
		    });
		    var current_value = $('#edit-field-bytbil-bilmodell-tid').val();
		    var current_marke_index = -1;
		    var current_modell_index = -1;
		    if(current_value!='All') {
		      for(var i=0, imax=bytbil_marke_modell.length; i<imax; i++) {
		        var found = false;
		        if(current_value==bytbil_marke_modell[i]['value']) { //Valet är den här huvudkategorin
		          current_marke_index = i;
		          break;
		        }
		        else { //Kolla underkategorierna också
		          for(var j=0, jmax=bytbil_marke_modell[i]['children'].length; j<jmax; j++) {
		            if(current_value==bytbil_marke_modell[i]['children'][j]['value']) { //Valet är den här underkategorin
		              current_marke_index = i;
		              current_modell_index = j;
		              found = true;
		              break;
		            }
		          }
		        }
		        if(found) break;
		      }
		    }
		    if($('#edit-marke-customselect-marke').length == 0) {
			    //Skapa första listboxen
			    var customselect_marke = $('<select id="edit-marke-customselect-marke"></select>');
			    if(current_marke_index==-1) {
			      $(customselect_marke).append('<option selected="selected" value="All">M&auml;rke (Alla)</option>');
			    }
			    else {
			      $(customselect_marke).append('<option value="All">M&auml;rke (Alla)</option>');
			    }
			    for(var i=0, imax=bytbil_marke_modell.length; i<imax; i++) {
			      var selected_string = '';
			      if(current_marke_index==i) selected_string = 'selected="selected" ';
			      $(customselect_marke).append('<option '+selected_string+'value="'+bytbil_marke_modell[i]['value']+'">'+bytbil_marke_modell[i]['text']+'</option>');
			    }
			
			    //Skapa den andra listboxen
			    var customselect_modell = $('<select id="edit-marke-customselect-modell"></select>');
			    if(current_modell_index==-1) {
			      $(customselect_modell).append('<option selected="selected" value="All">Modell (Alla)</option>');
			    }
			    else {
			      $(customselect_modell).append('<option value="All">Modell (Alla)</option>');
			    }
			    if(current_marke_index!=-1) {
			      for(var j=0, jmax=bytbil_marke_modell[current_marke_index]['children'].length; j<jmax; j++) {
			          var selected_string = '';
			          if(current_modell_index==j) selected_string = 'selected="selected" ';
			          $(customselect_modell).append('<option '+selected_string+'value="'+bytbil_marke_modell[current_marke_index]['children'][j]['value']+'">'+bytbil_marke_modell[current_marke_index]['children'][j]['text']+'</option>');
			      }
			    }
			
			    //Lägg in listboxarna
			    var select_parent = $('#edit-field-bytbil-bilmodell-tid-wrapper .form-item');

					$(select_parent).append(customselect_marke);
					$(select_parent).append(customselect_modell);
	
			    $('#edit-marke-customselect-marke').change(function() {
			      //Lägg in huvudkategorin som val i den riktiga listan
			      $('#edit-field-bytbil-bilmodell-tid').val($(this).val());
			      //Populera den andra listboxen dynamiskt
			      var customselect_modell = $('#edit-marke-customselect-modell');
			      //Töm eventuella befintliga alternativ
			      $(customselect_modell).hide(); //Workaround för en bugg i IE
			      $(customselect_modell).html('');
			      $(customselect_modell).append('<option selected="selected" value="All">Modell (Alla)</option>');
			      //Fyll den med de nya alternativen
			      for(var i=0, imax=bytbil_marke_modell.length; i<imax; i++) {
			        if(bytbil_marke_modell[i]['value']==$(this).val()) { //Hittat rätt huvudkategori
			          for(var j=0, jmax=bytbil_marke_modell[i]['children'].length; j<jmax; j++) {
			            $(customselect_modell).append('<option value="'+bytbil_marke_modell[i]['children'][j]['value']+'">'+bytbil_marke_modell[i]['children'][j]['text']+'</option>');
			          }
			          break;
			        }
			      }
			      $(customselect_modell).show(); //Workaround för en bugg i IE
			    });
			    $('#edit-marke-customselect-modell').change(function() {
			      //Lägg in underkategorin som val i den riktiga listan
			      if($(this).val()!='All') {
			        $('#edit-field-bytbil-bilmodell-tid').val($(this).val());
			      }
			      else {
			        $('#edit-field-bytbil-bilmodell-tid').val($('#edit-marke-customselect-marke').val());
			      }
			    });
				}
	    }
	    /*--------------Grupperar element---------------*/
	    if($('.group-left').length == 0) {
		    var group_left = $('<div class="group-left">');
				var group_right = $('<div class="group-right">');
				$('.view-bytbil-bilar .view-filters .views-exposed-form .views-exposed-widgets').append(group_left).append(group_right);
				var grouped_elements = {
					left: {
						0: 'bytbil-select-filter',
						1: 'views-widget-sort-wrapper',
						2: 'views-submit-button'
					},
					right: {
						0: 'bytbil-interval-filter'
					}
				};
		   	for (var g_class in grouped_elements['left']) {
					$(group_left).append($('.'+grouped_elements['left'][g_class]));
		   	}
		   	for (var g_class in grouped_elements['right']) {
					$(group_right).append($('.'+grouped_elements['right'][g_class]));
		   	}
			}

		  /*--------------Lägger in jQuery Slider-------------*/
		  if(Drupal.settings.hasOwnProperty('bytbil_xml') && Drupal.settings.bytbil_xml.hasOwnProperty('limits')) {
	      var min = null;
	      var max = null;
	      var filter_limits = Drupal.settings.bytbil_xml.limits;
	      for(fieldname in filter_limits) {

					var fieldname_adjusted = fieldname.replace(/_/g, '-');
	        min = $('input#edit-'+fieldname_adjusted+'-min');
	        max = $('input#edit-'+fieldname_adjusted+'-max');

	        if (!min.length || !max.length) {
	          return;
	        }

	        // Set default values or use those passed into the form
	        var init_min = ('' == min.val()) ? filter_limits[fieldname]['min'] : min.val();
	        var init_max = ('' == max.val()) ? filter_limits[fieldname]['max'] : max.val();

	        if($('.ui-slider').length < Object.size(filter_limits)) {
	          var min_parent = min.parent('.form-item');
	          //Flyttar eventuella suffix så att de lägger sig i fältet
	          var min_suffix = $(min_parent).next('.field-suffix');
	          if($(min_suffix).length > 0) {
							$(min_parent).append(min_suffix);
						}
						var max_parent = max.parent('.form-item');
						var max_suffix = $(max_parent).next('.field-suffix');
	          if($(max_suffix).length > 0) {
							$(max_parent).append(max_suffix);
						}

						var this_step = 1000;
	          if(init_max - init_min <= 1000) {
							this_step = 1;
						}
	          $(min_parent).after(
	            $('<div></div>').slider({
	              range: true,
	              min: filter_limits[fieldname]['min'],     // Adjust slider min and max to the range
	              max: filter_limits[fieldname]['max'],    // of the exposed filter.
	              step: this_step,
	              values: [init_min, init_max],
	              slide: function(event, ui){
	                parent_element = $(this).parent('.views-widget');
	                text_elements = $(parent_element).find('input');
	                $(text_elements).each(function() {
	                  if($(this).attr('id').search('min')!=-1) { //min-rutan
	                    $(this).val(ui.values[0]);
	                  }
	                  else if($(this).attr('id').search('max')!=-1) { //max-rutan
	                    $(this).val(ui.values[1]);
	                  }
	                });
	              }
	            })
	          );
	        }
	        /*------------Ser till att slidern flyttas om man ändrar i textrutan------------*/
	        min.change(function() {
	          var parent_element = $(this).parents('.views-widget');
	          var slider = $(parent_element).find('.ui-slider');
	          $(slider).slider('values',0,$(this).val());
	        });
	        max.change(function() {
	          var parent_element = $(this).parents('.views-widget');
	          var slider = $(parent_element).find('.ui-slider');
	          $(slider).slider('values',1,$(this).val());
	        });

	        /*------------------------------------------------------------------------------*/
	      }
	    }
    }
  }
})(jQuery);
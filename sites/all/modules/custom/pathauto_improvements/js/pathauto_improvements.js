(function($) {
	$(document).ready(function(){
    var urlstandard = $('#edit-pathauto-improvements > .fieldset-content > .form-item:first-child > input');
    var human_name = $('.form-item-name > input');
    var machine_name = $('.form-type-machine-name > input');
    var metatag = $('#edit-metatag-improvements > .fieldset-content > .form-item:first-child > input');
    if($(metatag).length>0) {
      var url_value = $(urlstandard).val();
      $(metatag).click(function() {
        var met_value = $(metatag).attr('checked');
        if(met_value) {
          if(url_value.substr(0, 1)=='[') {
            $(urlstandard).val(url_value);
          }
          else {
            if($(urlstandard).val()=='no-crawl/[node:title]') {
              $(urlstandard).val($(machine_name).val()+'/[node:title]');
            }          
          }
        }
        else {
          url_value = $(urlstandard).val();
          $(urlstandard).val('no-crawl/[node:title]');
        }
      });
    }
    $(human_name).keyup(function() {
      if($(metatag).length>0) {
        var met_value = $(metatag).attr('checked');
        if(met_value) {
          if($(urlstandard).val().substr(0, 1)!='[') {
            $(urlstandard).val($(machine_name).val()+'/[node:title]');
          }
        }
        else {
          $(urlstandard).val('no-crawl/[node:title]');
        }
      }
    });
    $(machine_name).keyup(function() {
      if($(metatag).length>0) {
        var met_value = $(metatag).attr('checked');
        if(met_value) {
          if($(urlstandard).val().substr(0, 1)!='[') {
            $(urlstandard).val($(machine_name).val()+'/[node:title]');
          }
        }
        else {
          $(urlstandard).val('no-crawl/[node:title]');
        }
      }
    });
  });
})(jQuery);
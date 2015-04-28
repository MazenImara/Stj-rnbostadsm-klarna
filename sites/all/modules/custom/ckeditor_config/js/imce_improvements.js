(function($) {
	$(document).ready(function() {
		if(imce !== undefined) {
			imce.updateUI = function() {
			  //file urls.
			  var furl = imce.conf.furl, isabs = furl.indexOf('://') > -1;
			  var absurls = imce.conf.absurls = imce.vars.absurls || imce.conf.absurls;
			  var host = location.host;
			  var baseurl = location.protocol + '//' + host;
			  if (furl.charAt(furl.length - 1) != '/') {
			    furl = imce.conf.furl = furl + '/';
			  }
			  imce.conf.modfix = imce.conf.clean && furl.indexOf(host + '/system/') > -1;
			  if (absurls && !isabs) {
			    imce.conf.furl = baseurl + furl;
			  }
			  else if ( !absurls && isabs && furl.indexOf(baseurl) == 0) {
			    imce.conf.furl = furl.substr(furl.indexOf('sites/'));
			  }
			  //convert button elements to input elements.
			  imce.convertButtons(imce.FW);
			  //ops-list
			  $('#ops-list').removeClass('tabs secondary').addClass('clear-block clearfix');
			  imce.opCloseLink = $(imce.newEl('a')).attr({id: 'op-close-link', href: '#', title: Drupal.t('Close')}).click(function() {
			    imce.vars.op && imce.opClick(imce.vars.op);
			    return false;
			  }).appendTo('#op-contents')[0];
			  //navigation-header
			  if (!$('#navigation-header').size()) {
			    $(imce.NW).children('.navigation-text').attr('id', 'navigation-header').wrapInner('<span></span>');
			  }
			  //log
			  $('#log-prv-wrapper').before($('#log-prv-wrapper > #preview-wrapper')).remove();
			  $('#log-clearer').remove();
			  //content resizer
			  $('#content-resizer').remove();
			  //message-box
			  imce.msgBox = imce.el('message-box') || $(imce.newEl('div')).attr('id', 'message-box').prependTo('#imce-content')[0];
			  //create help tab
			  var $hbox = $('#help-box');
			  $hbox.is('a') && $hbox.replaceWith($(imce.newEl('div')).attr('id', 'help-box').append($hbox.children()));
			  imce.hooks.load.push(function() {
			    imce.opAdd({name: 'help', title: $('#help-box-title').remove().text(), content: $('#help-box').show()});
			  });
			  //add ie classes
			  $.browser.msie && $('html').addClass('ie') && parseFloat($.browser.version) < 8 && $('html').addClass('ie-7');
			  // enable box view for file list
			  imce.vars.boxW && imce.boxView();
			  //scrolling file list
			  imce.syncScroll(imce.SBW, '#file-header-wrapper');
			  imce.syncScroll(imce.SBW, '#dir-stat', true);
			  //scrolling directory tree
			  imce.syncScroll(imce.NW, '#navigation-header');
			}
		}
	});
})(jQuery);
(function() {
   var i;
   for(i = 0; i < document.links.length; i++) {
       var url = document.links[i].href;
       if((url.substring(url.length-3, url.length) == 'pdf' || url.substring(url.length-3, url.length) == 'tif') && url.indexOf('?') == -1) {
          // document.links[i].href = 'javascript:window.open(\'http://www.atalasoft.com/31apps/ThinDoc/?url=' + escape(url) + '\',\'_thindoc\');';document.links[i].target = '_thindoc';
          //document.links[i].className += ' colorbox-load';
          document.links[i].rel = 'shadowbox;width=1000;height=800;';
          //document.links[i].href = 'http://storleden.se?width=500&height=500&iframe=true';
          document.links[i].href = 'http://www.atalasoft.com/31apps/ThinDoc/?url=' + escape(url);
          //document.links[i].href = 'http://www.atalasoft.com/31apps/ThinDoc/?url=' + escape(url) + '?width=500&height=500&iframe=true';
          //document.links[i].target = '_thindoc';
       }
   }
})();
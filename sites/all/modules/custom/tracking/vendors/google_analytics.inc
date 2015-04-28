<?php
function tracking_google_analytics_get_code($vendor) {
  $code = array();
  $host = general_get_host();
  $code['page_bottom'][] = <<<EOT
  <script type="text/javascript">
  var _gas = _gas || [];
  (function($) {
    if(Drupal.behaviors.cookiecontrol_improvements !== undefined && Drupal.behaviors.cookiecontrol_improvements.onaccept !== undefined) {
      Drupal.behaviors.cookiecontrol_improvements.onaccept.google_analytics = function() {
        _gas.push(['_setAccount', '{$vendor['id']}']);
        _gas.push(['_setDomainName', '$host']);
        _gas.push(['_trackPageview']);
        _gas.push(['_gasTrackForms']);
        _gas.push(['_gasTrackOutboundLinks']);
        _gas.push(['_gasTrackMaxScroll']);
        _gas.push(['_gasTrackDownloads']);
        _gas.push(['_gasTrackYoutube', {force: true}]);
        _gas.push(['_gasTrackVimeo', {force: true}]);
        _gas.push(['_gasTrackMailto']);

        var ga = document.createElement('script');
        ga.type = 'text/javascript';
        ga.async = true;
        ga.src = '//cdnjs.cloudflare.com/ajax/libs/gas/1.10.1/gas.min.js';
        var s = document.getElementsByTagName('script')[0];
        s.parentNode.insertBefore(ga, s);
      };
    }
    $(window).load(function() {
      if($('html').hasClass('cookies')) {
        Drupal.behaviors.cookiecontrol_improvements.onaccept.google_analytics();
      }
    });
  })(jQuery);
  </script>
EOT;
  return $code;
}
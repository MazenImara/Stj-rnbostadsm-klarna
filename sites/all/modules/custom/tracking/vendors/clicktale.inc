<?php
function tracking_clicktale_get_code($vendor) {
  $code = array();
  $code['page_top'][] = <<<'EOT'
    <!-- tracking Top part -->
    <script type="text/javascript">
    var WRInitTime=(new Date()).getTime();
    </script>
    <!-- tracking end of Top part -->
EOT;
  $code['page_bottom'][] = <<<EOT
    <!-- tracking Bottom part -->
    <div id="trackingDiv" style="display: none;"></div>
    <script type="text/javascript">
    (function($) {
      if(Drupal.behaviors.cookiecontrol_improvements !== undefined && Drupal.behaviors.cookiecontrol_improvements.onaccept !== undefined) {
        Drupal.behaviors.cookiecontrol_improvements.onaccept.clicktale = function() {
          if(document.location.protocol != 'https:') document.write(unescape("%3Cscript%20src='" +'http://cdn.tracking.net/www/' + "WRe0.js'%20type='text/javascript'%3E%3C/script%3E"));
          </script>
          <script type="text/javascript">
          trackingFetchFromWithCookies.setFromCookie("has_js");
          trackingFetchFrom = trackingFetchFromWithCookies.constructFetchFromUrl();
          if(typeof tracking=='function')  tracking({$vendor['id']}, 1);
        }
      }
      $(window).load(function() {
        if($('html').hasClass('cookies')) {
          Drupal.behaviors.cookiecontrol_improvements.onaccept.clicktale();
        }
      });
    })(jQuery);
    </script>
    <!-- tracking end of Bottom part --> 
EOT;
  return $code;
}
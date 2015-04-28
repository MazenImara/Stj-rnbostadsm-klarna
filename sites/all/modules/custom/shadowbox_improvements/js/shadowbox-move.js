/**
 * A shadowbox player that replaces the inline (html) player with one
 * that will move the content instead of copying it. This preserves
 * any callbacks registered on the content, fields populated by the
 * user, and ensures element id's are unique.
 *
 * To use just include after including shadowbox and the implementation
 * of the HTML player will be adjusted.
 *
 * Note this player is dependent of Prototype but could probably easily
 * be adjusted to use any adapter.
 */
(function($) {
  Shadowbox.html.prototype = {
    /**
      * Moves inline html into shadowbox
      *
      * @param   {HTMLElement}   body    The body element
      * @param   {Object}        dims    The current Shadowbox dimensions
      * @public
      */
    append: function(body, dims) {
      // Wrap in #id.html div container
      var selector = $(this.obj.link).attr('href');
      if(selector.substr(0, 1) === '#') {
        var $content = $(selector);
        if($content.length > 0 && !$content.parent().hasClass('placeholder')) {
          $content.wrap('<div class="placeholder"></div>');
          $content.addClass('html');
        }
        this.placeholder = $content.parent();
        $(body).append($content);
      }
    },

    /**
      * Moves inline html back out of shadowbox
      *
      * @public
      */
    remove: function() {
      var selector = $(this.obj.link).attr('href');
      if(selector.substr(0, 1) === '#') {
        var $content = $(selector);
        if($content.length > 0) {
          this.placeholder.append($content);
        }
      }
    }
  }
})(jQuery);
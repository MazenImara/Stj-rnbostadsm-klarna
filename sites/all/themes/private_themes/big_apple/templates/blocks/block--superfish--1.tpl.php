<?php $tag = $block->subject ? 'section' : 'div'; ?>
<<?php print $tag; ?><?php print $attributes; ?>>
  <div class="block-inner clearfix">
    <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
      <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php
      if(module_exists('provsite')) {
        if(module_exists('html_parser')) {
          $html = str_get_html($content);
          $loginlink = $html->find('a[href=/login]', 0);
          if($loginlink) {
            $destination = drupal_get_destination();
            $loginlink->href.='?destination='.$destination['destination'];
          }
          $content = $html->save();
          $html->clear();
          unset($html);
        }
      }
    ?>
    <div<?php print $content_attributes; ?>>
      <?php print '<div class="menu-bg"></div>'.$content; ?>
    </div>
  </div>
</<?php print $tag; ?>>

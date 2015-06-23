<article<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>
  
  <div<?php print $content_attributes; ?>>
    <?php unset($content['links']); ?>
    <?php print render($content); ?>
  </div>
</article>
<article<?php print $attributes; ?>>
  <?php print $user_picture; ?>
  <?php print render($title_prefix); ?>
  <?php if (!$page && $title): ?>
  <div class="header">
    <?php if($trimmed_view_mode === 'promo'): ?>
      <?php /*print "<h2 $title_attributes>$title</h2>";*/ ?>
    <?php else: ?>
      <?php print "<h2 $title_attributes><a href='$node_url' title='$title'>$title</a></h2>"; ?>
    <?php endif; ?>
  </div>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <div<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_sidhuvud']);

      print render($content);
    ?>
  </div>
  
  <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <nav class="links node-links clearfix"><?php print render($content['links']); ?></nav>
    <?php endif; ?>

    <?php print render($content['comments']); ?>
  </div>
</article>
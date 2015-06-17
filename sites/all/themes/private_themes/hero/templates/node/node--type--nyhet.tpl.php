<article<?php print $attributes; ?>>
  <?php if($trimmed_view_mode === 'slideshow'): ?>
    <?php if(!empty($user_picture)): ?>
      <?php print $user_picture; ?>
    <?php endif; ?>
  <?php elseif($trimmed_view_mode === 'full'): ?>
    <?php if(!empty($title)): ?>
      <div class="extratitel-wrapper">
        <span class="extratitel"><?php print $title; ?></span>
        <span class="delimiter"></span>
      </div>
    <?php endif; ?>
  <?php endif; ?>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>
  
  <div<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  <?php
    if($trimmed_view_mode === 'slideshow') {
      print l(t('Read more'), 'node/' . $node->nid, array('attributes' => array('title' => t('Read more'), 'class' => array('read-more'))));
    }
  ?>
  </div>
</article>
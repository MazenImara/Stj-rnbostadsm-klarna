<article<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>
  
  <div<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  <?php
    if($trimmed_view_mode === 'teaser') {
      print l(t('Read more'), 'node/' . $node->nid, array('attributes' => array('title' => t('Read more'), 'class' => array('read-more'))));
    }
  ?>
  </div>
</article>
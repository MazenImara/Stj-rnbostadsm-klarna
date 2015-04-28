<article<?php print $attributes; ?>>
  <?php print $user_picture; ?>
  <?php if(isset($content['field_sidhuvud'])) print render($content['field_sidhuvud']); ?>
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <div class="header">
    <?php if($page && $trimmed_view_mode === 'full'): ?>
      <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php else: ?>
      <?php if($trimmed_view_mode === 'promo'): ?>
        <h2<?php print $title_attributes; ?>><?php print $title ?></h2>
      <?php elseif($trimmed_view_mode !== 'carousel'): ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
      <?php endif; ?>
    <?php endif; ?>
    </div>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <div<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);

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
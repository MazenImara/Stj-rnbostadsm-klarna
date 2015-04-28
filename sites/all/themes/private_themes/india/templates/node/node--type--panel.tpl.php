<article<?php print $attributes; ?>>
  <?php if(isset($content['field_sidhuvud'])) print render($content['field_sidhuvud']); ?>
  <?php print $user_picture; ?>

  <div<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      if(isset($content['field_text'])) {
        hide($content['field_text']);
      }
      print render($content);
      ?>
      <div class="panel-content contextual-links-region">
      <?php print render($title_prefix); ?>
      <?php print render($title_suffix); ?>
      <?php if ($title): ?>
        <div class="header">
        <?php if($page): ?>
          <h1 class="title" id="page-title"><?php print $title; ?></h1>
        <?php else: ?>
          <?php if($trimmed_view_mode === 'promo'): ?>
            <h2<?php print $title_attributes; ?>><?php print $title ?></h2>
          <?php else: ?>
            <h2<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
          <?php endif; ?>
        <?php endif; ?>
        </div>
      <?php endif; ?>
    <?php
      if(isset($content['field_text'])) {
        print render($content['field_text']);
      }
    ?>
    </div>
  </div>
  <?php /*
  <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <nav class="links node-links clearfix"><?php print render($content['links']); ?></nav>
    <?php endif; ?>

    <?php print render($content['comments']); ?>
  </div>
  */ ?>
</article>
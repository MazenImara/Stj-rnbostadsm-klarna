<article<?php print $attributes; ?>>
  <?php print $user_picture; ?>
  <?php print render($title_prefix); ?>
  <?php if (!$page && $title): ?>
  <div class="header">
    <?php if($trimmed_view_mode === 'promo'): ?>
      <h2<?php print $title_attributes; ?>><?php print $title ?></h2>
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
      if(module_exists('html_parser')) {
        $html = str_get_html($content['body'][0]['#markup']);
        $paragraphs = $html->find('p');
        if($paragraphs) {
          $new_text = '';
          foreach($paragraphs as $p) {
            $new_text .= $p->innertext.', ';
          }
          $new_text = substr($new_text, 0, -2);
        }
        $content['body'][0]['#markup'] = '<p>'.$new_text.'</p>';
        $html->clear();
        unset($html);
      }
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
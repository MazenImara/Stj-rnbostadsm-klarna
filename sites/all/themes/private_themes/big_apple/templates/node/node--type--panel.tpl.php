<article<?php print $attributes; ?>>
  <?php if(isset($content['field_sidhuvud'])) print render($content['field_sidhuvud']); ?>
  <?php print $user_picture; ?>

  <div<?php print $content_attributes; ?>>
    <?php

      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      /*
      if(isset($content['field_text'])) {
        print render($content['field_text']);
      }
      */
      if(isset($content['field_text']) && isset($content['body']) && isset($content['body']['#markup'])) {
        if(module_exists('html_parser')) {
          $html = str_get_html($content['body']['#markup']);
          $pane_middle = $html->find('div[class=panel-panel grid-24 middle]', 0);
          if($pane_middle) {
            $title_data = '';
            $title_data .= render($title_prefix);
            if ($title) {
              $title_data .= '<div class="header">';
              if($page) {
                $title_data .= '<h1 class="title" id="page-title">'.$title.'</h1>';
              }
              else {
                if($trimmed_view_mode === 'promo') {
                  $title_data .= '<h2'.$title_attributes.'>'.$title.'</h2>';
                }
                else {
                  $title_data .= '<h2'.$title_attributes.'><a href="'.$node_url.'" title="'.$title.'">'.$title.'</a></h2>';
                }
              }
              $title_data .= '</div>';
            }
            $title_data .= render($title_suffix);
            $field_text = render($content['field_text']);
            $pane_middle->outertext = '<div class="main-content contextual-links-region">' . $title_data . $field_text . '</div>' . $pane_middle->outertext;
          }
          $content['body']['#markup'] = $html->save();
          $html->clear();
          unset($html);
        }
      }
      print render($content);
    ?>
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
<?php
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */
$summary_chars = 90; //Antalet tecken som facebookinlägg ska kortas av till
?>
<div class="<?php print $classes; ?>">
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <?php print $title; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>

  <?php if ($rows): ?>
    <div class="view-content">
      <?php 
        if(module_exists('html_parser')) {
          $html = str_get_html($rows);
          $html_rows = $html->find('div[class=views-row]');
          if($html_rows) {
            foreach($html_rows as $html_row) {
              //Content
              $text = $html_row->find('div[class=feed-content]', 0);
              if($text) {
                $text->innertext = str_replace('<br />', ' ', $text->innertext);
                $nytext = text_summary($text->innertext, NULL, $summary_chars);
                if(strlen($text->innertext)>strlen($nytext)) {
                  $nytext .= '...';
                  $text->innertext = $nytext;
                }
              }

              //Bild
              $temp = $html_row->find('img', 0);
              if($temp) {
                $image = '<div class="post-bild">'.$temp->outertext.'</div>';
                $parent = $temp->parent()->parent()->parent();
                $temp->outertext = '';
              }
              else {
                $current_facebook_icon_path = variable_get('general_functions_social_facebook_icon_path', '');
                if($current_facebook_icon_path) {
                  $facebook_icon_data = array(
                    'width' => '',
                    'height' => '',
                    'alt' => '',
                    'title' => '',
                    'path' => $current_facebook_icon_path,
                  );
                  $image = '<div class="feed-bild facebook-feed-bild">'.theme('image',$facebook_icon_data).'</div>';
                  $parent = $html_row->find('div[class=feed-content]', 0);
                }
              }
              if($parent) {
                $parent->outertext = $image . $parent->outertext;
              }

              //Länk
              $link = $html_row->find('div[class=feed-link]', 0)->children(0)->children(0);
              if($link) {
                $variable = 'general_functions_social_facebook_id';
                $value = variable_get($variable, '');
                $link->href = "http://www.facebook.com/$value";
                $link->innertext = t('read more').'...';
              }
            }
          }
          $rows = $html->save();
          $html->clear();
          unset($html);
        }
        print $rows; 
      ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div><?php /* class view */ ?>

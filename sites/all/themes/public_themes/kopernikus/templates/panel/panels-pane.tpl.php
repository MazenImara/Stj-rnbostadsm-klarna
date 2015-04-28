<?php
/**
 * @file panels-pane.tpl.php
 * Main panel pane template
 *
 * Variables available:
 * - $pane->type: the content type inside this pane
 * - $pane->subtype: The subtype, if applicable. If a view it will be the
 *   view name; if a node it will be the nid, etc.
 * - $title: The title of the content
 * - $content: The actual content
 * - $links: Any links associated with the content
 * - $more: An optional 'more' link (destination only)
 * - $admin_links: Administrative links associated with the content
 * - $feeds: Any feed icons or associated with the content
 * - $display: The complete panels display object containing all kinds of
 *   data including the contexts and all of the other panes being displayed.
 */
?>
<?php if ($pane_prefix): ?>
  <?php print $pane_prefix; ?>
<?php endif; ?>
<div class="<?php print $classes; print ' contextual-links-region'; ?>" <?php print $id; ?>>
  <?php if ($admin_links): ?>
    <?php print $admin_links; ?>
  <?php endif; ?>
  <?php if(is_array($content)): ?>
    <?php if(isset($content['field_sidhuvud']) && $content['field_sidhuvud']): ?>
      <?php 
        print render($content['field_sidhuvud']); 
        unset($content['field_sidhuvud']);
      ?>
    <?php endif; ?>
  <?php else: ?>
    <?php if(module_exists('html_parser')): ?>
      <?php
        $html = str_get_html($content);
        $temp = $html->find('div[class=contextual-links-wrapper]', 0);
        if($temp) {
          $cl = $temp->outertext;
          $temp->outertext = '';
          print $cl;
        }
        $temp = $html->find('div[class=field-name-field-sidhuvud]', 0);
        if($temp) {
          $sh = $temp->outertext;
          $temp->outertext = '';
          print $sh;
        }
        $content = $html->save();
        $html->clear();
        unset($html);
      ?>
    <?php endif; ?>
  <?php endif; ?>
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h2<?php print $title_attributes; ?>><?php print $title; ?></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($feeds): ?>
    <div class="feed">
      <?php print $feeds; ?>
    </div>
  <?php endif; ?>

  <div class="pane-content">
    <?php print render($content); ?>
  </div>

  <?php if ($links): ?>
    <div class="links">
      <?php print $links; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <div class="more-link">
      <?php print $more; ?>
    </div>
  <?php endif; ?>
</div>
<?php if ($pane_suffix): ?>
  <?php print $pane_suffix; ?>
<?php endif; ?>

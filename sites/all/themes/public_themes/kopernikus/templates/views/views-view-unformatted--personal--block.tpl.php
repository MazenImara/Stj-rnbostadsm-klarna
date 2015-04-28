<?php
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)): ?>
  <div class="personalomrade">
  <h2><?php print $title; ?></h2>
<?php endif; ?>
<?php foreach ($rows as $id => $row): ?>
  <?php
    if(module_exists('html_parser')) {
      $html = str_get_html($row);
      $clinks = $html->find('div[class=contextual-links-wrapper]', 0);
      if($clinks) {
        $classes_array[$id] .= ' contextual-links-region';
      }
      $html->clear();
      unset($html);
    }
  ?>
  <div class="<?php print $classes_array[$id]; ?>">
    <?php print $row; ?>
  </div>
<?php endforeach; ?>
<?php if (!empty($title)): ?>
  </div>
<?php endif; ?>
<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php
  $output = '';
  $nr_of_cols = 3;
  $items_per_col = 1;
  $row_count = count($rows);
  if($row_count > $nr_of_cols) {
    $items_per_col = (int) ($row_count / $nr_of_cols);
  }
  $cols = array_chunk($rows, $items_per_col, true);
  if(count($cols) > $nr_of_cols) {
    $cols[$nr_of_cols - 1] += array_pop($cols);
  }
  foreach($cols as $i => $col) {
    print "<div class='col col-$i'>";
    foreach($col as $id => $row) {
      $classes = $classes_array[$id] ? " class='{$classes_array[$id]}'" : '';
      print "<div$classes>$row</div>";
    }
    print "</div>";
  }
?>

<?php
/**
 * @file
 * Template file for extimgfield_slideshow_controls
 *
 *
 */
?>
<div id="extimgfield-slideshow-<?php print $slideshow_id; ?>-controls" class="extimgfield-slideshow-controls">
  <a href="#" class="prev"><?php print t('Prev'); ?></a>
  <?php if (!empty($controls_pause)) : ?>
    <a href="#" class="play"><?php print t('Play'); ?></a>
    <a href="#" class="pause"><?php print t('Pause'); ?></a>
  <?php endif; ?>
  <a href="#" class="next"><?php print t('Next'); ?></a>
</div>

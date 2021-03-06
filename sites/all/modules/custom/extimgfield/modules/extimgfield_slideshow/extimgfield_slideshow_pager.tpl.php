<?php
/**
 * @file
 * Template file for extimgfield_slideshow_pager
 *
 *
 */
?>
<?php if ($pager == 'number') : ?>

  <div id="extimgfield-slideshow-<?php print $slideshow_id; ?>-pager" class="extimgfield-slideshow-pager slides-<?php print count($items); ?>"></div>

<?php elseif ($pager == 'image' || $pager == 'carousel') : ?>

  <?php if ($pager == 'carousel') : ?>
    <div id="extimgfield-slideshow-<?php print $slideshow_id; ?>-carousel-wrapper" class="extimgfield-slideshow-carousel-wrapper<?php print ($pager == 'carousel' && $carousel_skin) ? " jcarousel-skin-" . $carousel_skin : ""; ?>">
      <?php if(!$carousel_skin): ?><a href="#" class="carousel-prev">«</a><?php endif; ?>
      <div id="extimgfield-slideshow-<?php print $slideshow_id; ?>-carousel" class="extimgfield-slideshow-carousel">
  <?php endif; ?>

  <?php print $thumbnails; ?>

  <?php if ($pager == 'carousel') : ?>
      </div>
      <?php if(!$carousel_skin): ?><a href="#" class="carousel-next">»</a><?php endif; ?>
    </div>
  <?php endif; ?>

<?php endif; ?>

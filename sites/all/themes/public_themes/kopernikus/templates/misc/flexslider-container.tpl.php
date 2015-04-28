<?php
/**
 * @file
 * Default output for a Flex Slider node.
*/
?>
<div class="flex-nav-container">
  <div class="flexslider-content flexslider clearfix" id="flexslider-<?php print $id; ?>">
    <ul class="slides">
    <?php foreach($items as $item) : ?>
      <li>
      <?php
        $image = array(
          'path' => (isset($item['#item']['uri'])) ? $item['#item']['uri'] : '',
          'alt' => (isset($item['#item']['alt'])) ? $item['#item']['alt'] : '',
        );
        // Gets image height and width attributes.
        if (isset($item['#item']['width']) && isset($item['#item']['height'])) {
          $image['width'] = $item['#item']['width'];
          $image['height'] = $item['#item']['height'];
        }
        // Gets image 'longdesc' attribute.
        if (drupal_strlen($item['#item']['longdesc']) > 0) {
          $image['longdesc'] = $item['#item']['longdesc'];
        }
        
        // Gets image 'caption' attribute.
        if (drupal_strlen($item['#item']['caption']) > 0) {
          $image['caption'] = $item['#item']['caption'];
        }
      
        // Gets anchor 'title' attribute.
        if (drupal_strlen($item['#item']['title']) > 0) {
          $image['title'] = $item['#item']['title'];
        }
        // Gets anchor 'rel' attribute.
        if (drupal_strlen($item['#item']['rel']) > 0) {
          $image['rel'] = $item['#item']['rel'];
        }
        // Gets anchor 'class' attribute.
        if (drupal_strlen($item['#item']['class']) > 0) {
          $image['class'] = $item['#item']['class'];
        }
        if (!empty($item['#image_style'])) {
          $image['style_name'] = $item['#image_style'];
          $output = theme('image_style', $image);
        }
        else {
          $output = theme('image', $image);
        }
        if(!empty($item['#item']['url'])) {
          $options = array(
            'html' => TRUE,
            'attributes' => array(
              'title' => (isset($item['#item']['title'])) ? $item['#item']['title'] : '',
              'target' => (isset($item['#item']['target'])) ? $item['#item']['target'] : '',
              'rel' => (isset($item['#item']['rel'])) ? $item['#item']['rel'] : '',
              'class' => (isset($item['#item']['class'])) ? $item['#item']['class'] : '',
            ),
          );
          $output = l($output, $item['#item']['url'], $options);
        }
      print $output;
      ?>
        <?php /*dpm($item['#item']);*/ ?>
        <?php if(!empty($item['#item']['caption'])): ?>
          <div class="flex-caption"><?php print $item['#item']['caption']; ?></div>
        <?php else: ?>
          <?php if(!empty($item['#item']['title']) || !empty($item['#item']['alt'])) : ?>
           <div class="flex-caption"><strong><?php print $item['#item']['title']; ?></strong>&nbsp;<?php print $item['#item']['alt'];?></div>
          <?php endif; ?>
        <?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>
  </div>
</div>
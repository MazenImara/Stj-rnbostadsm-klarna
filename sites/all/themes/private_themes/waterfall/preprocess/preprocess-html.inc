<?php
function waterfall_alpha_preprocess_html(&$vars) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Josefin+Slab:400,600,700,600italic', array('type' => 'external'));
  drupal_add_css('http://fonts.googleapis.com/css?family=PT+Sans+Narrow:400,700', array('type' => 'external'));
}
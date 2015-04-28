<?php
function india_alpha_preprocess_html(&$vars) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Milonga&subset=latin,latin-ext', array('type' => 'external'));
}
<?php
function pure_white_alpha_preprocess_html(&$vars) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Oswald:400,700,300&subset=latin,latin-ext', array('type' => 'external'));
}
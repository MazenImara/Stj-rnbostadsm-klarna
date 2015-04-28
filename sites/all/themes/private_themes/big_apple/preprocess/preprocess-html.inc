<?php
function big_apple_alpha_preprocess_html(&$vars) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Krona+One', array('type' => 'external'));
  drupal_add_css('http://fonts.googleapis.com/css?family=Allan:bold', array('type' => 'external'));
  drupal_add_css('http://fonts.googleapis.com/css?family=Nobile:400,400italic,700,700italic', array('type' => 'external'));
  drupal_add_css('http://fonts.googleapis.com/css?family=Open+Sans:400,600italic,800,700,400italic,300,300italic', array('type' => 'external'));
}
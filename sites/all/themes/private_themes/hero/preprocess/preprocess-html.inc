<?php
function hero_alpha_preprocess_html(&$vars) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Lato:400,900&subset=latin,latin-ext', array('type' => 'external'));
  drupal_add_css('http://fonts.googleapis.com/css?family=Raleway:400,600,700', array('type' => 'external'));
}
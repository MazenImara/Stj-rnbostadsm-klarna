<?php
function hero_alpha_preprocess_html(&$vars) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Lato&subset=latin,latin-ext', array('type' => 'external'));
}
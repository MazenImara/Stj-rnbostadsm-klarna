<?php
function kopernikus_alpha_preprocess_comment(&$vars) {
  $vars['created_formatted'] = strtotime($vars['datetime']);
  $vars['created_formatted'] = format_date($vars['created_formatted'], 'medium');
}
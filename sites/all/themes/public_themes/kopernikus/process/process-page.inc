<?php
function kopernikus_alpha_process_page(&$vars) {
  if(isset($vars['node'])) {
    $vars['theme_hook_suggestions'][] = 'page__type__'.$vars['node']->type;
  }
  //general_dpm_container($vars['theme_hook_suggestions']);
}
<?php
//Flyttar på kontaktformulärblocket på startsidan så att det hamnar bredvid maintexten.
function pure_white_alpha_preprocess_page(&$vars) {
  if($vars['is_front']) {
    $page_content =& $vars['page']['content']['content']['content'];
    if(isset($page_content['system_main']['nodes']['1'])) {
      $panel_content =& $page_content['system_main']['nodes']['1'];
      if(isset($page_content['webform_client-block-63'])) {
        $page_content['system_main']['nodes']['1']['webform_client-block-63'] = $page_content['webform_client-block-63'];
        unset($page_content['webform_client-block-63']);
      }
    }
  }
}
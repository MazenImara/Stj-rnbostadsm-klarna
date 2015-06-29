<?php
function hero_alpha_preprocess_block(&$vars) {
  if($vars['block']->module === 'superfish') {
    if($vars['block']->delta == 3) {
      if(module_exists('html_parser')) {
        $html = str_get_html($vars['content']);
        $ul = $html->find('ul', 0);
        if($ul) {
          //Language select
          module_load_include('inc', 'language_switcher_select_block', 'language_switcher_select_block.block');
          $language_switcher = language_switcher_select_block_content();
          if($language_switcher) {
            $li = '<li class="language-switcher-select-wrapper">';
            $li .= $language_switcher;
            $li .= '</li>';
            $ul->innertext .= $li;
          }
        }
        $vars['content'] = $html->save();
        $html->clear();
        unset($html);
        //Se till att chosen laddas
        drupal_add_library('chosen', 'drupal.chosen');
      }
    }
    else if($vars['block']->delta == 4) {
      if(module_exists('html_parser')) {
        $html = str_get_html($vars['content']);
        $ul = $html->find('ul', 0);
        if($ul) {
          //Language select
          module_load_include('inc', 'language_switcher_select_block', 'language_switcher_select_block.block');
          $language_switcher = language_switcher_select_block_content();
          if($language_switcher) {
            $li = '<li class="language-switcher-select-wrapper">';
            $li .= $language_switcher;
            $li .= '</li>';
            $ul->innertext .= $li;
          }
          //Login link
          $li = '<li class="login-link-wrapper">';
          $login_link = array(
            '#theme' => 'link',
            '#text' => t('Login'),
            '#path' => 'user/login',
            '#options' => array(
              'attributes' => array('class' => array('login-link')),
              'html' => FALSE,
            ),
          );
          $li .= render($login_link);
          $li .= '</li>';
          $ul->innertext .= $li;
        }
        $vars['content'] = $html->save();
        $html->clear();
        unset($html);
      }
    }
  }
}
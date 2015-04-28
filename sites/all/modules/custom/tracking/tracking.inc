<?php

function tracking_check_roles() {
  global $user;
  $role_match = true; //Om inga begränsningar har angivits så ska funktionen returnera true.
  if($roles = variable_get('general_admin_tracking_roles', array(1))) {
    if (!array_intersect($roles, array_keys($user->roles))) {
      $role_match = false;
    }
  }  
  return $role_match;
}

function tracking_check_paths() {
  $page_match = true; //Om inga begränsningar har angivits så ska funktionen returnera true.
  if ($pages = variable_get('general_admin_tracking_paths', "admin\radmin/*\rbatch\rnode/add*\rnode/*/*\ruser/*/*\r*imce*")) {
    // Convert path to lowercase. This allows comparison of the same path
    // with different case. Ex: /Page, /page, /PAGE.
    $pages = drupal_strtolower($pages);
    $visibility = variable_get('general_admin_tracking_visibility', BLOCK_VISIBILITY_NOTLISTED);
    // Convert the Drupal path to lowercase
    $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
    // Compare the lowercase internal and lowercase path alias (if any).
    $page_match = drupal_match_path($path, $pages);
    if ($path != $_GET['q']) {
      $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
    }
    // When $block->visibility has a value of 0 (BLOCK_VISIBILITY_NOTLISTED),
    // the block is displayed on all pages except those listed in $block->pages.
    // When set to 1 (BLOCK_VISIBILITY_LISTED), it is displayed only on those
    // pages listed in $block->pages.
    $page_match = !($visibility xor $page_match);
  }
  return $page_match;
}
<?php
class sweaver_improvements extends sweaver_plugin {
  /**
   * Return false to not show editor.
   */
  public function show_editor() {
    global $user;
    $default_values = variable_get('sweaver_improvements_display_widget', array());
    if(isset($default_values[$user->uid]) && !$default_values[$user->uid]) return false;
    return true;
  }
}
<?php

class views_improvements_handler_filter_fields_compare extends views_handler_filter_fields_compare {
  
  // exposed filter options
  var $always_multiple = TRUE;
  // Don't display empty space where the operator would be.
  var $no_operator = TRUE;
  // Whether to accept NULL as a false value or not
  var $accept_null = FALSE;

  /**
   * Overrides views_handler_filter_fields_compare::can_expose
   */
  function can_expose() {
    return TRUE;
  }

  /**
   * Overrides views_handler_filter_fields_compare::expose_form
   */
  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['#theme'] = 'views_ui_expose_filter_fields_compare_form';

    $form['expose']['value_labels'] = array(
      '#type' => 'fieldset',
      '#tree' => true,
      '#title' => t('Value labels'),
      'true' => array(
        '#type' => 'textfield',
        '#title' => t('True value'),
        '#default_value' => $this->options['expose']['value_labels']['true'],
        '#size' => 20,
      ),
      'false' => array(
        '#type' => 'textfield',
        '#title' => t('False value'),
        '#default_value' => $this->options['expose']['value_labels']['false'],
        '#size' => 20,
      ),
    );
  }
  //Overrides views_handler_filter_fields_compare::expose_submit
  function expose_submit($form, &$form_state) {
    $this->options['expose'] = $form_state['values']['options']['expose'];
  }
  /**
   * Overrides views_handler_filter_fields_compare::expose_options
   */
  function expose_options() {
    parent::expose_options();
    $this->options['expose']['value_labels']['true'] = 'True';
    $this->options['expose']['value_labels']['false'] = 'False';
    $this->options['expose']['required'] = FALSE;
  }  
  
  function fields_operator_get_opposite($operator) {
    $opposites = array(
      '<' => '>=',
      '<=' => '>',
      '=' => '<>',
      '<>' => '=',
      '>=' => '<',
      '>' => '<=',
    );
    if(!empty($opposites[$operator])) {
      return $opposites[$operator];
    }
    else {
      return $operator;
    }
  }

  /**
   * Overrides views_handler_filter_fields_compare::option_definition
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['left_field'] = $options['right_field'] = array('default' => '');
    $options['value']['default'] = 'All';
    return $options;
  }
  
  /**
   * Overrides views_handler_filter_fields_compare::value_form
   */
  function value_form(&$form, &$form_state) {
    $is_exposed = $this->options['exposed'];
    if($is_exposed) {
      $filter_form_type = 'select';
      $form['value'] = array(
        '#type' => $filter_form_type,
        '#title' => t('Default value'),
        '#options' => array(0 => t($this->options['expose']['value_labels']['false']), 1 => t($this->options['expose']['value_labels']['true'])),
        '#default_value' => $this->value,
      );
      $identifier = $this->options['expose']['identifier'];
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
      // Add an <Any> option.
      if (empty($this->options['expose']['required'])) {
        $any_label = variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? '<Any>' : t('- Any -');
        if ($form['value']['#type'] != 'select') {
          $any_label = check_plain($any_label);
        }
        $form['value']['#options'] = array('All' => $any_label) + $form['value']['#options'];
      }
    }
  }
  
  /**
   * Overrides views_handler_filter_fields_compare::query
   *
   * Build extra condition from existing fields (from existing joins).
   */
  function query() {
    $this->ensure_my_table();
    $left = $this->options['left_field'];
    $right = $this->options['right_field'];
    $is_exposed = $this->options['exposed'];
    if(!$is_exposed) {
      $operator = $this->options['operator'];
    }
    else {
      if($this->value) {
        $operator = $this->options['operator'];
      }
      else {
        $operator = $this->fields_operator_get_opposite($this->options['operator']);
      }
    }
    //Actual query
    // Get all existing field handlers.
    $field_handlers = $this->view->display_handler->get_handlers('field');

    // Make sure the selected fields still exist.
    if (!isset($field_handlers[$left], $field_handlers[$right])) {
      return;
    }

    // Get the left table and field.
    $left_handler = $field_handlers[$left];
    $left_handler->set_relationship();
    $left_table_alias = $this->query->ensure_table($left_handler->table, $left_handler->relationship);

    // Get the left table and field.
    $right_handler = $field_handlers[$right];
    $right_handler->set_relationship();
    $right_table_alias = $this->query->ensure_table($right_handler->table, $right_handler->relationship);

    // Build piece of SQL.
    $snippet =
      $left_table_alias . '.' . $left_handler->real_field .
      ' ' . $operator . ' ' .
      $right_table_alias . '.' . $right_handler->real_field;

    $this->query->add_where_expression($this->options['group'], $snippet);
  }
}
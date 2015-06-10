<?php

/**
 * Implementation of hook_field_info().
 */
function imagemapfield_field_info() {
  return array(
    'imagemap' => array(
      'label' => 'Image Map',
      'description' => t('Image map field. Emphasis on the map element.'),
      'default_widget' => 'imagemap_default',
      'default_formatter' => 'imagemap_default',
      'instance_settings' => array('responsive' => true),
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function imagemapfield_field_instance_settings_form($field, $instance) {
  // TODO: Support responsive image check box.
  $form = array();

  return $form;
}

/**
 * Implements hook_field_validate().
 */
function imagemapfield_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    // TODO: ensure that we have valid HTML and parse out any element that is not an <area>
  }
}

/**
 * Implements hook_field_is_empty().
 */
function imagemapfield_field_is_empty($item, $field) {
  if (!isset($item['value']) || $item['value'] === '') {
    return !isset($item['attach_to']) || $item['attach_to'] === '';
  }
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function imagemapfield_field_widget_info() {
  return array(
    'imagemapfield_default' => array(
      'label' => t('Default'),
      'field types' => array('imagemap'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function imagemapfield_field_widget_form(&$form, $form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element = array();
  $element['value'] = array(
    '#type' => 'textarea',
    '#label' => t('Area code'),
    '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
    '#attributes' => array(
      'style' => 'font-family:monospace;white-space:pre;',
    ),
  );

  $element['attach_to'] = array(
    '#type' => 'select',
    '#label' => t('Attach To'),
    '#options' => drupal_map_assoc(array('one', 'two', 'three')),
    '#default_value' => isset($items[$delta]['attach_to']) ? $items[$delta]['attach_to'] : NULL,
  );

  return $element;
}

/**
 * Implements hook_field_widget_error().
 */
function imagemapfield_field_widget_error($element, $error, $form, &$form_state) {}

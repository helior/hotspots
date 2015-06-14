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
 * Implements hook_field_is_empty().
 */
function imagemapfield_field_is_empty($item, $field) {
  if (!isset($item['coordinates']) || $item['coordinates'] === '') {
    return !isset($item['associate_image_field']) || $item['associate_image_field'] === '';
  }
  return FALSE;
}

/**
 * Implements hook_field_validate().
 */
function imagemapfield_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    // TODO: validate href
  }
}

/**
 * Implements hook_field_insert().
 */
function imagemapfield_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $value) {
    $items[$delta]['attributes'] = serialize($value['attributes']);
  }
}

/**
 * Implements hook_field_update().
 */
function imagemapfield_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $value) {
    $items[$delta]['attributes'] = serialize($value['attributes']);
  }
}

/**
 * Implements hook_field_load().
 */
function imagemapfield_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {

      $items[$id][$delta]['attributes'] = unserialize($item['attributes']);
    }
  }
}

/**
 * Implements hook_field_widget_info().
 */
function imagemapfield_field_widget_info() {
  return array(
    'imagemapfield_default' => array(
      'label' => t('Default'),
      'field types' => array('imagemap'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function imagemapfield_field_widget_form(&$form, $form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element += array('#type' => 'fieldset');

  $associate_image_field_options = array();
  foreach (array_intersect_key(field_info_instances($element['#entity_type'], $element['#bundle']), imagemapfield_image_field_map()) as $image_instance) {
    $associate_image_field_options[$image_instance['field_name']] = $image_instance['label'];
  }

  $element['associate_image_field'] = array(
    '#type' => 'select',
    '#title' => t('Associated image field'),
    '#options' => $associate_image_field_options,
    '#default_value' => isset($items[$delta]['associate_image_field']) ? $items[$delta]['associate_image_field'] : NULL,
  );

  $element['associate_image_delta'] = array(
    '#type' => 'select',
    '#title' => t('Associated image field delta'),
    '#options' => drupal_map_assoc(range(0, 20)),
    '#default_value' => isset($items[$delta]['associate_image_delta']) ? $items[$delta]['associate_image_delta'] : NULL,
  );

  $element['coordinates'] = array(
    '#type' => 'textarea',
    '#resizable' => FALSE,
    '#rows' => 2,
    '#title' => t('Coordinates'),
    '#default_value' => isset($items[$delta]['coordinates']) ? $items[$delta]['coordinates'] : NULL,
  );

  $element['shape'] = array(
    '#type' => 'select',
    '#title' => t('Shape'),
    '#options' => array('default' => t('Default'), 'circle' => t('Circle'), 'poly' => t('Polygon'), 'rect' => t('Rectangle')),
    '#default_value' => isset($items[$delta]['shape']) ? $items[$delta]['shape'] : NULL,
  );

  $element['href'] = array(
    '#type' => 'textfield',
    '#maxlength' => 2048,
    '#title' => t('Href'),
    '#default_value' => isset($items[$delta]['href']) ? $items[$delta]['href'] : NULL,
  );

  $element['attributes'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Attributes'),
    // '#tree' => TRUE,
  );

  $element['attributes']['class'] = array(
    '#type' => 'textfield',
    '#title' => 'Class',
    '#default_value' => isset($items[$delta]['attributes']['class']) ? $items[$delta]['attributes']['class'] : NULL,
  );

  $element['attributes']['rel'] = array(
    '#type' => 'textfield',
    '#title' => 'Rel',
    '#default_value' => isset($items[$delta]['attributes']['rel']) ? $items[$delta]['attributes']['rel'] : NULL,
  );

  $element['attributes']['alt'] = array(
    '#type' => 'textfield',
    '#title' => 'Alt',
    '#default_value' => isset($items[$delta]['attributes']['alt']) ? $items[$delta]['attributes']['alt'] : NULL,
  );

  $element['attributes']['target'] = array(
    '#type' => 'select',
    '#title' => 'target',
    '#options' => drupal_map_assoc(array('_self', '_blank', '_parent', '_top')),
    '#default_value' => isset($items[$delta]['attributes']['target']) ? $items[$delta]['attributes']['target'] : NULL,
  );

  return $element;
}

/**
 * Implements hook_field_widget_error().
 */
function imagemapfield_field_widget_error($element, $error, $form, &$form_state) {}

/**
 * Implementation of hook_field_formatter_info().
 */
function imagemapfield_field_formatter_info() {
  return array(
    'imagemap_default' => array(
      'label' => t('Image map element'),
      'description' => t('Injects the map coordinates into the HTML.'),
      'field types' => array('imagemap'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function imagemapfield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  foreach ($items as $delta => $item) {
    $id = drupal_html_id('imagemapfield');
    $element[$delta] = array(
      '#id' => $id,
      '#attributes' => array('name' => $id),
      '#type' => 'html_tag',
      '#tag' => 'map',
      '#value' => $item['value'],
    );
  }

  return $element;
}

/**
 * Implementation of hook_field_formatter_prepare_view().
*/
function imagemapfield_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  if ($field['type'] != 'imagemap') {
    return;
  }
}

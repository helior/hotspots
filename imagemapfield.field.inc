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
    $element[$delta] = array('#markup' => $item['value']);
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

/**
 * Implements hook_field_attach_view_alter().
 */
function imagemapfield_field_attach_view_alter(&$output, $context) {
  $x = 'x';
  $fields = array_filter(field_info_field_map(), '_imagemapfield_array_filter_fields');

  foreach ($fields as $field_name => $field_map) {
    // Drop out of loop if current entity does not match the entities the field is attached to.
    if (!in_array(array_keys($context['entity_type'], $field_map['bundles']))) {
      continue;
    }

    // Drop out if the field is not attached to this bundle.
    if (!isset($context['entity']->{$field_name})) {
      continue;
    }

    $lang = $context['entity']->language;

    foreach ($context['entity']->{$field_name}[$lang] as $delta => $field_value) {
      // Drop out if the image we're attaching to doesn't exist. Any other delta
      // here is just as lost.
      if (!isset($output[$field_value['attach_to']])) {
        break;
      }

      $identifier = $output[$field_name][$delta]['#id'];
    }
  }
  // Get field names of imagemap type;
  // Get field instance config of those fields.
  // iterate over instance config, match context with entity type & field on entity
  // iterate over deltas of imagemap field
  // search for "attach_to" field on entity.
  // If exists, get ID attribute of current imagemap, attach to the matching delta of image attributes on $output array.
}

function _imagemapfield_array_filter_fields($field) {
  return $field['type'] == 'imagemap';
}

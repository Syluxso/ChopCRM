<?php
module_load_include('inc', 'nytech_manager', 'nytech_manager.forms');

function screen_id_endpoints_list($entity_id) {
  $entity = entity_load_single('org', $entity_id);
  drupal_set_title($entity->title . ' endpoints');

  $view = views_embed_view('endpoints', 'default', $entity_id);
  $new = l('+ Endpoint', 'admin/structure/entity-type/endpoint/standard/add', [
    'query' => [
      'destination' => current_path(),
      'field_related_org' => $entity_id
    ],
    'attributes' => ['class' => ['btn', 'btn-sm', 'btn-success']],
  ]) . '<br />';
  return $new . $view;
}


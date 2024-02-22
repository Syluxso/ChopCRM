<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function screen_org_view($entity_id) {
  $entity = entity_load_single('organization', $entity_id);
  drupal_set_title($entity->title);

  $array = [
    'title' => $entity->title,
    'entity_id' => $entity->id,
  ];
  $form = drupal_get_form('nytech_org_update_form', $entity_id);
  return theme('nytech_org_page', $array) . drupal_render($form);
}

function screen_org_locations($entity_id) {
  $entity = entity_load_single('organization', $entity_id);
  drupal_set_title($entity->title . '\' Locations');

  $array = [
    'title' => $entity->title . ' Locations',
    'entity_id' => $entity->id,
  ];
  $output = l('+ Location', 'o/' . $entity_id . '/locations/new') . '<br /><br />';
  $locations = new NyTechOrgLocations($entity_id);
  $locations->go();
  $locations->output();
  $output .= '<div class="list-group">' . $locations->output . '</div>';
  return theme('nytech_org_page', $array) . $output;
}

function screen_org_assets($entity_id) {
  $entity = entity_load_single('organization', $entity_id);
  drupal_set_title($entity->title . '\' Assets');

  $array = [
    'title' => $entity->title . ' Assets',
    'entity_id' => $entity->id,
  ];
  $output = l('+ Asset', 'o/' . $entity_id . '/assets/new') . '<br /><br />';
  $assets = new NyTechOrgAssets($entity_id);
  $assets->go();
  $assets->output();
  $output .= '<div class="list-group">' . $assets->output . '</div>';
  return theme('nytech_org_page', $array) . $output;
}

function screen_org_members($entity_id) {
  $entity = entity_load_single('organization', $entity_id);
  drupal_set_title($entity->title . '\' Members');

  $array = [
    'title' => $entity->title . ' Members',
    'entity_id' => $entity->id,
  ];
  $members = new NyTechOrgMembers($entity);
  $members->go();
  $members->output();
  if(!empty($_GET['message'])) {
    drupal_set_message($_GET['message']);
  }
  $new_member = l('+ Manager', current_path(), ['attributes' => ['class' =>['btn', 'btn-sm', 'btn-link']], 'query' => ['message' => 'Adding a manager is only an example at this time.']]);
  $new_member .= l('+ Member', current_path(), ['attributes' => ['class' =>['btn', 'btn-sm', 'btn-link']], 'query' => ['message' => 'Adding a member is only an example at this time.']]);
  return theme('nytech_org_page', $array) . $new_member . '<br /><br />' . $members->output;
}

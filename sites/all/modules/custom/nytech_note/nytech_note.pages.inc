<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function api_note() {
  if(user_is_logged_in()) {
    $orgs = new NyTechUsersOrgs();
    $orgs->user();
    $orgs->go();
    if(!empty($orgs->objects)) {
      foreach($orgs->list as $item) {
        $org_id = $item['entity_id'];
        $destination = 'o/' . $org_id . '/notes';
      }
    }
  } else {
    $destination = 'login';
  }
  drupal_goto($destination);
}

function screen_o_notes($entity_id) {
  global $base_url;
  $notes = new NyTechOrgNotes($entity_id);
  $notes->query();
  drupal_set_title('(' . $notes->count . ') Notes');
  $output = '';

  $job_ids = [];
  if(!empty($notes->objects)) {
    foreach($notes->objects as $entity) {
      $job_id = $entity->field_n_related_job['und'][0]['target_id'];
      $job_ids[$job_id] = $job_id;
    }
  }
  $jobs = entity_load('job', $job_ids);
  if(!empty($notes->objects)) {
    foreach($notes->objects as $entity) {
      $note = new NyTechNote($entity);
      $array = [
        'url' => $base_url . '/' . drupal_get_path_alias('o/' . $entity_id . '/jobs/' . $note->job_id . '/notes/' . $entity->id . '/view'),
        'org_id' => $note->org_id,
        'job_id' => $note->job_id,
        'short' => shorten($entity->title, 140),
        'time_ago' => timeAgo($entity->created),
        'entity' => $entity,
        'job_title' => $jobs[$note->job_id]->title,
      ];
      $output .= theme('nytech_note_listed', $array);
    }
  } else {
    $output = '<div class="no-items-found"><h3 class="card-body">No notes yet.</div>';
  }
  return '<div class="list-group">' . $output . '</div>';
}

function screen_o_job_note_new($org_id, $job_id) {
  $form = drupal_get_form('nytech_note_new_form', $org_id, $job_id);
  $array = [
    'content' => drupal_render($form),
  ];
  return theme('nytech_modal_fake', $array);
}

function screen_o_job_notes($org_id, $job_id) {
  $entity = entity_load_single('job', $job_id);
  $job = new NyTechJob($entity);
  $entities = new NyTechJobEntities($org_id, $job_id, 'note', 'n');
  $entities->query();
  $notesOutput = new NyTechNotesListed($org_id, $job_id, $entities->objects);
  drupal_set_title($job->title . ' notes');
  $array = [
    'org_id' => $org_id,
    'job_id' => $job_id,
    'j' => $job,
    'content' => $notesOutput->output,
    'new' => l('+ Note', 'o/' . $org_id . '/jobs/' . $job_id . '/notes/new', ['attributes' => ['class' => ['btn', 'btn-sm', 'btn-primary', 'pull-right']]]),
  ];
  return theme('nytech_job_view', $array);
}

function screen_o_job_note_view($org_id, $job_id, $entity_id) {
  $entity = entity_load_single('note', $entity_id);
  $job = entity_load_single('job', $job_id);
  $note = new NyTechNote($entity);
  drupal_set_title('Note');

  /*
   * nytech_note_view
   */
  $array = [
    'n' => $note,
  ];
  $content = theme('nytech_note_view', $array);

  /*
   * nytech_modal_fake
   */
  $bottom = '
    <br />
    <div class="row">
      <div class="col-xs-6">
        ' . l('<i class="fa fa-times-circle"></i> Archive', 'api/v1/notes/' . $entity_id . '/delete', ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-danger']]]) . '
      </div>
      <div class="col-xs-6 text-right">
        ' . l('<i class="fa fa-pencil"></i>', 'o/' . $org_id . '/jobs/' . $job_id . '/notes/' . $entity_id . '/update', ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]) . '
      </div>
    </div>';
  $array = [
    'content' => $content,
    'bottom' => $bottom,
  ];
  $modal = theme('nytech_modal_fake', $array);

  /*
   * nytech_item_simple
   */
  $array = [
    'back' => l('<i class="fa fa-chevron-left"></i> Back to ' . $job->title, 'o/' . $org_id . '/jobs/' . $job_id . '/notes', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function screen_o_job_note_update($org_id, $job_id, $entity_id) {
  $entity = entity_load_single('note', $entity_id);
  $note = new NyTechNote($entity);
  drupal_set_title('Note');

  $form = drupal_get_form('nytech_note_update_form', $org_id, $job_id, $entity_id);
  /*
   * nytech_modal_fake
   */
  $array = [
    'content' => drupal_render($form),
  ];
  $modal = theme('nytech_modal_fake', $array);

  /*
   * nytech_item_simple
   */
  $array = [
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'o/' . $org_id . '/jobs/' . $job_id . '/notes', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function screen_o_note_notes($org_id, $entity_id) {
  $entity = entity_load_single('note', $entity_id);
  $job = new NyTechJob($entity);
  drupal_set_title($job->title);
  $array = [
    'n' => $job,
    'org_id' => $org_id,
    'content' => '',
  ];
  return theme('nytech_note_view', $array);
}

function api_note_delete($entity_id) {
  $entity = entity_load_single('note', $entity_id);
  $entity->status = 0;
  entity_save('note', $entity);
  drupal_set_message('Note archived');
  drupal_goto('o/' . $entity->field_n_related_org['und'][0]['target_id'] . '/jobs/' . $entity->field_n_related_job['und'][0]['target_id'] . '/notes');
}

function screen_notes_global_new() {
  if(!user_is_logged_in()) {
    drupal_goto('login', ['query' => ['destination' => current_path()]]);
  }
  global $user;
  $orgs = new NyTechUsersOrgs($user);
  $orgs->user($user);
  $orgs->go();
  $org_array = array_keys($orgs->orgs);
  $org_id = $org_array[0];
  $jobs = new NyTechOrgJobs($org_id);
  $jobs->query();
  $output = '';
  foreach($jobs->objects as $job) {
    $output .= l($job->title, 'o/' . $jobs->org_id . '/jobs/' . $job->id . '/notes/new', ['attributes' => ['class' => ['btn', 'btn-lg', 'btn-block', 'btn-primary']]]);
  }

  /*
   * nytech_modal_fake
   */
  $array = [
    'content' => '<h3 class="text-center">Add a note to</h3>' . $output,
  ];
  $modal = theme('nytech_modal_fake', $array);

  /*
   * nytech_item_simple
   */
  $array = [
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'notes', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);

}

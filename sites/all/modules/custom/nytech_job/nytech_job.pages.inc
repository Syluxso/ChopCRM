<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function api_jobs() {
  if(user_is_logged_in()) {
    $orgs = new NyTechUsersOrgs();
    $orgs->user();
    $orgs->go();
    if(!empty($orgs->objects)) {
      foreach($orgs->list as $item) {
        $org_id = $item['entity_id'];
        $destination = 'o/' . $org_id . '/jobs';
      }
    }
  } else {
    $destination = 'login';
  }
  drupal_goto($destination);
}
function screen_o_jobs($entity_id) {
  $jobs = new NyTechOrgJobs($entity_id);
  $jobs->query();
  drupal_set_title('(' . $jobs->count . ') Jobs');
  $output = '';
  if(!empty($jobs->objects)) {
    foreach($jobs->objects as $job) {
      $j = new NyTechJob($job);
      $array = [
        'j' => $j,
      ];
      $output .= theme('nytech_job_listed', $array);
    }
  } else {
    $output = '<div class="no-items-found"><h3 class="card-body">No jobs yet. ' . l('Create one', 'o/' . $entity_id . '/jobs/new', ['attributes' => ['class' => ['btn', 'btn-sm', 'btn-success']]]) . '</div>';
  }
  return '<div class="list-group">' . $output . '</div>';
}

function screen_o_job_new($entity_id) {
  $form = drupal_get_form('nytech_job_new_form', $entity_id);
  $array = [
    'content' => drupal_render($form),
  ];
  return theme('nytech_modal_fake', $array);
}

function screen_o_job_update($org_id, $job_id) {
  $form = drupal_get_form('nytech_job_update_form', $org_id, $job_id);
  $array = [
    'content' => drupal_render($form),
  ];
  return theme('nytech_modal_fake', $array);
}

function screen_o_job_view($org_id, $entity_id) {
  $entity = entity_load_single('job', $entity_id);
  $job = new NyTechJob($entity);
  drupal_set_title($job->title);

  $images = new NyTechJobImages($org_id, $entity_id);
  $images->query(6);
  $imagesOutput = new NyTechImagesListed($org_id, $entity_id, $images->objects);

  $receipts = new NyTechJobReceipts($org_id, $entity_id);
  $receipts->query(3);
  $receiptOutput = new NyTechReceiptsListed($org_id, $entity_id, $receipts->objects);

  $notes = new NyTechJobNotes($org_id, $entity_id);
  $notes->query(3);
  $notesOutput = new NyTechNotesListed($org_id, $entity_id, $notes->objects);


  $array = [
    'j' => $job,
    'org_id' => $org_id,
    'content' => '
      <h6 class="text-muted">DOCUMENTS</h6>
      <h6 class="text-muted">IMAGES</h6>' . $imagesOutput->output . '
      <h6 class="text-muted">RECEIPTS</h6>' . $receiptOutput->output . '
      <h6 class="text-muted">NOTES</h6>' . $notesOutput->output . '
    ',
  ];
  return theme('nytech_job_view', $array);
}

function api_o_job_archive($entity_id) {
  $entity = entity_load_single('job', $entity_id);
  $entity->status = 0;
  entity_save('job', $entity);
  drupal_set_message('Job archived');
  drupal_goto('jobs');
}

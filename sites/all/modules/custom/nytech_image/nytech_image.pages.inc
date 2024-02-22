<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function api_image() {
  if(user_is_logged_in()) {
    $orgs = new NyTechUsersOrgs();
    $orgs->user();
    $orgs->go();
    if(!empty($orgs->objects)) {
      foreach($orgs->list as $item) {
        $org_id = $item['entity_id'];
        $destination = 'o/' . $org_id . '/images';
      }
    }
  } else {
    $destination = 'login';
  }
  drupal_goto($destination);
}

function screen_o_images($entity_id) {
  global $base_url;
  $images = new NyTechOrgImages($entity_id);
  $images->query();
  drupal_set_title('(' . $images->count . ') Image');
  $output = '';

  $job_ids = [];
  if(!empty($images->objects)) {
    foreach($images->objects as $entity) {
      $job_id = $entity->field_img_related_job['und'][0]['target_id'];
      $job_ids[$job_id] = $job_id;
    }
  }
  $jobs = entity_load('job', $job_ids);
  if(!empty($images->objects)) {
    foreach($images->objects as $entity) {
      $image = new NyTechImage($entity);
      $array = [
        'url' => $base_url . '/' . drupal_get_path_alias('o/' . $entity_id . '/jobs/' . $image->job_id . '/images/' . $entity->id . '/view'),
        'org_id' => $image->org_id,
        'job_id' => $image->job_id,
        'date' => date('M d Y, g:ia', $entity->created),
        'entity' => $entity,
        'job_title' => $jobs[$image->job_id]->title,
        'image' => $image->image_output_thumbnail,
        'memo' => shorten($image->memo),
      ];
      $output .= theme('nytech_image_listed', $array);
    }
  } else {
    $output = '<div class="no-items-found"><h3 class="card-body">No images yet.</div>';
  }
  return '<div class="row">' . $output . '</div>';
}

function screen_o_job_image_new($org_id, $job_id) {
  $form = drupal_get_form('nytech_image_image_new_form', $org_id, $job_id);
  $array = [
    'content' => drupal_render($form),
  ];
  return theme('nytech_modal_fake', $array);
}

function screen_o_job_images($org_id, $job_id) {
  $entity = entity_load_single('job', $job_id);
  $job = new NyTechJob($entity);
  $entities = new NyTechJobEntities($org_id, $job_id, 'image', 'img');
  $entities->query();
  $outputClass = new NyTechImagesListed($org_id, $job_id, $entities->objects);
  drupal_set_title($job->title . ' images');
  $array = [
    'org_id' => $org_id,
    'job_id' => $job_id,
    'j' => $job,
    'content' => $outputClass->output,
    'new' => l('+ Image', 'o/' . $org_id . '/jobs/' . $job_id . '/images/new', ['attributes' => ['class' => ['btn', 'btn-sm', 'btn-primary', 'pull-right']]]),
  ];
  return theme('nytech_job_view', $array);
}

function screen_o_job_image_view($org_id, $job_id, $entity_id) {
  $entity = entity_load_single('image', $entity_id);
  $job = entity_load_single('job', $job_id);
  $image = new NyTechImage($entity);
  drupal_set_title('Image');

  /*
   * nytech_image_view
   */
  $array = [
    'img' => $image,
  ];
  $content = theme('nytech_image_view', $array);

  /*
   * nytech_modal_fake
   */
  $bottom = '
    <br />
    <div class="row">
      <div class="col-xs-6">
        ' . l('<i class="fa fa-times-circle"></i> Archive', 'api/v1/images/' . $entity_id . '/delete', ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-link']]]) . '
      </div>
      <div class="col-xs-6 text-right">
        ' . l('<i class="fa fa-pencil"></i>', 'o/' . $org_id . '/jobs/' . $job_id . '/images/' . $entity_id . '/update', ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]) . '
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
    'back' => l('<i class="fa fa-chevron-left"></i> Back to ' . $job->title, 'o/' . $org_id . '/jobs/' . $job_id . '/images', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function screen_o_job_image_update($org_id, $job_id, $entity_id) {
  $entity = entity_load_single('image', $entity_id);
  $image = new NyTechImage($entity);
  drupal_set_title('Image');

  $form = drupal_get_form('nytech_image_image_update_form', $org_id, $job_id, $entity_id);
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
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'o/' . $org_id . '/jobs/' . $job_id . '/images/' . $entity_id . '/view', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function screen_o_image_images($org_id, $entity_id) {
  $entity = entity_load_single('image', $entity_id);
  $job = new NyTechJob($entity);
  drupal_set_title($job->title);
  $array = [
    'n' => $job,
    'org_id' => $org_id,
    'content' => '',
  ];
  return theme('nytech_image_view', $array);
}

function api_image_delete($entity_id) {
  $entity = entity_load_single('image', $entity_id);
  $entity->status = 0;
  entity_save('image', $entity);
  drupal_set_message('Image archived');
  drupal_goto('o/' . $entity->field_img_related_org['und'][0]['target_id'] . '/jobs/' . $entity->field_img_related_job['und'][0]['target_id'] . '/images');
}

function screen_image_global_new() {
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
    $output .= l($job->title, 'o/' . $jobs->org_id . '/jobs/' . $job->id . '/images/new', ['attributes' => ['class' => ['btn', 'btn-lg', 'btn-block', 'btn-primary']]]);
  }

  /*
   * nytech_modal_fake
   */
  $array = [
    'content' => '<h3 class="text-center">Add a image to</h3>' . $output,
  ];
  $modal = theme('nytech_modal_fake', $array);

  /*
   * nytech_item_simple
   */
  $array = [
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'images', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);

}

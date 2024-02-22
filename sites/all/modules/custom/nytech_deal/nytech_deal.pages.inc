<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function api_deal() {
  $destination = 'login';
  if(user_is_logged_in()) {
    $destination = 'deals';
  }
  drupal_goto($destination);
}

function screen_deals() {
  global $user;
  $deals = new NyTechUsersDeals($user);
  $deals->query();
  drupal_set_title('(' . $deals->count . ') Deals');
  $output = '';

  if(!empty($deals->objects)) {
    foreach($deals->objects as $entity) {
      $deal = new NyTechDeal($entity);
      $array = [
        'url' => $deal->view_url,
        'eClass' => $deal,
      ];
      $output .= theme('nytech_deal_listed', $array);
    }
  } else {
    $output = '<div class="no-items-found"><h3 class="card-body">No deals.</div>';
  }

  $head = '
  <div class="list-group-item">
    <div class="row">
      <div class="col-xs-3 col-sm-3 col-md-2">
        <h6><span class="text-muted">STATE</span></h6>
      </div>
      <div class="col-xs-6 col-sm-6 col-md-6">
        <h6><span class="text-muted">DEAL</span></h6>
      </div>
      <div class="col-xs-3 col-sm-3 col-md-4 text-right">
        <h6><span class="text-muted">VALUE</span></h6>
      </div>
    </div>
  </div>
  ';
  $footer = '
  <div class="list-group-item">
    <div class="row">
      <div class="col-xs-3 col-sm-3 col-md-2">
        <h6><span class="text-muted">TOTAL</span></h6>
      </div>
      <div class="col-xs-6 col-sm-6 col-md-6">
        <h6><span class="text-muted"></span></h6>
      </div>
      <div class="col-xs-3 col-sm-3 col-md-4 text-right">
        <h6><span class="text-muted">' . $deals->total_value_pretty . '</span></h6>
      </div>
    </div>
  </div>
  ';
  return deal_tabs() . '<br /><br /><div class="list-group">' . $head . $output . $footer . '</div>';
}

function screen_deal_by_state($state) {
  global $user;
  $state = ucfirst($state);
  $deals = new NyTechUsersDeals($user);
  $deals->set_state($state);
  $deals->query();
  drupal_set_title('(' . $deals->count . ') ' . $state . ' Deals');
  $output = '';

  if(!empty($deals->objects)) {
    foreach($deals->objects as $entity) {
      $deal = new NyTechDeal($entity);
      $array = [
        'url' => $deal->view_url,
        'eClass' => $deal,
      ];
      $output .= theme('nytech_deal_listed', $array);
    }
  } else {
    $output = '<div class="no-items-found"><h3 class="card-body">No deals.</div>';
  }

  $head = '
  <div class="list-group-item">
    <div class="row">
      <div class="col-xs-3 col-sm-3 col-md-2">
        <h6><span class="text-muted">STATE</span></h6>
      </div>
      <div class="col-xs-6 col-sm-6 col-md-6">
        <h6><span class="text-muted">DEAL</span></h6>
      </div>
      <div class="col-xs-3 col-sm-3 col-md-4 text-right">
        <h6><span class="text-muted">VALUE</span></h6>
      </div>
    </div>
  </div>
  ';
  $footer = '
  <div class="list-group-item">
    <div class="row">
      <div class="col-xs-3 col-sm-3 col-md-2">
        <h6><span class="text-muted">TOTAL</span></h6>
      </div>
      <div class="col-xs-6 col-sm-6 col-md-6">
        <h6><span class="text-muted"></span></h6>
      </div>
      <div class="col-xs-3 col-sm-3 col-md-4 text-right">
        <h6><span class="text-muted">' . $deals->total_value_pretty . '</span></h6>
      </div>
    </div>
  </div>
  ';
  return deal_tabs() . '<br /><br /><div class="list-group">' . $head . $output . $footer . '</div>';
}

function screen_deal_new() {
  $form = drupal_get_form('nytech_deal_deal_new_form');
  $array = [
    'content' => drupal_render($form),
  ];
  return theme('nytech_item_simple_small', $array);
}

function screen_deal_view($entity_id) {
  $entity = entity_load_single('deal', $entity_id);
  $deal = new NyTechDeal($entity);
  $deal->author($entity);
  drupal_set_title($deal->title . ' deal');

  /*
   * nytech_deal_view
   */
  $array = [
    'eClass' => $deal,
  ];
  $content = theme('nytech_deal_view', $array);

  /*
   * nytech_modal_fake
   */
  $bottom = '
    <br />
    <div class="row">
      <div class="col-xs-6">
        ' . l('<i class="fa fa-times-circle"></i> Archive', $deal->archive_url, ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-link']]]) . '
      </div>
      <div class="col-xs-6 text-right">
        ' . l('<i class="fa fa-pencil"></i>', $deal->update_url, ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]) . '
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
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'deals', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function screen_deal_update($entity_id) {
  $entity = entity_load_single('deal', $entity_id);
  $deal = new NyTechDeal($entity);
  drupal_set_title('Deal');

  $form = drupal_get_form('nytech_deal_deal_update_form', $deal);
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
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'deals/' . $entity_id . '/view', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function api_deal_archive($entity_id) {
  $entity = entity_load_single('deal', $entity_id);
  $entity->status = 0;
  entity_save('deal', $entity);
  drupal_set_message('Deal archived');
  drupal_goto('deals');
}

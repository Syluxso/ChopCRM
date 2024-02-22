<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function api_todo() {
  if(user_is_logged_in()) {
    $orgs = new NyTechUsersOrgs();
    $orgs->user();
    $orgs->go();
    if(!empty($orgs->objects)) {
      foreach($orgs->list as $item) {
        $org_id = $item['entity_id'];
        $destination = 't/todos';
      }
    }
  } else {
    $destination = 'login';
  }
  drupal_goto($destination);
}

function screen_todos() {
  $todos = new NyTechUsersTodos();
  $todos->query();
  drupal_set_title('(' . $todos->count . ') Todos');
  $output = '';
  if(!empty($todos->objects)) {
    foreach($todos->objects as $entity) {
      $todo = new NyTechTodo($entity);
      $array = [
        'eClass' => $todo,
      ];
      $output .= theme('nytech_todo_listed', $array);
    }
  } else {
    $output = '<div class="no-items-found"><h3 class="card-body">No todos.</div>';
  }
  return '<div class="list-group">' . $output . '</div>';
}

function screen_todo_new() {
  global $user;
  dumpk($user);
  $form = drupal_get_form('nytech_note_note_new_form');
  $array = [
    'content' => drupal_render($form),
  ];
  return theme('nytech_modal_fake', $array);
}

function screen_todo_view($entity_id) {
  $entity = entity_load_single('todo', $entity_id);
  $todo = new NyTechTodo($entity);
  drupal_set_title($todo->title);

  /*
   * nytech_todo_view
   */
  $array = [
    'eClass' => $todo,
  ];
  $content = theme('nytech_todo_view', $array);

  /*
   * nytech_modal_fake
   */
  $bottom = '
    <br />
    <div class="row">
      <div class="col-xs-6">
        ' . l('<i class="fa fa-times-circle"></i> Archive', $todo->archive_url, ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-danger']]]) . '
      </div>
      <div class="col-xs-6 text-right">
        ' . l('<i class="fa fa-pencil"></i>', $todo->update_url, ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]) . '
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
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'todos', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function screen_todo_update($entity_id) {
  $entity = entity_load_single('todo', $entity_id);
  $todo = new NyTechTodo($entity);
  drupal_set_title($todo->title);

  $form = drupal_get_form('nytech_todo_update_form', $entity_id);
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
    'back' => l('<i class="fa fa-chevron-left"></i> Back', 'todos', ['html' => true]),
    'content' => $modal,
  ];
  return theme('nytech_item_simple', $array);
}

function api_todo_archive($entity_id) {
  $entity = entity_load_single('todo', $entity_id);
  $entity->status = 0;
  entity_save('todo', $entity);
  drupal_set_message('Todo archived');
  drupal_goto('todos');
}


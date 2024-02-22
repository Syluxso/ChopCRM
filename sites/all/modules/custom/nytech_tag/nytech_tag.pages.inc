<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');

function screen_admin_tags_new() {
  $form_id = 'nytech_tag_admin_tag_new_form';
  $form = drupal_get_form($form_id);
  new NyTechTagConfigCreate('todo_states');
  return drupal_render($form);
}


/*
 * API Routs
 */

function api_tags_filter_tag_new($filter) {
  if(!empty($filter) && !empty($_GET['title'])) {
    $tag = new NyTechSubmitTag($filter, $_GET['title']);
    $tag = new NyTechTag($tag->entity);
    $result = new stdClass();
    $result->status = 200;
    $result->message = 'OK';
    $result->data = [
      'tag' => $tag,
    ];
  } else {
    $result = new stdClass();
    $result->data = [
      'message' => 'There was an issue processing this result',
    ];
  }

  return $result;

}

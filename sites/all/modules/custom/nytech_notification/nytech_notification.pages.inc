<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function screen_my_notifications() {
  $userObject = get_account();
  $notifications = new NyTechUserNotifications($userObject);
  $notifications->query();
  drupal_set_title('(' . $notifications->count . ') Notifications');
  $output = '';
  if(!empty($notifications->objects)) {
    foreach($notifications->objects as $notification) {
      $n = new NyTechNotification($notification);
      $array = [
        'n' => $n,
        'user' => $userObject,
      ];
      $output .= theme('nytech_notification_listed', $array);
    }
  } else {
    $output = '<div class="no-items-found"><h3 class="card-body">No notifications. You\'r all set!</div>';
  }
  return '<div class="list-group">' . $output . '</div>';
}

function api_notifications_test_new() {
  $edit = [
    'type' => 'standard',
    'title' => 'Example Notification ' . rand(1000,9999),
    'status' => 1,
    'level' => 0,
    'uid' => 1,
  ];
  $entity = entity_create('notification', $edit);
  $entity->field_note_message['und'][0]['value'] = 'This is the message default body. There is not much to see here. You can say or do what you like and even ad html: <a href="#">Link</a>.';
  entity_save('notification', $entity);
  return new NyTechNotification($entity);
}

function api_notifications_delete($entity_id) {
  entity_delete('notification', $entity_id);
  drupal_set_message('Notification removed');
  drupal_goto('notifications');
}

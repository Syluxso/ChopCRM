<?php
function nytech_slate_preprocess_page(&$variables) {
  $base_path = base_path();
  $file_url = $base_path . drupal_get_path('theme', 'nytech_slate') . '/logo.png';
  $file_url_sm = $base_path . drupal_get_path('theme', 'nytech_slate') . '/logo.png';
  if(!user_is_logged_in()) {
    $variables['logo_lg'] = '<img src="' . $file_url . '" class="img-responsive" />';
    $variables['logo_sm'] = '<img src="' . $file_url_sm . '" class="img-responsive" />';
  } else {
    $path = current_path();
    if($path == 'account/new') {
      $variables['logo_lg'] = '<img src="' . $file_url . '" class="img-responsive" />';
      $variables['logo_sm'] = '<img src="' . $file_url_sm . '" class="img-responsive" />';
    }
  }
}

function nytech_slate_status_messages(array $variables) {
  $display = $variables['display'];
  $output = '';
  $html = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
    'info' => t('Informative message'),
  );

  // Map Drupal message types to their corresponding Bootstrap classes.
  // @see http://twitter.github.com/bootstrap/components.html#alerts
  $status_class = array(
    'status' => 'success',
    'error' => 'danger',
    'warning' => 'warning',
    // Not supported, but in theory a module could send any type of message.
    // @see drupal_set_message()
    // @see theme_status_messages()
    'info' => 'info',
  );

  // Retrieve messages.
  $message_list = drupal_get_messages($display);

  // Allow the disabled_messages module to filter the messages, if enabled.
  if (module_exists('disable_messages') && variable_get('disable_messages_enable', '1')) {
    $message_list = disable_messages_apply_filters($message_list);
  }

  foreach ($message_list as $type => $messages) {
    foreach($messages as $message) {
      drupal_add_js('jQuery(document).ready(function() {nytech_master_message(' . json_encode($message) . ', "' . $status_class[$type] . '")});', array(
        'type' => 'inline',
        'scope' => 'footer',
        'weight' => 5,
      ));
    }
  }
  return $html;
}

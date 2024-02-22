<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function api_a_router() {
  if(user_is_logged_in()) {
    $destination = 'account';
  } else {
    $destination = 'login';
  }
  drupal_goto($destination);
}

function screen_register() {
  // Forward users to
  auth_goto(URL_ACCOUNT);
  $form = drupal_get_form('nytech_user_register_form');
  return drupal_render($form);
}

function screen_login() {
  // Forward users to
  auth_goto(URL_ACCOUNT);
  $form = drupal_get_form('nytech_user_login_form');
  return drupal_render($form);
}

function screen_password() {
  auth_goto(URL_ACCOUNT);
  $form = drupal_get_form('nytech_user_password_form');
  return drupal_render($form);
}

function screen_account() {
  non_auth_goto(URL_LOGIN);
  global $user;
  $account = user_load($user->uid);
  $accountClass = new NyTechAccount($account);
  $array = [
    'name' => $accountClass->name,
    'email' => $accountClass->email,
    'phone' => $accountClass->phone_pretty,
    'user_id' => $accountClass->user_id,
  ];
  return theme('nytech_account', $array);
}

function screen_account_new() {
  // Forward users to
  non_auth_goto(URL_LOGIN);
  global $user;
  $account = user_load($user->uid);
  $member = new NyTechUserMemberships;
  $form = drupal_get_form('nytech_user_account_new_form', $account);
  return drupal_render($form);
}

function screen_account_edit() {
  non_auth_goto(URL_LOGIN);
  global $user;
  $account = user_load($user->uid);
  $accountClass = new NyTechAccount($account);
  $array = [
    'name' => $accountClass->name,
    'email' => $accountClass->email,
    'phone' => $accountClass->phone_pretty,
    'user_id' => $accountClass->user_id,
  ];
  $form = drupal_get_form('nytech_user_account_edit_form');
  return theme('nytech_account', $array) . drupal_render($form);
}

function screen_account_memberships() {
  non_auth_goto(URL_LOGIN);
  global $user;
  $account = user_load($user->uid);
  $accountClass = new NyTechAccount($account);
  $array = [
    'name' => $accountClass->name,
    'email' => $accountClass->email,
    'phone' => $accountClass->phone_pretty,
    'user_id' => $accountClass->user_id,
  ];
  $orgs = new NyTechUsersOrgs;
  $orgs->user();
  $orgs->go();
  $orgs->output();
  $output = '<div class="row">
    <div class="col-sm-6 text-left"><h4>Company</h4></div>
    <div class="col-sm-6 text-right"><h4>Role</h4></div>
  </div>';
  $output .= '<div class="list-group">' . $orgs->output . '</div>';
  return theme('nytech_account', $array) . $output;
}

function api_s_router() {
  if(user_is_logged_in()) {
    $destination = 'deals';
  } else {
    $destination = 'login';
  }
  drupal_goto($destination);
}

function screen_account_start() {

  $jobs = new NyTechAppIcon('deals', 'fa-zap', 'Deal', 'deals');
  $jobs->output();

  $app_array = [
    'r1c1' => $jobs->output,
    'r1c2' => '',
    'r1c3' => '',
    'r2c1' => '',
    'r2c2' => '',
    'r2c3' => '',
  ];
  $content = theme('nytech_user_apps', $app_array);
  $array = [
    'content' => $content,
  ];
  return theme('nytech_modal_fake', $array);
}

function screen_account_start_update() {
  $account = get_account();

  $array = [
    '1x1' => [
      'id' => 'account',
      'path' => 'account',
      'label' => 'Account',
      'icon' => 'fa-user',
    ],
  ];
  return theme('nytech_user_apps_edit', $array);
}


// Click on "HELP" above to see examples of handling input/debug/output
// Format the output as specified in the problem statement and write the result to stdout
// IMPORTANT: Remove all debug statements for your final submission


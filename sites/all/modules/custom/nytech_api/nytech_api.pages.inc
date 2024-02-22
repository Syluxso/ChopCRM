<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.callbacks');

/*
 * This is an example of how to use the nytech_api endpoint system.
 */
function endpoint_test($value) {
  $callback = new NyTechTest();
  $callback->add_property('path_var', $value);
  return (new Synt($callback));
}

function endpoint_account() {
  $callback = new NyTechResourceAccount;
  return (new Synt($callback));
}

function endpoint_notifications() {
  $callback = new NyTechResourceNotifications;
  return (new Synt($callback));
}

function endpoint_deals() {
  $args = [
    'methods'  => 'GET,POST',
    'callback' => 'ClassnameHere',
  ];
  $biz = new DealsDel($args);
  $callback = new Callback($iz);
  return (new Synt($callback));
}

function endpoint_register() {
  $callback = new NyTechResourceRegister();
  return (new Synt($callback));
}

function endpoint_login() {
  $callback = new NyTechResourceLogin();
  return (new Synt($callback));
}


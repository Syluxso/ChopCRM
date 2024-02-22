<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.forms');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');

function screen_reports_kpi_default() {
  global $user;
  if(in_array('founder', $user->roles)) {
    return 'Report';
  } else {
    return 'YOU SHALL NOT PASS!!!!!';
  }
}

function api_reports_kpi_default() {
  $users = new NyTechReportQuery('users', 'uid');
  $orgs = new NyTechReportQuery('eck_organization', 'id');
  $locations = new NyTechReportQuery('eck_location', 'id');
  $assets = new NyTechReportQuery('eck_asset', 'id');
  $moves = new NyTechReportQuery('eck_move', 'id');
  return [
    'users'     => $users->count,
    'orgs'      => $orgs->count,
    'locations' => $locations->count,
    'assets'    => $assets->count,
    'moves'     => $moves->count,
  ];
}

class NyTechReportQuery {
  public $count;
  function __construct($table, $col = 'id') {
    $query = 'SELECT e.' . $col . ' FROM {' . $table . '} e';
    $args = [];
    $q = db_query($query, $args);
    $result = [];
    foreach($q as $row) {
      if($table == 'users') {
        if($row->uid != 0) {
          $result[] = $row;
        }
      } else {
        $result[] = $row;
      }
    }
    $this->count = count($result);
  }
}

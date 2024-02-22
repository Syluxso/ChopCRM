<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.callbacks');

// TODO: Consider data extract and data processing in two steps.

/*
 * Get and format all inbound information
 * - Headers
 * - Body
 * - Method
 * - Post data
 * - Get data
 * - user agent
 * - Path arguments /api/v1/%/%/% etc.
 */
class NyTechRequestmeta {
  protected $get;
  protected $post;
  protected $session;
  protected $path;

  function __construct() {
    $this->server();
    $this->session();
    $this->get();
    $this->post();
    $this->path();
  }

  function server() {
    if(!empty($_SERVER)) {
      $this->server = $_SERVER;
    }
  }

  function session() {
    if(!empty($_SESSION)) {
      $this->session = $_SESSION;
    }
  }

  function get() {
    $get = $_GET;
    unset($get['q']);
    if(!empty($get)) {
      $this->get = $get;
    }
  }

  function post() {
    $post = $_POST;
    if(!empty($post)) {
      $this->post = $post;
    }
  }

  function path() {
    $this->path = current_path();
  }
}

class NyTechRequestMetaSynt extends NyTechRequestmeta {
  protected $get;
  protected $post;
  protected $agent;
  protected $session;
  protected $basic;
  protected $method;
  protected $synt = [];

  function __construct() {
    // Auto __construct() new NyTechRequestMeta that gathers all onbound data.
    $this->synt_authenticate();
    $this->synt_get();
    $this->synt_post();
    $this->synt_agent();
    $this->synt_method();
  }

  function synt_authenticate() {
    /*
     * Currently we are only looking for basic auth and session/sid auth data.
     */
    if(!empty($this->server['HTTP_AUTHORIZATION'])) {
      $base64 = substr($this->serve['HTTP_AUTHORIZATION'],6);
      $user_pass_string = base64_decode($base64);
      $user_pass = explode(':', $user_pass_string);
      $basic = new stdClass();
      $basic->user = $user_pass[0];
      $basic->pass = $user_pass[1];
      $this->basic = $basic;

    }
    if(!empty($this->serve['HTTP_SID'])) {
      $session = new stdClass();
      $session->sid = $this->serve['HTTP_SID'];
      $this->user_session = $session;
    }
    if(!empty($this->user_session)) {
      unset($this->basic);
    }
  }

  function synt_get() {
    $get = $_GET;
    unset($get['q']);
    if(!empty($get)) {
      $this->get = $get;
    }
  }

  function post() {
    $post = $_POST;
    if(!empty($post)) {
      $this->post = $post;
    }
  }

  function agent() {
    if(!empty($_SERVER['HTTP_USER_AGENT'])) {
      $this->agent = $_SERVER['HTTP_USER_AGENT'];
    }
  }

  function method() {
    $this->method = $_SERVER['REQUEST_METHOD'];
  }

  function add_synt($group, $key, $value) {
    $array = $this->synt;
    $array[$key] = $value;
    $this->synt = $array;
  }
}

/*
 * No-auth
 * Basic Auth
 * Session Auth
 */
// TODO: Refactor to protected, no more dynamic properties buddy!
class Auth {
  public $sid        = false;
  public $type       = false;
  public $user_id    = false;
  public $user       = false;
  public $authorized = false;

  function __construct($data) {
    // TODO: $this->authenticate(); // Validate identity.
    // TODO: $this->authorize(); // Validate access, method, etc.
    if($data->non_auth = true) {
      $this->non_auth();
    } elseif(!empty($data->session)) {
      $this->set_session($data);
    } elseif(!empty($data->basic)) {
      $this->set_basic($data);
    }
  }

  private function set_session($data) {
    if(!empty($data->session)) {
      $this->type = 'session';
      $session = new SessionAuth($data->session->sid);
      if($session->access == true) {
        $user = user_load($session->user_id);
        $this->set_user($user);
        $this->sid = $session->sid;
        $this->authorized = true;
      }
      unset($this->basic);
    }
  }

  private function set_basic($data) {
    if(!empty($data->basic)) {
      $this->type = 'basic';
      $basic = new BasicAuth($data->basic->user, $data->basic->pass);
      $this->sid = $basic->sid;
      $this->set_user($basic->user);
    }
  }

  private function set_user($user) {
    if($user != false) {
      $this->user = $user;
      $this->user_id = (int) $this->user->uid;
      $this->authorized = true;
    }
  }

  public function non_auth() {
    $this->type = 'non_auth';
    $this->authorized = true;
  }
}


/*
 * The formatter class for getting data back to the requester.
 * Properties include
 * - status: required
 * - code: required
 * - message: optional.
 * - data: required.
 * - errors: optional.
 */
class Synt {
  /*
   * The only goal of this class is to dynamically set the properties based on the xClass passed into it.
   */
  public $code = 200;
  public $message = 'Ok.';
  public $data = false;
  public $errors;

  private $xClass;

  function __construct($callbac) {
    $this->xClass = $callbac;
    $this->data($callbac);
    $this->status($callbac);
    $this->errors($callbac);
    $this->finish();
  }

  function status($callbac) {
    if(!empty($callbac->code) && !empty($callbac->message)) {
      $this->code = (int) $callbac->code;
      $this->message = $callbac->message;
    }
  }

  function data($callbac) {
    if(!empty($callbac->data)) {
      $this->data = $callbac->data;
    }
  }

  function errors($callbac) {
    if(!empty($callbac->errors)) {
      $this->errors = $callbac->errors;
      // Update status;
      foreach($callbac->errors as $error) {
        $this->code = $error->code;
        $this->message = $error->message;
      }
    } else {
      unset($this->errors);
    }
  }

  function finish() {
    drupal_add_http_header('Status', $this->code . ' ' . $this->message);
  }

  public function debug() {
    $this->debug = $this->xClass;
  }

}


/*
 * AUTHORIZATION
 */
class BasicAuth {

  public $access = false;
  public $user_id = false;
  public $user = false;
  public $sid = false;

  function __construct($user_name, $pass) { // On registration $user->name is synced to $user->mail.
    $id = user_authenticate($user_name, $pass);
    $this->set_user_id($id);

    if ($this->user_id > 0) {
      $account = array('uid' => $this->user_id);
      user_login_submit(array(), $account);
      global $user;
      $this->user = user_load($user->uid);
      $sid = session_id();
      $hashed_session_id = drupal_hash_base64($sid);
      $this->set_sid($hashed_session_id);
      $this->set_access(TRUE);
    } else {
      $this->user = false;
    }
  }

  private function set_user_id($id) {
    $this->user_id = (int) $id;
  }

  private function set_sid($sid) {
    $this->sid = $sid;
  }

  private function set_access($new_access) {
    $this->access = $new_access;
  }
}

/*
 * SessionAuth is the method to authenticate a user if they have a $sid that is valid and current.
 */
class SessionAuth {
  public $access = false;
  public $user_id = 0;
  public $sid = null;

  function __construct($sid) {
    if(!empty($sid)) {
      $query = 'SELECT s.uid FROM {sessions} s WHERE s.sid = :sid';
      $args = array(
        ':sid' => $sid,
      );
      $result = db_query($query, $args);
      foreach ($result as $record) {
        $this->set_access(TRUE);
        $this->set_user_id($record->uid);
        $this->set_sid($sid);
      }
    }
  }

  private function set_user_id($id) {
    $this->user_id = (int) $id;
  }

  private function set_sid($sid) {
    $this->sid = $sid;
  }

  private function set_access($new_access) {
    $this->access = $new_access;
  }
}

class AnonAuth {
  public $access = true;
  public $user_id = 0;
  public $sid = null;

  function __construct($auth) {
    if(!empty($auth->user_id) ) {
      $this->user_id = $auth->user_id;
    }
    if(!empty($auth->sid)) {
      $this->sid = $auth->sid;
    }
  }
}

class NyTechError extends NyTechStatus {
  public $code;
  public $message;
  public function __construct($code, $message) {
    $this->code = $code;
    $this->message = $message;
  }
}

class NyTechStatus {
  public $code;
  public $message;
  public function __construct($code, $message) {
    $this->code = $code;
    $this->message = $message;
  }

  public function status() {
    return [
      'type' => 'status',
      'data' => $this,
    ];
  }

  public function error() {
    return [
      'type' => 'error',
      'data' => $this,
    ];
  }
}

/*
 * All other classes in this file are to extend this class.
 */

class NyTechCallback {
  public $code = 200;
  public $message = 'OK';
  public $data;
  public $errors;

  public $authorized = false;
  public $account = false;

  public $request;
  public $auth;
  public $non_auth = false;
  public $callback = false;

  public $methods = 'GET';

  function __construct($callback, $args = false) {
    drupal_add_http_header('Access-Control-Allow-Origin', "*");
    $this->set_callback($callback);
    $this->variables($args);
    $this->request();
    $this->auth();
  }

  function callback($callback) {
    $this->callback = new $callback;
  }

  function variables($array) {
    if(is_array($array)) {
      foreach($array as $key => $value) {
        $this->add_property($key, $value);
      }
    }
  }

  function request() {
    $this->request = new RequestMeta();
  }

  function auth() {
    $non_auth = $this->non_auth;
    switch ($non_auth) {
      case 'non_auth':
        $non_auth = new stdClass();
        $non_auth->non_auth = true;
        $auth = new Auth($non_auth);
        $this->authorized = true;
        break;
      default:
        $auth = new Auth($this->request);
        $this->authorized = false;
        if($auth->authorized != true) {
          $this->add_error(403, 'Unauthorized');
        } elseif($auth->authorized == true) {
          $this->authorized = true;
        }
        if($this->authorized == true && !empty($auth->user)) {
          $this->account = new NyTechAccount($auth->user);
        }
        break;
    }
    $this->auth = $auth;
  }

  function check_methods() {
    $access = false;
    drupal_add_http_header('Access-Control-Allow-Methods', $this->methods);
    $array = explode(',', $this->methods);
    if(in_array($this->request->method, $array)) {
      $access = true;
    }
    if($access == false) {
      $this->add_error(405, 'Method Not Allowed');
    }
    return $access;
  }

  function add_property($key, $data) {
    $this->$key = $data;
  }

  function data($data) {
    if($this->authorized != false) {
      $this->data = $data;
    }
  }

  function status($code, $message) {
    $this->code = (int) $code;
    $this->message = $message;
  }

  function add_error($code, $ping) {
    $errors = [];
    if(!empty($this->errors)) {
      $errors = $this->errors;
    }
    $errors[] = new NyTechError($code, $ping);
    $this->errors = $errors;
  }

  function account_load($user_id) {
    $this->account = new NyTechAccount(user_load($user_id));
  }
}

class eClass {
  public $entity_id;
  public $entity_type;

  public $bundle;
  public $user_id;
  public $status;
  public $created;
  public $created_pretty;
  public $changed;
  public $changed_pretty;
  public $uuid;

  public $urls;

  function __construct($entity, $entity_type, $bundle) {
    $this->set_entity($entity, $entity_type, $bundle);
    $this->set_user($entity);
    $this->set_status($entity);
    $this->set_created($entity);
    $this->set_uuid($entity);
  }

  public function set_entity($entity, $entity_type, $bundle) {
    $this->entity_id = $entity->id;
    $this->entity_type = $entity_type;
    $this->bundle = $bundle;
  }

  public function set_user($entity) {
    $this->user_id = $entity->uid;
  }

  public function set_status($entity) {
    if(property_exists($entity, 'status')) {
      $this->status = $entity->status;
    }
  }

  public function set_created($entity) {
    $this->created = (int) $entity->created;
    $this->created_pretty = date('M d, Y g:ia', $this->created);
    $this->changed = (int) $entity->changed;
    $this->changed_pretty = date('M d, Y g:ia', $this->changed);
  }

  public function set_uuid($entity) {
    $this->uuid = $entity->uuid;
  }

}

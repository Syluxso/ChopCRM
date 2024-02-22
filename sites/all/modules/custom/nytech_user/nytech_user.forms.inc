<?php

function nytech_user_register_form($form, &$form_state) {
  $form = new NyTechForm('', '');
  $form->set_actions('nytech_user_register_form_validate', 'nytech_user_register_form_submit');

  $field = new NyTechFormItem('email', 'Email', 'textfield');
  $field->placeholder('name@example.com');
  $form->field($field->field());

  $field = new NyTechFormItem('phone', 'Phone', 'textfield');
  $field->phone_validate('edit-phone');
  $field->placeholder('(xxx) xxx - xxxx');
  $form->field($field->field());

  $field = new NyTechFormItem('pass', 'Password', 'password');
  $field->placeholder('Password');
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Register', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());


  return $form->form();
}

function nytech_user_register_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $phone = NyTechPhone::clean_phone($values['phone']);
  if(strlen($phone) != 10) {
    form_set_error('phone', 'Phone number must be 10 digits (xxx) xxx - xxxx');
  } else {
    $check_phone = user_load_by_phone($phone);
    if($check_phone != false) {
      form_set_error('phone', NyTechPhone::pretty_phone($values['phone']) . ' is already in use.');
    }
  }

  $email_validates = filter_var($values['email'], FILTER_VALIDATE_EMAIL);
  if ($email_validates == false) {
    form_set_error('email', 'Please enter a valid email address.');
  } else {
    $check_email = user_load_by_mail($values['email']);
    if($check_email != false) {
      form_set_error('email', $values['email'] . ' is already in use.');
    }
  }

  if(strlen($values['pass']) < 4) {
    form_set_error('pass', 'Password must be at least 4 characters.');
  }

}

function nytech_user_register_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $account = new NyTechRegister($values['email'], $values['phone'], $values['pass']);
  if($account != false) {
    drupal_goto('account/new');
  } else {
    drupal_set_message('Error...', 'error');
  }
}

function nytech_user_login_form($form, &$form_state) {
  $form = new NyTechForm('', '');
  $form->set_actions('nytech_user_login_form_validate', 'nytech_user_login_form_submit');

  $field = new NyTechFormItem('phone', 'Phone', 'textfield');
  $field->phone_validate('edit-phone');
  $field->placeholder('(xxx) xxx - xxxx');
  $form->field($field->field());

  $field = new NyTechFormItem('pass', 'Password', 'password');
  $field->placeholder('Password');
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Login', 'submit');
  $field->classes(['btn-sm', 'btn-primary']);
  $form->field($field->field());
  return $form->form();
}

function nytech_user_login_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $phone = NyTechPhone::clean_phone($values['phone']);
  if(strlen($phone) != 10) {
    form_set_error('phone', 'Phone number must be 10 digits (xxx) xxx - xxxx');
  }
}

function nytech_user_login_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $phone = NyTechPhone::clean_phone($values['phone']);
  $check_user = user_load_by_phone($phone);
  if($check_user != false) {
    $account = new NyTechLogin($check_user->name, $values['pass']);
    if($account->validated_auth == true) {
      global $user;
      $account = user_load($user->uid);
      $accountClass = new NyTechAccount($account);
      drupal_set_message('Welcome ' . $accountClass->first . '!');
      drupal_goto('deals');
    }
  }
  drupal_set_message('Please check your phone or password.', 'error');
}

function nytech_user_account_new_form($form, &$form_state, $account) {
  $form = new NyTechForm('', '');
  $form->set_actions('nytech_user_account_new_form_validate', 'nytech_user_account_new_form_submit');

  $field = new NyTechFormItem('example', '', 'markup');
  $field->output('<h4 class="text-center">Complete registration and enter your personal information.</h4><br />');
  $form->field($field->field());

  $field = new NyTechFormItem('first', 'First name', 'textfield');
  $field->placeholder('John');
  $form->field($field->field());

  $field = new NyTechFormItem('last', 'Last name', 'textfield');
  $field->placeholder('Smith');
  $form->field($field->field());

  $field = new NyTechFormItem('org', 'Company Name', 'textfield');
  $field->placeholder('Acme Co.');
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Complete Registration', 'submit');
  $field->classes(['btn-sm', 'btn-primary']);
  $form->field($field->field());
  return $form->form();
}

function nytech_user_account_new_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_user_account_new_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  global $user;

  $account = user_load($user->uid);
  $accountObject = new NyTechAccount($account);

  $array = [];
  $array['field_user_first_name']['und'][0]['value'] = $values['first'];
  $array['field_user_last_name']['und'][0]['value']  = $values['last'];
  $array['name'] = $account->mail;
  user_save($account, $array);

  // Create the org.
  $org = new NyTechOrgCreate;
  $org->user($account);
  $org->title($values['org']);
  $org->create();

  // Create membership with manager role for the user.
  $membership = new NyTechMembershipCreate;
  $membership->user($user);
  $membership->org($org->entity);
  $membership->role('manager');
  $membership->create();

  // Create two notifications.
  $title = 'Welcome ' . $values['first'] . '!';
  $message = 'We are glad you joined Hammerhead Docs. You can update your account ' . l('here', 'account/update');
  new NyTechNotificationCreate($user->uid, $title, $message);
  new NyTechNotificationCreate($accountObject->user_id, 'Notifications keep you up to date!', 'You can clear your notifications by selecting the red "x"');

  // Fist thing they need to do is create a deal
  $destination = 'deals/new';

  drupal_set_message('Registration is completed. Create your first deal.');
  drupal_goto($destination);
}

function nytech_user_password_form($form, &$form_state) {
  $form = new NyTechForm('', '');
  $form->set_actions('nytech_user_password_form_validate', 'nytech_user_password_form_submit');

  if(empty($_GET['success'])) {
    $field = new NyTechFormItem('example', '', 'markup');
    $field->output('<h4 class="text-center">Enter your cell number for a new pass code.</h4><br />');
    $form->field($field->field());

    $field = new NyTechFormItem('phone', 'Phone', 'textfield');
    $field->phone_validate('edit-phone');
    $field->placeholder('(xxx) xxx - xxxx');
    $form->field($field->field());

    $field = new NyTechFormItem('submit', 'Get Code', 'submit');
    $field->classes(['btn-sm', 'btn-primary']);
    $form->field($field->field());

  } else {
    $field = new NyTechFormItem('success', '', 'markup');
    $field->output('<h3 >Passcode has been sent to your phone</h3>' . l('Start over', current_path(), ['attributes' => ['class' => ['btn', 'btn-sm', 'btn-success']]]));
    $form->field($field->field());
  }

  return $form->form();
}

function nytech_user_password_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $phone = NyTechPhone::clean_phone($values['phone']);
  if(strlen($phone) != 10) {
    form_set_error('phone', 'Phone number must be 10 digits (xxx) xxx - xxxx');
  } else {
    $check_user = user_load_by_phone($phone);
    if($check_user == false) {
      form_set_error('phone', NyTechPhone::pretty_phone($values['phone']) . ' is not connected with an account.');
    }
  }
}

function nytech_user_password_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $phone = NyTechPhone::pretty_phone($values['phone']);
  new NyTechPassword($phone);
  drupal_set_message('Please login with the code we sent to your phone ' . $phone);
  drupal_goto('login', ['query' => ['success' => true]]);
}


/*
 * Account edit
 */
function nytech_user_account_edit_form($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $accountClass = new NyTechAccount($account);

  $form = new NyTechForm('', l('Back', URL_ACCOUNT));
  $form->set_actions('nytech_user_account_edit_form_validate', 'nytech_user_account_edit_form_submit');

  $field = new NyTechFormItem('account', '', 'markup');
  $field->output('<h4>Account</h4>');
  $form->field($field->field());

  $field = new NyTechFormItem('email', 'Email', 'textfield');
  $field->placeholder('name@example.com');
  $field->default($accountClass->email);
  $form->field($field->field());

  $field = new NyTechFormItem('phone', 'Phone', 'textfield');
  $field->phone_validate('edit-phone');
  $field->placeholder('(xxx) xxx - xxxx');
  $field->default($accountClass->phone_pretty);
  $form->field($field->field());

  $field = new NyTechFormItem('pass', 'Password', 'password');
  $field->placeholder('Password');
  $field->required(false);
  $form->field($field->field());

  $field = new NyTechFormItem('first', 'First name', 'textfield');
  $field->placeholder('John');
  $field->default($accountClass->first);
  $form->field($field->field());

  $field = new NyTechFormItem('last', 'Last name', 'textfield');
  $field->placeholder('Smith');
  $field->default($accountClass->last);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Update', 'submit');
  $field->classes(['pull-right', 'btn-sm', 'btn-primary']);
  $form->field($field->field());
  return $form->form();
}

function nytech_user_account_edit_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $phone = NyTechPhone::clean_phone($values['phone']);

  global $user;
  $account = user_load($user->uid);

  // Validate a 10 digit phone number
  if(strlen($phone) != 10) {
    form_set_error('phone', 'Phone number must be 10 digits (xxx) xxx - xxxx');
  }

  // Validate the phone is not in use by another user.
  $check_user = user_load_by_phone($phone);
  if($check_user != false) {
    if($check_user->uid != $account->uid) {
      form_set_error('phone', NyTechPhone::pretty_phone($values['phone']) . ' is already in use.');
    }
  }

  $email_validates = filter_var($values['email'], FILTER_VALIDATE_EMAIL);
  if ($email_validates == false) {
    form_set_error('email', 'Please enter a valid email address.');
  } else {
    $check_email = user_load_by_mail($values['email']);
    if($check_email != false) {
      if($check_email->uid != $account->uid) {
        form_set_error('email', $values['email'] . ' is already in use.');
      }
    }
  }

  $pass_len = strlen($values['pass']);
  if($pass_len > 0 && $pass_len < 4) {
    form_set_error('pass', 'Password must be at least 4 characters.');
  }
}

function nytech_user_account_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  new NyTechAccountUpdate($values);
  if(!empty($values['pass'])) {
    $accountObject = get_account();
    $title = 'Password changed.';
    $message = 'Your password was updated.';
    new NyTechNotificationCreate($accountObject->user_id, $title, $message, 1);
  }
  drupal_set_message('Account updated.');
}

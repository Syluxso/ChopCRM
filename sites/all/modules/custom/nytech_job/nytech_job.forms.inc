<?php


function nytech_job_new_form($form, &$form_state, $entity_id) {
  $form = new NyTechForm('Create New Job', '');
  $form->set_actions('nytech_job_new_form_validate', 'nytech_job_new_form_submit');

  $field = new NyTechFormItem('title', 'Job name', 'textfield');
  $field->placeholder('Job name');
  $form->field($field->field());

  $field = new NyTechFormItem('address', 'Job address', 'textfield');
  $field->placeholder('123 Awesome Street, Los Angeles, CA. 90210');
  $form->field($field->field());

  $field = new NyTechFormItem('entity_id', 'Related Org', 'hidden');
  $field->value($entity_id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Create Job', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());


  return $form->form();
}

function nytech_job_new_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_job_new_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $values['user_id'] = $user->uid;
  $job = new NyTechJobCreate($values);
  drupal_set_message('Job created.');
  drupal_goto('o/' . $values['entity_id'] . '/jobs');
}

function nytech_job_update_form($form, &$form_state, $org_id, $entity_id) {
  $entity = entity_load_single('job', $entity_id);
  $job = new NyTechJob($entity);

  $form = new NyTechForm('Create New Job', '');
  $form->set_actions('nytech_job_update_form_validate', 'nytech_job_update_form_submit');

  $field = new NyTechFormItem('title', 'Job name', 'textfield');
  $field->placeholder('Job name');
  $field->default($job->title);
  $form->field($field->field());

  $field = new NyTechFormItem('address', 'Address', 'textfield');
  $field->placeholder('1000 First Street, Fresno CA. 90210');
  $field->default($job->address);
  $form->field($field->field());

  $field = new NyTechFormItem('org_id', 'Related Org', 'hidden');
  $field->value($org_id);
  $form->field($field->field());

  $field = new NyTechFormItem('entity_id', 'Entity ID', 'hidden');
  $field->value($entity_id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Update Job', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());


  return $form->form();
}

function nytech_job_update_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_job_update_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $entity = entity_load_single('job', $values['entity_id']);
  $job = new NyTechJob($entity);
  new NyTechJobUpdate($entity, $values);
  drupal_set_message('Job updated.');
  drupal_goto($job->view_url);
}

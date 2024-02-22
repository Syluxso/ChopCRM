<?php


function nytech_note_new_form($form, &$form_state, $org_id, $job_id) {
  $form = new NyTechForm('Create New Note', '');
  $form->set_actions('nytech_note_new_form_validate', 'nytech_note_new_form_submit');

  $field = new NyTechFormItem('message', 'Note', 'textarea');
  $field->placeholder('Details...');
  $form->field($field->field());

  $field = new NyTechFormItem('org_id', 'Related Org', 'hidden');
  $field->value($org_id);
  $form->field($field->field());


  $field = new NyTechFormItem('job_id', 'Related Job', 'hidden');
  $field->value($job_id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Create Note', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());

  return $form->form();
}

function nytech_note_new_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_note_new_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $values['user_id'] = $user->uid;
  $note = new NyTechNoteCreate($values);
  drupal_set_message('Note created.');
  drupal_goto('o/' . $values['org_id'] . '/jobs/' . $values['job_id'] . '/notes');
}

function nytech_note_update_form($form, &$form_state, $org_id, $job_id, $entity_id) {
  $entity = entity_load_single('note', $entity_id);
  $note = new NyTechNote($entity);

  $form = new NyTechForm('', '');
  $form->set_actions('nytech_note_update_form_validate', 'nytech_note_update_form_submit');

  $field = new NyTechFormItem('message', 'Note', 'textarea');
  $field->placeholder('Details...');
  $field->default($note->message);
  $form->field($field->field());

  $field = new NyTechFormItem('org_id', 'Related Org', 'hidden');
  $field->value($org_id);
  $form->field($field->field());

  $field = new NyTechFormItem('job_id', 'Related Job', 'hidden');
  $field->value($job_id);
  $form->field($field->field());

  $field = new NyTechFormItem('entity_id', 'Entity ID', 'hidden');
  $field->value($entity_id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Update Note', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());

  return $form->form();
}

function nytech_note_update_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_note_update_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $values['user_id'] = $user->uid;
  $note = new NyTechNoteUpdate($values);

  drupal_set_message('Note updated.');
  drupal_goto('o/' . $values['org_id'] . '/jobs/' . $values['job_id'] . '/notes/' . $values['entity_id'] . '/view');
}

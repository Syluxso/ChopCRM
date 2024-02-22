<?php


function nytech_image_image_new_form($form, &$form_state, $org_id, $job_id) {
  $form = new NyTechForm('Create New Images', '');
  $form->set_actions('nytech_image_image_new_form_validate', 'nytech_image_image_new_form_submit');

  $field = new NyTechFormItem('image_label', '', 'markup');
  $field->output('<h5>Image (required)</h5>');
  $form->field($field->field());

  $field = new NyTechFormItem('image', 'Image', 'managed_file');
  $field->file();
  $form->field($field->field());

  $field = new NyTechFormItem('space_1', '', 'markup');
  $field->output('<br />');
  $form->field($field->field());

  $field = new NyTechFormItem('title', 'Label', 'textfield');
  $field->placeholder('Label for this image');
  $form->field($field->field());

  $field = new NyTechFormItem('memo', 'Memo', 'textarea');
  $field->placeholder('This was the "after" picture I took of the project.');
  $field->required(false);
  $form->field($field->field());

  $field = new NyTechFormItem('org_id', 'Related Org', 'hidden');
  $field->value($org_id);
  $form->field($field->field());

  $field = new NyTechFormItem('job_id', 'Related Job', 'hidden');
  $field->value($job_id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Add Images', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());

  return $form->form();
}

function nytech_image_image_new_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_image_image_new_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $values['user_id'] = $user->uid;
  $destination = build_uuid_dir_path([
    'organization' => $values['org_id'],
    'job'          => $values['job_id']
  ]);
  $fid         = $values['image'];
  $file_object = file_load($fid);
  $fileMoved   = new NyTechFileMove($file_object);
  $fileMoved->move($destination, 'images');
  $values['image'] = $fileMoved->file_array();
  $image = new NyTechImageCreate($values);
  drupal_set_message('Image created.');
  drupal_goto('o/' . $values['org_id'] . '/jobs/' . $values['job_id'] . '/images');
}

function nytech_image_image_update_form($form, &$form_state, $org_id, $job_id, $entity_id) {
  $entity = entity_load_single('image', $entity_id);
  $image = new NyTechImage($entity);
  $fid = false;
  if(!empty($image->fids)) {
    $fid = $image->fids[0];
  }

  $form = new NyTechForm('Create New Images', '');
  $form->set_actions('nytech_image_image_update_form_validate', 'nytech_image_image_update_form_submit');

  $field = new NyTechFormItem('image_label', '', 'markup');
  $field->output('<h5>Image (required)</h5>');
  $form->field($field->field());

  $field = new NyTechFormItem('image', 'Image', 'managed_file');
  $field->default($fid);
  $field->file();
  $form->field($field->field());

  $field = new NyTechFormItem('space_1', '', 'markup');
  $field->output('<br />');
  $form->field($field->field());

  $field = new NyTechFormItem('title', 'Label', 'textfield');
  $field->placeholder('Label for this image');
  $field->default($image->title);
  $form->field($field->field());

  $field = new NyTechFormItem('memo', 'Memo', 'textarea');
  $field->placeholder('This was the "after" picture I took of the project.');
  $field->default($image->memo);
  $field->required(false);
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

  $field = new NyTechFormItem('submit', 'Add Images', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());

  return $form->form();
}

function nytech_image_image_update_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_image_image_update_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $values['user_id'] = $user->uid;
  $image = new NyTechImageUpdate($values);
  drupal_set_message('Image updated.');
  drupal_goto('o/' . $values['org_id'] . '/jobs/' . $values['job_id'] . '/images/' . $values['entity_id'] . '/view');
}

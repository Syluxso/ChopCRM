<?php

function nytech_org_update_form($form, &$form_state, $entity_id) {
  $entity = entity_load_single('organization', $entity_id);

  $form = new NyTechForm('', '');
  $form->set_actions('nytech_org_update_form_validate', 'nytech_org_update_form_submit');

  $field = new NyTechFormItem('title', 'Company Name', 'textfield');
  $field->default($entity->title);
  $field->placeholder('Acme Co.');
  $form->field($field->field());

  $field = new NyTechFormItem('entity_id', 'Entity_id', 'hidden');
  $field->value($entity->id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Update', 'submit');
  $field->classes(['pull-right', 'btn-sm', 'btn-primary']);
  $form->field($field->field());
  return $form->form();
}

function nytech_org_update_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_org_update_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $entity = entity_load_single('organization', $values['entity_id']);
  $entity->title = $values['title'];
  entity_save('organization', $entity);
  drupal_set_message('Company updated.');
  drupal_goto('o/' . $entity->id . '/view');
}



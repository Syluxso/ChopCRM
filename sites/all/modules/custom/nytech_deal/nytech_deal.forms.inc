<?php


function nytech_deal_deal_new_form($form, &$form_state) {
  $form = new NyTechForm('New Deal', '');
  $form->set_actions('nytech_deal_deal_new_form_validate', 'nytech_deal_deal_new_form_submit');

  $field = new NyTechFormItem('title', 'Label', 'textfield');
  $field->placeholder('Acme Co deal');
  $form->field($field->field());

  $field = new NyTechFormItem('value', 'Value', 'textfield');
  $field->placeholder('3,000');
  $field->required(false);
  $field->is_number(true);
  $form->field($field->field());

  $deal_states = new NyTecDealStages();
  $field = new NyTechFormItem('state', 'State', 'toggle');
  $field->toggle($deal_states->objects, 'buttons', '');
  $form->field($field->field());

  $field = new NyTechFormItem('state', 'State', 'hidden');
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Add Deal', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());

  return $form->form();
}

function nytech_deal_deal_new_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_deal_deal_new_form_submit($form, &$form_state) {
  global $user;
  $values            = $form_state['values'];
  $values['user_id'] = $user->uid; // Inject the global user yo!
  new NyTechDealCreate($values);
  drupal_set_message('Deal created.');
  drupal_goto('deals');
}

function nytech_deal_deal_update_form($form, &$form_state, $deal) {
  $form = new NyTechForm('', '');
  $form->set_actions('nytech_deal_deal_update_form_validate', 'nytech_deal_deal_update_form_submit');

  $field = new NyTechFormItem('title', 'Deal memo', 'textfield');
  $field->placeholder('Contractor box');
  $field->default($deal->title);
  $form->field($field->field());

  $field = new NyTechFormItem('value', 'Value', 'textfield');
  $field->placeholder('3,000');
  $field->required(false);
  $field->is_number(true);
  $field->default($deal->value);
  $form->field($field->field());

  $deal_states = new NyTecDealStages();
  $field = new NyTechFormItem('state', 'State', 'toggle');
  $field->toggle($deal_states->objects, 'buttons', $deal->state);
  $form->field($field->field());

  $field = new NyTechFormItem('state', 'State', 'hidden');
  $field->default($deal->state);
  $form->field($field->field());

  $field = new NyTechFormItem('entity_id', 'Entity ID', 'hidden');
  $field->value($deal->entity_id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Update Deal', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());

  return $form->form();
}

function nytech_deal_deal_update_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_deal_deal_update_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  new NyTechDealUpdate($values);
  drupal_set_message('Deal updated.');
  drupal_goto('deals/' . $values['entity_id'] . '/view');
}

<?php


function nytech_tag_admin_tag_new_form($form, &$form_state) {
  $form = new NyTechForm('Create a Tag', '');
  $form->set_actions('nytech_tag_admin_tag_new_form_validate', 'nytech_tag_admin_tag_new_form_submit');

  $field = new NyTechFormItem('filter', 'filter', 'textfield');
  $field->placeholder('category_name');
  $form->field($field->field());

  $field = new NyTechFormItem('title', 'Title', 'textfield');
  $field->placeholder('My tag name yo!');
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Create Tag', 'submit');
  $field->classes(['btn-sm', 'btn-primary', 'btn-block', 'auth-action']);
  $form->field($field->field());

  return $form->form();
}

function nytech_tag_admin_tag_new_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_tag_admin_tag_new_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $tag = new NyTechSubmitTag($values['filter'], $values['title']);
}

<?php

function nytech_master_site_settings_form($form, &$form_state) {

  $form = new NyTechForm('', '');
  $form->set_actions('nytech_master_site_settings_form_validate', 'nytech_master_site_settings_form_submit');

  $field = new NyTechFormItem('nytech_bootstrap_size_icon', 'Bootstrap Size Icon', 'select');
  $field->options(['on' => 'ON', 'off' => 'OFF']);
  $field->default(variable_get('nytech_bootstrap_size_icon', 'off'));
  $field->suffix('If ON this shows a size icon in the content footer for what bootstrap size is being displayed.');
  $form->field($field->field());

  return system_settings_form($form->form());
}

function nytech_master_site_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

}

function nytech_master_site_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
}


<?php

function nytech_manager_site_settings_form($form, &$form_state) {

  $form = new NyTechForm('', '');
  $form->set_actions('nytech_master_site_settings_form_validate', 'nytech_master_site_settings_form_submit');

  $field = new NyTechFormItem('ad_country_cats', 'Country Categories', 'textarea');
  $field->default(variable_get('ad_country_cats'));
  $form->field($field->field());

  $field = new NyTechFormItem('ad_types', 'Ad Types', 'textarea');
  $field->default(variable_get('ad_types'));
  $form->field($field->field());

  $field = new NyTechFormItem('ad_bike_cats', 'Bike Categories', 'textarea');
  $field->default(variable_get('ad_bike_cats'));
  $form->field($field->field());

  $field = new NyTechFormItem('ad_gear_cats', 'Gear Categories', 'textarea');
  $field->default(variable_get('ad_gear_cats'));
  $form->field($field->field());

  $field = new NyTechFormItem('ad_parts_cats', 'Parts Categories', 'textarea');
  $field->default(variable_get('ad_parts_cats'));
  $form->field($field->field());

  $field = new NyTechFormItem('ad_year_cats', 'Year Categories', 'textarea');
  $field->default(variable_get('ad_year_cats'));
  $form->field($field->field());

  $field = new NyTechFormItem('ad_make_cats', 'Make Categories', 'textarea');
  $field->default(variable_get('ad_make_cats'));
  $form->field($field->field());

  $field = new NyTechFormItem('authorize_live_pk', 'Authorize.net LIVE: Private Key', 'textfield');
  $field->default(variable_get('authorize_live_pk'));
  $form->field($field->field());

  $field = new NyTechFormItem('authorize_test_pk', 'Authorize.net TEST: Private Key', 'textfield');
  $field->default(variable_get('authorize_test_pk'));
  $form->field($field->field());

  $field = new NyTechFormItem('ad_year_cats', 'Year Categories', 'textarea');
  $field->default(variable_get('ad_year_cats'));
  $form->field($field->field());

  $q = db_query('SELECT e.id, e.title, e.price FROM {eck_offering} e');
  $results = '';
  foreach($q as $row) {
    $results .= $row->title . ' $' . $row->price . ' (ID <code>' . $row->id . '</code>)<br />';
  }
  $field = new NyTechFormItem('featured_ids', 'Featured Offerings', 'textarea');
  $field->default(variable_get('featured_ids'));
  $field->suffix($results);
  $form->field($field->field());

  return system_settings_form($form->form());
}

function nytech_master_site_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

}

function nytech_master_site_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
}

function nytech_manager_listing_edit_form($form, &$form_state, $entity_id) {
  $entity = entity_load_single('ad', $entity_id);
  $form = new NyTechForm('', '');
  $form->set_actions('nytech_manager_listing_edit_form_validate', 'nytech_manager_listing_edit_form_submit');

  /*
   * Categories section.
   */
  $field = new NyTechFormItem('category_section', '', 'markup');
  $field->output('<h3>Category</h3>');
  $form->field($field->field());

  $field = new NyTechFormItem('category', 'Category', 'select');
  $field->options(['Bike', 'Cat', 'Taco']);
  $form->field($field->field());

  $field = new NyTechFormItem('sub_category', 'Sub Category', 'select');
  $field->options(['Tree', 'Water', 'Sward']);
  $form->field($field->field());

  /*
   * Description section.
   */
  $field = new NyTechFormItem('description_section', '', 'markup');
  $field->output('<h3 class="form-section">Description</h3>');
  $form->field($field->field());

  $ad_type = $entity->ad_type;
  $title = '';
  $price = '';
  $details = '';
  $years = get_category('ad_year_cats');
  $year = date('Y');
  $makes = get_category('ad_make_cats');
  $make = '';
  $model = '';
  $millage = '';

  if(!empty($entity->title)) {
    $title = $entity->title;
  }

  if(!empty($entity->price)) {
    $price = $entity->price;
  }

  if(!empty($entity->field_ad_description)) {
    $details = $entity->field_ad_description['und'][0]['value'];
  }

  // Bike stuff..
  if(!empty($entity->year)) {
    $year = $entity->year;
  }

  if(!empty($entity->make)) {
    $make = $entity->make;
  }

  if(!empty($entity->model)) {
    $model = $entity->model;
  }

  if(!empty($entity->millage)) {
    $millage = $entity->millage;
  }

  if($ad_type == 'bike') {
    $title = 'Title will be automatically generated.';
    $field = new NyTechFormItem('title', 'Title', 'hidden');
    $field->placeholder('The title will be automatically generated.');
    $field->value($title);
    $form->field($field->field());
  } else {
    $field = new NyTechFormItem('title', 'Title', 'textfield');
    $field->placeholder('The title of my ad post');
    $field->default($title);
    $form->field($field->field());
  }

  $field = new NyTechFormItem('price', 'Price $', 'textfield');
  $field->placeholder('100');
  $field->default($price);
  $field->attributes([' type' => 'number']);
  $form->field($field->field());

  if($entity->ad_type == 'bike') {

    $field = new NyTechFormItem('year', 'Year', 'select');
    $field->options($years);
    $field->default($year);
    $form->field($field->field());

    $field = new NyTechFormItem('make', 'Make', 'select');
    $field->options($makes);
    $field->default($make);
    $form->field($field->field());

    $field = new NyTechFormItem('model', 'Model', 'textfield');
    $field->placeholder('Rider');
    $field->default($model);
    $form->field($field->field());

    $field = new NyTechFormItem('millage', 'Millage', 'textfield');
    $field->placeholder('10,000');
    $field->default($millage);
    $field->attributes([' type' => 'number']);
    $form->field($field->field());
  }

  $field = new NyTechFormItem('description', 'Description', 'textarea');
  $field->default($details);
  $field->placeholder('Details about my post');
  $form->field($field->field());

  /*
   * Image section
   */
  $field = new NyTechFormItem('image_section', '', 'markup');
  $field->output('<h3>Images</h3>');
  $form->field($field->field());

  if(!empty($_GET['delta'])) {
    unset($entity->field_ad_images['und'][$_GET['delta'] - 1]);
    $entity->save();
    drupal_set_message('Image removed.');
    drupal_goto(current_path());
  }

  $images = new NyTechAdImages(entity_load_single('ad', $entity_id));
  $field = new NyTechFormItem('image_list', '', 'markup');
  $field->output('<h5>' . $images->count . ' images</h5>' . $images->output() . '<br /><div class="clearfix"></div>');
  $form->field($field->field());

  $count = 0;
  if(!empty($entity->field_ad_images)) {
    $count = count($entity->field_ad_images['und']);
  }
  if($count < 10) {
    $field = new NyTechFormItem('images', 'Import File', 'managed_file');
    $field->file('public://listings/', 'jpg jpeg png');
    $form->field($field->field());

    $field = new NyTechFormItem('submit', 'Save Image', 'submit');
    $field->classes(['btn-sm', 'btn-primary']);
    $field->suffix(l('Next', 'ads/' . $entity_id . '/contact', ['attributes' => ['class'=> ['btn', 'btn-sm', 'btn-primary', 'pull-right']]]));
    $form->field($field->field());
  } else {
    $field = new NyTechFormItem('submit', 'Next', 'submit');
    $field->classes(['btn-sm', 'btn-primary', 'pull-right']);
    $form->field($field->field());
  }

  /*
   * Contact section
   */
  $field = new NyTechFormItem('contact_section', '', 'markup');
  $field->output('<h3>Contact</h3>');
  $form->field($field->field());

  $countries = get_category('ad_country_cats');
  $country = '';
  $phone = '';
  $zip = '';
  $address = '';
  $email = '';
  $website = '';
  $country = 'united states';
  // If the user has theme values.
  if(!empty($account->phone_pretty)) {
    $phone = $account->phone_pretty;
  }

  if(!empty($account->email)) {
    $email = $account->email;
  }

  if(!empty($account->address)) {
    $address = $account->address;
  }

  // If the entity has these values.
  if(!empty($entity->country)) {
    $country = $entity->country;
  }

  if(!empty($entity->phone)) {
    $phone = $entity->phone;
  }

  if(!empty($entity->zip)) {
    $zip = $entity->zip;
  }

  if(!empty($entity->field_ad_address)) {
    $address = $entity->field_ad_address['und'][0]['value'];
  }

  if(!empty($entity->model)) {
    $model = $entity->model;
  }

  if(!empty($entity->email)) {
    $email = $entity->email;
  }

  if(!empty($entity->website)) {
    $website = $entity->website;
  }

  $field = new NyTechFormItem('country', 'Country', 'select');
  $field->options($countries);
  $field->default($country);
  $form->field($field->field());

  $field = new NyTechFormItem('phone', 'phone', 'textfield');
  $field->placeholder('(xxx) xxx - xxxx');
  $field->phone_validate('edit-phone');
  $field->default($phone);
  $form->field($field->field());

  $field = new NyTechFormItem('zip', 'ZIP', 'textfield');
  $field->placeholder('90210');
  $field->default($zip);
  $field->required(false);
  $form->field($field->field());

  $field = new NyTechFormItem('address', 'Address', 'textarea');
  $field->placeholder("1234 Main Street\nSmallville, CA, 90210");
  $field->default($address);
  $field->required(false);
  $form->field($field->field());

  $field = new NyTechFormItem('email', 'Email', 'textfield');
  $field->placeholder('name@example.com');
  $field->default($email);
  $field->required(false);
  $form->field($field->field());

  $field = new NyTechFormItem('website', 'Website', 'textfield');
  $field->placeholder('https://www.example.com');
  $field->default($website);
  $field->required(false);
  $form->field($field->field());


  /*
   * Hidden and submit fields.
   */
  $field = new NyTechFormItem('entity_id', 'Entity ID', 'hidden');
  $field->value($entity_id);
  $form->field($field->field());

  $field = new NyTechFormItem('submit', 'Save & Next', 'submit');
  $field->classes(['pull-right', 'btn-sm', 'btn-primary']);
  $form->field($field->field());
  return $form->form();



  return system_settings_form($form->form());
}

function nytech_manager_listing_edit_form_validate($form, &$form_state) {
  $values = $form_state['values'];

}

function nytech_manager_listing_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
}


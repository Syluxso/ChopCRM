<?php global $base_url; ?>
<div class="card card-body">
  <div class="row">
    <div class="col-sm-12 col-md-2">
      <img src="<?php print $base_url . '/' . drupal_get_path('theme', 'nytech_slate') . '/img/default_org.jpg'; ?>" class="img-responsive account--pic"/>
    </div>
    <div class="col-sm-12 col-md-10">
      <h2><?php print $title; ?></h2>
      <br />
    </div>

  </div>
</div>
<div class="account--tabs">
  <?php print org_tabs($entity_id); ?>
</div>
<br />

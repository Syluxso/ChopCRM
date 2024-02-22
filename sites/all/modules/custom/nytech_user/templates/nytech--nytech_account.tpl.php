<?php global $base_url; ?>
<div class="card card-body">
  <div class="row">
    <div class="col-sm-12 col-md-2">
      <img src="<?php print $base_url . '/' . drupal_get_path('theme', 'nytech_slate') . '/img/default_user.png'; ?>" class="img-responsive account--pic"/>
    </div>
    <div class="col-sm-12 col-md-10">
      <?php print l('Logout', 'user/logout', ['attributes' => ['class' => ['pull-right']]]); ?>
      <h2><?php print $name; ?></h2>
      <?php print $email; ?><br />
      <?php print $phone; ?><br />
      <br />
    </div>

  </div>
</div>
<div class="account--tabs">
  <?php print user_tabs(); ?>
</div>
<br />

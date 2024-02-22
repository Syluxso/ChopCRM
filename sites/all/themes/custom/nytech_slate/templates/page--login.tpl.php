

<div class="main-container <?php print $container_class; ?>">

  <header role="banner" id="page-header">
    <?php if (!empty($site_slogan)): ?>
      <p class="lead"><?php print $site_slogan; ?></p>
    <?php endif; ?>

    <?php print render($page['header']); ?>
  </header> <!-- /#page-header -->

  <div class="row">
    <div class="col-sm-12">
      <div class="row">

        <div class="col-sm-12 col-md-6">
          <?php if (!empty($page['highlighted'])): ?>
            <div class="highlighted"><?php print render($page['highlighted']); ?></div>
          <?php endif; ?>
        </div>
      </div>
      <?php print $messages; ?>
      <?php if (!empty($page['help'])): ?>
        <?php print render($page['help']); ?>
      <?php endif; ?>
      <?php if (!empty($action_links)): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
    </div>
  </div>
  <div class="row visible-xs visible-sm">
    <div class="col-xs-3 col-sm-3">
      <div class="logo-page">
        <?php print $logo_sm; ?>
      </div>
    </div>
    <div class="col-xs-9 col-sm-9 text-right" >
      <div class="logo-text ">hammerhead</div>
    </div>
  </div>
  <div class="row">
    <div class="col-sm-12 col-md-6 visible-md visible-lg visible-xl">
      <div class="logo-page">
        <?php print $logo_lg; ?>
      </div>
    </div>
    <div class="col-xs-12 col-sm-12 col-md-1 hidden-lg hidden-xl"></div>
    <div class="col-xs-12 col-sm-12 col-md-6 col-lg-5">
      <div class="card card-body auth-boxes">
        <div class="text-center">
          <h2><?php print $title; ?></h2>
        </div>
        <?php print render($page['content']); ?>
        <?php print pre_auth_links(); ?>
        <div class="auth-boxes--footer text-center">
          <?php print l('Home', '/'); ?>
          <?php print l('Contact Us', '/'); ?>
          <?php print l('Terms', '/'); ?>
        </div>
      </div>
    </div>
  </div>
</div>

<?php if (!empty($page['footer'])): ?>
  <footer class="footer <?php print $container_class; ?>">
    <?php print render($page['footer']); ?>
  </footer>
<?php endif; ?>
<?php if($messages) { ?>
  <?php print $messages; ?>
<?php } ?>

<?php if(!empty($pinned_tickets_model)) { ?>
  <?php print $pinned_ticket_toggle; ?>
  <?php print $pinned_tickets_model; ?>
<?php } ?>

<script>
  const phoneInput = document.getElementById('edit-phone');
  if (phoneInput) {
    phoneInput.type = 'tel';
  } else {
    console.log('Element with id "edit-phone" not found.');
  }

  if (typeof localStorage !== "undefined") {
    const userPhone = localStorage.getItem("userPhone");
    document.addEventListener("DOMContentLoaded", function() {
      document.getElementById("edit-phone").value = userPhone;
    });
  }
</script>

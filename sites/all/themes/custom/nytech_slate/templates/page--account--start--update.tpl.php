<?php
$account = get_account();
?>
asdf
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
  <div class="row">
    <div class="col-xs-12 col-sm-12 col-md-6 col-md-offset-3">
      <div class="start--apps--icons card-shadow">
        <div class="text-center">
          <h2 class="start--apps--title">Manage <?php print $account->first; ?>'s Apps</h2>
          <?php print render($page['content']); ?>
          <div class="text-left" style="padding: 15px;">
            <?php print l('Back', 'account/start', ['attributes' => ['class' => ['pull-right']]]); ?>
            <br />
            <br />
          </div>
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

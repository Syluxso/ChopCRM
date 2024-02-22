<?php
  $account = get_account();
  $users = new NyTechReportQuery('users', 'uid');
  $orgs = new NyTechReportQuery('eck_organization', 'id');
  $locations = new NyTechReportQuery('eck_location', 'id');
  $assets = new NyTechReportQuery('eck_asset', 'id');
  $moves = new NyTechReportQuery('eck_move', 'id');
  global $base_url;
?>

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
    <div class="col-xs-12 col-sm-12 col-md-8 col-md-offset-2">
      <div class="start--apps--icons card-shadow">
        <div class="text-center">
          <h6>Welcome <?php print $account->first; ?></h6>
          <h2 class="start--apps--title">Key Performance Indicators</h2>
          <div class="row">
            <div class="col-xs-12 col-sm-12 col-md-4">
              <div class="widget">
                <h3 class="widget--number">
                  <span id="reportKPIassets"><?php print $assets->count; ?></span>
                </h3>
                <h6 class="widget--label">Assets</h6>
              </div>
            </div>

            <div class="col-xs-12 col-sm-12 col-md-4">
              <div class="widget">
                <h3 class="widget--number">
                  <span id="reportKPIlocations"><?php print $locations->count; ?></span>
                </h3>
                <h6 class="widget--label">Locations</h6>
              </div>
            </div>

            <div class="col-xs-12 col-sm-12 col-md-4">
              <div class="widget">
                <h3 class="widget--number">
                  <span id="reportKPIorganizations"><?php print $orgs->count; ?></span>
                </h3>
                <h6 class="widget--label">Organizations</h6>
              </div>
            </div>

            <div class="col-xs-12 col-sm-12 col-md-4">
              <div class="widget">
                <h3 class="widget--number">
                  <span id="reportKPIusers"><?php print $users->count; ?></span>
                </h3>
                <h6 class="widget--label">Users</h6>
              </div>
            </div>

            <div class="col-xs-12 col-sm-12 col-md-4">
              <div class="widget">
                <h3 class="widget--number">
                  <span id="reportKPImoves"><?php print $moves->count; ?></span>
                </h3>
                <h6 class="widget--label">Moves</h6>
              </div>
            </div>

          </div>
        </div>

        <h5 class="text-center">
          <?php print l('Back', 'account/start'); ?>
          <br />
          <br />
        </h5>
      </div>
    </div>
  </div>
</div>

<script>
  setInterval(get_count, 5000);
  function get_count(){
    var reportKPIusers = document.getElementById("reportKPIusers");
    var reportKPIorganizations = document.getElementById("reportKPIorganizations");
    var reportKPIlocations = document.getElementById("reportKPIlocations");
    var reportKPIassets = document.getElementById("reportKPIassets");
    var reportKPImoves = document.getElementById("reportKPImoves");
    jQuery.ajax({
      url: "<?php print $base_url; ?>/api/v1/reports/kpi/default",
      success: function(result){
        if(parseInt(reportKPIusers.innerText) !== result.users) {
          animateValue(reportKPIusers, parseInt(reportKPIusers.innerText), result.users, 1000);
        }
        if(parseInt(reportKPIorganizations.innerText) !== result.orgs) {
          animateValue(reportKPIorganizations, parseInt(reportKPIorganizations.innerText), result.orgs, 1100);
        }
        if(parseInt(reportKPIlocations.innerText) !== result.locations) {
          animateValue(reportKPIlocations, parseInt(reportKPIlocations.innerText), result.locations, 1200);
        }
        if(parseInt(reportKPIassets.innerText) !== result.assets) {
          animateValue(reportKPIassets, parseInt(reportKPIassets.innerText), result.assets, 1300);
        }
        if(parseInt(reportKPImoves.innerText) !== result.moves) {
          animateValue(reportKPImoves, parseInt(reportKPImoves.innerText), result.moves, 1400);
        }
        console.log(result);
      }
    });
  }

  function animateValue(obj, start, end, duration) {
    let startTimestamp = null;
    const step = (timestamp) => {
      if (!startTimestamp) startTimestamp = timestamp;
      const progress = Math.min((timestamp - startTimestamp) / duration, 1);
      obj.innerHTML = Math.floor(progress * (end - start) + start);
      if (progress < 1) {
        window.requestAnimationFrame(step);
      }
    };
    window.requestAnimationFrame(step);
  }
</script>

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

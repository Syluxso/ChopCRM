<div class="row">
  <div class="col-xs-12 col-sm-12 col-md-12 col-lg-6 col-lg-offset-3">
    <?php if(!empty($top)) { print $top; }; ?>
  </div>
</div>

<div class="row">
  <div class="col-xs-12 col-sm-12 col-md-12 col-lg-6 col-lg-offset-3">
    <div class="card card-body">
      <?php print $content; ?>
    </div>
  </div>
</div>

<div class="row">
  <div class="col-xs-12 col-sm-12 col-md-12 col-lg-6 col-lg-offset-3">
    <?php if(!empty($bottom)) { print $bottom; }; ?>
  </div>
</div>

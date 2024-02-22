<?php global $base_url; ?>
<a href="<?php print $base_url . '/' . $url; ?>" class="list-group-item">
  <div class="row">
    <div class="col-xs-3 col-sm-3 col-md-2">
      <h4><?php print $eClass->state_output; ?></h4>
    </div>
    <div class="col-xs-6 col-sm-6 col-md-6">
      <h4><?php print $eClass->title; ?></h4>
    </div>
    <div class="col-xs-3 col-sm-3 col-md-4 text-right">
      <h4><?php print $eClass->value_pretty; ?></h4>
    </div>
  </div>
</a>

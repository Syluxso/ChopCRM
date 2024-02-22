<a href="<?php print $url; ?>" class="list-group-item">
  <div class="row">
    <div class="col-xs-12 col-sm-12 col-md-10">
      <?php if(!empty($job_title)) {?>
        <span class="note--listed--project-title"><?php print $job_title;  ?></span>
      <?php } ?>
      <?php print $short; ?>
    </div>
    <div class="col-xs-12 col-sm-12 col-md-2 text-right">
      <span class="label label-default"><?php print $time_ago; ?></span>
    </div>
  </div>
</a>

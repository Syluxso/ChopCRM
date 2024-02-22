<div class="list-group-item">
  <h6 class="pull-right"><?php print $n->created_time_ago; ?></h6>
  <h4><?php print $n->level; ?><?php print $n->title; ?></h4>
  <div class="pull-right">
    <a href="<?php print $n->delete_url; ?>" class="btn btn-xs btn-danger"><i class="fa fa-times-circle"></i></a>
  </div>
  <?php print $n->message; ?>
  <br />
</div>

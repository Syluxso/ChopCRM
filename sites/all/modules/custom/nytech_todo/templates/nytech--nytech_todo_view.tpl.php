<div class="row">
  <div class="col-xs-12">
    <?php print $n->message; ?>
    <hr />
  </div>
</div>
<div class="row">
  <div class="col-xs-6 col-sm-6 col-md-6">
    <h6>Author <?php print $n->author->name; ?></h6>
  </div>
  <div class="col-xs-6 col-sm-6 col-md-6 text-right">
    <h6>Created <?php print $n->created_pretty; ?></h6>
  </div>
</div>

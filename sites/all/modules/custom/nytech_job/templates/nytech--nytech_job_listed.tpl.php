<div class="job--listed card card-body card-shadow">
  <div class="row">
    <div class="col-xs-12 col-sm-12 col-md-4">
      <h3><?php print l($j->title, 'o/' . $j->org_id . '/jobs/' . $j->entity_id . '/view'); ?></h3>
      <h6 class="text-muted">Last Updated <?php print $j->changed_pretty; ?></h6>
    </div>
    <div class="col-xs-12 col-sm-12 col-md-4 text-right">
      <br />
      <?php // print l('Docs', $j->docs_url, ['attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]); ?>
      <?php print l('<i class="fa fa-image"></i> Images', $j->images_url, ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]); ?>
      <?php print l('<i class="fa fa-dollar"></i> Receipts', $j->receipts_url, ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]); ?>
      <?php print l('<i class="fa fa-pencil"></i> Notes', $j->notes_url, ['html' => true, 'attributes' => ['class' => ['btn', 'btn-sm', 'btn-default']]]); ?>
    </div>
    <div class="col-xs-12 col-sm-12 col-md-4 text-right">
      <br />
      <h6><i class="fa fa-user"></i> <?php print $j->author->name; ?></h6>
    </div>
  </div>
</div>

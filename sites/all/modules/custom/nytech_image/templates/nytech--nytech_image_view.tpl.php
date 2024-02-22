<div class="row">
  <div class="col-xs-12 col-sm-12 col-md-6">
    <?php print $img->image_output; ?>
  </div>
  <div class="col-xs-12 col-sm-12 col-md-6">
    <h1><?php print $img->title; ?></h1>
    <hr />
    <?php if(!empty($img->memo)) { ?>
      <p><?php print $img->memo; ?></p>
      <hr />
    <?php } ?>
    <h6><i class="fa fa-user"></i> <?php print $img->author->name; ?></h6>
    <h6><i class="fa fa-calendar"></i> <?php print $img->created_pretty; ?></h6>
  </div>
</div>

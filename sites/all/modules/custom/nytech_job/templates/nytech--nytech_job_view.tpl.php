<h1><?php print $j->title; ?></h1>
<h5 class="job--address_link"><?php print $j->address_url; ?></h5>
<div class="job--pills">
  <ul class="nav nav-tabs">
    <li><?php // print l('Docs', $j->docs_url); ?></li>
    <li><?php print l('<i class="fa fa-image"></i> Images', $j->images_url, ['html' => true]); ?></li>
    <li ><?php print l('<i class="fa fa-dollar"></i> Receipts', $j->receipts_url, ['html' => true]); ?></li>
    <li><?php print l('<i class="fa fa-pencil"></i> Notes', $j->notes_url, ['html' => true]); ?></li>
  </ul>
</div>
<br />
<div class="job--content">
  <?php if(!empty($new)) { print $new; } ?>
  <?php print $content; ?>
</div>

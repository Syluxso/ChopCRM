<h5><?php print ucfirst($toggle_name); ?></h5>
<?php foreach($list as $item) { ?>
  <?php
    $class = 'btn-default';
    if($item['item_label'] == $default) {
      $class = 'btn-primary';
    }
  ?>
  <a href="#"
     data-<?php print $toggle_name; ?>="<?php print $item['item_id']; ?>"
     id="<?php print $toggle_name; ?>-<?php print $item['item_id']; ?>"
     onclick="Select<?php print ucfirst($toggle_name); ?>(this)"
     class="related-<?php print $toggle_name; ?>-option btn btn-sm <?php print $class; ?>"><?php print $item['item_label']; ?></a>
<?php } ?>

<script>
  function Select<?php print ucfirst($toggle_name); ?>(n){
    document.getElementsByName('<?php print $toggle_name; ?>')[0].value = n.getAttribute('data-<?php print $toggle_name; ?>');
    const <?php print $toggle_name; ?>Options = document.querySelectorAll('.related-<?php print $toggle_name; ?>-option');
    <?php print $toggle_name; ?>Options.forEach(element => element.classList.remove('btn-primary'));
    <?php print $toggle_name; ?>Options.forEach(element => element.classList.add('btn-default'));
    n.classList.remove('btn-default');
    n.classList.add('btn-primary');
  }
</script>

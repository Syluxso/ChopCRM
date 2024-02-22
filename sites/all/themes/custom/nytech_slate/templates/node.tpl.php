<?php if($page) { ?>
    <div class="blog-post">
        <p class="blog-post-meta border-bottom "><?php print date('M d, Y', $node->created); ?> by <?php print $node->name; ?><br /><br /></p>
        <?php print render($content); ?>
    </div><!-- /.blog-post -->
<?php } else { ?>
    <div class="blog-post">
        <h2 class="blog-post-title"><?php print $title; ?></h2>
        <p class="blog-post-meta"><?php print date('M d, Y', $node->created); ?> by <?php print $node->name; ?></p>
        <?php print render($content); ?>
    </div><!-- /.blog-post -->
<?php } ?>
<!-- gestion du logout clef -->
<?php
global $user;
drupal_add_library('system', 'drupal.ajax');
drupal_add_library('system', 'jquery.form');
$u = user_load($user->uid);
$field = field_get_items('user', $u, 'field_logout_at');
$field_value = intval($field[0]['value']);
if ($field_value > 0 && $field_value < date('U')) {
	module_load_include('pages.inc', 'user');
	user_logout();
	session_destroy();
}
?>
<script>
(function ($) {
	setInterval(function() {
		$.ajax({
			type: 'GET',
			dataType: 'json',
			url: '/dedikam/ajax/logout/<?php echo $user->uid; ?>',
			success: function (data) {
				if (data.logout == "1") {
					$(location).attr('href','/user/logout');
				}
			}
		});
   	}, 5000)
})(jQuery);;
</script>
<div id="content_global">
<!-- #top bar -->
<div id="top_bar">
    <div class="container_12"><?php print render($page['top_bar']); ?></div>
</div>

<!-- #header -->
<div id="header">
	<!-- #header-inside -->
    <div id="header-inside" class="container_12 clearfix">
    	<!-- #header-inside-left -->
        <?php if($user->uid == 0):?>
			<div id="connexion_bottom">
				<!-- <div class="connexion_auto"><p>Se connecter automatiquement  : </p><?php   echo clef_login_button(); ?></div> -->
				<div class="connexion_manu"><p>Connexion : </p><input value="Se connecter" id="input_compte_connexion" type="button"></div>				
			</div>
        <?php endif; ?>
        <div id="header-inside-left" class="grid_3">

            <?php if ($logo): ?>
                <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
            <?php endif; ?>

            <?php if ($site_name || $site_slogan): ?>
            <div class="clearfix">
            <?php if ($site_name): ?>
            <span id="site-name"><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></span>
            <?php endif; ?>
            <?php if ($site_slogan): ?>
            <span id="slogan"><?php print $site_slogan; ?></span>
            <?php endif; ?>
            </div>
            <?php endif; ?>

        </div><!-- EOF: #header-inside-left -->

        <div class="grid_9">
            <div id="navigation" class="clearfix">
            <?php if ($page['navigation']) :?>
            <?php print drupal_render($page['navigation']); ?>

            <?php else :
                if (module_exists('i18n_menu')) {
                    $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
                } else {
                    $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
                }
                    print drupal_render($main_menu_tree);
                endif; ?>
            </div>

        </div><!-- #header-inside-right -->
    </div><!-- end #header-inside -->
</div><!-- end #header -->

<!-- #content -->
<div id="content">

	<!-- #content-inside -->
    <div id="content-inside" class="container_12 clearfix">

        <?php if ($page['sidebar_second']) { ?>
            <div id="main" class="grid_8">
        <?php } else { ?>
            <div id="main" class="grid_12">
        <?php } ?>

        <?php if (theme_get_setting('breadcrumb_display','corporateclean')): print $breadcrumb; endif; ?>

        <?php if ($messages): ?>
            <div id="console" class="clearfix">
                <?php print $messages; ?>
            </div>
        <?php endif; ?>

        <?php if ($page['help']): ?>
            <div id="help">
                <?php print render($page['help']); ?>
            </div>
        <?php endif; ?>

        <?php if ($page['content_top']) :?>
            <div id="content_top">
                <div id="content_top-inner" class="container_12 clearfix">
                    <?php print render($page['content_top']); ?>
                </div>
            </div>
        <?php endif; ?>

        <?php if ($action_links): ?>
            <ul class="action-links">
                <?php print render($action_links); ?>
            </ul>
        <?php endif; ?>
        <?php if(drupal_is_front_page()){
        ?>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
                <h1><?php print $title ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
        <?php
        }else{?>
		<?php print render($title_prefix); ?>
        <?php if ($title): ?>
            <div class="tp-banner-container"><h1><?php print $title ?></h1></div>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php } ?>
        <?php if ($tabs): ?><?php print render($tabs); ?><?php endif; ?>

        <?php if ($page['slider']) :?>
            <?php print render($page['slider']); ?>
        <?php endif; ?>

        <?php print render($page['content']); ?>

        <?php if ($page['highlighted']) :?>
        </div><!-- end #content-inside -->
        </div><!-- end #main -->
            <div id="highlighted">
                <div id="highlighted-inner" class="container_12 clearfix">
                    <?php print render($page['highlighted']); ?>
                </div>
            </div>
        <div id="content-inside" class="container_12 clearfix">
         <div id="main" class="grid_12">
        <?php endif; ?>

        <?php if ($page['content_bottom']) :?>
            <div id="content_bottom">
                <div id="content_bottom-inner" class="container_12 clearfix">
                    <?php print render($page['content_bottom']); ?>
                </div>
            </div>
        <?php endif; ?>

        </div><!-- end #main -->

        <?php if ($page['sidebar_second']) :?>
        <!-- #sidebar-second -->
        <div id="sidebar-second" class="grid_4">
            <?php print render($page['sidebar_second']); ?>
        </div><!-- end #sidebar-second -->
        <?php endif; ?>

    </div><!-- end #content-inside -->

</div><!-- end #content -->



</div><!-- #footer -->
<div id="footer">
	<!-- #footer-inside -->
    <div id="footer-inside" class="container_12 clearfix">
        <div id="credits" class="grid_8">

            <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('class' => array('secondary-menu', 'links', 'clearfix')))); ?>

            <?php print render($page['footer']); ?>

            <p>Dedikam 2008-2020 - <a href="https://www.dedikam.com/qui-sommes-nous/">Qui sommes-nous ?</a> - <a href="https://www.dedikam.com/mentions-legales/">Mentions légales</a> - <a href="https://www.dedikam.com/cgu/">RGPD</a> - <a href="https://www.dedikam.com/partenaires/">Partenaires</a></p>
            <p class="credits">Refonte graphique par Adrien</p>
        </div><!-- end #credits -->
        
    </div><!-- end #footer-inside -->
</div><!-- end #footer -->

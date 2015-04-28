<?php
function general_admin_settings_form($form, &$form_state, $rid = NULL) {
  if(isset($_SESSION['adminpanel_rebuild']) && $_SESSION['adminpanel_rebuild']) {
    if(function_exists('adminpanel_rebuild_all')) {
      adminpanel_rebuild_all();
    }
  }
  drupal_add_css(drupal_get_path('module', 'general_admin').'/css/general_admin_form.css');
  drupal_add_js(drupal_get_path('module', 'general_admin').'/js/general_admin_form.js');
  drupal_add_js('misc/collapse.js');
  $form['#submit'][] = 'general_admin_settings_form_submit';
  
  module_load_include('inc', 'system', 'system.admin');
  module_load_include('inc', 'locale', 'locale.admin');
  //Börja alltid med language settings
  $batch_operations[] = 'general_admin_settings_form_language_settings';
  //Fortsätt alltid med att sätta temat
  $batch_operations[] = 'general_admin_settings_form_theme';
/*-----------Varningsruta för testa.storleden.nu-----------------*/
	$host = general_get_host();
	if($host !== 'testa.storleden.nu') {
		global $databases;
		if($databases['default']['default']['database'] === 'storled1_testa') {
			$form['#prefix'] = '<div class="domain-warning">'.utf8_encode('Den här webbplatsen är kopplad till databasen för testa.storleden.nu! Ändra inställningarna i settings.php innan du fortsätter.').'</div>';
		}
	}
/*------------------Återförsäljare-----------------*/
	$batch_operations[] = 'general_admin_settings_form_reseller';
	$form['reseller'] = array(
		'#title' => t('Reseller'),
		'#type' => 'fieldset',
	);
	$form['reseller']['reseller'] = array(
		'#title' => t('Reseller'),
		'#type' => 'select',
		'#default_value' => variable_get('general_reseller', ''),
		'#options' => array('reco' => 'Reco'),
		'#empty_option' => t('- None -'),
		'#description' => t('If the site is ordered from a reseller, select the organization here'),
	);
	$form['reseller']['reco'] = array(
		'#type' => 'container',
		'#states' => array(
      'visible' => array(
       ':input[name="reseller"]' => array('value' => 'reco'),
      ),
    ),
		'reco_id' => array(
			'#title' => t('ID'),
			'#type' => 'textfield',
			'#default_value' => variable_get('general_reco_id', ''),
			'#description' => t('The ID of this organization\'s page at Reco.se'),
		),
		'reco_widget_width' => array(
			'#title' => t('Widget width'),
			'#type' => 'textfield',
			'#default_value' => variable_get('general_reco_width', 900),
		),
		'reco_url' => array(
			'#title' => t('Link URL'),
			'#type' => 'textfield',
			'#default_value' => variable_get('general_reco_url', 'http://www.reco.se/reco-se'),
			'#description' => t('The URL of this organization\'s page at Reco.se'),
		),
	);
/*---------------Funktioner------------------------*/
  $form['functions'] = array(
    '#title' => t('Functions'),
    '#type' => 'fieldset',
  );
  $form['functions']['contact'] = array(
    '#title' => t('Contact form'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_contact', true),
  );
  $form['functions']['stafflist'] = array(
    '#title' => t('Staff list'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_stafflist', true),
  );
  $form['functions']['slideshow'] = array(
    '#title' => t('Slideshow'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_slideshow', true),
  );
  $form['functions']['gallery'] = array(
    '#title' => t('Gallery'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_gallery', true),
  );
  $form['functions']['news'] = array(
    '#title' => t('News'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_news', true),
  );
  $form['functions']['blog'] = array(
    '#title' => t('Blog'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_blog', true),
  );
  $form['functions']['promo'] = array(
    '#title' => t('Movable promo areas'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_promo', true),
  );
  $form['functions']['vertical_scroll'] = array(
    '#title' => t('Vertical scroll'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_vertical_scroll', false),
    '#description' => t('Function for using menu links to scroll to inline content'),
  );
  $form['functions']['tracking_basic'] = array(
    '#type' => 'checkbox',
    '#title' => t('Basic tracking'),
    '#default_value' => variable_get('tracking_basic', false),
  );
  $ga_account = variable_get('googleanalytics_account', 'UA-');
  $form['functions']['googleanalytics_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Analytics Web Property ID'),
    '#default_value' => $ga_account ? $ga_account : 'UA-',
    '#states' => array(
      'invisible' => array(
       ':input[name="tracking_basic"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['functions']['tracking_advanced'] = array(
    '#type' => 'checkbox',
    '#title' => t('Advanced tracking'),
    '#default_value' => variable_get('tracking_advanced', false),
  );
  $form['functions']['clicktale_project_id'] = array(
    '#type' => 'textfield',
    '#title' => 'Clicktale Project ID',
    '#default_value' => variable_get('clicktale_project_id', ''),
    '#states' => array(
      'invisible' => array(
       ':input[name="tracking_advanced"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['functions']['vwo_id'] = array(
    '#type' => 'textfield',
    '#title' => 'Visual Web Optimizer ID',
    '#default_value' => variable_get('vwo_id', ''),
    '#states' => array(
      'invisible' => array(
       ':input[name="tracking_advanced"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['functions']['tracking_settings'] = array(
    '#title' => t('Tracking settings'),
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('collapsible', 'collapsed')),
    '#states' => array(
      'invisible' => array(
       ':input[name="tracking_basic"]' => array('checked' => FALSE),
       ':input[name="tracking_advanced"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['functions']['tracking_settings']['tracking_pages_setting'] = array(
    '#title' => t('Track pages'),
    '#type' => 'radios',
    '#options' => array(
      BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
      BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
    ),
    '#default_value' => variable_get('general_admin_tracking_visibility', BLOCK_VISIBILITY_NOTLISTED),
  );
  $form['functions']['tracking_settings']['tracking_pages_list'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('general_admin_tracking_paths', "admin\radmin/*\rbatch\rnode/add*\rnode/*/*\ruser/*/*\r*imce*"),
  );
  $role_options = array_map('check_plain', user_roles());
  $form['functions']['tracking_settings']['tracking_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable tracking for specific roles'),
    '#default_value' => variable_get('general_admin_tracking_roles', array(1)),
    '#options' => $role_options,
    '#description' => t('Enable tracking only for the selected role(s). If you select no roles, tracking will be enabled for all users.'),
  );
  
  //Sociala medier
  $form['functions']['sm'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Media'),
    '#attributes' => array('class' => array('collapsible')),
  );
  $indicative_vars = array(
    'general_functions_facebook_like_button',
    'general_functions_facebook_like_box',
    'general_functions_social_links',
    'general_functions_social',
  );
  $open = false;
  foreach($indicative_vars as $var) {
    if(variable_get($var, false)) {
      $open = true;
      break;
    }
  }
  if(!$open) {
    $form['functions']['sm']['#attributes']['class'][] = 'collapsed';
  }
  $form['functions']['sm']['social_facebook_id'] = array(
    '#title' => t('Facebook page id'),
    '#type' => 'textfield',
    '#default_value' => variable_get('general_functions_social_facebook_id', ''),
  );
  $form['functions']['sm']['social_twitter_name'] = array(
    '#title' => t('Twitter name'),
    '#type' => 'textfield',
    '#default_value' => variable_get('general_functions_social_twitter_name', ''),
  );
  /*-----------Facebook like button----------*/
  $form['functions']['sm']['facebook_like_button'] = array(
    '#title' => t('Facebook Like Button'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_facebook_like_button', false),
  );
  $form['functions']['sm']['facebook_like_button_address'] = array(
    '#type' => 'radios',
    '#options' => array(
      'local' => t('Local page'),
      'facebook' => t('Facebook page'),
    ),
    '#default_value' => variable_get('general_facebook_like_button_address', 'local'),
  );
  /*-------------Facebook like box-----------*/
  $form['functions']['sm']['facebook_like_box'] = array(
    '#title' => t('Facebook Like Box'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_facebook_like_box', false),
  );
  /*-----------------Sociala länkar-------------------*/
  $form['functions']['sm']['social_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Social links'),
    '#default_value' => variable_get('general_functions_social_links', false),
  );
  /*-----------------Socialt innehållsflöde-------------------*/
  $form['functions']['sm']['social'] = array(
    '#title' => t('Display social feed'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_functions_social', true),
  );

  $current_facebook_icon_path = variable_get('general_functions_social_facebook_icon_path', '');
  if($current_facebook_icon_path) {
    $facebook_icon_data = array(
      'width' => '',
      'height' => '',
      'alt' => '',
      'title' => '',
      'path' => $current_facebook_icon_path,
    );
    $facebook_icon_data = theme('image',$facebook_icon_data);
    $form['functions']['sm']['social_current_facebook_icon'] = array(
      '#markup' => '<div class="social-current-facebook-icon"><div class="description">'.t('Current Facebook icon').':</div>'.$facebook_icon_data.'</div>',
    );
  }
  $form['functions']['sm']['social_facebook_icon_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to default Facebook icon'),
    '#default_value' => $current_facebook_icon_path,
  );
  $form['functions']['sm']['social_facebook_icon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload default Facebook icon'),
    '#maxlength' => 40,
    '#size' => 0,
  );
  $current_twitter_icon_path = variable_get('general_functions_social_twitter_icon_path', '');
  if($current_twitter_icon_path) {
    $twitter_icon_data = array(
      'width' => '',
      'height' => '',
      'alt' => '',
      'title' => '',
      'path' => $current_twitter_icon_path,
    );
    $twitter_icon_data = theme('image',$twitter_icon_data);
    $form['functions']['sm']['social_current_twitter_icon'] = array(
      '#markup' => '<div class="social-current-twitter-icon"><div class="social-current-twitter-icon description">'.t('Current Twitter icon').':</div>'.$twitter_icon_data.'</div>',
    );
  }
  $form['functions']['sm']['social_twitter_icon_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to default Twitter icon'),
    '#default_value' => variable_get('general_functions_social_twitter_icon_path', ''),
  );
  $form['functions']['sm']['social_twitter_icon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload default Twitter icon'),
    '#maxlength' => 40,
    '#size' => 0,
  );

  // Inject human-friendly values for twitter and facebook icons
  foreach (array('social_facebook_icon' => '', 'social_twitter_icon' => '') as $type => $default) {
    if (isset($form['functions']['sm'][$type . '_path'])) {
      $element = &$form['functions']['sm'][$type . '_path'];
      // If path is a public:// URI, display the path relative to the files
      // directory; stream wrappers are not end-user friendly.
      $original_path = $element['#default_value'];
      $friendly_path = NULL;
      if (file_uri_scheme($original_path) == 'public') {
        $friendly_path = file_uri_target($original_path);
        $element['#default_value'] = $friendly_path;
      }
    }
  }
  $batch_operations[] = 'general_admin_settings_form_functions';
/*-------------------------------------------------*/
/*--------------Välj tema---------------*/
  $themes = list_themes();
  $options = array();
  foreach($themes as $name => $theme) {
    if($name !== 'kopernikus_admin' && $theme->status) {
      $options[$name] = $theme->info['name'];
    }
  }
  $form['general_admin_theme'] = array(
    '#title' => t('Select theme'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('theme_default', ''),
  );
  $form['general_admin_install_theme'] = array(
    '#type' => 'file',
    '#title' => t('Install a new theme'),
    '#description' => t('The following file extensions are supported: %extensions.', array(
      '%extensions' => archiver_get_extensions(),
    )),
  );
/*--------------------------------------*/
/*--------------------Språk------------------------*/
  $options = array(
    'single' => t('Single language'),
    'multi' => t('Multiple languages'),
  );
  $form['general_admin_language'] = array(
    '#title' => '',
    '#type' => 'fieldset',
  );
  $form['general_admin_language']['general_language_settings'] = array(
    '#title' => '',
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('general_language_settings', 'single'),
  );

  require_once DRUPAL_ROOT . '/includes/language.inc';
  $form['#language_types'] = language_types_configurable(FALSE);
  $form['#language_types_info'] = language_types_info();
  $form['#language_providers'] = language_negotiation_info();
  foreach($form['#language_types'] as $type) {
    _locale_languages_configure_form_language_table($form, $type);
  }
  $form['language']['#access'] = false;

  $locale_languages_configure_form = drupal_get_form('locale_languages_configure_form');
  $form_state['storage']['language_settings'] = $locale_languages_configure_form['#submit'];

  $form['general_admin_language']['locale_language_negotiation_url_part'] = array(
    '#title' => '',
    '#type' => 'select',
    '#options' => array(
      LOCALE_LANGUAGE_NEGOTIATION_URL_PREFIX => t('Path prefix'),
      LOCALE_LANGUAGE_NEGOTIATION_URL_DOMAIN => t('Domain'),
    ),
    '#default_value' => variable_get('locale_language_negotiation_url_part', LOCALE_LANGUAGE_NEGOTIATION_URL_PREFIX),
  );
  $form['general_admin_language']['locale_language_negotiation_url_part']['#access'] = false; //Ta bort när stöd för subdomäner finns

  $locale_language_providers_url_form = drupal_get_form('locale_language_providers_url_form');
  $form_state['storage']['language_negotiation'] = $locale_language_providers_url_form['#submit'];
  /*--------------------------------------------------*/
  /*--------------------Toggla responsive-----------------*/
  //system.admin.inc är redan inkluderad

  $theme_settings = variable_get('theme_kopernikus_settings', null);
  if($theme_settings) {
    $options = array(
      0 => t('Static design'),
      1 => t('Responsive design'),
    );
    $form['general_admin_responsive'] = array(
      '#title' => '',
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $theme_settings['alpha_responsive'],
    );
    $batch_operations[] = 'general_admin_settings_form_responsive';
  }
  /*--------------------------------------------------*/
  /*--------------Sätta variabler------------------*/
  $batch_operations[] = 'general_admin_settings_form_variables';
  //system.admin.inc är redan laddad
  $form['general_admin_variables'] = array(
    '#title'=>'',
    '#type' => 'fieldset',
  );
  $form['general_admin_variables']['site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#default_value' => variable_get('site_name', 'Drupal'),
    '#required' => TRUE,
  );
  $form['general_admin_variables']['site_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Site e-mail address'),
    '#default_value' => variable_get('site_mail', ini_get('sendmail_from')),
    '#description' => t("The <em>From</em> address in automated e-mails sent during registration and new password requests, and other notifications. (Use an address ending in your site's domain to help prevent this e-mail being flagged as spam.)"),
    '#required' => TRUE,
  );

  $system_site_information_settings = drupal_get_form('system_site_information_settings');
  $form_state['storage']['site_variables'] = $system_site_information_settings['#submit'];
  $form['#validate'][] = 'general_admin_settings_form_validate';
  $form['#realm_variables'] = $system_site_information_settings['#realm_variables'];
  $form['#realm_keys'] = $system_site_information_settings['#realm_keys'];

  //Hämtar den första ägaren i databasen och räknar med att det är huvudägaren
  $uid = db_select('users_roles', 'ur')
    ->fields('ur', array('uid'))
    ->condition('ur.rid', 4)
    ->range(0,1)
    ->execute()
    ->fetchField();
  if($uid) {
    $account = user_load($uid);
    module_load_include('inc', 'user', 'user.pages');
    $user_profile_form = drupal_get_form('user_profile_form', $account);
    $form_state['storage']['user_profile_form'] = $user_profile_form;
    $form['#user'] = $user_profile_form['#user'];
    $form['#user_category'] = $user_profile_form['#user_category'];
    $form['general_admin_variables']['name'] = array(
      '#type' => 'textfield',
      '#title' => t("Owner's username"),
      '#maxlength' => USERNAME_MAX_LENGTH,
      '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, apostrophes, and underscores.'),
      '#required' => TRUE,
      '#attributes' => array('class' => array('username')),
      '#default_value' => $account->name,
    );
    $form['general_admin_variables']['mail'] = array(
      '#type' => 'textfield',
      '#title' => t("Owner's e-mail address"),
      '#maxlength' => EMAIL_MAX_LENGTH,
      '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
      '#required' => TRUE,
      '#default_value' => $account->mail,
    );

    $form['#validate'] = array_merge($form['#validate'], $user_profile_form['#validate']);
  }
  
  //Logga och favikon
  $default_theme = variable_get('theme_default', 'kopernikus');
  $current_theme_settings = variable_get('theme_'.$default_theme.'_settings', array());
  $default_theme_settings = variable_get('theme_kopernikus_settings', array());

  // Logo settings
  $form['general_admin_variables']['logo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logo image settings'),
    '#description' => t('If toggled on, the following logo will be displayed.'),
    '#attributes' => array('class' => array('theme-settings-bottom')),
  );
  $current_logo = array(
    'style_name' => 'thumbnail',
    'width' => '',
    'height' => '',
    'alt' => '',
    'title' => '',
  );
  $display_current_logo = false;
  if($current_theme_settings) {
    if(!empty($current_theme_settings['default_logo'])) {
      $current_logo['path'] = 'default/druplicon.png';
      $display_current_logo = true;
    }
    else {
      if(isset($current_theme_settings['logo_path'])) {
        $current_logo['path'] = $current_theme_settings['logo_path'];
        $display_current_logo = true;
      }
    }
  }
  else {
    if(!empty($default_theme_settings['default_logo'])) {
      $current_logo['path'] = 'default/druplicon.png';
      $display_current_logo = true;
    }
    else {
      if($default_theme_settings && isset($default_theme_settings['logo_path'])) {
        $current_logo['path'] = $default_theme_settings['logo_path'];
        $display_current_logo = true;
      }
    }
  }
  if($display_current_logo) {
    $current_logo = theme('image_style',$current_logo);
    $form['general_admin_variables']['logo']['current_logo'] = array(
      '#markup' => '<div class="description">'.t('Current logo').':</div>'.$current_logo,
    );
  }
  $form['general_admin_variables']['logo']['default_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default logo'),
    '#default_value' => $current_theme_settings ? theme_get_setting('default_logo', $default_theme) : theme_get_setting('default_logo', 'kopernikus'),
    '#tree' => FALSE,
    '#description' => t('Check here if you want the theme to use the logo supplied with it.')
  );
  $form['general_admin_variables']['logo']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the logo settings when using the default logo.
      'invisible' => array(
        'input[name="default_logo"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['general_admin_variables']['logo']['settings']['logo_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom logo'),
    '#description' => t('The path to the file you would like to use as your logo file instead of the default logo.'),
    '#default_value' => $current_theme_settings ? theme_get_setting('logo_path', $default_theme) : theme_get_setting('logo_path', 'kopernikus'),
  );
  $form['general_admin_variables']['logo']['settings']['logo_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload logo image'),
    '#maxlength' => 40,
    '#size' => 0,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your logo.")
  );

  $form['general_admin_variables']['favicon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shortcut icon settings'),
    '#description' => t("Your shortcut icon, or 'favicon', is displayed in the address bar and bookmarks of most browsers."),
  );
  $current_favicon = array(
    'style_name' => 'thumbnail',
    'width' => '',
    'height' => '',
    'alt' => '',
    'title' => '',
  );
  $display_current_favicon = false;
  if($current_theme_settings) {
    if(!empty($current_theme_settings['default_favicon'])) {
      $current_favicon['path'] = 'sites/default/files/default/favicon.ico';
      $display_current_favicon = true;
    }
    else {
      if(isset($current_theme_settings['favicon_path'])) {
        $current_favicon['path'] = $current_theme_settings['favicon_path'];
        $display_current_favicon = true;
      }
    }
  }
  else {
    if(!empty($default_theme_settings['default_favicon'])) {
      $current_favicon['path'] = 'sites/default/files/default/favicon.ico';
      $display_current_favicon = true;
    }
    else {
      if(isset($default_theme_settings['favicon_path'])) {
        $current_favicon['path'] = $default_theme_settings['favicon_path'];
        $display_current_favicon = true;
      }
    }
  }
  if($display_current_favicon) {
    $current_favicon = theme('image',$current_favicon);
    $form['general_admin_variables']['favicon']['current_favicon'] = array(
      '#markup' => '<div class="description">'.t('Current favicon').':</div>'.$current_favicon,
    );
  }
  $form['general_admin_variables']['favicon']['default_favicon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default shortcut icon.'),
    '#default_value' => $current_theme_settings ? theme_get_setting('default_favicon', $default_theme) : theme_get_setting('default_favicon', 'kopernikus'),
    '#description' => t('Check here if you want the theme to use the default shortcut icon.')
  );
  $form['general_admin_variables']['favicon']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the favicon settings when using the default favicon.
      'invisible' => array(
        'input[name="default_favicon"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['general_admin_variables']['favicon']['settings']['favicon_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom icon'),
    '#description' => t('The path to the image file you would like to use as your custom shortcut icon.'),
    '#default_value' => $current_theme_settings ? theme_get_setting('favicon_path', $default_theme) : theme_get_setting('favicon_path', 'kopernikus'),
  );
  $form['general_admin_variables']['favicon']['settings']['favicon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload icon image'),
    '#size' => 0,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your shortcut icon.")
  );

  // Inject human-friendly values for logo and favicon.
  foreach (array('logo' => 'logo.png', 'favicon' => 'favicon.ico') as $type => $default) {
    if (isset($form['general_admin_variables'][$type]['settings'][$type . '_path'])) {
      $element = &$form['general_admin_variables'][$type]['settings'][$type . '_path'];

      // If path is a public:// URI, display the path relative to the files
      // directory; stream wrappers are not end-user friendly.
      $original_path = $element['#default_value'];
      $friendly_path = NULL;
      if (file_uri_scheme($original_path) == 'public') {
        $friendly_path = file_uri_target($original_path);
        $element['#default_value'] = $friendly_path;
      }
    }
  }
/*------------------------------------------------------*/  
  /*---------------Reset passwords-------------------*/
	$form['general_admin_reset_passwords'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset all passwords'),
		'#default_value' => false,
  );
  /*--------------------------------------------------*/
  /*--------------Återställ spärrade ip-adresser----------*/
  $batch_operations[] = 'general_admin_reset_flood';
  $form['general_admin_reset_flood'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('Reset blocked ip addresses'),
  	'#default_value' => false,
  );
  /*--------------Rensa testnoder---------------------*/
  $batch_operations[] = 'general_admin_settings_form_clear_test_nodes';
  $form['general_admin_clear_test_nodes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear all test nodes'),
    '#default_value' => false,
    '#description' => t('Deletes all nodes that are not indexable, with the exception of webform templates and subcontent'),
  );
  /*--------------------------------------------------*/
  /*-----------------Öppna webbplatsen------------------*/
  if(module_exists('redirect_anonymous')) {
    $batch_operations[] = 'general_admin_settings_form_open_website';
    $form['general_admin_open_website'] = array(
      '#type' => 'checkbox',
  		'#default_value' => variable_get('redirect_anonymous_open', false),
      '#title' => t('The website is open to the public'),
  	);
  }
  /*-----------------------------------------------------*/
  /*--------------Produktionstoggling--------------*/
  $options = array(
    'development' => t('Development mode'),
    'production' => t('Production mode'),
  );
  $form['general_admin_production'] = array(
    '#title' => '',
    '#type' => 'fieldset',
  );
  $form['general_admin_production']['general_admin_cache_toggle'] = array(
    '#title' => t('Caching activated'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_cache_toggle', false),
    '#weight' => -1,
  );
  $form['general_admin_production']['general_admin_cache_css_js'] = array(
    '#title' => t('Even CSS and javascript'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_cache_css_js', false),
    '#weight' => 0,
  );
  $options = array(
    'varnish' => t('Varnish'),
    'boost' => t('Boost'),
    'drupal' => t('Drupal page cache'),
  );
  if(!module_exists('vanish')) {
    unset($options['varnish']);
  }
  if(!module_exists('boost')) {
    unset($options['boost']);
  }
  $default_option = array_keys($options);
  $default_option = $default_option[0];
  $form['general_admin_production']['general_admin_pagecache_backend'] = array(
    '#title' => t('Select caching backend'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('general_pagecache_backend', $default_option),
    '#weight' => 1,
    '#disabled' => true,
  );

  if(module_exists('xmlsitemap_engines')) {
    $form['general_admin_production']['general_admin_sitemap_toggle'] = array(
      '#title' => t('XML Sitemap activated'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('general_sitemap_toggle', false),
      '#weight' => 2,
    );
  }
  /*
  $form['general_admin_production']['general_admin_provsite_toggle'] = array(
    '#title' => t('Test user activated'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_provsite_toggle', false),
    '#weight' => 3,
  );
  */
  $form['general_admin_production']['general_admin_modules_toggle'] = array(
    '#title' => t('Developer modules activated'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_modules_toggle', false),
    '#weight' => 4,
  );
  $batch_operations[] = 'general_admin_settings_form_production_toggle';
  $system_performance_settings = drupal_get_form('system_performance_settings');
  $form_state['storage']['production_toggle'] = $system_performance_settings['#submit'];
/*--------------------------------------------------*/

  $form_state['storage']['batch_operations'] = $batch_operations;
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  // By default, render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }
  return $form;
}
function general_admin_settings_form_validate(&$form, &$form_state) {
  module_load_include('inc', 'system', 'system.admin');
  //Sitens e-post
  if(isset($form_state['values']['site_mail'])) {
    if ($error = user_validate_mail($form_state['values']['site_mail'])) {
      form_set_error('site_mail', $error);
    }
  }

  //Användaruppgifter
  if(isset($form['#user'])) {
    $user_profile_form = $form_state['storage']['user_profile_form'];
    $form['account'] = $user_profile_form['account'];
    $form['account']['#access'] = false;
  }
  
  //Hantera filuppladdningar
  $files = array(
    array(
      'base_name' => 'social_facebook_icon',
      'validators' => array('file_validate_is_image' => array()),
    ),
    array(
      'base_name' => 'social_twitter_icon',
      'validators' => array('file_validate_is_image' => array()),
    ),
    array(
      'base_name' => 'logo',
      'validators' => array('file_validate_is_image' => array()),
    ),
    array(
      'base_name' => 'favicon',
      'validators' => array('file_validate_extensions' => array('ico png gif jpg jpeg apng svg')),
    ),

  );
  
  foreach($files as $file_info) {
    $file = file_save_upload($file_info['base_name'].'_upload', $file_info['validators']);
    if (isset($file)) {
      if ($file) {
        $form_state['values'][$file_info['base_name'].'_upload'] = $file;
      }
      else {
        form_set_error($file_info['base_name'].'_upload', t('The file could not be uploaded.'));
      }
    }
    if ($form_state['values'][$file_info['base_name'].'_path']) {
      $path = _system_theme_settings_validate_path($form_state['values'][$file_info['base_name'].'_path']);
      if (!$path) {
        form_set_error($file_info['base_name'].'_path', t('The custom path is invalid.'));
      }
    }
  }
  
  //Hantera temauppladdning
	$file = file_save_upload('general_admin_install_theme', array('file_validate_extensions' => array(archiver_get_extensions())));
	if (isset($file)) {
    if ($file) {
      $form_state['values']['general_admin_install_theme'] = $file;
    }
    else {
      form_set_error('general_admin_install_theme', t('The file could not be uploaded.'));
    }
	}

}
function general_admin_settings_form_submit(&$form, &$form_state) {
  //dpm($form);
  //dpm($form_state);
  $_SESSION['general_admin_settings_form'] =& $form;
  $_SESSION['general_admin_settings_form_state'] =& $form_state;

  $batch = array(
    'title' => t('Processing').'...',
    'finished' => 'general_admin_settings_form_finished',
    'file' => drupal_get_path('module', 'general_admin').'/general_admin.admin.inc',
  );
  foreach($form_state['storage']['batch_operations'] as $function) {
    $batch['operations'][] = array($function, array());
  }
  batch_set($batch);
}
function general_admin_settings_form_language_settings(&$context) {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  $old_option = variable_get('general_language_settings', 'single');
  $new_option = $form_state['values']['general_language_settings'];
  if($old_option!==$new_option) {
    include_once DRUPAL_ROOT . '/includes/language.inc';
    variable_set('general_language_settings', $new_option);
    module_load_include('inc', 'locale', 'locale.admin');
    $permissions = array('translate content');
    if($new_option==='single') {
      $form_state['values']['language']['enabled']['locale-url'] = FALSE;
      user_role_revoke_permissions(4, $permissions);
      user_role_revoke_permissions(5, $permissions);
      //Inaktivera alla definierade språk som inte är default
      general_admin_update_enabled_languages(false);
    }
    elseif($new_option==='multi') {
      $form_state['values']['language']['enabled']['locale-url'] = TRUE;
      user_role_grant_permissions(4, $permissions);
      user_role_grant_permissions(5, $permissions);
      //Aktivera alla definierade språk
      general_admin_update_enabled_languages(true);
    }
    $form_state['values']['form_id'] = 'locale_languages_configure_form';
    $form_state['storage']['language_settings'] = array_unique($form_state['storage']['language_settings']);
    foreach($form_state['storage']['language_settings'] as $function) {
      if(in_array($function, array('ocupload_change_files_status', 'system_settings_form_submit'))) continue;
      if(function_exists($function)) {
        call_user_func_array($function, array(&$form, &$form_state));
      }
    }
    $context['results']['adminpanel_rebuild'] = true;
  }

  $old_option = variable_get('locale_language_negotiation_url_part', LOCALE_LANGUAGE_NEGOTIATION_URL_PREFIX);
  $new_option = $form_state['values']['locale_language_negotiation_url_part'];
  if($old_option!=$new_option) {
    module_load_include('inc', 'locale', 'locale.admin');
    $form_state['values']['form_id'] = 'locale_language_providers_url_form';
    $form_state['storage']['language_negotiation'] = array_unique($form_state['storage']['language_negotiation']);
    foreach($form_state['storage']['language_negotiation'] as $function) {
      if(in_array($function, array('ocupload_change_files_status', 'system_settings_form_submit'))) continue;
      if(function_exists($function)) {
        call_user_func_array($function, array(&$form, &$form_state));
      }
    }
  }
}
//Uppdaterar aktiveringsstatus för språk
function general_admin_update_enabled_languages($status) {
  $languages = language_list();
  $default_langcode = language_default('language');
  $enabled_count = 0;

  foreach ($languages as $langcode => $language) {
    if($status) { //Aktivera alla definierade språk
      $enabled_count++;
      $language->enabled = 1;
    }
    else { //Inaktivera alla definierade språk som inte är defaultspråk
      if($langcode === $default_langcode) {
        $enabled_count++;
      }
      else {
        $language->enabled = 0;
      }
    }

    db_update('languages')
      ->fields(array(
        'enabled' => $language->enabled,
      ))
      ->condition('language', $langcode)
      ->execute();

    $languages[$langcode] = $language;
  }
  variable_set('language_count', $enabled_count);
}
function general_admin_settings_form_reseller(&$context) {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];

  $old_option = variable_get('general_reseller', '');
  $new_option = $form_state['values']['reseller'];
  //Array över noder som ska visas eller döljas beroende på valt alternativ
  $option_nids = array(
  	'reco' => array(215),
  );
  if($old_option !== $new_option) {
		variable_set('general_reseller', $new_option);
		$dead_nodes = variable_get('general_dead_nodes', array());
		//Uppdatera gamla noder
		if($old_option) {
			$nids = $option_nids[$old_option];
			$nodes = node_load_multiple($nids);
			foreach($nodes as $nid => $node) {
				$node->status = 0;
	      $dead_nodes[$nid] = $nid;
		    node_save($node);
			}
		}
		//Uppdatera nya noder
		if($new_option) {
			$nids = $option_nids[$new_option];
			$nodes = node_load_multiple($nids);
			foreach($nodes as $nid => $node) {
				$node->status = 1;
	      unset($dead_nodes[$nid]);
		    node_save($node);
			}
		}
    variable_set('general_dead_nodes', $dead_nodes);
    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
	}
	
  $old_option = variable_get('general_reco_id', '');
  $new_option = $form_state['values']['reco_id'];
  if($old_option !== $new_option) {
		variable_set('general_reco_id', $new_option);
	}
	
	$old_option = variable_get('general_reco_width', 900);
  $new_option = $form_state['values']['reco_widget_width'];
  if($old_option != $new_option) {
		variable_set('general_reco_width', $new_option);
	}
	
	$old_option = variable_get('general_reco_url', 'http://www.reco.se/reco-se');
  $new_option = $form_state['values']['reco_url'];
  if($old_option !== $new_option) {
		variable_set('general_reco_url', $new_option);
	}
}
function general_admin_settings_form_functions(&$context) {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];

  /*-----------------Kontaktformulär------------------*/
  $old_option = variable_get('general_functions_contact', true);
  $new_option = $form_state['values']['contact'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_contact', $new_option);

    $type = 'webform';
    $modules = array(
      'webform_improvements',
      'webform_template',
      'webform',
    );
    _general_admin_settings_form_functions_update_stuff($new_option, $type, 0, false, $modules);
		//Reglera licenstagarens rätt att skapa nodtypen
    _general_admin_settings_form_functions_update_license_permissions($type);
    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
  }
  /*-----------------------------------------------*/
  /*------------------Personallista----------------*/
  $old_option = variable_get('general_functions_stafflist', true);
  $new_option = $form_state['values']['stafflist'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_stafflist', $new_option);

    $type = 'personal';
    $sidtyp_tid = 21;
    _general_admin_settings_form_functions_update_stuff($new_option, $type, $sidtyp_tid, true);
    
    $vocabularies = taxonomy_vocabulary_get_names();
    if(isset($vocabularies['personalomrade'])) {
      $vid = $vocabularies['personalomrade']->vid;

      $permissions = array("add terms in $vid", "edit terms in $vid", "delete terms in $vid");
      if($new_option) {
        user_role_grant_permissions(4, $permissions);
        user_role_grant_permissions(5, $permissions);
      }
      else {
        user_role_revoke_permissions(4, $permissions);
        user_role_revoke_permissions(5, $permissions);   
      }
    }

    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
  }
  /*-----------------------------------------------*/
  /*-----------------Bildspel------------------*/
  $old_option = variable_get('general_functions_slideshow', true);
  $new_option = $form_state['values']['slideshow'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_slideshow', $new_option);

    $type = 'bildspel';
    $modules = array(
      'linkimagefield_caroufredsel',
      'linkimagefield_nivoslider',
    );
    _general_admin_settings_form_functions_update_stuff($new_option, $type, 0, false, $modules);
    
    //Reglera licenstagarens rätt att skapa nodtypen
    _general_admin_settings_form_functions_update_license_permissions($type);

    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
  }
  /*-----------------------------------------------*/
  /*-----------------Galleri------------------*/
  $old_option = variable_get('general_functions_gallery', true);
  $new_option = $form_state['values']['gallery'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_gallery', $new_option);

    $type = 'galleribild';
    $sidtyp_tid = 3;
    _general_admin_settings_form_functions_update_stuff($new_option, $type, $sidtyp_tid, true);

    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
  }
  /*-----------------------------------------------*/
  /*-----------------Nyheter--------------------*/
  $old_option = variable_get('general_functions_news', true);
  $new_option = $form_state['values']['news'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_news', $new_option);

    $type = 'nyhet';
    $sidtyp_tid = 2;
    _general_admin_settings_form_functions_update_stuff($new_option, $type, $sidtyp_tid, true);

    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
  }
  /*-----------------------------------------------*/
  /*-----------------Blogg--------------------*/
  $old_option = variable_get('general_functions_blog', true);
  $new_option = $form_state['values']['blog'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_blog', $new_option);

    $type = 'enterprise_blog';
    $sidtyp_tid = 4;
    $modules = array(
      'realname',
      'enterprise_blog',
      'enterprise_blog_improvements',
    );
    _general_admin_settings_form_functions_update_stuff($new_option, $type, $sidtyp_tid, true, $modules);
    
    $vocabularies = taxonomy_vocabulary_get_names();
    if(isset($vocabularies['categories']) && isset($vocabularies['tags'])) {
      $vid1 = $vocabularies['categories']->vid;
      $vid2 = $vocabularies['tags']->vid;
  
      $permissions = array("add terms in $vid1", "edit terms in $vid1", "delete terms in $vid1", "add terms in $vid2", "edit terms in $vid2", "delete terms in $vid2");
      if($new_option) {
        user_role_grant_permissions(4, $permissions);
        user_role_grant_permissions(5, $permissions);
      }
      else {
        user_role_revoke_permissions(4, $permissions);
        user_role_revoke_permissions(5, $permissions);   
      }
    }
    
    //Lägg in blocket
    $region = '-1';
    if($new_option) {
      $region = 'sidebar_second';
    }
    db_update('block')
      ->condition('module', 'views')
      ->condition('delta', array('8c9536928b1e6f7a9c9847b85a95a926', 'enterprise_blog-tags_block'))
      ->condition('theme', 'kopernikus_admin', '<>')
      ->fields(array(
        'status' => $new_option,
        'region' => $region,
      ))
      ->execute();

    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
  }
  /*-----------------------------------------------*/
  /*--------------------SOCIALA MEDIER-------------------*/
  /*------------------Facebook Like Button----------------*/
  $old_option = variable_get('general_functions_facebook_like_button', false);
  $new_option = $form_state['values']['facebook_like_button'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_facebook_like_button', $new_option);
    //Lägg in blocket i innehållsdelen på default-temat
    $region = '-1';
    if($new_option) {
      $region = 'content';
    }
    db_merge('block')
      ->key(array('delta' => '2', 'module' => 'block'))
      ->condition('theme', 'kopernikus_admin', '<>')
      ->fields(array(
        'status' => $new_option,
        'region' => $region,
      ))
      ->execute();
  }
  $old_option = variable_get('general_facebook_like_button_address', '');
  $new_option = $form_state['values']['facebook_like_button_address'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_facebook_like_button_address', $new_option);
  }
  /*------------------Facebook Like Box-------------------*/
  $old_option = variable_get('general_functions_facebook_like_box', false);
  $new_option = $form_state['values']['facebook_like_box'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_facebook_like_box', $new_option);
  }
  /*-----------------Sociala länkar-------------------*/
  $old_option = variable_get('general_functions_social_links', false);
  $new_option = $form_state['values']['social_links'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_social_links', $new_option);
  }
  /*------------------Socialt innehållsflöde----------------*/
  if(module_exists('aggregator')) {
    $old_option = variable_get('general_functions_social', false);
    $new_option = $form_state['values']['social'];
    if($old_option!=$new_option) {
      //Sätt variabeln
      variable_set('general_functions_social', $new_option);
    }
    //Anpassa beroende på individuella feeds
    _general_admin_settings_form_functions_update_social_feeds($form_state, $old_option, $new_option, 'facebook');
    _general_admin_settings_form_functions_update_social_feeds($form_state, $old_option, $new_option, 'twitter');
  }
  /*-----------------------------------------------*/
  /*-----------------Puffar--------------------*/
  $old_option = variable_get('general_functions_promo', true);
  $new_option = $form_state['values']['promo'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_promo', $new_option);

    $type = 'panel';
    $modules = array(
      'panels',
      'panels_node',
      'panels_ipe',
      'panels_improvements',
    );
    _general_admin_settings_form_functions_update_stuff($new_option, $type, 0, false, $modules);
    //Reglera licenstagarens rätt att skapa nodtypen
    _general_admin_settings_form_functions_update_license_permissions($type);

    //Flagga för att adminpanelen ska byggas om
    $context['results']['adminpanel_rebuild'] = true;
  }
  /*-----------------------------------------------*/
  /*---------------Vertikal scrollning------------------*/
  $old_option = variable_get('general_functions_vertical_scroll', false);
  $new_option = $form_state['values']['vertical_scroll'];
  if($old_option!=$new_option) {
    //Sätt variabeln
    variable_set('general_functions_vertical_scroll', $new_option);
    $modules = array(
      'vertical_scroll',
    );
    _general_admin_settings_form_functions_update_stuff($new_option, '', 0, false, $modules);
  }
  /*---------------------Tracking----------------------*/
   //Tracking settings
  $old_option = variable_get('general_admin_tracking_visibility', '');
  $new_option = $form_state['values']['tracking_pages_setting'];
  if($old_option != $new_option) {
    variable_set('general_admin_tracking_visibility', $new_option);
  }
  $old_option = variable_get('general_admin_tracking_paths', '');
  $new_option = $form_state['values']['tracking_pages_list'];
  if($old_option != $new_option) {
    variable_set('general_admin_tracking_paths', $new_option);
  }
  $old_option = variable_get('general_admin_tracking_roles', '');
  $new_option = $form_state['values']['tracking_roles'];
  if($old_option != $new_option) {
    variable_set('general_admin_tracking_roles', $new_option);
  }

  //Basic tracking
  $basic_tracking_old_option = variable_get('tracking_basic', false);
  $basic_tracking_new_option = $form_state['values']['tracking_basic'];
  if($basic_tracking_old_option != $basic_tracking_new_option) {
    //Sätt variabeln
    variable_set('tracking_basic', $basic_tracking_new_option);
    $modules = array(
      'oauth_common',
      'google_analytics_api',
      'chart',
      'google_analytics_reports',
      'google_analytics_reports_improvements',
    );
    _general_admin_settings_form_functions_update_stuff($basic_tracking_new_option, '', 0, false, $modules);
  }
  if($basic_tracking_new_option) {
    //Google Analytics
    $old_option = variable_get('googleanalytics_account', 'UA-');
    $new_option = $form_state['values']['googleanalytics_account'];
    if($old_option != $new_option) {
      if($new_option === 'UA-') {
        $new_option = '';
      }
      variable_set('googleanalytics_account', $new_option);
    }
  }
  else {
    //Google Analytics
    variable_del('googleanalytics_account');
  }
  //Advanced tracking
  $advanced_tracking_old_option = variable_get('tracking_advanced', false);
  $advanced_tracking_new_option = $form_state['values']['tracking_advanced'];
  if($advanced_tracking_old_option != $advanced_tracking_new_option) {
    //Sätt variabeln
    variable_set('tracking_advanced', $advanced_tracking_new_option);
    if($advanced_tracking_new_option) {
      //ClickTale
      $old_option = variable_get('clicktale_project_id', '');
      $new_option = $form_state['values']['clicktale_project_id'];
      if($old_option != $new_option) {
        variable_set('clicktale_project_id', $new_option);
      }
      //Visual Website Optimizer
      $old_option = variable_get('vwo_id', '');
      $new_option = $form_state['values']['vwo_id'];
      if($old_option != $new_option) {
        variable_set('vwo_id', $new_option);
      }
    }
    else {
      //ClickTale
      variable_del('clicktale_project_id');
      //Visual Website Optimizer
      variable_del('vwo_id');
    }
  }
  //Aktivera eller inaktivera trackingmodulen
  $modules = array('tracking');
  if($basic_tracking_new_option || $advanced_tracking_new_option) {
    _general_admin_settings_form_functions_update_stuff(true, '', 0, false, $modules);
  }
  else {
    _general_admin_settings_form_functions_update_stuff(false, '', 0, false, $modules);
  }
}
function _general_admin_settings_form_functions_update_social_feeds($form_state, $old_main_option, $new_main_option, $media_name) {
  switch($media_name) {
    case 'facebook':
      $variable = 'general_functions_social_facebook_id';
      $new_id_option = $form_state['values']['social_facebook_id'];
      $fid = 1;
      $url = 'https://www.facebook.com/feeds/page.php?format=atom10&id='.$new_id_option;
      $file_base_name = 'social_facebook_icon';
    break;
    case 'twitter':
      $variable = 'general_functions_social_twitter_name';
      $new_id_option = $form_state['values']['social_twitter_name'];
      $fid = 2;
      $url = 'http://api.twitter.com/1/statuses/user_timeline.rss?screen_name='.$new_id_option;
      $file_base_name = 'social_twitter_icon';
    break;
    default:
      return;
  }
  $old_id_option = variable_get($variable, '');
  if($old_id_option != $new_id_option) {
    //Sätt variabeln
    variable_set($variable, $new_id_option);
  }
  if($new_main_option) {
    if($old_id_option != $new_id_option) {
      if($new_id_option) {
        //Positivt ID, uppdatera aggregator feed
        $feed = aggregator_feed_load($fid);
        aggregator_remove($feed);
        $feed->url = $url;
        $edit = get_object_vars($feed);
        aggregator_save_feed($edit);
        aggregator_refresh($feed);
      }
    }
    //Uppdatera standardikon
    module_load_include('inc', 'system', 'system.admin');
    $values =& $form_state['values'];
    if ($file = $values[$file_base_name.'_upload']) {
      unset($values[$file_base_name.'_upload']);
      $filename = file_unmanaged_copy($file->uri);
      $values[$file_base_name.'_path'] = $filename;
    }
    if (!empty($values[$file_base_name.'_path'])) {
      $values[$file_base_name.'_path'] = _system_theme_settings_validate_path($values[$file_base_name.'_path']);
    }
    variable_set('general_functions_'.$file_base_name.'_path', $values[$file_base_name.'_path']);
  }
}
function _general_admin_settings_form_functions_update_license_permissions($type = '') {
	//Reglera licenstagarens rätt att skapa nodtypen
	if($type) {
    $permissions = array("create $type content");
    if($new_option) {
			user_role_grant_permissions(5, $permissions);
		}
		else {
			user_role_revoke_permissions(5, $permissions);
		}
	}
}
function _general_admin_settings_form_functions_update_stuff($new_option = false, $type = '', $sidtyp_tid = 0, $update_standard_permissions = false, $update_these_modules = array()) {
  /* Lite dokumentation
  _general_admin_settings_form_functions_update_stuff(
    $new_option = false,
    $type = '',
    $sidtyp_tid = 0,
    $update_standard_permissions = false,
    $update_these_modules = array()
  )
  */

  //Ändra meta tag settings och permissions, uppdatera moduler
  if($type) {
    $adminpanel_allowed_types = variable_get('adminpanel_node_types', array());
  }
  if($sidtyp_tid) {
    $adminpanel_allowed_sidtyp = variable_get('adminpanel_sidtyper', array());
  }
  if($update_standard_permissions) {
    $permissions = array("create $type content", "edit any $type content", "delete any $type content");
  }
  if($new_option) {
    if($type) {
      $adminpanel_allowed_types[$type] = $type;
    }
    if($sidtyp_tid) {
      $adminpanel_allowed_sidtyp[$sidtyp_tid] = $sidtyp_tid;
    }
    if($update_standard_permissions) {
      user_role_grant_permissions(4, $permissions);
      user_role_grant_permissions(5, $permissions);
    }
    if($update_these_modules) {
      module_enable($update_these_modules);
    }
  }
  else {
    if($type) {
      $adminpanel_allowed_types[$type] = 0;
    }
    if($sidtyp_tid) {
      $adminpanel_allowed_sidtyp[$sidtyp_tid] = 0;
    }
    if($update_standard_permissions) {
      user_role_revoke_permissions(4, $permissions);
      user_role_revoke_permissions(5, $permissions);
    }
    if($update_these_modules) {
      module_disable($update_these_modules);
    }
  }
  if($type) {
    variable_set('adminpanel_node_types', $adminpanel_allowed_types);
  }
  if($sidtyp_tid) {
    variable_set('adminpanel_sidtyper', $adminpanel_allowed_sidtyp);
  }
  //Uppdatera nodstatus
  $nids = array();
  if($type) {
    $result = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type', $type)
    ->execute();
    if($result) {
      while($nid = $result->fetchField()) {
        $nids[] = $nid;
      }
    }
  }
  if($sidtyp_tid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
    ->fieldCondition('field_sidtyp', 'tid', $sidtyp_tid);
    $result = $query->execute();
    if (isset($result['node'])) {
      $nids = array_merge($nids, array_keys($result['node']));
    }
  }
  if($nids) {
    $dead_nodes = variable_get('general_dead_nodes', array());
    $nodes = node_load_multiple($nids);
    foreach($nodes as $nid => $node) {
      $node->status = $new_option;
      node_save($node);
      if($new_option) {
        unset($dead_nodes[$nid]);
      }
      else {
        $dead_nodes[$nid] = $nid;
      }
    }
    variable_set('general_dead_nodes', $dead_nodes);
  }
}
function general_admin_settings_form_production_toggle() {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  
  $default_option = array_keys($form['general_admin_production']['general_admin_pagecache_backend']['#options']);
  $default_option = $default_option[0];
  $old_backend_cache_option = variable_get('general_pagecache_backend', $default_option);
  $new_backend_cache_option = $form_state['values']['general_admin_pagecache_backend'];
  if($old_backend_cache_option!==$new_backend_cache_option) {
    variable_set('general_pagecache_backend', $new_backend_cache_option);
  }

  $old_caching_option = variable_get('general_cache_toggle', false);
  $new_caching_option = $form_state['values']['general_admin_cache_toggle'];
  if($old_caching_option!==$new_caching_option || $old_backend_cache_option!==$new_backend_cache_option) {
    variable_set('general_cache_toggle', $new_caching_option);
    if($new_caching_option) {
      general_admin_settings_form_toggle_cache('on', $new_backend_cache_option);
    }
    else {
      general_admin_settings_form_toggle_cache('off', $new_backend_cache_option);
    }
  }

  $old_cache_cssjs_option = variable_get('general_cache_css_js', false);
  $new_cache_cssjs_option = $form_state['values']['general_admin_cache_css_js'];
  if($old_cache_cssjs_option!==$new_cache_cssjs_option) {
    variable_set('general_cache_css_js', $new_cache_cssjs_option);
    if($new_cache_cssjs_option) {
      variable_set('preprocess_css', 1);
      variable_set('preprocess_js', 1);
    }
    else {
      variable_set('preprocess_css', 0);
      variable_set('preprocess_js', 0);
    }
  }
  
  if($old_caching_option!==$new_caching_option || $old_cache_cssjs_option!==$new_cache_cssjs_option) {
    module_load_include('inc', 'system', 'system.admin');
    $form_state['values']['form_id'] = 'system_performance_settings';
    $form_state['storage']['production_toggle'] = array_unique($form_state['storage']['production_toggle']);
    foreach($form_state['storage']['production_toggle'] as $function) {
      if(in_array($function, array('ocupload_change_files_status', 'system_settings_form_submit'))) continue;
      if(function_exists($function)) {
        call_user_func_array($function, array(&$form, &$form_state));
      }
    }
  }
  if(isset($form_state['values']['general_admin_sitemap_toggle'])) {
    $old_sitemap_option = variable_get('general_sitemap_toggle', false);
    $new_sitemap_option = $form_state['values']['general_admin_sitemap_toggle'];
    if($old_sitemap_option!==$new_sitemap_option) {
      variable_set('general_sitemap_toggle', $new_sitemap_option);
      if($new_sitemap_option) {
        global $base_url;
        variable_set('xmlsitemap_base_url', $base_url);
        $engines = array('bing', 'google');
        variable_set('xmlsitemap_engines_engines', $engines);

      }
      else {
        variable_set('xmlsitemap_engines_engines', array());
      }
    }
  }

	/*
  $old_provsite_option = variable_get('general_provsite_toggle', false);
  $new_provsite_option = $form_state['values']['general_admin_provsite_toggle'];
  if($old_provsite_option!==$new_provsite_option) {
    variable_set('general_provsite_toggle', $new_provsite_option);
    if($new_provsite_option) {
      module_enable(array('provsite' => 'provsite'));
    }
    else {
      module_disable(array('provsite' => 'provsite'));
    }
  }
  */


  $old_modules_option = variable_get('general_modules_toggle', false);
  $new_modules_option = $form_state['values']['general_admin_modules_toggle'];
  if($old_modules_option!==$new_modules_option) {
    variable_set('general_modules_toggle', $new_modules_option);
    $module_list = array(
      'field_ui' => 'field_ui',
      'bulk_export' => 'bulk_export',
      'rules_admin' => 'rules_admin',
      'omega_tools' => 'omega_tools',
      'devel' => 'devel',
      'views_ui' => 'views_ui',
    );
    if($new_modules_option) {
      module_enable($module_list);
    }
    else {
      module_disable($module_list);
    }
  }
}
function general_admin_settings_form_theme() {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  $old_option = variable_get('theme_default', '');
  $new_option = $form_state['values']['general_admin_theme'];
  if($old_option !== $new_option) {
    variable_set('theme_default', $new_option);
  }

  //Uppladdning av tema
  $directory = 'sites/all/themes/public_themes';
  $values =& $form_state['values'];
  if ($file = $values['general_admin_install_theme']) {
    unset($values['general_admin_install_theme']);

    $filename = file_unmanaged_copy($file->uri, $directory);
    if($filename) {
	    $archiver = archiver_get_archiver($filename);
	    if ($archiver) {
				$files = $archiver->listContents();
				$theme_name = strtok($files[0], './\\');
				$extract_location = $directory . '/' . $theme_name;
			  if (file_exists($extract_location)) {
			    file_unmanaged_delete_recursive($extract_location);
			  }

			  $archiver->extract($directory);
			  //Enabla temat och sätt det som defaulttema
			  system_rebuild_theme_data();
			  theme_enable(array($theme_name));
			  variable_set('theme_default', $theme_name);
			  drupal_set_message(t('Theme was installed successfully!'));
			}
			else {
				drupal_set_message(t('Theme was uploaded but is not a supported archive type.'), 'error');
			}
			file_unmanaged_delete($filename);
		}

/*
function update_manager_archive_extract($file, $directory) {
  $archiver = archiver_get_archiver($file);
  if (!$archiver) {
    throw new Exception(t('Cannot extract %file, not a valid archive.', array ('%file' => $file)));
  }

  // Remove the directory if it exists, otherwise it might contain a mixture of
  // old files mixed with the new files (e.g. in cases where files were removed
  // from a later release).
  $files = $archiver->listContents();

  // Unfortunately, we can only use the directory name to determine the project
  // name. Some archivers list the first file as the directory (i.e., MODULE/)
  // and others list an actual file (i.e., MODULE/README.TXT).
  $project = strtok($files[0], '/\\');

  $extract_location = $directory . '/' . $project;
  if (file_exists($extract_location)) {
    file_unmanaged_delete_recursive($extract_location);
  }

  $archiver->extract($directory);
  return $archiver;
}
*/
  }
}
function general_admin_settings_form_responsive() {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  $theme_settings = variable_get('theme_kopernikus_settings', null);
  $old_option = $theme_settings['alpha_responsive'];
  $new_option = $form_state['values']['general_admin_responsive'];
  if($old_option!==$new_option) {
    $themes = list_themes();
    foreach($themes as $name => $theme) {
      if($theme->status) {
        $settings = variable_get('theme_'.$name.'_settings', array());
        if(isset($settings['alpha_responsive'])) {
          $settings['alpha_responsive'] = $new_option;
          variable_set('theme_'.$name.'_settings', $settings);
        }
      }
    }
    variable_set('general_responsive', $new_option);
  }
}
function general_admin_settings_form_variables() {
  global $theme_key;
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];

  global $language;
  $form_state['values']['i18n_variable_language'] = $language;

  module_load_include('inc', 'system', 'system.admin');
  $form_state['values']['form_id'] = 'system_site_information_settings';
  $form_state['storage']['site_variables'] = array_unique($form_state['storage']['site_variables']);
  foreach($form_state['storage']['site_variables'] as $function) {
    if(in_array($function, array('ocupload_change_files_status', 'system_settings_form_submit'))) continue;
    if(function_exists($function)) {
      call_user_func_array($function, array(&$form, &$form_state));
    }
  }
  $old_option = variable_get('site_name', '');
  $new_option = $form_state['values']['site_name'];
  if($old_option!==$new_option) {
    variable_set('site_name', $new_option);
  }
  $old_option = variable_get('site_mail', '');
  $new_option = $form_state['values']['site_mail'];
  if($old_option!==$new_option) {
    variable_set('site_mail', $new_option);
  }

  if(isset($form['#user'])) {
    $edit = array();
    if($form_state['values']['name']!==$form['#user']->name) {
      $edit['name'] = $form_state['values']['name'];
    }
    if($form_state['values']['mail']!==$form['#user']->mail) {
      $edit['mail'] = $form_state['values']['mail'];
    }
    if($edit) {
      user_save($form['#user'], $edit);
    }
  }

  //Logga och favikon
  $values = $form_state['values'];
  $toggle_logo = module_exists('logo_block') ? false : true;

  // If the user uploaded a new logo or favicon, save it to a permanent location
  // and use it in place of the default theme-provided file.
  if ($file = $values['logo_upload']) {
    unset($values['logo_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['default_logo'] = 0;
    $values['logo_path'] = $filename;
    if($toggle_logo) {
      $values['toggle_logo'] = 1;
    }
  }
  if ($file = $values['favicon_upload']) {
    unset($values['favicon_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['default_favicon'] = 0;
    $values['favicon_path'] = $filename;
    $values['toggle_favicon'] = 1;
  }

  // If the user entered a path relative to the system files directory for
  // a logo or favicon, store a public:// URI so the theme system can handle it.
  if (!empty($values['logo_path'])) {
    $values['logo_path'] = _system_theme_settings_validate_path($values['logo_path']);
  }
  if (!empty($values['favicon_path'])) {
    $values['favicon_path'] = _system_theme_settings_validate_path($values['favicon_path']);
  }

  if (empty($values['default_favicon']) && !empty($values['favicon_path'])) {
    $values['favicon_mimetype'] = file_get_mimetype($values['favicon_path']);
  }
  $current_settings = variable_get('theme_'.$theme_key.'_settings', array());
  $changed = false;
  if($current_settings) {
    $vars = array(
      'default_logo' => 'default_logo',
      'logo_path' => 'logo_path',
      'default_favicon' => 'default_favicon',
      'favicon_path' => 'favicon_path',
      'toggle_favicon' => 'toggle_favicon',
      'favicon_mimetype' => 'favicon_mimetype',
    );
    if($toggle_logo) {
      $vars['toggle_logo'] = 'toggle_logo';
    }
    foreach($vars as $var) {
      if(isset($current_settings[$var])) {
        if(isset($values[$var])) {
          if($values[$var] !== $current_settings[$var]) {
            $changed = true;
            break;
          }
        }
      }
      else {
        $changed = true;
        break;
      }
    }
  }
  if($changed) {
    $themes = list_themes();
    foreach($themes as $name => $theme) {
      if($theme->status) {
        $settings = variable_get('theme_'.$name.'_settings', array());
        if(isset($values['default_logo'])) $settings['default_logo'] = $values['default_logo'];
        if(isset($values['logo_path'])) $settings['logo_path'] = $values['logo_path'];
        if($toggle_logo) {
          if(isset($values['toggle_logo'])) $settings['toggle_logo'] = $values['toggle_logo'];
        }
        if(isset($values['default_favicon'])) $settings['default_favicon'] = $values['default_favicon'];
        if(isset($values['favicon_path'])) $settings['favicon_path'] = $values['favicon_path'];
        if(isset($values['toggle_favicon'])) $settings['toggle_favicon'] = $values['toggle_favicon'];
        if(isset($values['favicon_mimetype'])) $settings['favicon_mimetype'] = $values['favicon_mimetype'];
        variable_set('theme_'.$name.'_settings', $settings);
      }
    }
  }
}
function general_admin_settings_form_reset_passwords() {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  if(isset($form_state['values']['general_admin_reset_passwords']) && $form_state['values']['general_admin_reset_passwords']) {
    general_admin_resetpasswords();
  }
}
function general_admin_resetpasswords() {
  global $user;
  $excluded_users = array(
    0,
    3,
  );
  if($user->uid != 1) {
    $excluded_users[] = 1;
  }
  $result = db_select('users', 'u')
  ->distinct()
  ->fields('u', array('uid'))
  ->condition('u.uid', $excluded_users, 'NOT IN')
  ->execute();

	$body = '<p>'.utf8_encode('Nya användaruppgifter').':</p><p>';
	while($uid = $result->fetchField()) {
		$pass = general_generate_random_string(8);
		$tempuser = user_load($uid);
		$body .= utf8_encode('Användarnamn').': '.$tempuser->name.'<br />'.utf8_encode('Lösenord').': '.$pass.'<br /><br />';
		user_save($tempuser, array('pass'=>$pass));
	}
	$body .= '</p>';
  drupal_set_message($body);
}
function general_admin_reset_flood() {
	$form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  if(isset($form_state['values']['general_admin_reset_flood']) && $form_state['values']['general_admin_reset_flood']) {
		db_truncate('flood')->execute();
	}
}
function general_admin_settings_form_clear_test_nodes() {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  if(isset($form_state['values']['general_admin_clear_test_nodes']) && $form_state['values']['general_admin_clear_test_nodes']) {
    //Tar bort noder som inte använder metataggar
    $types = node_type_get_names();
    $indexed_types = variable_get('metatag_improvements_allowed_types', array_keys($types));
    $temp = array_filter($indexed_types, 'general_admin_array_filter_inverse');
    //Undantag: Webform Template och Subinnehåll
    if(isset($temp['webform_template'])) {
      unset($temp['webform_template']);
    }
    if(isset($temp['subinnehall'])) {
      unset($temp['subinnehall']);
    }
    //Tillägg: Nyheter och blogginlägg
    if(isset($indexed_types['nyhet'])) {
      $temp['nyhet'] = $indexed_types['nyhet'];
    }
    if(isset($indexed_types['enterprise_blog'])) {
      $temp['enterprise_blog'] = $indexed_types['enterprise_blog'];
    }
    $unindexed_types = array();
    foreach($temp as $key => $value) {
      $unindexed_types[] = $key;
    }
    $result = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->condition('type', $unindexed_types)
      ->execute();
    $nids = array();
    while($nid = $result->fetchField()) {
      $nids[] = $nid;
    }
    //Tar också bort alla noder som inte hör till aktiverade språk
    $excluded_languages = array();
    $language_list = language_list();
    foreach($language_list as $lang => $language) {
      if(!$language->enabled) {
        $excluded_languages[] = $lang;
      }
    }
    //Undantag: Webform templates och subinnehåll
    $type_exceptions = array('webform_template', 'subinnehall');
    if($excluded_languages) { //Det finns inaktiverade språk
      $result = db_select('node', 'n')
        ->fields('n', array('nid'))
        ->condition('language', $excluded_languages)
        ->condition('type', $type_exceptions, 'NOT IN')
        ->execute();
      while($nid = $result->fetchField()) {
        $nids[] = $nid;
      }
    }
    $nids = array_unique($nids);

    node_delete_multiple($nids);
  }
}
function general_admin_array_filter_inverse($var) {
  return !$var;
}
function general_admin_settings_form_open_website() {
  $form =& $_SESSION['general_admin_settings_form'];
  $form_state =& $_SESSION['general_admin_settings_form_state'];
  $old_option = variable_get('redirect_anonymous_open', false);
  $new_option = $form_state['values']['general_admin_open_website'];
  if($old_option!=$new_option) {
    variable_set('redirect_anonymous_open', $new_option);
  }
  $image = variable_get('general_image', '');
  if(!$image) variable_set('general_image', 'http://storleden.se/ajax/lsd-cron/public.png');
}
function general_admin_settings_form_finished($success, $results, $operations) {
  general_admin_settings_form_reset_passwords();
  unset($_SESSION['general_admin_settings_form']);
  unset($_SESSION['general_admin_settings_form_state']);
  if(isset($results['adminpanel_rebuild']) && $results['adminpanel_rebuild']) {
    $_SESSION['adminpanel_rebuild'] = true;
  }
  else {
    $_SESSION['adminpanel_rebuild'] = false;
  }
  drupal_flush_all_caches();
}
function general_admin_settings_form_toggle_cache($op, $type) {
  switch($op) {
    case 'on':
      switch($type) {
        case 'varnish':
          variable_set('cache', 0);
          variable_set('page_compression', 0);
          break;

        case 'boost':
          if(module_exists('boost')) {
            variable_set('boost_enabled_application/rss', 1);
            variable_set('boost_enabled_application/rss+xml', 1);
            variable_set('boost_enabled_application/xml', 1);
            variable_set('boost_enabled_text/html', 1);
            variable_set('boost_enabled_text/xml', 1);
          }

          variable_set('cache', 0);
          variable_set('page_compression', 0);
          break;

        case 'drupal':
          if(module_exists('boost')) {
            variable_set('boost_enabled_application/rss', 0);
            variable_set('boost_enabled_application/rss+xml', 0);
            variable_set('boost_enabled_application/xml', 0);
            variable_set('boost_enabled_text/html', 0);
            variable_set('boost_enabled_text/xml', 0);
          }

          variable_set('cache', 1);
          variable_set('page_compression', 1);
          break;
      }
      break;

    case 'off':
      switch($type) {
        case 'varnish':
          if(module_exists('boost')) {
            variable_set('boost_enabled_application/rss', 0);
            variable_set('boost_enabled_application/rss+xml', 0);
            variable_set('boost_enabled_application/xml', 0);
            variable_set('boost_enabled_text/html', 0);
            variable_set('boost_enabled_text/xml', 0);
          }
          break;

        case 'boost':
          break;

        case 'drupal':
          variable_set('cache', 0);
          variable_set('page_compression', 0);
          break;
      }
      break;
  }
}
function general_admin_finetune_form($form, $form_state, $rid = NULL) {
  $form['general_thindoc_toggle'] = array(
    '#title' => t('Open PDF files in integrated third-party reader'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('general_thindoc_toggle', false),
  );

  return system_settings_form($form);
}
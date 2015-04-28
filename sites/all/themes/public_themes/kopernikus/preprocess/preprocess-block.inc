<?php
function kopernikus_alpha_preprocess_block(&$vars) {
	//Tar bort clearfixklassen i block-content
	if(isset($vars['content_attributes_array']) && isset($vars['content_attributes_array']['class'])) {
		$key = array_search('clearfix', $vars['content_attributes_array']['class']);
		if($key !== false) {
			unset($vars['content_attributes_array']['class'][$key]);
		}
	}
}
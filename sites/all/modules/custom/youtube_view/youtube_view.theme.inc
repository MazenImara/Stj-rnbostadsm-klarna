<?php

function theme_youtube_view($variables) {
	$output = '';
	if(!empty($variables['id_list'])) {
		$id_list = $variables['id_list'];
		$output .= '<ul class="video-list">';
		for($i = 0, $itemcount = count($id_list); $i < $itemcount; $i++) {
			$width = '200';
			$height = '200';
			$classes = array('video-list-item video-list-item-'.($i+1));
			if($i % 2 == 0) {
				$classes[] = 'odd';
			}
			else {
				$classes[] = 'even';
			}
			if($i == 0) {
				$classes[] = 'first';
				$width = '720';
				$height = '360';
			}
			elseif($i == $itemcount - 1) {
				$classes[] = 'last';
			}
			$output .= '<li class="'.implode(' ', $classes).'">';
			$output .= '<iframe width="'.$width.'" height="'.$height.'" src="http://www.youtube.com/embed/'.$id_list[$i].'" frameborder="0" allowfullscreen></iframe>';
			$output .= '</li>';
		}
	}
  return $output;
}

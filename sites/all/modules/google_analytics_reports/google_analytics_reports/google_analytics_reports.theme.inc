<?php
/**
 * @file
 * Theme preprocess functions for google analytics.
 */

function template_preprocess_google_analytics_reports_summary(&$vars) {
  $vars = array_merge($vars, $vars['usage']);

  if ($vars['sessions'] <= 0) {
    $vars['pages_per_visit'] = '0.00';
    $vars['new_visits'] = '0.00%';
    $vars['time_on_site'] = '0:00';
  }
  else {
    $vars['pages_per_visit'] = number_format(round($vars['pageviews'] / $vars['sessions'], 2), 2);
    $vars['new_visits'] = number_format(round(($vars['newUsers'] / $vars['sessions']) * 100, 2), 2) . '%';
    $vars['time_on_site'] = format_interval($vars['sessionDuration'] / $vars['sessions']);
  }

  if ($vars['entrances'] <= 0) {
    $vars['bounces'] = '0.00%';
  }
  else {
    $vars['bounces'] = number_format(round(($vars['bounces'] / $vars['entrances']) * 100, 2), 2) . '%';
  }

  $vars['entrances'] = number_format($vars['entrances']);
  $vars['pageviews'] = number_format($vars['pageviews']);

  $pages = array();
  foreach ($vars['pages'] as $page) {
    if ($page['pagePath'] == '/index.html') {
      $page['pagePath'] = variable_get('google_analytics_reports_default_page', '/');
    }
    $pages[] = l($page['pageTitle'], "http://$page[hostname]$page[pagePath]", array('external' => TRUE, 'html' => TRUE)) . ' - ' . format_plural($page['pageviews'], '1 View', '@views Views', array('@views' => number_format($page['pageviews'])));
  }
  $vars['pages'] = theme('item_list', array('items' => $pages, 'type' => 'ol'));

  $referrals = array();
  foreach ($vars['referrals'] as $referral) {
    $referrals[] = '<strong>' . $referral['source'] . '</strong> - ' . format_plural($referral['sessions'], '1 Session', '@sessions Sessions', array('@sessions' => number_format($referral['sessions'])));
  }
  $vars['referrals'] = theme('item_list', array('items' => $referrals, 'type' => 'ol'));

  $keywords = array();
  foreach ($vars['keywords'] as $keyword) {
    $keywords[] = '<strong>' . $keyword['keyword'] . '</strong> - ' . format_plural($keyword['sessions'], '1 Session', '@sessions Sessions', array('@sessions' => number_format($keyword['sessions'])));
  }
  $vars['keywords'] = theme('item_list', array('items' => $keywords, 'type' => 'ol'));
}

function template_preprocess_google_analytics_reports_detail(&$vars) {
  $vars['pageviews'] = number_format($vars['stats']['pageviews']);
  $vars['unique_pageviews'] = number_format($vars['stats']['uniquePageviews']);
  $vars['avg_time_on_page'] = format_interval($vars['stats']['avgTimeOnPage']);
  $vars['bounce_rate'] = round($vars['stats']['bounceRate'], 2);
  $vars['exit_rate'] = round($vars['stats']['exitRate'], 2);
  if ($vars['stats']['uniquePageviews'] > 0) {
    $vars['goal_value'] = money_format('%i', $vars['stats']['goalValueAll'] / $vars['stats']['uniquePageviews']);
  }
  else {
    $vars['goal_value'] = '0.00';
  }

  $referrals = array();
  foreach ($vars['referrals'] as $referral) {
    $referrals[] = '<strong>' . $referral['source'] . '</strong> - ' . format_plural($referral['sessions'], '1 Session', '@sessions Sessions', array('@sessions' => number_format($referral['sessions'])));
  }
  $vars['referrals'] = theme('item_list', array('items' => $referrals, 'type' => 'ol'));

  $keywords = array();
  foreach ($vars['keywords'] as $keyword) {
    $keywords[] = '<strong>' . $keyword['keyword'] . '</strong> - ' . format_plural($keyword['sessions'], '1 Session', '@sessions Sessions', array('@sessions' => number_format($keyword['sessions'])));
  }
  $vars['keywords'] = theme('item_list', array('items' => $keywords, 'type' => 'ol'));
}

function template_preprocess_google_analytics_reports_dashboard(&$vars) {
  $pages = array();
  foreach ($vars['pages'] as $page) {
    if ($page['pagePath'] == '/index.html') {
      $page['pagePath'] = variable_get('google_analytics_reports_default_page', '/');
    }
    $pages[] = l($page['pageTitle'], "http://$page[hostname]$page[pagePath]", array('external' => TRUE, 'html' => TRUE)) . ' - ' . format_plural($page['pageviews'], '1 View', '@views Views', array('@views' => number_format($page['pageviews'])));
  }
  $vars['pages'] = theme('item_list', array('items' => $pages, 'type' => 'ol'));
}

/**
 * Theme function for path_mini block.
 */
function theme_google_analytics_reports_path_mini($report) {
  return $report['chart'] . '<p>' . $report['views'] . '</p>';
}

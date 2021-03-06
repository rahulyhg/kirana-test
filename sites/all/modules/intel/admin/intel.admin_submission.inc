<?php
/**
 * @file
 * Administration of submission data
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */

function intel_submission_page($sid) {
  
  $submission = intel_submission_load($sid);
  $visitor = intel_visitor_load((int)$submission->vid);
  //if (!empty($submission->visitorid)) {
    //$visitor = intel_visitor_load_by_visitorid($submission->visitorid);
  //}
  if (!empty($_GET['debug'])) {
    dpm($submission);//
  }
  $type = str_replace('_', ' ', $submission->type);
  drupal_set_title(t('@type submission', array('@type' => $type)), PASS_THROUGH);  
  $output = '';
  
  $form = array();
  
  // TODO encapsulate this back into hook in module or add field to store in submission table
  $link = '';
  if ($submission->type == 'webform') {
    $link = l(t('View submission details'), "node/{$submission->fid}/submission/{$submission->fsid}");
  }
  elseif ($submission->type == 'disqus_comment') {
    $link = l(t('View comment'), substr($submission->form_page_path, 1), array( 'fragment' => "comment-{$submission->fsid}"));
  }
  
  if ($link) {
    $form['data']['photo'] = array(
      '#type' => 'markup',
      '#markup' => $link . "<br>\n<br>\n",
    );
  }
  
  if ($visitor->name) {
    $form['submitted_by'] = array(
      '#type' => 'item',
      '#title' => t('Submitted by'),
      '#markup' => l($visitor->name, 'visitor/' . $visitor->vtkid),
    );
  }
  elseif ($visitor->visitorid) {
    $form['submitted_by'] = array(
      '#type' => 'item',
      '#title' => t('Submitted by'),
      '#markup' => l($visitor->getLabel(), 'visitor/' . $visitor->vtkid),
    );
  }
  else {
    $form['submitted_by'] = array(
      '#type' => 'item',
      '#title' => t('Submitted by'),
      '#markup' => t('(not set)'),
    );    
  }
  
  $form['submitted_at'] = array(
    '#type' => 'item',
    '#title' => t('Submitted at'),
    '#markup' => date("Y-m-d H:i"),
  ); 

  $url = "http://" . $submission->response_page_host . $submission->response_page_path;
  if ($submission->response_page_id) {
    $url = 'node/' . $submission->response_page_id;
  }
  $form['reponse_page'] = array(
      '#type' => 'item',
      '#title' => t('Response page'),
      '#markup' => l(url($url), $url),
  );
  
  if ($submission->form_page_path) {
    $url = "http://" . $submission->form_page_host . $submission->form_page_path;
    if ($submission->form_page_id) {
      $url = 'node/' . $submission->form_page_id;
    }
    $markup = l(url($url), $url);
  }
  else {
    $markup = '(not set)';
  }  
  $form['form_page'] = array(
    '#type' => 'item',
    '#title' => t('Form page'),
    '#markup' => $markup,
  );

  if ($submission->cta_page_path) {
    $url = "http://" . $submission->cta_page_host . $submission->cta_page_path;
    if ($submission->form_page_id) {
      $url = 'node/' . $submission->cta_page_id;
    }
    $markup = l(url($url), $url);
  }
  else {
    $markup = '(not set)';
  }  
  $form['cta_page'] = array(
    '#type' => 'item',
    '#title' => t('CTA page'),
    '#markup' => $markup,
  ); 

  if ($submission->cta_id) {
    $url = 'block/' . $submission->cta_id;
    $markup = l(url($url), $url);
  }
  else {
    $markup = '(not set)';
  }  
  $form['cta'] = array(
    '#type' => 'item',
    '#title' => t('CTA clicked'),
    '#markup' => $markup,
  );   
  
  $output = render($form);  
  
  return $output;
}

function intel_admin_people_submissions($filter = array()) {
  $output = "";
  $api_level = variable_get('intel_api_level', '');
  $header = array();
  $header[] = array(
    'data' => t('Name'),
    'field' => 'v.name',
  );
  if ($api_level == 'pro') {
    $header[] = array(
      'data' => t('Location'),
    );
  }
  $header[] = array(
      'data' => t('Submitted'),
      'field' => 's.submitted',
      'sort' => 'desc',
  );
  $header[] = array(
    'data' => t('Type'),
    'field' => 's.type',
  );
  $header[] = array(
    'data' => t('Form'),
    //'field' => 's.type',
  );
  $header[] = array(
    'data' => t('Operations'),
  );

  if (empty($filter['conditions'])) {
    $filter['conditions'] = array(
      array('s.vid', 0, '!='),
    );
  }
  $options = array();
  $result = intel_load_filtered_submission_result($filter, $options, $header, 50);

  $hs_portal_id = variable_get('hubspot_portalid', '');
  
  $webforms = array();

  $submissions = array();
  $vids = array();
  while ($r = $result->fetchObject()) {
    $vids[$r->vid] = $r->vid;
    $submissions[$r->sid] = $r;
  }
  $vids = array_values($vids);
  $visitors = intel_visitor_load_multiple($vids);

  $rows = array();
  foreach ($submissions AS $sid => $submission) {

    // if visitor doesn't exist (anymore) skip
    if (empty($visitors[$submission->vid])) {
      continue;
    }
    $visitor = $visitors[$submission->vid];

    $row = array();
    $ops = l(t('meta'), 'submission/' . $submission->sid);
    $row[] = $visitor->label_link();
    if ($api_level == 'pro') {      
      $row[] = $visitor->location();
    }
    $row[] = ((REQUEST_TIME - $submission->submitted) > 604800) ? format_date($submission->submitted, 'short') : format_interval(REQUEST_TIME - $submission->submitted) . t(' ago');
    $row[] = $submission->type;
    if ($submission->type == 'webform') {
      if (!isset($webforms[$submission->fid])) {
        $webform[$submission->fid] = node_load($submission->fid);
      }
      $row[] = l($webform[$submission->fid]->title, 'node/' . $submission->fid);
      $ops .= ' | ' . l(t('submission'), 'node/' . $submission->fid . '/submission/' . $submission->fsid);
    }
    else if ($submission->type == 'disqus_comment') {
      $a = explode('#', substr($submission->details_url, 1));
      $options = array(
        'fragment' => isset($a[1]) ? $a[1] : '',
      );
      $row[] = l(t('Comment'), $a[0], $options);
      $ops .= ' | ' . l(t('comment'), $a[0], $options);
    }
    else if ($submission->type == 'hubspot') {
      $form_name = intel_hubspot_get_form_name($submission->fid);
      $row[] = ($form_name) ? $form_name : 'NA';
    }
    else {
      $row[] = 'NA';
    }
    $row[] = $ops;
    $rows[] = $row;
  }

  $vars = array(
    'header' => $header, 
    'rows' => $rows, 
    'attributes' => array('id' => 'kwresearch-site-keywords')
  );
  $output .= theme('table', $vars);
  //pager_default_initialize($total, 1, $element = 0);
  $output .= theme('pager');

  return $output;
}
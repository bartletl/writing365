<?php

/**
 * @file
 * Page callback file for the Writing365 module.
 */

/**
 * Menu callback; displays a Drupal page containing recent journal entries of a given user.
 */
function writing365_page_user($account) {
  global $user;

  drupal_set_title($title = t("@name's Writing 365 Journal", array('@name' => format_username($account))), PASS_THROUGH);

  $build = array();

  $query = db_select('node', 'n')->extend('PagerDefault');
  $nids = $query
    ->fields('n', array('nid', 'sticky', 'created'))
    ->condition('type', 'writing365')
    ->condition('uid', $account->uid)
    ->condition('status', 1)
    ->orderBy('sticky', 'DESC')
    ->orderBy('created', 'DESC')
    ->limit(variable_get('default_nodes_main', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  if (!empty($nids)) {
    $nodes = node_load_multiple($nids);
    $build += node_view_multiple($nodes);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  }
  else {
    if ($account->uid == $user->uid) {
      drupal_set_message(t('You have not created any journal entries.'));
    }
    else {
      drupal_set_message(t('!author has not created any journal entries.', array('!author' => theme('username', array('account' => $account)))));
    }
  }
  drupal_add_feed('writing365/' . $account->uid . '/feed', t('RSS - !title', array('!title' => $title)));

  return $build;
}

/**
 * Menu callback; displays a Drupal page containing recent journal entries of all users.
 */
function writing365_page_last() {
  global $user;
  $build = array();

  $query = db_select('node', 'n')->extend('PagerDefault');
  $nids = $query
    ->fields('n', array('nid', 'sticky', 'created'))
    ->condition('type', 'writing365')
    ->condition('status', 1)
    ->orderBy('sticky', 'DESC')
    ->orderBy('created', 'DESC')
    ->limit(variable_get('default_nodes_main', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  if (!empty($nids)) {
    $nodes = node_load_multiple($nids);
    $build += node_view_multiple($nodes);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  }
  else {
    drupal_set_message(t('No journal entries have been created.'));
  }
  drupal_add_feed('journal/feed', t('RSS - journals'));

  return $build;
}

/**
 * Menu callback; displays an RSS feed containing recent blog entries of a given user.
 */
function writing365_feed_user($account) {

  $nids = db_select('node', 'n')
    ->fields('n', array('nid', 'created'))
    ->condition('type', 'writing365')
    ->condition('uid', $account->uid)
    ->condition('status', 1)
    ->orderBy('created', 'DESC')
    ->range(0, variable_get('feed_default_items', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  $channel['title'] = t("!name's journal", array('!name' => format_username($account)));
  $channel['link'] = url('writing365/' . $account->uid, array('absolute' => TRUE));

  node_feed($nids, $channel);
}

/**
 * Menu callback; displays an RSS feed containing recent journal entries of all users.
 */
function writing365_feed_last() {
  $nids = db_select('node', 'n')
    ->fields('n', array('nid', 'created'))
    ->condition('type', 'writing365')
    ->condition('status', 1)
    ->orderBy('created', 'DESC')
    ->range(0, variable_get('feed_default_items', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  $channel['title'] = t('!site_name journals', array('!site_name' => variable_get('site_name', 'Drupal')));
  $channel['link'] = url('journal', array('absolute' => TRUE));

  node_feed($nids, $channel);
}

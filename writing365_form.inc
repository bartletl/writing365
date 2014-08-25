<?php
function writing365_form($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Testing of form for Writing 365'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
  );
  $form['content'] = array(
  '#type' => 'textfield',
  '#title' => t('Journal Entry'),
  );

  return $form;
}
?>
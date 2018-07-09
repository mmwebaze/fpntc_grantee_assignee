<?php
use \Drupal\taxonomy\Entity\Term;
use \Drupal\fpntc_grantee_assignee\util\Util;
use \Drupal\user\Entity\User;
/**
 * Implements hook_drush_command().
 */
function fpntc_grantee_assignee_drush_command(){
  $items = array();
  $items['grantee-update'] = [
    'description' => 'Assign grantee to user',
    'aliases' => ['gau'],
  ];
  $items['grantee-create'] = [
    'description' => 'Creates grantee term',
    'aliases' => ['gac'],
  ];


  return $items;
}
function drush_fpntc_grantee_assignee_grantee_create(){
  $core_path = \Drupal::service('file_system');
  $path = $core_path->realpath("public://");
//drush_print($path);die();
  $file = fopen($path."/grantees.csv","r");

  $tempZips = [];
  $name = '';
  while(! feof($file)){
    $row = fgetcsv($file);
    if (!empty($row[0])){
      if (count($tempZips) > 0){
        $term = Term::create([
          'name' => $name,
          'vid' => 'grantees',
          'field_zip_codes' => $tempZips,
        ])->save();

        $tempZips = [];
      }
      $name = $row[0];
      array_push($tempZips, ['value' => $row[1]]);
    }
    else{
      array_push($tempZips, ['value' => $row[1]]);
    }
  }
  //print("Migration complete");
  fclose($file);
  \Drupal::logger('fpntc_grantee_assignee')
    ->notice('Taxonomy migrations or creation complete');
}
function drush_fpntc_grantee_assignee_grantee_update(){

  $results = Util::getUsers();
  foreach ($results as $result){

    $tid = Util::getTaxonomy($result->field_zip_code_value);
    if ($tid == 0){
      continue;
    }
    $user = User::load($result->entity_id);
    $user->set('field_grantees', $tid);
    $user->save();

    //print('User id: '.$result->entity_id.' zip code: '.$result->field_zip_code_value." updated\n");
  }
}
// uses entity type manager
function getTaxonomy($zipcode){
  $termStorage = Drupal::service('entity_type.manager')->getStorage('taxonomy_term');
  $ids = $termStorage->getQuery()
    ->condition('vid', 'grantees')
    ->condition('field_zip_codes', $zipcode)->execute();
  $terms = $termStorage->loadMultiple($ids);

  if (count($terms) > 1 || count($terms) < 1){
    //print('No taxonomy associated with zipcode: '.$zipcode."\n");
    return 0;
  }
  print_r(current($terms)->id().' -- '.current($terms)->getName()."\n");

  return current($terms)->id();
}


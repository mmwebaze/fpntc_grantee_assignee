<?php

namespace Drupal\fpntc_grantee_assignee\util;


class Util {
  public static function getUsers(){
    $connection = \Drupal::service('database');
    $results = $connection->select('user__field_zip_code', 'fz')
      ->fields('fz', ['entity_id', 'field_zip_code_value'])->execute()->fetchAll();

    return $results;
  }
  public static function getTaxonomy($zipcode){
    $connection = \Drupal::service('database');
    $result = $connection->select('taxonomy_term__field_zip_codes', 'fz')
      ->fields('fz', ['entity_id', 'field_zip_codes_value'])
      ->condition('field_zip_codes_value', $zipcode)->execute()->fetchAll();
    if (count($result) > 1 || count($result) < 1){
      \Drupal::logger('fpntc_grantee_assignee')
        ->notice('No taxonomy associated with zipcode: '.$zipcode);
     // print('No taxonomy associated with zipcode: '.$zipcode."\n");
      return 0;
    }
    return current($result)->entity_id;
  }
}
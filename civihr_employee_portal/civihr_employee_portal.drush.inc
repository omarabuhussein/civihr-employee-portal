<?php

/**
 * Implements hook_drush_help().
 */
function civihr_employee_portal_drush_help($command) {
    switch ($command) {
        case 'drush:refresh-node-export-files':
            return dt('Refreshes the node webform files');
        case 'drush:civihr-blocks-cleanup':
            return dt('Hides the blocks not used for CiviHR employee portal');
    }
}

/**
 * Implements hook_drush_command().
 */
function civihr_employee_portal_drush_command() {
    $items = array();

    $items['refresh-node-export-files'] = array(
        'description' => dt('Refresh Webform node export files.'),
        'arguments'   => array(),
        'examples' => array(
            'Standard example' => 'drush refresh-node-export-files',
        ),
        'aliases' => array('rnef'),
    );

    $items['civihr-blocks-cleanup'] = array(
        'description' => dt('Hides the blocks not used for CiviHR employee portal.'),
        'arguments'   => array(),
        'examples' => array(
            'Standard example' => 'drush civihr-blocks-cleanup',
        ),
        'aliases' => array('civihrbc'),
    );
    return $items;
}

/**
 * Callback function for drush refresh-node-export-files.
 * Callback is called by using drush_hook_command() where
 * the Drush command with all "-" characters converted to "_" characters (refresh-node-export-files)
 *
 */
function drush_civihr_employee_portal_refresh_node_export_files() {

    // Check for all node export files
    $files = file_scan_directory(drupal_get_path('module', 'civihr_employee_portal') . '/features/node_export_files', '/.*\.export$/');

    // Loop the export files and import them
    foreach ($files as $filepath => $file) {

        $info = node_export_import(file_get_contents($filepath));
        drush_log(print_r($info), 'ok');

    }

    // Log to the command line with an OK status
    drush_log('Node Webform refresh finished', 'ok');
}

/**
 * Callback function for drush civihr-blocks-cleanup.
 * Callback is called by using drush_hook_command() where
 * the Drush command with all "-" characters converted to "_" characters (civihr-blocks-cleanup)
 *
 */
function drush_civihr_employee_portal_civihr_blocks_cleanup() {

    // Disable not needed blocks
    db_update('block')
        ->fields(array('region' => -1))
        ->condition('theme', 'civihr_default_theme')
        ->condition('region', 'content')
        ->condition('delta', 'main', '!=')
        ->execute();

    // Log to the command line with an OK status
    drush_log('CiviHR not used blocks were hidden', 'ok');
}
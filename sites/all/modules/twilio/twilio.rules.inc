<?php
/**
 * @file
 * Rules module integration
 */

/**
* Implementation hook_rules_data_info().
*/
function twilio_rules_data_info() {
  return array(
    'sms' => array(
      'label' => t('Twilio incoming SMS'),
      'wrap' => TRUE,
      'property info' => array(
        'number' => array(
          'type' => 'text',
          'label' => t('Number'),
          'description' => t('The phone number of the incoming SMS.'),
          'getter callback' => 'entity_property_verbatim_get',
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t('The message of the incoming SMS.'),
          'getter callback' => 'entity_property_verbatim_get',
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'media' => array(
          'type' => 'text',
          'label' => t('Media'),
          'description' => t('The media attached to the incoming SMS.'),
          'getter callback' => 'entity_property_verbatim_get',
          'setter callback' => 'entity_property_verbatim_set',
        ),
      ),
    ),
    'voice' => array(
      'label' => t('Twilio incoming Voice'),
      'wrap' => TRUE,
      'property info' => array(
        'number' => array(
          'type' => 'text',
          'label' => t('Number'),
          'description' => t('The phone number of the incoming call.'),
          'getter callback' => 'entity_property_verbatim_get',
          'setter callback' => 'entity_property_verbatim_set',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_event_info().
 */
function twilio_rules_event_info() {
  $defaults = array(
    'group' => t('Twilio'),
    'module' => 'twilio',
  );
  return array(
    'twilio_sms_incoming' => $defaults + array(
      'label' => t('Drupal receives an SMS'),
      'variables' => array(
        'sms' => array(
          'type' => 'sms',
          'label' => t('Twilio incoming SMS'),
        ),
      ),
    ),
    'twilio_voice_incoming' => $defaults + array(
      'label' => t('Drupal receives a voice call'),
      'variables' => array(
        'voice' => array(
          'type' => 'voice',
          'label' => t('Twilio incoming Voice'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function twilio_rules_condition_info() {
  return array(
    'twilio_rules_condition_number_belongs_to_user' => array(
      'label' => t('Number belongs to a valid user'),
      'arguments' => array(
        'number' => array(
          'type' => 'text',
          'label' => t('Phone Number'),
        ),
      ),
      'group' => t('Twilio'),
      'module' => 'twilio',
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function twilio_rules_action_info() {
  $defaults = array(
    'group' => t('Twilio'),
    'module' => 'twilio',
  );
  $actions['twilio_send_sms_to_user'] = $defaults + array(
    'label' => t('Send an SMS to a user'),
    'base' => 'twilio_rules_action_send_sms_to_user',
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('User'),
        'save' => TRUE,
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
      ),
      'media' => array(
        'type' => 'text',
        'optional' => TRUE,
        'label' => t('Media path'),
      ),
    ),
  );
  $actions['twilio_send_sms_to_number'] = $defaults + array(
    'label' => t('Send an SMS to a number'),
    'base' => 'twilio_rules_action_send_sms_to_number',
    'parameter' => array(
      'country' => array(
        'type' => 'text',
        'label' => t('Country code'),
        'options list' => 'twilio_country_codes',
      ),
      'number' => array(
        'type' => 'text',
        'label' => t('Number'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
      ),
      'media' => array(
        'type' => 'text',
        'optional' => TRUE,
        'label' => t('Media path'),
      ),
    ),
  );
  return $actions;
}

/**
 * Action: Send an SMS to a user.
 */
function twilio_rules_action_send_sms_to_user($account, $message, $media) {
  if (!empty($account->twilio_user['number']) && $account->twilio_user['status'] == 2) {
    twilio_send($account->twilio_user['number'], $message,$account->twilio_user['country'], $media);
  }
}

/**
 * Action: Send an SMS to a number.
 */
function twilio_rules_action_send_sms_to_number($country, $number, $message, $media) {
  twilio_send($number, $message, $country, $media);
}

/**
 * Condition: Validate if a number belongs to a user on the site.
 */
function twilio_rules_condition_number_belongs_to_user($number) {
  return twilio_verify_number($number);
}

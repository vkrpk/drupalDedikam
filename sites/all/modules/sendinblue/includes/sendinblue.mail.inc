<?php

/**
 * @file
 * Implements SendInBlue as a Drupal MailSystemInterface.
 */

/**
 * Modify the Drupal mail system to use SendInBlue when sending e-mails.
 */
class SendInBlueMailSystem implements MailSystemInterface {

  /**
   * {@inheritdoc}
   */
  public function format(array $message) {
    // Join the body array into one string.
    $message['body'] = implode("\n\n", $message['body']);

    return $message;
  }

  /**
   * {@inheritdoc}
   */
  public function mail(array $message) {
    $loggerFactory = new SendInBlueLoggerFactory();
    $sendinblueMailin = new SendinblueMailin();

    try {
      $to = [
        'email' => $message['to'],
      ];

      $from = [
        'email' => $message['from'],
      ];

      $message['reply-to'] = !empty($message['reply-to']) ? $message['reply-to'] : $message['from'];
      $replyTo = [
        'email' => $message['reply-to'],
      ];

      unset($message['headers']['Content-Type']);

      $result = $sendinblueMailin->sendEmail(
        $to,
        $message['subject'],
        nl2br($message['body']),
        $message['body'],
        $from,
        $replyTo,
        [],
        [],
        [],
        $message['headers']
      );

      if (empty($result->getMessageId())) {
        $loggerFactory
          ->error('[SENDINBLUE] - Error sending email (from %from to %to with reply-to %reply).', [
            '%from' => $message['from'],
            '%to' => $message['to'],
            '%reply' => $message['reply-to'] ? $message['reply-to'] : 'not set',
          ]);

        return FALSE;
      }

      $loggerFactory
        ->info('[SENDINBLUE] - Sending email %messageId (from %from to %to).', [
          '%from' => $message['from'],
          '%to' => $message['to'],
          '%messageId' => $result->getMessageId(),
        ]);
      return TRUE;
    }
    catch (Exception $e) {
      $loggerFactory
        ->error('[SENDINBLUE] - Error sending email (from %from to %to with reply-to %reply) [%error].', [
          '%from' => $message['from'],
          '%to' => $message['to'],
          '%reply' => $message['reply-to'] ? $message['reply-to'] : 'not set',
          '%error' => $e->getMessage(),
        ]);
      return FALSE;
    }
  }
}

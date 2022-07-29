<?php

/**
 * Manage client's return after a successful payment
 */

/**
 * Handles a canceled Website Payments Standard sale.
 */
function uc_bluepaid_cancel() {
  unset($_SESSION['cart_order']);

  drupal_set_message(t('Votre paiement Bluepaid a été annulé.'));

  drupal_goto(variable_get('uc_bluepaid_url_return', 'cart'));
}

function uc_bluepaid_complete()
{
	if (!isset($_SESSION['cart_order']) || !isset($_GET['id_client']) || intval($_SESSION['cart_order']) != $_GET['id_client']) 
	{
	  drupal_set_message(t('Merci pour votre contribution.'));
	  drupal_goto('cart');
	}
	//TODO AUTHENTICATE BLUEPAID SERVER
	$from_server = true;	

	// Retrieve the order
	$order_id = $_GET['id_client'];
	$order = uc_order_load($order_id);
	if ($order == FALSE) 
	{
		watchdog(
			'uc_bluepaid',
			'Payment check attempted on non existing order : %orderID',
			array('%orderID' => $order_id),
			WATCHDOG_ERROR
		);
		
		if ($from_server)
		{
			die('order_not_found');
		} 
		else 
		{
			drupal_set_message(t('Order not found'), 'warning');
			return;
		}
	}
		
	// Ensure the payment method is Bluepaid.
	if ($order->payment_method != 'uc_bluepaid') 
	{
		drupal_goto('cart');
	}

	// This lets us know it's a legitimate access of the complete page.
	$_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
	drupal_goto(variable_get('uc_bluepaid_url_success', 'cart/checkout/complete'));
}


/**
 * Treat payment confirmation requests
 */
function uc_bluepaid_ipn()
{
	if (empty($_POST)) 
	{
		watchdog('My Payment Gateway',
		   'Received an empty or incomplete response.  Response details: @request_details',
		   array('@request_details' =>  print_r($_POST,true)), WATCHDOG_ERROR);
	
		return 'There was a problem with your payment';
	}	
			
	$bluepaid = new Bluepaid();
	$bluepaid->__set('boutic_id', variable_get('uc_bluepaid_merchant_id', ''));
		
	// Ensure datas provided by Bluepaid.
	$from_server = false;
	if ($authorized_server = $bluepaid->getAuthorizedServerForNotification()) 
	{
		if(!in_array($_SERVER['REMOTE_ADDR'], $authorized_server)) {
			die(t('UNAUTOHORIZED IP ADDRESS '.$_SERVER['REMOTE_ADDR']));
		}	
		$from_server = true;	
	}
	else
	{
		die(t('I CAN\'T GET LIST OF SERVER OF BLUEPAID !'));
	}
	//foreach ($_REQUEST as $key => $value)
	foreach ($_POST as $key => $value)
	{
		$bluepaid->__set('notify_'.$key, $value);
	}
	
	//Check if payment was made on your account
	if((string)$bluepaid->__get('boutic_id') !== (string)$bluepaid->__get('notify_id_boutique'))
	{
		die(t('INVALID BOUTIC ID '.$bluepaid->__get('notify_id_boutique').' !'));
	}
	
	if (!$bluepaid->__get('notify_id_client')) 
	{
	  die('Missing parameters to complete order!');
	}
	$order = uc_order_load($bluepaid->__get('notify_id_client'));
	if ($order == FALSE) 
	{
		watchdog(
			'uc_bluepaid',
			'Payment check attempted on non existing order : %orderID',
			array('%orderID' => $bluepaid->__get('notify_id_client')),
			WATCHDOG_ERROR
		);
		
		if($from_server) 
		{
			die('order_not_found');
		} 
		else 
		{
			die(t('Order not found'));
			return;
		}
	}
	
	// Ensure the payment method is Bluepaid.
	if ($order->payment_method != 'uc_bluepaid') 
	{
		die(t('INVALID PAYMENT METHOD !'));
	}
	
	
	$bluepaid->setDebugMode(variable_get('uc_bluepaid_ctx_mode', false));
		
	$status_trx = false;
	$comment = false;	
	if(!$bluepaid->isAvailableMode($bluepaid->__get('notify_mode')))
	{
		$status_trx = 'failed';
		/* Verfify if error is due to debug mode */
		if (($bluepaid->__get('notify_mode') === 'test' && !$bluepaid->isDebugMode()) 
		|| ($bluepaid->__get('notify_etat') === 'test' && !$bluepaid->isDebugMode()) )
		{
			$comment = t('Trying to pay order with a test card in production mode');
		}
	}
	if (!$status_trx && !$bluepaid->isAvailableStatus($bluepaid->__get('notify_etat')))
	{
		$status_trx = 'failed';
		/* Verfify if error is due to debug mode */
		if (($bluepaid->__get('notify_mode') === 'test' && !$bluepaid->isDebugMode()) 
		|| ($bluepaid->__get('notify_etat') === 'test' && !$bluepaid->isDebugMode()) )
		{
			$comment = t('Trying to pay order with a test card in production mode');
		}
		
	}
	if (!$status_trx)
	{
		$status_trx = $bluepaid->convertResultTrans($bluepaid->__get('notify_etat'));
		if ($status === false)
			die('Error while convert transaction status '.$bluepaid->__get('notify_etat'));
	}
	// Treat response
	$not_paid_statuses = array('canceled', 'in_checkout');
	/* ALREADY PAID ? */
	if(in_array($order->order_status, $not_paid_statuses, true)) {
		switch($status_trx)
		{
			case 'completed':
			case 'test':
				$amount = $bluepaid->__get('notify_montant');
				if (abs($amount - $order->order_total) > 0.01) 
				{
					watchdog('uc_bluepaid', 'Payment @txn_id for order @order_id did not equal the order total.', 
						array('@txn_id' => $bluepaid->__get('notify_id_trans'), '@order_id' => $bluepaid->__get('notify_id_client')), 
						WATCHDOG_WARNING, l(t('view'), 'admin/store/orders/' . $bluepaid->__get('notify_id_client')));
				}
			
				// Update order status
				uc_order_update_status($bluepaid->__get('notify_id_client'), uc_order_state_default('payment_received'));	
				// Complete the order
				uc_payment_enter($bluepaid->__get('notify_id_client'), 'uc_bluepaid', $amount, $order->uid, NULL, $comment);
				uc_cart_complete_sale($order);
				uc_order_comment_save($bluepaid->__get('notify_id_client'),
					 0, 
					 t('Bluepaid IPN reported a payment of @amount @currency - @txn_id.', 
					 array('@amount' => uc_currency_format($amount, FALSE), 
					 '@txn_id' => $bluepaid->__get('notify_id_trans'),
					 '@currency' => $bluepaid->__get('notify_devise'))));			
				die( 'Thank you for your purchase' );
				break;
			case 'failed':
				if (!$comment) $comment = 'The customer\'s attempted payment from a bank account failed.';
				uc_order_comment_save($bluepaid->__get('notify_id_client'), 0, t($comment), 'admin');
				if ($comment) die($comment);
				break;	
			case 'error':
				if (!$comment) $comment = 'The customer\'s attempted payment from a bluepaid server failed.';
				uc_order_comment_save($bluepaid->__get('notify_id_client'), 0, t($comment), 'admin');
				if ($comment) die($comment);
				break;	
			case 'pending':
			default :			
				die('NO TREAT...');				
		}	
	}
	else /* YES == > ALREADY PAID ! */
	{
		if($status_trx === 'completed' || $status_trx === 'test') {
			// Successful payment, already registered
			if($from_server){
				uc_order_comment_save($bluepaid->__get('notify_id_client'), 0, t('payment notification on order already done : transaction @txn_id', array('@txn_id' => $bluepaid->__get('notify_id_trans'))), 'admin');
				die ('payment_ok_already_done');
			}
		} else if($status_trx === 'error' || $status_trx === 'failed') { // NOT FOR A PENDING STATUS
			// Payment has been registered as successful, but we receive a payment error message !
			watchdog(
				'uc_vads',
				'Invalid payment confirmation received for an already accepted order',
				array(),
				WATCHDOG_ERROR
			);
			
			if($from_server){
				uc_order_comment_save($bluepaid->__get('notify_id_client'), 0, t('payment notification on order already done : transaction @txn_id', array('@txn_id' => $bluepaid->__get('notify_id_trans'))), 'admin');
				die ('payment_ko_on_order_ok');
			}
		}
	}
}
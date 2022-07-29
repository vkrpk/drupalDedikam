The initial idea of this module was to allow custom PHP code to be associated
with a product in order to adjust the price of the product. The nature of
allowing an admin to enter a block of PHP code to be executed though, allows
for much more elaborate and *potentially dangerous* operations.

Upon installation, this module will add a Custom Code field to your products.

Sample uses of this code block are:

    $item->price = $item->price;

This will do nothing....but it goes in by default.
$item->price refers to the price before attribute adjustments.

    $item->price = $item->price;
    if (date('j', time()) == '1') {
      $item->price = $item->price/2;
    }

This puts the product on sale for half price on the first of each month.

    $item->price = $item->price;
    $item->qty = isset($item->qty) ? $item->qty : 1;
    if ($item->qty > 5) {
      $item->price = [product:cost] * 1.1;
    }

If more than 5 items are ordered, this sets the product price to a 10% mark-up
of the product cost.

Note that custom price code written to operate on items in the cart does not
work the same way as code written to operate on the product node view, because
the two contexts are very different. For example, $item->qty is defined in the
cart, but not on the product page, since the quantity has not yet been entered
by the customer. Likewise if you're basing your price calculation on attribute
selections or other aspects of the product that only exist in the cart context.
Because of this, you need to make sure your code takes appropriate measures to
return the desired result independent of context.  That's why we do an isset()
in the example above.

Product tokens are exposed to this code, so tokens like [product:cost],
[product:sell_price], [product:weight], [product:weight-value], etc. can
be used in your calculation.

For additional examples which use attributes for price calculations, refer to
the Attribute Tokens module (http://drupal.org/project/uc_attribute_tokens).

There are limitless things to do here, but if you are going to get complicated
with it you might as well just write a useful module. This also makes no
attempt to notify the customer of any of these adjustments, so you'll need to
do so in the description of the product.

Thanks to <a href="http://drupal.org/user/202205">cYu</a> for the original code

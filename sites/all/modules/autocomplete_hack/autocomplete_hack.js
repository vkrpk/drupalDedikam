/**
 * $Id$
 * Override the built-in autocomplete search
 */
Drupal.behaviors.autocomplete_hack = {
  attach: function (context, settings) {
    if (typeof Drupal.ACDB != 'undefined') {
      var acdb = Drupal.ACDB.prototype;
      if (typeof acdb._search == 'undefined') {
        acdb._search = acdb.search;
        acdb.search = function(searchString) {
          this._search(searchString.replace(/\//g, '~$~$~$~'));
        }
      }
    }
  }
};

_StorePurchaseStatus = function () {
  this.succeeded = 0;
  this.alreadyPurchased = 1;
  this.notPurchased = 2;
  this.networkError = 3;
  this.serverError = 4;
}
exports.StorePurchaseStatus = new _StorePurchaseStatus();

StoreProduct = (function () {
  var cls = function StoreProduct() {
    this.inAppOfferToken = new String();
    this.price = new StorePrice();
    this.storeId = new String();
  };

  return cls;
})();
exports.StoreProduct = StoreProduct;

StorePurchaseProperties = (function () {
  var cls = function StorePurchaseProperties() {
    this.name = new String();
    this.extendedJsonData = new String();
  };

  var cls = function StorePurchaseProperties(name) {
    this.name = new String();
    this.extendedJsonData = new String();
  };


  return cls;
})();
exports.StorePurchaseProperties = StorePurchaseProperties;

StoreLicense = (function () {
  var cls = function StoreLicense() {
    this.inAppOfferToken = new String();
    this.isActive = new Boolean();
  };


  return cls;
})();
exports.StoreLicense = StoreLicense;

StorePrice = (function () {
  var cls = function StorePrice() {
    this.currencyCode = new String();
    this.formattedBasePrice = new String();
    this.formattedPrice = new String();
    this.formattedRecurrencePrice = new String();
  };


  return cls;
})();
exports.StorePrice = StorePrice;

StorePurchaseResult = (function () {
  var cls = function StorePurchaseResult() {
    this.extendedError = new Number();
    this.status = new StorePurchaseStatus();
  };


  return cls;
})();
exports.StorePurchaseResult = StorePurchaseResult;

StoreContext = (function () {
  var cls = function StoreContext() {
    this.user = new Object();
  };

  cls.prototype.initialize = function initialize(hwnd) {
    /// <signature>
    /// <summary>Initializes the StoreContext with the handle to current window.</summary>
    /// <param name="hwnd" type="Uint8Array">Handle to window.</param>
    /// <returns type="StoreContext" />
    /// </signature>
    return new Boolean()
  }

  cls.prototype.getCustomerPurchaseIdAsync = function getCustomerPurchaseIdAsync(serviceTicket, publisherUserId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="serviceTicket" type="String">A param.</param>
    /// <param name="publisherUserId" type="String">A param.</param>
    /// </signature>
  }

  cls.prototype.getAppLicenseAsync = function getAppLicenseAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }

  cls.prototype.getAssociatedStoreProductsAsync = function getAssociatedStoreProductsAsync(productKinds, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productKinds" type="Object">A param.</param>
    /// </signature>
  }

  cls.prototype.requestPurchaseAsync = function requestPurchaseAsync(storeId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="storeId" type="String">A param.</param>
    /// </signature>
  }

  cls.prototype.requestPurchaseAsync = function requestPurchaseAsync(storeId, storePurchaseProperties, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="storeId" type="String">A param.</param>
    /// <param name="storePurchaseProperties" type="StorePurchaseProperties">A param.</param>
    /// </signature>
  }

  return cls;
})();
exports.StoreContext = StoreContext;

StoreAppLicense = (function () {
  var cls = function StoreAppLicense() {
    this.addOnLicenses = new Object();
  };


  return cls;
})();
exports.StoreAppLicense = StoreAppLicense;

StoreProductResult = (function () {
  var cls = function StoreProductResult() {
    this.extendedError = new Number();
    this.product = new StoreProduct();
  };


  return cls;
})();
exports.StoreProductResult = StoreProductResult;

StoreProductQueryResult = (function () {
  var cls = function StoreProductQueryResult() {
    this.extendedError = new Number();
    this.products = new Object();
  };


  return cls;
})();
exports.StoreProductQueryResult = StoreProductQueryResult;

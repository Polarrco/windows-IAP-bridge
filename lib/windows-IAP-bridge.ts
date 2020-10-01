declare module "polarr_windows_store" {

    export enum StorePurchaseStatus {
        succeeded,
        alreadyPurchased,
        notPurchased,
        networkError,
        serverError,
    }

    export enum StoreRateAndReviewStatus {
        succeeded,
        canceledByUser,
        networkError,
        Error,
    }

    export class StoreProduct {
        inAppOfferToken: String;
        price: StorePrice;
        storeId: String;
        constructor();

    }

    export class StorePurchaseProperties {
        name: String;
        extendedJsonData: String;
        constructor();
        constructor(name: String);

    }

    export class StoreLicense {
        inAppOfferToken: String;
        isActive: Boolean;
        constructor();

    }

    export class StorePrice {
        currencyCode: String;
        formattedBasePrice: String;
        formattedPrice: String;
        formattedRecurrencePrice: String;
        constructor();

    }

    export class StorePurchaseResult {
        extendedError: Number;
        status: StorePurchaseStatus;
        constructor();

    }

    export class StoreRateAndReviewResult  {
        extendedError: Number;
        ExtendedJsonData: String;
        status: StoreRateAndReviewStatus;
        wasUpdated: Boolean;
        constructor();
    }

    export class StoreContext {
        user: Object;
        constructor();

        initialize(hwnd: Uint8Array): boolean;

        static getDefault(): StoreContext;

        getCustomerPurchaseIdAsync(serviceTicket: String, publisherUserId: String, callback: (error: Error, result: String) => void): void;

        getAppLicenseAsync(callback: (error: Error, result: StoreAppLicense) => void): void;

        getAssociatedStoreProductsAsync(productKinds: Object, callback: (error: Error, result: StoreProductQueryResult) => void): void;

        requestPurchaseAsync(storeId: String, callback: (error: Error, result: StorePurchaseResult) => void): void;
        requestPurchaseAsync(storeId: String, storePurchaseProperties: StorePurchaseProperties, callback: (error: Error, result: StorePurchaseResult) => void): void;

        requestRateAndReviewAppAsync(callback: (error: Error, result: StoreRateAndReviewResult) => void): void;
    }

    export class StoreAppLicense {
        addOnLicenses: Object;
        constructor();

    }

    export class StoreProductResult {
        extendedError: Number;
        product: StoreProduct;
        constructor();

    }

    export class StoreProductQueryResult {
        extendedError: Number;
        products: Object;
        constructor();

    }

}

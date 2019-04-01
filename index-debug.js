'use strict'

const windowsStore = require('./lib/main');
const store = new windowsStore.WindowsStore()

function getAssociatedStoreProducts() {
    store.getAssociatedStoreProductsAsync([1, 2, 3], console.log.bind(this))
}

function getAppLocalStorageFolder() {
    console.log(store.getAppLocalStorageFolder())
}

getAppLocalStorageFolder()
getAssociatedStoreProducts()
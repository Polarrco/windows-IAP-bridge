'use strict'

const windowsStore = require('./lib/main')
const store = new windowsStore.StoreContext()

function getAssociatedStoreProducts() {
    store.getAssociatedStoreProductsAsync(['Durable'], console.log.bind(this))
}

function getAppLocalStorageFolder() {
    console.log(store.getAppLocalStorageFolder())
}

getAppLocalStorageFolder()
getAssociatedStoreProducts()
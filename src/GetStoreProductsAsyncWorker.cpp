#include "GetStoreProductsAsyncWorker.h"
#include <Windows.h>
#include <iostream>
#include <napi.h>
#include <string>

GetStoreProductsAsyncWorker::GetStoreProductsAsyncWorker(const Napi::Function &callback, Napi::Array &productKinds,
                                                         WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_productKinds(productKinds), m_pImpl(pImpl), m_result(NULL) {
  std::cout << "GetStoreProductsAsyncWorker::GetStoreProductsAsyncWorker" << std::endl;
}

void GetStoreProductsAsyncWorker::Execute() {
  std::cout << "GetStoreProductsAsyncWorker::Execute" << std::endl;
  m_result = m_pImpl->GetStoreProducts(m_productKinds);
}

void GetStoreProductsAsyncWorker::OnOK() {
  std::cout << "GetStoreProductsAsyncWorker::OnOK" << std::endl;
  Napi::Env env = Env();
  Napi::Object obj = Napi::Object::New(env);
  std::cout << "has Current: " << m_result.HasCurrent() << std::endl;
  while (m_result.HasCurrent()) {
    Napi::Object storeProd = Napi::Object::New(env);
    Napi::Object storePrice = Napi::Object::New(env);
    winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring,
                                                           winrt::Windows::Services::Store::StoreProduct>
        current = m_result.Current();

    storeProd.Set("inAppOfferToken", winrt::to_string(current.Value().InAppOfferToken()));

    auto price = current.Value().Price();
    storePrice.Set("formattedRecurrencePrice", winrt::to_string(price.FormattedRecurrencePrice()));
    storePrice.Set("currencyCode", winrt::to_string(price.CurrencyCode()));
    storeProd.Set("price", storePrice);
    storeProd.Set("storeId", winrt::to_string(current.Key()));

    obj.Set(winrt::to_string(current.Key()), storeProd);
    m_result.MoveNext();
  }
  Callback().MakeCallback(Receiver().Value(), {
                                                  env.Null(), // error first callback
                                                  obj         // value sent back to the callback
                                              });
}

void GetStoreProductsAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}

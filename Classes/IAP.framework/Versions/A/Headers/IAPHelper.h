//
//  IAPHelper.h
//  IAPDemo
//
//  Created by huxf on 14/11/5.
//
//
#include "IAPDelegate.h"

@interface IAPHelper : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (retain) NSSet                *identifiers;
@property IAPDelegate                   *delegate;
@property (retain) SKProductsRequest    *request;


+ (id)instance;
- (void)requestProducts;
- (id)initWithProductIdentifiers;
- (void)buyProductByIdentifier:(NSString*)identifier;

@end

//
//  IAPAdapter.h
//  IAPDemo
//
//  Created by ZivHoo on 14/11/6.
//
//

#ifndef __IAPDemo__IAPAdapter__
#define __IAPDemo__IAPAdapter__

#include "IAPDelegate.h"
#include <string>

class IAPAdapter {
    
public:
    
    IAPAdapter(IAPDelegate&);

public:
    
    void requestProducts();
    
    void buyProduct(const char*);
    
protected:
    
    
private:
    
    IAPDelegate& _delegate;
    
};



#endif /* defined(__IAPDemo__IAPAdapter__) */

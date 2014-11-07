//
//  IAPDelegate.h
//  IAPDemo
//
//  Created by huxf on 14/11/6.
//
//

#ifndef __IAPDemo__IAPDelegate__
#define __IAPDemo__IAPDelegate__

#include "Product.h"
#include <vector>

using namespace std;

class IAPDelegate
{
    
public:
    static int testStaticMethod();

public:
	
    virtual void products(vector<Product>);
    
protected:
	

private:
	

};

#endif /* defined(__IAPDemo__IAPDelegate__) */

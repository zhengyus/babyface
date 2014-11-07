//
//  K4IOSAudioEngine.h
//  Lol
//
//  Created by kc on 14-11-6.
//
//

#ifndef __Lol__K4IOSAudioEngine__
#define __Lol__K4IOSAudioEngine__

class K4IOSAudioEngine
{
public:
	K4IOSAudioEngine();
	virtual ~K4IOSAudioEngine();
    
	static K4IOSAudioEngine* shareK4IOSAudioEngine();
    
    void playVibrate();//震动
    
};


#endif /* defined(__Lol__K4IOSAudioEngine__) */


//
//  K4IOSAudioEngine.cpp
//  Lol
//
//  Created by kc on 14-11-6.
//
//

#include "K4IOSAudioEngine.h"
#import <AudioToolbox/AudioToolbox.h>

static K4IOSAudioEngine* g_K4IOSAudioEngine = NULL;

K4IOSAudioEngine::K4IOSAudioEngine()
{
	// TODO Auto-generated constructor stub
    
}

K4IOSAudioEngine::~K4IOSAudioEngine()
{
	// TODO Auto-generated destructor stub
}

K4IOSAudioEngine* K4IOSAudioEngine::shareK4IOSAudioEngine()
{
	if(g_K4IOSAudioEngine == NULL)
	{
		g_K4IOSAudioEngine = new K4IOSAudioEngine;
	}
    
	return g_K4IOSAudioEngine;
}

void K4IOSAudioEngine::playVibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}
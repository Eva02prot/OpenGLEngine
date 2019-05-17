//
//  EventMgr.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef EventMgr_h
#define EventMgr_h

#include "../common.h"
#include "uievent.h"


class EventMgr
{
    
    DeclareSington(EventMgr)
    
public:
    
    void RegistEvt(UIEvent* evt)
    {
        ui_events.push_back(evt);
    }
    
    void RemoveEvt(UIEvent* evt)
    {
        for(vector<UIEvent*>::iterator iter=ui_events.begin(); iter!=ui_events.end();iter++)
        {
            if(*iter == evt)
            {
                ui_events.erase(iter);
                break;
            }
        }
    }
    
    void Triger(float x, float y, int action)
    {
        size_t size = ui_events.size();
        for (size_t i=0; i<size; i++)
        {
            UIEvent* et = ui_events[i];
            if(action == GLFW_RELEASE && et->IsHit(x, y))
            {
                et->OnTriger();
                et->Dispacher();
            }
        }
    }
    
    
private:
    std::vector<UIEvent*> ui_events;
};


#endif /* EventMgr_h */

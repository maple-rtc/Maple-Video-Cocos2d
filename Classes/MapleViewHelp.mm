//
//  MapleViewHelp.mm
//
//

#include "MapleViewHelp.h"
#import <UIKit/UIKit.h>

void MapleViewHelp::RemoveSurfaceView(void* view){
    UIView* view_ = (__bridge UIView*)view;
    
    if(view_){
        [view_ removeFromSuperview];
    }
}

void MapleViewHelp::SetSurfaceView(void* view, int x, int y)
{
    UIWindow* rootvc = [[UIApplication sharedApplication] keyWindow];
    UIView* view_ = (__bridge UIView*)view;
    
    CGFloat window_width = [rootvc frame].size.width;
    CGFloat view_width = window_width / 4 ;

    CGFloat window_height = [rootvc frame].size.height;
    CGFloat view_height = window_height / 4;

    [view_ setFrame:CGRectMake(view_width * x,view_height * y, view_width, view_height)];
    [view_ setBackgroundColor:[UIColor whiteColor]];
    [rootvc addSubview:view_];
}




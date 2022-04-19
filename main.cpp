#include "RenderApplication.h"

/*此处非常之坑，貌似是x86平台需要子系统是console，而x64平台需要子系统是windows，
否则在程序退出的时候会崩溃，具体原因不是很清楚。此处浪费了我一个小时时间去解决问题
2020.5.14 郑天季
*/

#ifdef _WIN32
#pragma comment(linker, "/subsystem:console")
#endif

int main()
{
    RenderApplication app;
    app.Run();

    return 0;


}


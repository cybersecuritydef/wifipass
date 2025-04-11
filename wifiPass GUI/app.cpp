#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "app.h"
#include "main.h"

IMPLEMENT_APP(App);

bool App::OnInit(){
    MainFrame* frame = new MainFrame(0L, _("WifiPass GUI"));
    frame->SetSize(800, 600);
    frame->Center();
    frame->SetIcon(wxIcon("title"));
    frame->Show();
    return true;
}

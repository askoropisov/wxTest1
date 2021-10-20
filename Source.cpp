#include <wx/wx.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "rpcrt4.lib")

#pragma comment(lib, "wxbase31ud.lib")
#pragma comment(lib, "wxmsw31ud_core.lib")
#pragma comment(lib, "wxmsw31ud_adv.lib")
#pragma comment(lib, "wxzlibd.lib")
#pragma comment(lib, "wxpngd.lib")

#pragma comment(linker, "/SUBSYSTEM:windows")



enum type {
            resistor,
            transistor,
            source
};

class element {
public:
    int x_pos,y_pos;
    type type;
};

enum {
    wxMENU_ITEM_OPEN = wxID_HIGHEST + 1,
    wxMENU_ITEM_SAVE,
    wxMENU_ITEM_RESISTOR,
    wxMENU_ITEM_TRANSISTOR,
    wxMENU_ITEM_SOURCE,
    wxEVT_CLICK_MOUSE
};

class Frame : public wxFrame {
public:
    Frame(const wxString& title);
private:
    void OnMenu_FileQuit(wxCommandEvent& evt);
    void Save(wxCommandEvent& evt);
    void Open(wxCommandEvent& evt);
    void ClicMouse(wxMouseEvent& evt);

    void OnMouseMove(wxMouseEvent& evt);

    void OnPaint(wxPaintEvent& evt);

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_LEFT_DOWN(Frame::ClicMouse)
wxEND_EVENT_TABLE()


void Frame::OnMenu_FileQuit(wxCommandEvent& evt) {
    Close(true);
}

void Frame::Save(wxCommandEvent& evt) {
    if (evt.IsChecked()) wxMessageBox(wxT("Checked!"));
    else wxMessageBox(wxT("Unchecked!"));
}

void Frame::Open(wxCommandEvent& evt) {
    wxMessageBox(wxT("Hello"));
}

void Frame::OnMouseMove(wxMouseEvent& evt) {
    SetStatusText(wxString::Format("[ %d, %d ]", evt.GetX(), evt.GetY(), 1));
}



wxPoint mouse_pos(100,100);
void Frame::ClicMouse(wxMouseEvent& evt) {
     mouse_pos=evt.GetPosition();
     Refresh();
}




int element = 2;
void Frame::OnPaint(wxPaintEvent& evt) {
    wxMouseEvent evn;
    wxPaintDC dc(this);
    wxPoint lineT(30,0);

    wxBrush brush(wxColor(0, 255, 50));
    dc.SetBrush(brush);

    wxPen pen(wxColor(0, 0, 0), 3);
    dc.SetPen(pen);

    switch (element)
    {
    case 1: {
        dc.DrawLine(mouse_pos, mouse_pos+wxPoint(50,0));
        dc.DrawRectangle(mouse_pos+wxPoint(50, -20), wxSize(100, 40));
        dc.DrawLine(mouse_pos+wxPoint(150,0), mouse_pos+wxPoint(200,0)); 
        break;
    }
    case 2: {

        dc.DrawLine(mouse_pos, mouse_pos+wxPoint(50,0));
        dc.DrawRectangle(mouse_pos+wxPoint(50,-30), wxSize(120,60));
        dc.DrawLine(mouse_pos+wxPoint(170, 0), mouse_pos+wxPoint(220, 0));

        dc.DrawLine(mouse_pos+wxPoint(90, -30), mouse_pos+wxPoint(90,30));
        dc.DrawLine(mouse_pos+wxPoint(130, -30), mouse_pos+wxPoint(130, 30));

        dc.DrawLine(mouse_pos+wxPoint(110, 30), mouse_pos+wxPoint(110, 70));
        break;
    }
    case 3: {
        dc.DrawLine(wxPoint(210, 100), wxPoint(210, 130));
        dc.DrawCircle(wxPoint(210,160), wxCoord(30));
        dc.DrawLine(wxPoint(210, 190), wxPoint(210, 220));
        break;
    }
    default:
        break;
    }
   

    //dc.DrawRectangle(150, 150, 400, 200);

    dc.SetTextForeground(wxColour(0, 0, 255));

}

Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {

    wxMenuBar* p_menubar = new wxMenuBar;
    wxMenu* p_menuFile = new wxMenu;
    wxMenu* p_menuElement = new wxMenu;
   // wxToolBar* p_toolBar = CreateToolBar();

   
    p_menuFile->Append(wxMENU_ITEM_OPEN, wxT("&Open File"));
    p_menuFile->Append(wxMENU_ITEM_SAVE, wxT("Save As"));
    p_menuFile->AppendSeparator();
    p_menuFile->Append(wxID_EXIT, wxT("&Exit"));
    p_menubar->Append(p_menuFile, wxT("&File"));

    p_menuElement->Append(wxMENU_ITEM_RESISTOR, wxT("&Resistor"));
    p_menuElement->Append(wxMENU_ITEM_TRANSISTOR, wxT("&Transistor"));
    p_menuElement->Append(wxMENU_ITEM_SOURCE, wxT("&Source"));
    p_menubar->Append(p_menuElement, wxT("&Element"));


    SetMenuBar(p_menubar);

   /* wxImage::AddHandler(new wxPNGHandler);
    wxBitmap bmp_exit(wxT("exit.png"), wxBITMAP_TYPE_PNG);*/


    /*p_toolBar->AddTool(wxID_EXIT, wxT("Exit application"), bmp_exit, wxT("Click the button"));
    p_toolBar->Realize();*/

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenu_FileQuit));
    Connect(wxMENU_ITEM_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Open));
    Connect(wxMENU_ITEM_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Save));

    Connect(wxEVT_MOTION, wxMouseEventHandler(Frame::OnMouseMove));
    Connect(wxEVT_CLICK_MOUSE, wxMouseEventHandler(Frame::ClicMouse));

    Connect(wxEVT_PAINT, wxPaintEventHandler(Frame::OnPaint));

    CreateStatusBar(3);
    SetStatusText(wxT("Ready"), 0);

    Centre();
}

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit() {
    Frame* frame = new Frame(wxT("Simple"));
    frame->Show(true);

    return true;
}

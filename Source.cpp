#include <wx/wx.h>
#include <wx/wfstream.h>
#include <string> 
#include <iostream>
#include <fstream>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "rpcrt4.lib")

#pragma comment(lib, "wxbase31ud.lib")
#pragma comment(lib, "wxmsw31ud_core.lib")
#pragma comment(lib, "wxmsw31ud_adv.lib")
#pragma comment(lib, "wxzlibd.lib")
#pragma comment(lib, "wxpngd.lib")

#pragma comment(linker, "/SUBSYSTEM:windows")

using std::fstream;


fstream file;
fstream last_data;

enum type {
            resistor,
            transistor,
            source
};

class Element {
public:
    int x_pos,y_pos;
    int type;
} item;

enum {
    wxMENU_ITEM_OPEN = wxID_HIGHEST + 1,
    wxMENU_ITEM_SAVE,
    wxMENU_ITEM_RESISTOR,
    wxMENU_ITEM_TRANSISTOR,
    wxMENU_ITEM_SOURCE,
    wxEVT_CLICK_MOUSE, 
};

void ReadLastData(Element &item);
class Window : public wxWindow {
public:
    Window(wxWindow* p_parent);
    void ReadFile(fstream& file);
    void WriteFile(fstream& file);
    void ClicMouse(wxMouseEvent& evt);
    void OnPaint(wxPaintEvent& evt);

private:
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Window, wxWindow)
EVT_LEFT_DOWN(Window::ClicMouse)
wxEND_EVENT_TABLE()

Window::Window(wxWindow* p_parent)
    : wxWindow(p_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {

    Connect(wxEVT_PAINT, wxPaintEventHandler(Window::OnPaint));
    Connect(wxEVT_CLICK_MOUSE, wxMouseEventHandler(Window::ClicMouse));
    Connect(wxEVT_PAINT, wxPaintEventHandler(Window::OnPaint));

}


class Frame : public wxFrame {
public:
    Frame(const wxString& title);
private:
    Window* p_wnd;

    void OnMenu_FileQuit(wxCommandEvent& evt);
    void Save(wxCommandEvent& evt);
    void Open(wxCommandEvent& WXUNUSED(event));
    void OnMouseMove(wxMouseEvent& evt);
    void Paint_Resistor(wxCommandEvent&);
    void Paint_Transistor(wxCommandEvent&);
    void Paint_Source(wxCommandEvent&);

};

void Window::ClicMouse(wxMouseEvent& evt) {
    item.x_pos = evt.GetX();
    item.y_pos = evt.GetY();
    Refresh();
}

void Window::ReadFile(fstream& file) {
    std::string str_element;
    file >> item.x_pos;
    file >> item.y_pos;
    file >> str_element;
    if (str_element == "RESISTOR") item.type = 1;
    if (str_element == "TRANSISTOR") item.type = 2;
    if (str_element == "SOURCE") item.type = 3;
    if (str_element != "RESISTOR" && str_element != "TRANSISTOR" && str_element != "SOURCE") {
        wxMessageBox(wxT("The file haved invalid data's"));
    }
    Refresh();
}

void Frame::Open(wxCommandEvent& WXUNUSED(event))
{

    wxFileDialog
        openFileDialog(this, _("Open text file"), "", "",
            "texts files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; 

    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
    else {
        std::string name_file= openFileDialog.GetPath();
        file.open(name_file);
        p_wnd->ReadFile(file);
        file.close();
    }
}

wxPoint mouse_pos(item.x_pos, item.y_pos);
void Window::WriteFile(fstream& file) {
        file<<item.x_pos<<" "<<item.y_pos<<" ";
        switch (item.type)
        {
        case 1:
            file << "RESISTOR";
            break;
        case 2:
            file << "TRANSISTOR";
            break;
        case 3:
            file << "SOURCE";
            break;
        default:
            break;
        }
}

void ReadLastData (Element &item){
    std::string temp;
    last_data.open("last_data.txt");
    last_data>>item.x_pos;
    last_data>>item.y_pos;
    last_data>>temp;
    if (temp=="RESISTOR") item.type=1;
    if (temp=="TRANSISTOR") item.type=2;
    if (temp=="SOURCE") item.type=3;
    last_data.close();
}

void Frame::OnMenu_FileQuit(wxCommandEvent& evt) {  

    int dialog_return_value = wxNO;
    wxMessageDialog* dial = new wxMessageDialog(NULL, _("Close program?"), _("Quit"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dialog_return_value = dial->ShowModal();

    switch (dialog_return_value)
    {
    case 5103:{

        last_data.open("last_data.txt", std::ofstream::out | std::ofstream::trunc);

            last_data << item.x_pos << " " << item.y_pos << " ";
            switch (item.type)
            {
            case 1: last_data << "RESISTOR"; break;
            case 2: last_data << "TRANSISTOR"; break;
            case 3: last_data << "SOURCE"; break;
            default:
                break;
            }

        last_data.close();
        Close(true);
            }
    case wxNO:
        break;
    default:
        break;
    }
}

void Frame::Save(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog
        saveFileDialog(this, _("Save txt file"), "", "",
            "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return; 

    wxFileOutputStream output_stream(saveFileDialog.GetPath());
    if (!output_stream.IsOk())
    {
        wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }
    else {
        std::string name_file = saveFileDialog.GetPath();
        file.open(name_file);
        p_wnd->WriteFile(file);
        file.close();
    }

}

void Frame::OnMouseMove(wxMouseEvent& evt) {
    SetStatusText(wxString::Format("[ %d, %d ]", evt.GetX(), evt.GetY(), 1));
}



void Frame::Paint_Resistor(wxCommandEvent& evt) {
    item.type=1; 
    Refresh();
}

void Frame::Paint_Transistor(wxCommandEvent& evt) {
    item.type = 2;
    Refresh();
}

void Frame::Paint_Source(wxCommandEvent& evt) {
    item.type = 3;
    Refresh();
}

void Window::OnPaint(wxPaintEvent& evt) {
    wxMouseEvent evn;
    wxPaintDC dc(this);

    wxBrush brush(wxColor(0, 255, 50));

    dc.SetBrush(brush);

    wxPen pen(wxColor(0, 0, 0), 2);
    dc.SetPen(pen);

        switch (item.type) {
        case 1: {
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos), wxPoint(item.x_pos, item.y_pos) + wxPoint(50, 0));
            dc.DrawRectangle(wxPoint(item.x_pos, item.y_pos) + wxPoint(50, -20), wxSize(100, 40));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(150, 0), wxPoint(item.x_pos, item.y_pos) + wxPoint(200, 0));
            break;
        }
        case 2: {
            dc.DrawCircle(wxPoint(item.x_pos, item.y_pos) + wxPoint(40, 0), wxCoord(30));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos), wxPoint(item.x_pos, item.y_pos) + wxPoint(30, 0));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(30, -20), wxPoint(item.x_pos, item.y_pos) + wxPoint(30, 20));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(30, -7), wxPoint(item.x_pos, item.y_pos) + wxPoint(70, -25));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(30, 7),  wxPoint(item.x_pos, item.y_pos) + wxPoint(70, 25));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(60, 20), wxPoint(item.x_pos, item.y_pos) + wxPoint(57, 13));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(60, 22), wxPoint(item.x_pos, item.y_pos) + wxPoint(50, 22));
            break;
        }
        case 3: {
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos), wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 30));
            dc.DrawCircle(wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 60), wxCoord(30));

            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 40), wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 80));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 40), wxPoint(item.x_pos, item.y_pos) + wxPoint(-10, 50));
            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 40), wxPoint(item.x_pos, item.y_pos) + wxPoint(10, 50));

            dc.DrawLine(wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 90), wxPoint(item.x_pos, item.y_pos) + wxPoint(0, 120));
            break;
        }
        default:
            break;
        }

    dc.SetTextForeground(wxColour(0, 0, 255));

}

Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600)) {

    wxMenuBar* p_menubar = new wxMenuBar;
    wxMenu* p_menuFile = new wxMenu;
    wxMenu* p_menuElement = new wxMenu;
    p_wnd = new Window(this);
   
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

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenu_FileQuit));
    Connect(wxMENU_ITEM_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Open));
    Connect(wxMENU_ITEM_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Save));
    Connect(wxMENU_ITEM_RESISTOR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Paint_Resistor));
    Connect(wxMENU_ITEM_TRANSISTOR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Paint_Transistor));
    Connect(wxMENU_ITEM_SOURCE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Paint_Source));
    Connect(wxEVT_MOTION, wxMouseEventHandler(Frame::OnMouseMove));
;

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
    ReadLastData(item);
    Frame* frame = new Frame(wxT("Simulator"));
    frame->Show(true);

    return true;
}

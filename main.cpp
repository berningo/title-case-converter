#include <wx/app.h>

#include "mainwindow.h"

class TitleCaseApp final : public wxApp
{
public:
    bool OnInit() override
    {
        SetAppName("Title Case Converter");
        SetVendorName("berningo.com");
        auto *window = new MainWindow();
        window->Show();
        return true;
    }
};

wxIMPLEMENT_APP(TitleCaseApp);

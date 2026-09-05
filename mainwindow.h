#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <wx/frame.h>
#include <wx/string.h>

class wxCommandEvent;
class wxStaticText;
class wxTextCtrl;

class MainWindow final : public wxFrame
{
public:
    MainWindow();

private:
    void OnConvert(wxCommandEvent &event);
    void OnCopy(wxCommandEvent &event);
    void OnClear(wxCommandEvent &event);
    void OnLoadFile(wxCommandEvent &event);
    void OnSaveFile(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnInputChanged(wxCommandEvent &event);
    void UpdateWordCount();

    bool IsMinorWord(const wxString &word) const;
    wxString TitleCase(const wxString &input) const;
    void ShowStatus(const wxString &message);

    wxTextCtrl *inputEdit = nullptr;
    wxTextCtrl *outputEdit = nullptr;
    wxStaticText *wordCountLabel = nullptr;

    wxDECLARE_EVENT_TABLE();
};

#endif

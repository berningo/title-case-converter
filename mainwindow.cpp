#include "mainwindow.h"

#include <wx/button.h>
#include <wx/clipbrd.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/tokenzr.h>

#include <set>

namespace
{
enum
{
    ID_Convert = wxID_HIGHEST + 1,
    ID_Copy,
    ID_Reset,
    ID_Open,
    ID_Save
};

wxArrayString SplitWords(const wxString &text)
{
    wxArrayString words;
    wxStringTokenizer tokenizer(text, " \t\r\n", wxTOKEN_STRTOK);
    while (tokenizer.HasMoreTokens())
        words.Add(tokenizer.GetNextToken());
    return words;
}

bool IsUppercaseAbbreviation(const wxString &word)
{
    bool hasLetter = false;
    bool allUppercase = true;

    for (const wxUniChar character : word) {
        if (!wxIsalpha(character))
            continue;
        hasLetter = true;
        if (!wxIsupper(character)) {
            allUppercase = false;
            break;
        }
    }

    return hasLetter && allUppercase;
}
} // namespace

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_BUTTON(ID_Convert, MainWindow::OnConvert)
    EVT_BUTTON(ID_Copy, MainWindow::OnCopy)
    EVT_BUTTON(ID_Reset, MainWindow::OnClear)
    EVT_BUTTON(ID_Open, MainWindow::OnLoadFile)
    EVT_BUTTON(ID_Save, MainWindow::OnSaveFile)
    EVT_TEXT(wxID_ANY, MainWindow::OnInputChanged)
    EVT_MENU(ID_Open, MainWindow::OnLoadFile)
    EVT_MENU(ID_Save, MainWindow::OnSaveFile)
    EVT_MENU(wxID_COPY, MainWindow::OnCopy)
    EVT_MENU(ID_Reset, MainWindow::OnClear)
    EVT_MENU(wxID_EXIT, MainWindow::OnExit)
    EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
wxEND_EVENT_TABLE()

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Title Case Converter", wxDefaultPosition,
              wxSize(700, 500), wxDEFAULT_FRAME_STYLE)
{
    SetMinSize(wxSize(500, 400));

    auto *fileMenu = new wxMenu();
    fileMenu->Append(ID_Open, "&Open...\tCtrl-O", "Load text from file");
    fileMenu->Append(ID_Save, "&Save...\tCtrl-S", "Save converted text to file");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tCtrl-Q", "Exit application");

    auto *editMenu = new wxMenu();
    editMenu->Append(wxID_COPY, "&Copy\tCtrl-C", "Copy converted text to clipboard");
    editMenu->Append(ID_Reset, "&Reset\tDelete", "Reset all fields");

    auto *helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About", "About this application");

    auto *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(editMenu, "&Edit");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

    auto *mainSizer = new wxBoxSizer(wxVERTICAL);
    auto *inputBox = new wxStaticBoxSizer(wxVERTICAL, this, "Input");
    inputEdit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                               wxDefaultSize, wxTE_MULTILINE);
    inputEdit->SetFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    inputEdit->SetHint("Enter text here...");
    inputBox->Add(inputEdit, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(inputBox, 1, wxEXPAND | wxALL, 10);

    auto *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    auto addButton = [this, buttonSizer](int id, const wxString &label,
                                         const wxString &help) {
        auto *button = new wxButton(this, id, label);
        button->SetToolTip(help);
        buttonSizer->Add(button, 0, wxRIGHT, 10);
    };
    addButton(ID_Open, "Load", "Load text from file");
    addButton(ID_Save, "Save", "Save converted text to file");
    buttonSizer->AddStretchSpacer();
    addButton(ID_Convert, "Convert", "Convert text to title case");
    addButton(ID_Reset, "Reset", "Reset all fields");
    buttonSizer->AddStretchSpacer();
    addButton(ID_Copy, "Copy", "Copy converted text to clipboard");
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    auto *outputBox = new wxStaticBoxSizer(wxVERTICAL, this, "Output");
    outputEdit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    outputEdit->SetFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    outputBox->Add(outputEdit, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(outputBox, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

    wordCountLabel = new wxStaticText(this, wxID_ANY, "Word count: 0");
    mainSizer->Add(wordCountLabel, 0, wxALIGN_RIGHT | wxALL, 10);
    SetSizer(mainSizer);
    CreateStatusBar();
    ShowStatus("Ready");
}

bool MainWindow::IsMinorWord(const wxString &word) const
{
    static const std::set<wxString> minorWords = {
        "a", "an", "the", "and", "but", "for", "nor", "or", "so", "yet",
        "as", "at", "by", "in", "of", "off", "on", "per", "to", "up", "via"};
    return minorWords.count(word.Lower()) != 0;
}

wxString MainWindow::TitleCase(const wxString &input) const
{
    wxArrayString words = SplitWords(input);
    if (words.IsEmpty())
        return wxEmptyString;
    for (size_t i = 0; i < words.size(); ++i) {
        const wxString originalWord = words[i];
        words[i].MakeLower();

        if (originalWord.length() > 1 &&
            IsUppercaseAbbreviation(originalWord) &&
            !IsMinorWord(words[i])) {
            words[i] = originalWord;
            continue;
        }

        if (i == 0 || !IsMinorWord(words[i]))
            words[i][0] = wxToupper(words[i][0]);
    }
    return wxJoin(words, ' ');
}

void MainWindow::UpdateWordCount()
{
    wordCountLabel->SetLabel(wxString::Format("Word count: %lu", static_cast<unsigned long>(SplitWords(inputEdit->GetValue()).size())));
}

void MainWindow::ShowStatus(const wxString &message)
{
    SetStatusText(message);
}

void MainWindow::OnInputChanged(wxCommandEvent &event)
{
    if (event.GetEventObject() == inputEdit) {
        UpdateWordCount();
        const wxString text = inputEdit->GetValue();
        wxString trimmed = text;
        trimmed.Trim(true).Trim(false);
        outputEdit->ChangeValue(trimmed.IsEmpty() ? wxString() : TitleCase(text));
    }
    event.Skip();
}

void MainWindow::OnConvert(wxCommandEvent &)
{
    const wxString input = inputEdit->GetValue();
    if (SplitWords(input).IsEmpty()) {
        wxMessageBox("Please enter a text!", "Error", wxOK | wxICON_WARNING, this);
        ShowStatus("Error: No text entered");
        return;
    }
    outputEdit->SetValue(TitleCase(input));
    ShowStatus(wxString::Format("Conversion successful (%lu words)", static_cast<unsigned long>(SplitWords(input).size())));
}

void MainWindow::OnCopy(wxCommandEvent &)
{
    const wxString text = outputEdit->GetValue();
    if (text.IsEmpty()) {
        wxMessageBox("No text to copy!", "Error", wxOK | wxICON_WARNING, this);
        ShowStatus("Error: No text to copy");
        return;
    }
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(text));
        wxTheClipboard->Close();
        ShowStatus("Text copied");
    }
}

void MainWindow::OnClear(wxCommandEvent &)
{
    inputEdit->Clear();
    outputEdit->Clear();
    UpdateWordCount();
    ShowStatus("All reset");
}

void MainWindow::OnLoadFile(wxCommandEvent &)
{
    wxFileDialog dialog(this, "Open text file", wxEmptyString, wxEmptyString,
                        "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dialog.ShowModal() != wxID_OK)
        return;
    wxFile file(dialog.GetPath());
    wxString content;
    if (!file.IsOpened() || !file.ReadAll(&content)) {
        wxMessageBox("Cannot open file:\n" + dialog.GetPath(), "Error", wxOK | wxICON_ERROR, this);
        return;
    }
    inputEdit->SetValue(content);
    ShowStatus("File opened: " + dialog.GetPath());
}

void MainWindow::OnSaveFile(wxCommandEvent &)
{
    const wxString text = outputEdit->GetValue();
    if (text.IsEmpty()) {
        wxMessageBox("No text to save!", "Error", wxOK | wxICON_WARNING, this);
        ShowStatus("Error: No text to save");
        return;
    }
    wxFileDialog dialog(this, "Save output", wxEmptyString, "title_case_output.txt",
                        "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dialog.ShowModal() != wxID_OK)
        return;
    wxFile file(dialog.GetPath(), wxFile::write);
    if (!file.IsOpened() || file.Write(text) != text.length()) {
        wxMessageBox("Cannot save file:\n" + dialog.GetPath(), "Error", wxOK | wxICON_ERROR, this);
        return;
    }
    ShowStatus("Saved: " + dialog.GetPath());
}

void MainWindow::OnExit(wxCommandEvent &)
{
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent &)
{
    wxMessageBox("Title Case Converter\n\nVersion 0.8\nA tool to convert text to APA-7 Title Case.\n\nDeveloped with wxWidgets and C++17\nDeveloped by Oliver Berning\nLicense: GPL v3",
                 "About Title Case Converter", wxOK | wxICON_INFORMATION, this);
}

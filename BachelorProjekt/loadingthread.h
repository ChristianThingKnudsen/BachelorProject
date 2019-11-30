#ifndef LOADINGTHREAD_H
#define LOADINGTHREAD_H
#include <QThread>

#include "cbctrecon.h"
#include "cbctregistration.h"
#include "cbctregistration_test.hpp"

//QT
#include <QStandardItemModel>

class LoadingThread : public QThread
{
    Q_OBJECT
public:
    // For threading
    explicit LoadingThread(Scui *parent=nullptr);
    void run();
    Scui* m_parent; // Used to call objects and methods in the main class
    bool Stop = false;

    // Relevant variables
    CbctRecon* m_cbctrecon; // Main object to acces
    CbctRegistration* m_cbctregistration; // just for convienience
    std::unique_ptr<QStandardItemModel> m_pTableModel;
    std::unique_ptr<CbctRegistrationTest> m_dlgRegistration;
    void init_DlgRegistration(QString &str_dcm_uid) const;
    FilterReaderType::Pointer ReadBowtieFileWhileProbing(const QString &proj_path, std::tuple<bool, bool> &answers);
    std::tuple<bool, bool> probeUser(const QString);
    void SelectComboExternal(const int idx, const enREGI_IMAGES iImage);
    void UpdateListOfComboBox(const int idx) const;
    void LoadImgFromComboBox(const int idx, QString &strSelectedComboTxt);
    void UpdateVOICombobox(const ctType ct_type) const;
    UShortImageType::Pointer m_spFixed;  // pointer only, for display
    UShortImageType::Pointer m_spMoving; // pointer only, for display
    ctType get_ctType(const QString &selText);
    void whenFixedImgLoaded() const;

private slots:
    void SLT_SetHisDir();
    void SLT_LoadSelectedProjFiles(QString &path);
    void SLT_DoBowtieCorrection();
    void SLT_DoReconstruction();

signals:
    void SignalMessageBox(int, QString,QString);
    void Signal_SetButtonsAfterLoad();
    void Signal_UpdateSlider(int);
    void Signal_DisconnectSlider();
    void Signal_ReConnectSlider(int);
    void Signal_UpdateProgressBarLoad(int);
    void Signal_LThreadIsDone();
};
#endif // LOADINGTHREAD_H

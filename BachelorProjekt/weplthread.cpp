#include "weplthread.h"
#include <QThread>
#include <QtCore>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "mha_io.h"
#include "cbctrecon_io.h"
#include "scui.h"

WEPLThread::WEPLThread(Scui *parent) : QThread(dynamic_cast<QObject*>(parent)) // Constructor
{
    this->m_parent = parent;
    this->m_cbctrecon = parent->m_cbctrecon.get();
    this->m_cbctregistration = parent->m_cbctregistration.get();
}

void WEPLThread::run(){ // Run method
    emit Signal_UpdateProgressBarWEPL(20);
    this->SLT_WEPLcalc(m_parent->Structure);
}

void WEPLThread::SLT_WEPLcalc(QString structure) { // Method for calc WEPL
  //Get VIO
  m_parent->m_spMovingImg = m_parent->m_cbctrecon->m_spDeformedCT_Final.GetPointer();
  const auto voi_name = structure.toStdString();

  const auto gantry_angle = 0;//Hardcoded value (from spinBox_GantryAngle)
  const auto couch_angle = 0;//Hardcoded value (from spinBox_CouchAngle)

  const auto ct_type = get_ctType("DEFORMED_CT_FINAL");// from (comboBoxImgMoving)
  const auto ss = m_cbctrecon->m_structures->get_ss(ct_type);
  m_cbctregistration->cur_voi = ss->get_roi_by_name(voi_name);
  emit Signal_UpdateProgressBarWEPL(60);
  const auto wepl_voi =
      CalculateWEPLtoVOI(m_cbctregistration->cur_voi.get(), gantry_angle,
                         couch_angle, m_parent->m_spMovingImg, m_parent->m_spFixedImg);
  m_cbctregistration->WEPL_voi = std::make_unique<Rtss_roi_modern>(*wepl_voi);
  emit Signal_UpdateProgressBarWEPL(80);
  // Draw WEPL
  m_parent->m_spMovingImg = m_parent->m_cbctrecon->m_spScatCorrReconImg.GetPointer();
  emit Signal_DrawWEPL();
  emit Signal_UpdateProgressBarWEPL(100);
}

ctType WEPLThread::get_ctType(const QString &selText) { // Returns the CT type
  emit Signal_UpdateProgressBarWEPL(40);
  if (selText.compare("REF_CT") == 0) {
    return PLAN_CT;
  }
  if (selText.compare("MANUAL_RIGID_CT") == 0 ||
      selText.compare("AUTO_RIGID_CT") == 0) {
    return RIGID_CT;
  }
  if (selText.compare("DEFORMED_CT1") == 0 ||
      selText.compare("DEFORMED_CT2") == 0 ||
      selText.compare("DEFORMED_CT3") == 0 ||
      selText.compare("DEFORMED_CT_FINAL") == 0) {
    return DEFORM_CT;
  }
  return PLAN_CT;
}

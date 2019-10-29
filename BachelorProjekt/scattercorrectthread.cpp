#include "scattercorrectthread.h"

#include <QThread>
#include <QtCore>

#include "qyklabel.h"
#include <qfiledialog.h>

#include "cbctrecon.h"
#include "cbctregistration.h"
#include "mainwindow.h"

enum enCOLOR {
  RED,
  GREEN,
};

ScatterCorrectThread::ScatterCorrectThread(MainWindow *parent) : QThread(dynamic_cast<QObject*>(parent))
{
    this->m_parent = parent;
    this->m_cbctrecon = parent->m_cbctrecon.get();
    this->m_cbctregistration = parent->m_cbctregistration.get();

    //m_YKImgFixed = &m_cbctregistration->m_YKImgFixed[0];
    //m_YKImgMoving = &m_cbctregistration->m_YKImgMoving[0];
    //m_YKDisp = &m_cbctregistration->m_YKDisp[0];
    //m_DoseImgFixed = &m_cbctregistration->m_DoseImgFixed[0];
    //m_DoseImgMoving = &m_cbctregistration->m_DoseImgMoving[0];
    //m_AGDisp_Overlay = &m_cbctregistration->m_AGDisp_Overlay[0];
    //m_YKDisp = parent->m_cbctregistration->m_YKDisp;
    //m_spFixedImg = parent->m_spFixedImg.GetPointer();
    //m_spMovingImg = parent->m_spMovingImg.GetPointer();
}
void ScatterCorrectThread::run(){
    m_parent->SLT_FixedImageSelected(QString("RAW_CBCT"));
    m_parent->SLT_MovingImageSelected(QString("MANUAL_RIGID_CT"));

    this->SLT_ManualMoveByDCMPlanOpen();
    this->SLT_DoRegistrationRigid();
}

// change origin of moving image by shift value acquired from gradient
// registration
/*void ScatterCorrectThread::ImageManualMoveOneShot(
    const float shiftX, const float shiftY,
    const float shiftZ) // DICOM coordinate
{
  if (m_spMovingImg == nullptr) {
    return;
  }

  // USHORT_ImageType::SizeType imgSize =
  // m_spMovingImg->GetRequestedRegion().GetSize(); //1016x1016 x z
  using USPointType = UShortImageType::PointType;
  auto imgOrigin = m_spMovingImg->GetOrigin();
  // USHORT_ImageType::SpacingType imgSpacing = m_spFixedImg->GetSpacing();
  imgOrigin[0] = imgOrigin[0] - static_cast<USPointType::ValueType>(shiftX);
  imgOrigin[1] = imgOrigin[1] - static_cast<USPointType::ValueType>(shiftY);
  imgOrigin[2] = imgOrigin[2] - static_cast<USPointType::ValueType>(shiftZ);

  m_spMovingImg->SetOrigin(imgOrigin);

  emit SignalDrawImageWhenSliceChange();
  //SLT_DrawImageWhenSliceChange();

  // Display relative movement
  // Starting point? RefCT image
  // Only Valid when Moving image is the ManualMove
  auto imgOriginRef = this->m_cbctrecon->m_spRefCTImg->GetOrigin(); //m_pParent replaced with this

  QString strDelta;
  strDelta.sprintf(
      "delta(mm): %3.1f, %3.1f, %3.1f", imgOrigin[0] - imgOriginRef[0],
      imgOrigin[1] - imgOriginRef[1], imgOrigin[2] - imgOriginRef[2]);
  //this->ui.lineEditOriginChanged->setText(strDelta);
}*/

void ScatterCorrectThread::SLT_ManualMoveByDCMPlanOpen() {
  auto filePath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\Plan CT\\E_PT1 plan\\RN.1.2.246.352.71.5.361940808526.11351.20190611075823.dcm");
  /*
  auto filePath = QFileDialog::getOpenFileName(
      this, "Open DCMRT Plan file", p_parent->m_strPathDirDefault,
      "DCMRT Plan (*.dcm)", nullptr, nullptr);
  */
  if (filePath.length() < 1) {
    return;
  }

  const auto planIso = m_cbctregistration->GetIsocenterDCM_FromRTPlan(filePath);

  if (fabs(planIso.x) < 0.001 && fabs(planIso.y) < 0.001 &&
      fabs(planIso.z) < 0.001) {
    std::cout
        << "Warning!!!! Plan iso is 0 0 0. Most likely not processed properly"
        << std::endl;
  } else {
    std::cout << "isocenter was found: " << planIso.x << ", " << planIso.y
              << ", " << planIso.z << std::endl;
  }

  if (m_cbctrecon->m_spRefCTImg == nullptr ||
      m_cbctrecon->m_spManualRigidCT == nullptr) {
      std::cerr << "No moving image available for isocenter move\n";
    return;
  }

  m_parent->ImageManualMoveOneShot(static_cast<float>(planIso.x),
                         static_cast<float>(planIso.y),
                         static_cast<float>(planIso.z));

  /* Should be done in confirm manual!
  const auto trn_vec =
      FloatVector{static_cast<float>(planIso.x), static_cast<float>(planIso.y),
                  static_cast<float>(planIso.z)};
  auto &structs = m_cbctregistration->m_pParent->m_structures;
  structs->ApplyVectorTransform_InPlace<PLAN_CT>(trn_vec);
  */

  //UpdateListOfComboBox(0); // combo selection signalis called
  //UpdateListOfComboBox(1);

  //SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  //SelectComboExternal(1, REGISTER_MANUAL_RIGID);
  m_parent->SLT_FixedImageSelected(QString("RAW_CBCT"));
  m_parent->SLT_MovingImageSelected(QString("MANUAL_RIGID_CT"));
}

// External method implemented from DlgRegistration
// Is called when the "Scatter Correct" button is pushed
void ScatterCorrectThread::SLT_DoRegistrationRigid() // plastimatch auto registration
{
    //Added by us:
    /*
    auto p_dspykimg = this->m_cbctrecon->m_dspYKReconImage.get();
    p_dspykimg->m_bDrawFOVCircle = false;
    p_dspykimg->m_bDrawTableLine = false;

    p_dspykimg->FillPixMapMinMax(0,2031);//this->ui.sliderReconImgMin->value(),
                                 //this->ui.sliderReconImgMax->value());
    this->ui->labelImageCor->SetBaseImage(p_dspykimg);
    this->ui->labelImageCor->update();
    //
    */

    //This code is adde by us and is found from the callback from the comboboxes.
    m_parent->SLT_FixedImageSelected(QString("RAW_CBCT"));
    m_parent->SLT_MovingImageSelected(QString("MANUAL_RIGID_CT"));

    //Callback to Open RT plan:


  // 1) Save current image files
   // m_spFixedImg replaced with m_spRawReconImg
    // m_spMovingImg replaced with m_spRefCTImg
  if (m_cbctrecon->m_spRawReconImg == nullptr || m_cbctrecon->m_spRefCTImg == nullptr) {
    return;
  }

  // Before the registration, remove background and fill the air-bubble of the
  // cbct  1) calculate manual shift value  PreProcess_CBCT(); // remove skin
  // and fill bubble before registration

  if (this->m_cbctrecon->m_spRefCTImg == nullptr ||
      this->m_cbctrecon->m_spManualRigidCT == nullptr) {
    return;
  }
 // In Andreases code this was not checked so we make it false (checkBoxKeyMoving) This is moving the pictures with the keyboard
  /*
  if (false){// this->ui.checkBoxKeyMoving->isChecked()) {
    SLT_KeyMoving(false);
  }
  */
  /*- Make a synthetic std::vector field according to the translation
        plastimatch synth-vf --fixed [msk_skin.mha] --output
  [xf_manual_trans.mha] --xf-trans "[origin diff (raw - regi)]" plastimatch
  synth-vf --fixed E:\PlastimatchData\DicomEg\OLD\msk_skin.mha --output
  E:\PlastimatchData\DicomEg\OLD\xf_manual_trans.mha --xf-trans "21 -29 1"

        - Move the skin contour according to the std::vector field
        plastimatch warp --input [msk_skin.mha] --output-img
  [msk_skin_manRegi.mha] --xf [xf_manual_trans.mha] plastimatch warp --input
  E:\PlastimatchData\DicomEg\OLD\msk_skin.mha --output-img
  E:\PlastimatchData\DicomEg\OLD\msk_skin_manRegi.mha --xf
  E:\PlastimatchData\DicomEg\OLD\xf_manual_trans.mha*/

  std::cout << "1: writing temporary files" << std::endl;

  // Both image type: Unsigned Short
  auto filePathFixed =
      m_cbctregistration->m_strPathPlastimatch + "/" + "fixed_rigid.mha";
  auto filePathMoving =
      m_cbctregistration->m_strPathPlastimatch + "/" + "moving_rigid.mha";
  auto filePathOutput =
      m_cbctregistration->m_strPathPlastimatch + "/" + "output_rigid.mha";
  const auto filePathXform =
      m_cbctregistration->m_strPathPlastimatch + "/" + "xform_rigid.txt";
  auto filePathROI = m_cbctregistration->m_strPathPlastimatch + "/" +
                     "fixed_roi_rigid.mha"; // optional

  using writerType = itk::ImageFileWriter<UShortImageType>;

  auto writer1 = writerType::New();
  writer1->SetFileName(filePathFixed.toLocal8Bit().constData());
  writer1->SetUseCompression(true);
  writer1->SetInput(this->m_parent->m_spFixedImg);

  auto writer2 = writerType::New();
  writer2->SetFileName(filePathMoving.toLocal8Bit().constData());
  writer2->SetUseCompression(true);
  writer2->SetInput(this->m_parent->m_spMovingImg);

  writer1->Update();
  writer2->Update();
  // In Andreases code this was checked so we make it true (checkBoxUseROIForRigid)
  if (true){//this->ui.checkBoxUseROIForRigid->isChecked()) {
    std::cout << "Creating a ROI mask for Rigid registration " << std::endl;
    // In Andreases code the value was set (lineEditFOVPos)
    auto strFOVGeom = QString("0.0,0.0,190.0"); //this->ui.lineEditFOVPos->text();

    auto strListFOV = strFOVGeom.split(",");
    if (strListFOV.count() == 3) {
      const auto FOV_DcmPosX = strListFOV.at(0).toFloat(); // mm
      const auto FOV_DcmPosY = strListFOV.at(1).toFloat();
      const auto FOV_Radius = strListFOV.at(2).toFloat();

      // Create Image using FixedImage sp

      // Image Pointer here
      UShortImageType::Pointer spRoiMask;
      AllocateByRef<UShortImageType, UShortImageType>(m_cbctrecon->m_spRawReconImg, spRoiMask);
      this->m_cbctrecon->GenerateCylinderMask(spRoiMask, FOV_DcmPosX,
                                                   FOV_DcmPosY, FOV_Radius);

      auto writer3 = writerType::New();
      writer3->SetFileName(filePathROI.toLocal8Bit().constData());
      writer3->SetUseCompression(true);
      writer3->SetInput(spRoiMask);
      writer3->Update();
    }
    std::cout << "2.A:  ROI-based Rigid body registration will be done"
              << std::endl;
  } else {
    filePathROI = QString("");
  }

  // Preprocessing
  // 2) move CT-based skin mask on CBCT based on manual shift
  //  if (m_strPathCTSkin)

  // m_strPathCTSkin is prepared after PreProcessCT()

  std::cout << "2: Creating a plastimatch command file" << std::endl;

  const auto fnCmdRegisterRigid = QString("cmd_register_rigid.txt");
  // QString fnCmdRegisterDeform = "cmd_register_deform.txt";
  auto pathCmdRegister =
      m_cbctregistration->m_strPathPlastimatch + "/" + fnCmdRegisterRigid;

  const auto strDummy = QString("");

  // In Andreases code this was not checked so we make it false (radioButton_mse)
  const auto mse = false;//this->ui.radioButton_mse->isChecked();
  // In Andreases code this was checked but we do not use cuda, so we make it false (radioButton_UseCUDA)
  const auto cuda = false;//m_pParent->ui.radioButton_UseCUDA->isChecked();
  // In Andreases code this was initialized (lineEditGradOption)
  auto GradOptionStr = QString("0.7,0.7,0.7");//this->ui.lineEditGradOption->text();
  // For Cropped patients, FOV mask is applied.

  m_cbctregistration->GenPlastiRegisterCommandFile(
      pathCmdRegister, filePathFixed, filePathMoving, filePathOutput,
      filePathXform, PLAST_RIGID, strDummy, strDummy, strDummy, filePathROI,
      mse, cuda, GradOptionStr);

  std::string str_command_filepath = pathCmdRegister.toLocal8Bit().constData();

  m_cbctregistration->CallingPLMCommand(str_command_filepath);

  std::cout << "5: Reading output image-CT" << std::endl;

  auto reader = itk::ImageFileReader<UShortImageType>::New();
  reader->SetFileName(filePathOutput.toLocal8Bit().constData());
  reader->Update();
  this->m_cbctrecon->m_spAutoRigidCT = reader->GetOutput();

  std::cout << "6: Reading is completed" << std::endl;

  const QFile xform_file(filePathXform);

  // If rigid structure already exists copy it to plan-ct structure before
  // applying rigid again.
  if (!m_cbctregistration->m_pParent->m_structures->is_ss_null<RIGID_CT>()) {
    auto rigid_ss = std::make_unique<Rtss_modern>(
        *(m_cbctregistration->m_pParent->m_structures->get_ss(RIGID_CT)));
    m_cbctregistration->m_pParent->m_structures->set_planCT_ss(
        std::move(rigid_ss));
  }

  const auto transform_success =
      m_cbctregistration->m_pParent->m_structures->ApplyTransformTo<PLAN_CT>(
          xform_file);
  if (transform_success) {
    std::cout << "7: Contours registered" << std::endl;
  }
  // We don't use UpdateListOfComboBox
  /*
  UpdateListOfComboBox(0); // combo selection signalis called
  UpdateListOfComboBox(1);
  */
  /*
  SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  SelectComboExternal(1, REGISTER_AUTO_RIGID);
  */

  m_cbctregistration->m_strPathXFAutoRigid = filePathXform; // for further use

  SLT_DoRegistrationDeform(); // Is added by us. Defrom registration
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DoRegistrationRegid
void ScatterCorrectThread::SLT_KeyMoving(const bool bChecked) // Key Moving check box
{
  // In Andreases code he uses checkboxes therefore this is outcommented (radioButton_mse)
  //this->ui.lineEditMovingResol->setDisabled(bChecked);
  if (bChecked) {
    SelectComboExternal(1,REGISTER_MANUAL_RIGID); // should be
  }
  /*
  this->ui.comboBoxImgFixed->setDisabled(bChecked);
  this->ui.comboBoxImgMoving->setDisabled(bChecked);
  this->ui.pushButtonRestoreOriginal->setDisabled(bChecked);
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DoRegistrationRegid
void ScatterCorrectThread::UpdateListOfComboBox(const int idx) const {
// In Andreases code he uses comboboxes, but we don't, therefore we outcomment these(comboBoxImgFixed, comboBoxImgMoving)
    /*
QComboBox *crntCombo;

  if (idx == 0) {
    crntCombo = this->ui.comboBoxImgFixed;
  } else {
    crntCombo = this->ui.comboBoxImgMoving;
  }

  // remove all the list
  crntCombo->clear();
  */
  /*
  const auto p_parent = m_cbctregistration->m_pParent;

  if (p_parent->m_spRawReconImg != nullptr) {
    crntCombo->addItem("RAW_CBCT");
  }

  if (p_parent->m_spRefCTImg != nullptr) {
    crntCombo->addItem("REF_CT");
  }

  if (p_parent->m_spManualRigidCT != nullptr) {
    crntCombo->addItem("MANUAL_RIGID_CT");
  }

  if (p_parent->m_spAutoRigidCT != nullptr) {
    crntCombo->addItem("AUTO_RIGID_CT");
  }

  if (p_parent->m_spDeformedCT1 != nullptr) {
    crntCombo->addItem("DEFORMED_CT1");
  }

  if (p_parent->m_spDeformedCT2 != nullptr) {
    crntCombo->addItem("DEFORMED_CT2");
  }

  if (p_parent->m_spDeformedCT3 != nullptr) {
    crntCombo->addItem("DEFORMED_CT3");
  }

  if (p_parent->m_spDeformedCT_Final != nullptr) {
    crntCombo->addItem("DEFORMED_CT_FINAL");
  }

  if (p_parent->m_spScatCorrReconImg != nullptr) {
    crntCombo->addItem("COR_CBCT");
  }
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DoRegistrationRegid

// externally change  combo box value
void ScatterCorrectThread::SelectComboExternal(const int idx, const enREGI_IMAGES iImage) {
  // In Andreases code he uses comboboxes, but we don't, therefore we outcomment these(comboBoxImgFixed, comboBoxImgMoving)
  /*
  QComboBox *crntCombo;

  if (idx == 0) {
    crntCombo = this->ui.comboBoxImgFixed;
  } else if (idx == 1) {
    crntCombo = this->ui.comboBoxImgMoving;
  } else {
    std::cerr << "What did you do to get here?" << std::endl;
    return;
  }

  auto findIndx = -1;
  switch (iImage) {
  case REGISTER_RAW_CBCT:
    findIndx = crntCombo->findText("RAW_CBCT");
    break;
  case REGISTER_REF_CT:
    findIndx = crntCombo->findText("REF_CT");
    break;
  case REGISTER_MANUAL_RIGID:
    findIndx = crntCombo->findText("MANUAL_RIGID_CT");
    break;
  case REGISTER_AUTO_RIGID:
    findIndx = crntCombo->findText("AUTO_RIGID_CT");
    break;
  case REGISTER_DEFORM1:
    findIndx = crntCombo->findText("DEFORMED_CT1");
    break;
  case REGISTER_DEFORM2:
    findIndx = crntCombo->findText("DEFORMED_CT2");
    break;
  case REGISTER_DEFORM3:
    findIndx = crntCombo->findText("DEFORMED_CT3");
    break;
  case REGISTER_DEFORM_FINAL:
    findIndx = crntCombo->findText("DEFORMED_CT_FINAL");
    break;
  case REGISTER_COR_CBCT:
    findIndx = crntCombo->findText("COR_CBCT");
    break;
  case REGISTER_DEFORM_SKIP_AUTORIGID:
    findIndx = crntCombo->findText("REGISTER_DEFORM_SKIP_AUTORIGID");
    break;
  }
  // std::cout << "setCurrentIndx " << findIndx << std::endl;

  if (findIndx < 0) { //-1 if not found
    return;
  }

  crntCombo->setCurrentIndex(findIndx); // will call "SLT_FixedImageSelected"

  // Below are actually redendency. don't know why setCurrentIndex sometimes
  // doesn't trigger the SLT_MovingImageSelected slot func.

  const auto crntStr = crntCombo->currentText();
  if (idx == 0) {
    SLT_FixedImageSelected(crntStr);
  } else if (idx == 1) {
    SLT_MovingImageSelected(crntStr);
  }
  // If it is called inside the Dlg, SLT seemed not to conneced
  */
    /*
  SLT_FixedImageSelected("RAW_CBCT");
  SLT_MovingImageSelected("MANUAL_RIGID_CT"); // This is added by us
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SelectComboExternal
/*void ScatterCorrectThread::SLT_FixedImageSelected(QString selText) {
  // QString strCrntText = this->ui.comboBoxImgFixed->currentText();
  LoadImgFromComboBox(0, selText); // here, m_spMovingImg and Fixed images are determined
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SelectComboExternal
void ScatterCorrectThread::SLT_MovingImageSelected(QString selText) {
  // QString strCrntText = this->ui.comboBoxImgMoving->currentText();
  // std::cout << "SLT_MovingImageSelected" << std::endl;
  LoadImgFromComboBox(1, selText);
  const auto cur_ct = get_ctType(selText);
  UpdateVOICombobox(cur_ct);
}*/
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_MovingImageSelected
ctType ScatterCorrectThread::get_ctType(const QString &selText) {
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
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_MovingImageSelected
void ScatterCorrectThread::UpdateVOICombobox(const ctType ct_type) const {
  auto struct_set =
      m_cbctregistration->m_pParent->m_structures->get_ss(ct_type);
  if (struct_set == nullptr) {
    return;
  }
  if (struct_set->slist.empty()) {
    std::cerr << "Structures not initialized yet" << std::endl;
    return;
  }
  // In Andreases code he uses checkboxes therefore this is outcommented (comboBox_VOI)
  /*
  this->ui.comboBox_VOI->clear();
  for (const auto &voi : struct_set->slist) {
    this->ui.comboBox_VOI->addItem(QString(voi.name.c_str()));
    this->ui.comboBox_VOItoCropBy->addItem(QString(voi.name.c_str()));
    this->ui.comboBox_VOItoCropBy_copy->addItem(QString(voi.name.c_str()));
  }
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_FixedImageSelected and SLT_MovingImageSelected
/*void ScatterCorrectThread::LoadImgFromComboBox(const int idx, QString &strSelectedComboTxt) // -->when fixed image loaded will be called here!
{
  UShortImageType::Pointer spTmpImg;
  if (strSelectedComboTxt.compare(QString("RAW_CBCT"), Qt::CaseSensitive) ==
      0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spRawReconImg;
  } else if (strSelectedComboTxt.compare(QString("REF_CT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spRefCTImg;
  } else if (strSelectedComboTxt.compare(QString("MANUAL_RIGID_CT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spManualRigidCT;
  } else if (strSelectedComboTxt.compare(QString("AUTO_RIGID_CT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spAutoRigidCT;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT1"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT1;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT2"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT2;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT3"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT3;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT_FINAL"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT_Final;
  } else if (strSelectedComboTxt.compare(QString("COR_CBCT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spScatCorrReconImg;
  }

  if (spTmpImg == nullptr) {
    return;
  }

  if (idx == 0) {
    m_spFixedImg = spTmpImg.GetPointer();

    m_parent->whenFixedImgLoaded(); //commented out
  } else if (idx == 1) {
    m_spMovingImg = spTmpImg.GetPointer();
    // In Andreases code this does nothing so far and is therefore  outcommented (whenMovingImgLoaded())
    // m_parent->whenMovingImgLoaded();
  }
   emit SignalDrawImageWhenSliceChange();
  //SLT_DrawImageWhenSliceChange();
}*/
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by set_points_by_slice()
template <enCOLOR color> auto get_qtpoint_vector(qyklabel *window) {
  switch (color) {
  case RED:
    return &window->m_vPt;
  case GREEN:
  default:
    return &window->m_vPt_green;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DrawImageWhenSliceChange()
template <typename ImageBase, enPLANE plane, enCOLOR color>
auto set_points_by_slice(qyklabel *window, Rtss_roi_modern *voi,
                         std::array<double, 3> curPhysPos,
                         typename ImageBase::SpacingType imgSpacing,
                         typename ImageBase::PointType imgOriginFixed,
                         typename ImageBase::SizeType imgSize) {
  auto *Wnd_contour = get_qtpoint_vector<color>(window);
  Wnd_contour->clear();

  auto wnd_size = window->size();
  auto wnd_height = wnd_size.rheight();
  const auto wnd_width = wnd_size.rwidth();
  auto x_scale = 1.0 / static_cast<double>(wnd_width);
  auto y_scale = 1.0 / static_cast<double>(wnd_height);
  switch (plane) {
  case PLANE_AXIAL:
    x_scale *= imgSpacing[0] * imgSize[0];
    y_scale *= imgSpacing[1] * imgSize[1];
    break;
  case PLANE_FRONTAL:
    x_scale *= imgSpacing[0] * imgSize[0];
    y_scale *= imgSpacing[2] * imgSize[2];
    break;
  case PLANE_SAGITTAL:
    x_scale *= imgSpacing[1] * imgSize[1];
    y_scale *= imgSpacing[2] * imgSize[2];
  }

  for (auto contour : voi->pslist) {
    if (contour.coordinates.empty()) {
      continue;
    }
    const auto first_point = contour.coordinates.at(0);
    // Axial
    if (first_point.z > curPhysPos[0] - imgSpacing[2] &&
        first_point.z < curPhysPos[0] + imgSpacing[2] && plane == PLANE_AXIAL) {
      for (auto point : contour.coordinates) {
        Wnd_contour->emplace_back((point.x - imgOriginFixed[0]) / x_scale,
                                  (point.y - imgOriginFixed[1]) / y_scale);
      }
    }
    for (auto &point : contour.coordinates) {
      // Frontal
      if (point.y > curPhysPos[1] - imgSpacing[1] &&
          point.y < curPhysPos[1] + imgSpacing[1] && plane == PLANE_FRONTAL) {
        Wnd_contour->emplace_back((point.x - imgOriginFixed[0]) / x_scale,
                                  wnd_height -
                                      (point.z - imgOriginFixed[2]) / y_scale);
      }
      // Sagittal
      if (point.x > curPhysPos[2] - imgSpacing[0] &&
          point.x < curPhysPos[2] + imgSpacing[0] && plane == PLANE_SAGITTAL) {
        Wnd_contour->emplace_back((point.y - imgOriginFixed[1]) / x_scale,
                                  wnd_height -
                                      (point.z - imgOriginFixed[2]) / y_scale);
      }
    }
  }

  window->m_bDrawPoints = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
//Is called by LoadImgFromComboBox()
void ScatterCorrectThread::SLT_DrawImageWhenSliceChange() {
  if (m_spFixedImg == nullptr) {
    return;
  }
  // Added by us:
  if(m_cbctrecon->m_spRawReconImg == nullptr){
      return;
  }
  auto imgSize = m_cbctrecon->m_spRawReconImg->GetBufferedRegion().GetSize();
  const auto curPosZ = static_cast<int>(imgSize[2]/2);
  const auto curPosY = static_cast<int>(imgSize[1]/2);
  const auto curPosX = static_cast<int>(imgSize[0]/2);
  //

  int sliderPosIdxZ, sliderPosIdxY, sliderPosIdxX;
  // In Andreases code it's hard to tell what the slider values should be, and therefor will they be initialized as 0.
  switch (m_enViewArrange) {
  case AXIAL_FRONTAL_SAGITTAL:
    sliderPosIdxZ = curPosZ;//this->ui.sliderPosDisp1->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
    sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
    break;
  case FRONTAL_SAGITTAL_AXIAL:
    sliderPosIdxZ = curPosZ;//this->ui.sliderPosDisp1->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
    sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
    break;
  case SAGITTAL_AXIAL_FRONTAL:
    sliderPosIdxZ = curPosZ;//this->ui.sliderPosDisp1->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
    sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
    break;
  default:
    sliderPosIdxZ = curPosZ;//this->ui.sliderPosDisp1->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
    sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
    break;
  }

  //auto imgSize = m_spFixedImg->GetBufferedRegion().GetSize(); // 1016x1016 x z
  auto imgOrigin = m_spFixedImg->GetOrigin();
  auto imgSpacing = m_spFixedImg->GetSpacing();

  auto curPhysPos = std::array<double, 3>{{
      imgOrigin[2] + sliderPosIdxZ * imgSpacing[2], // Z in default setting
      imgOrigin[1] + sliderPosIdxY * imgSpacing[1], // Y
      imgOrigin[0] + sliderPosIdxX * imgSpacing[0]  // Z
  }};

  const auto refIdx = 3 - m_enViewArrange;
  // In Andreases code this checkbox was checked (checkBoxDrawCrosshair)
  if (true){//this->ui.checkBoxDrawCrosshair->isChecked()) {
    m_YKDisp[refIdx % 3].m_bDrawCrosshair = true;
    m_YKDisp[(refIdx + 1) % 3].m_bDrawCrosshair = true;
    m_YKDisp[(refIdx + 2) % 3].m_bDrawCrosshair = true;

    // m_YKDisp[0]// Left Top image, the largest

    m_YKDisp[refIdx % 3].m_ptCrosshair.setX(sliderPosIdxX); // axial
    m_YKDisp[refIdx % 3].m_ptCrosshair.setY(sliderPosIdxY);

    m_YKDisp[(refIdx + 1) % 3].m_ptCrosshair.setX(sliderPosIdxX); // Frontal
    m_YKDisp[(refIdx + 1) % 3].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                                  sliderPosIdxZ - 1);

    m_YKDisp[(refIdx + 2) % 3].m_ptCrosshair.setX(sliderPosIdxY); // Sagittal
    m_YKDisp[(refIdx + 2) % 3].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                                  sliderPosIdxZ - 1);

    m_YKImgFixed[0].m_bDrawCrosshair = true;
    m_YKImgFixed[1].m_bDrawCrosshair = true;
    m_YKImgFixed[2].m_bDrawCrosshair = true;

    m_YKImgFixed[0].m_ptCrosshair.setX(sliderPosIdxX); // sagittal slider
    m_YKImgFixed[0].m_ptCrosshair.setY(sliderPosIdxY);

    m_YKImgFixed[1].m_ptCrosshair.setX(sliderPosIdxX); // sagittal slider
    m_YKImgFixed[1].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                       sliderPosIdxZ - 1);

    m_YKImgFixed[2].m_ptCrosshair.setX(sliderPosIdxY); // sagittal slider
    m_YKImgFixed[2].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                       sliderPosIdxZ - 1);
  } else {
    m_YKDisp[0].m_bDrawCrosshair = false;
    m_YKDisp[1].m_bDrawCrosshair = false;
    m_YKDisp[2].m_bDrawCrosshair = false;

    m_YKImgFixed[0].m_bDrawCrosshair = false;
    m_YKImgFixed[1].m_bDrawCrosshair = false;
    m_YKImgFixed[2].m_bDrawCrosshair = false;
  }

  // center of the split value is passed by m_YKImgFixed;

  if (m_spMovingImg != nullptr) {
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spMovingImg, PLANE_AXIAL, curPhysPos[0], m_YKImgFixed[0],
        m_YKImgMoving[0]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spMovingImg, PLANE_FRONTAL, curPhysPos[1], m_YKImgFixed[1],
        m_YKImgMoving[1]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spMovingImg, PLANE_SAGITTAL, curPhysPos[2], m_YKImgFixed[2],
        m_YKImgMoving[2]);
    if (m_cbctregistration->dose_loaded) {
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spMovingDose, PLANE_AXIAL, curPhysPos[0],
          m_DoseImgFixed[0], m_DoseImgMoving[0]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spMovingDose, PLANE_FRONTAL, curPhysPos[1],
          m_DoseImgFixed[1], m_DoseImgMoving[1]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spMovingDose, PLANE_SAGITTAL, curPhysPos[2],
          m_DoseImgFixed[2], m_DoseImgMoving[2]);
    }
  } else {
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spFixedImg, PLANE_AXIAL, curPhysPos[0], m_YKImgFixed[0],
        m_YKImgMoving[0]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spFixedImg, PLANE_FRONTAL, curPhysPos[1], m_YKImgFixed[1],
        m_YKImgMoving[1]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spFixedImg, PLANE_SAGITTAL, curPhysPos[2], m_YKImgFixed[2],
        m_YKImgMoving[2]);
    if (m_cbctregistration->dose_loaded) {
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spFixedDose, PLANE_AXIAL, curPhysPos[0],
          m_DoseImgFixed[0], m_DoseImgMoving[0]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spFixedDose, PLANE_FRONTAL, curPhysPos[1],
          m_DoseImgFixed[1], m_DoseImgMoving[1]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spFixedDose, PLANE_SAGITTAL, curPhysPos[2],
          m_DoseImgFixed[2], m_DoseImgMoving[2]);
    }
  }

  // Update position lineEdit
  QString strPos1, strPos2, strPos3;
  strPos1.sprintf("%3.1f", curPhysPos[0]);
  strPos2.sprintf("%3.1f", curPhysPos[1]);
  strPos3.sprintf("%3.1f", curPhysPos[2]);
  // In Andreases code he sets these strings, but we don't. Therefore we outcomment them.
  /*
  this->ui.lineEditCurPosX->setText(strPos3);
  this->ui.lineEditCurPosY->setText(strPos2);
  this->ui.lineEditCurPosZ->setText(strPos1);
  */
/*
  ////Update Origin text box
  auto imgOriginFixed = m_spFixedImg->GetOrigin();
  QString strOriFixed;
  strOriFixed.sprintf("%3.4f, %3.4f, %3.4f", imgOriginFixed[0], imgOriginFixed[1], imgOriginFixed[2]);
  // In Andreases code he set the text on the ui. Therefore we outcomment this (lineEditOriginFixed)
  //this->ui.lineEditOriginFixed->setText(strOriFixed);

  if (m_spMovingImg != nullptr) {
    const auto imgOriginMoving = m_spMovingImg->GetOrigin();
    QString strOriMoving;
    strOriMoving.sprintf("%3.4f, %3.4f, %3.4f", imgOriginMoving[0],
                         imgOriginMoving[1], imgOriginMoving[2]);
    // In Andreases code he sets these strings, but we don't. Therefore we outcomment them.
    //this->ui.lineEditOriginMoving->setText(strOriMoving);
  }
  // In Andreases code he have 3 labels on the registration ui, but we only have one. Therefor the below has been changed.
  /*
  auto arr_wnd = std::array<qyklabel *, 3>{{this->ui.labelOverlapWnd1,
                                            this->ui.labelOverlapWnd2,
                                            this->ui.labelOverlapWnd3}};
                                            */
/*
  auto wnd = this->ui->labelImageCor;

  if (m_cbctregistration->cur_voi != nullptr) {
    const auto p_cur_voi = m_cbctregistration->cur_voi.get();

    set_points_by_slice<UShortImageType, PLANE_AXIAL, RED>(
        wnd, p_cur_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

  }

  if (m_cbctregistration->WEPL_voi != nullptr) {
    const auto p_wepl_voi = m_cbctregistration->WEPL_voi.get();

    set_points_by_slice<UShortImageType, PLANE_AXIAL, GREEN>(
        wnd, p_wepl_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

  }

  SLT_DrawImageInFixedSlice();
}
*/
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DrawImageWhenSliceChange()
/*
void ScatterCorrectThread::SLT_DrawImageInFixedSlice() const
// Display Swap here!
{
  // Constitute m_YKDisp from Fixed and Moving
  // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
  if (true){//this->ui.checkBoxDrawSplit->isChecked()) {
    for (auto i = 0; i < 3; i++) {
      auto idxAdd = m_enViewArrange; // m_iViewArrange = 0,1,2
      if (idxAdd + i >= 3) {
        idxAdd = idxAdd - 3;
      }

      m_YKDisp[i].SetSpacing(m_YKImgFixed[i + idxAdd].m_fSpacingX,
                             m_YKImgFixed[i + idxAdd].m_fSpacingY);

      m_YKDisp[i].SetSplitOption(PRI_LEFT_TOP);
      // m_YKDisp[i].SetSplitCenter(QPoint dataPt);//From mouse event
      if (!m_YKDisp[i].ConstituteFromTwo(m_YKImgFixed[i + idxAdd],
                                         m_YKImgMoving[i + idxAdd])) {
        std::cout << "Image error " << i + 1 << " th view" << std::endl;
      }
    }
  } else {
    for (auto i = 0; i < 3; i++) {
      auto addedViewIdx = m_enViewArrange;
      if (i + addedViewIdx >= 3) {
        addedViewIdx = addedViewIdx - 3;
      }

      m_YKDisp[i].CloneImage(m_YKImgFixed[i + addedViewIdx]);
    }
  }

  // For dose overlay
  if (m_cbctregistration->dose_loaded) {
      // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
    if (false){//this->ui.checkBoxDrawSplit->isChecked()) {
      for (auto i = 0; i < 3; i++) {
        auto idxAdd = m_enViewArrange; // m_iViewArrange = 0,1,2
        if (idxAdd + i >= 3) {
          idxAdd = idxAdd - 3;
        }

        m_AGDisp_Overlay[i].SetSpacing(m_DoseImgFixed[i + idxAdd].m_fSpacingX,
                                       m_DoseImgFixed[i + idxAdd].m_fSpacingY);

        m_AGDisp_Overlay[i].SetSplitOption(PRI_LEFT_TOP);
        if (!m_AGDisp_Overlay[i].ConstituteFromTwo(
                m_DoseImgFixed[i + idxAdd], m_DoseImgMoving[i + idxAdd])) {
          std::cout << "Dose Image error " << i + 1 << " th view" << std::endl;
        }
      }
    } else {
      for (auto i = 0; i < 3; i++) {
        auto addedViewIdx = m_enViewArrange;
        if (i + addedViewIdx >= 3) {
          addedViewIdx = addedViewIdx - 3;
        }

        m_AGDisp_Overlay[i].CloneImage(m_DoseImgFixed[i + addedViewIdx]);
      }
    }
  }
  // In Andreases code this is set to 2000 (sliderFixedW)
  const auto sliderW1 = 2000;//this->ui.sliderFixedW->value();
  // In Andreases code this is set to 2000 (sliderMovingW)
  const auto sliderW2 = 2000;//this->ui.sliderMovingW->value();
  // In Andreases code this is set to 1024 (sliderFixedL)
  const auto sliderL1 = 1024;//this->ui.sliderFixedL->value();
  // In Andreases code this is set to 1024 (sliderMovingL)
  const auto sliderL2 = 1024;//this->ui.sliderMovingL->value();

  m_YKDisp[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
  m_YKDisp[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
  m_YKDisp[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

  // In Andreases code he plots this on the registration ui, but we want to plot this on our ui
  this->ui->labelImageCor->SetBaseImage(&m_YKDisp[0]); //this->ui.labelOverlapWnd1->SetBaseImage(&m_YKDisp[0]);
  /*
  this->ui.labelOverlapWnd2->SetBaseImage(&m_YKDisp[1]);
  this->ui.labelOverlapWnd3->SetBaseImage(&m_YKDisp[2]);
  */

  // here gPMC results could be checked for and displayed, possibly with
  // modification to the qyklabel class /AGA 02/08/2017
/*
  if (m_cbctregistration->dose_loaded) {
    m_AGDisp_Overlay[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_AGDisp_Overlay[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_AGDisp_Overlay[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    // In Andreases code he plots this on the registration ui, but we want to plot this on our ui
    this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[0]); //this->ui.labelOverlapWnd1->SetOverlayImage(&m_AGDisp_Overlay[0]);
    /*
    this->ui.labelOverlapWnd2->SetOverlayImage(&m_AGDisp_Overlay[1]);
    this->ui.labelOverlapWnd3->SetOverlayImage(&m_AGDisp_Overlay[2]);
    */
//  }

  // In Andreases code he plots this on the registration ui, but we only have one window
  //this->ui->labelImageCor->update();//this->ui.labelOverlapWnd1->update();
  /*
  this->ui.labelOverlapWnd2->update();
  this->ui.labelOverlapWnd3->update();
  */
/*
}
*/

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code this method is called when deform registraion button is pushed. Find out where to implement this!

void ScatterCorrectThread::SLT_DoRegistrationDeform() {
  if (m_cbctrecon->m_spRawReconImg == nullptr || m_cbctrecon->m_spRefCTImg == nullptr) {
    return;
  }
  //This code is added by us and is found from the callback from the comboboxes.
  m_parent->SLT_FixedImageSelected(QString("RAW_CBCT"));
  m_parent->SLT_MovingImageSelected(QString("AUTO_RIGID_CT"));

  // In Andreases code this was checked, so we replace this with true (checkBoxCropBkgroundCT)
  const auto bPrepareMaskOnly = !true;//this->ui.checkBoxCropBkgroundCT->isChecked();

  std::cout << "0: DoRegistrationDeform: writing temporary files" << std::endl;

  // Both image type: Unsigned Short
  auto filePathFixed =
      m_cbctregistration->m_strPathPlastimatch + "/" + "fixed_deform.mha";
  auto filePathMoving =
      m_cbctregistration->m_strPathPlastimatch + "/" + "moving_deform.mha";
  auto filePathROI = m_cbctregistration->m_strPathPlastimatch + "/" +
                     "fixed_roi_DIR.mha"; // optional

  const auto filePathOutput =
      m_cbctregistration->m_strPathPlastimatch + "/" + "output_deform.mha";
  const auto filePathXform =
      m_cbctregistration->m_strPathPlastimatch + "/" + "xform_deform.txt";

  auto filePathOutputStage1 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage1.mha";
  auto filePathOutputStage2 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage2.mha";
  auto filePathOutputStage3 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage3.mha";

  using writerType = itk::ImageFileWriter<UShortImageType>;

  auto writer1 = writerType::New();
  writer1->SetFileName(filePathFixed.toLocal8Bit().constData());
  writer1->SetUseCompression(true);
  writer1->SetInput(m_parent->m_spFixedImg);

  auto writer2 = writerType::New();
  writer2->SetFileName(filePathMoving.toLocal8Bit().constData());
  writer2->SetUseCompression(true);
  writer2->SetInput(m_parent->m_spMovingImg);
  writer1->Update();
  writer2->Update();

  // Create a mask image based on the fixed sp image
  // In Andreases code this was checked, so we replace this with true (checkBoxUseROIForDIR)
  if (true){//this->ui.checkBoxUseROIForDIR->isChecked()) {
    std::cout << "Creating a ROI mask for DIR.. " << std::endl;

    auto strFOVGeom = QString("0.0,0.0,190.0"); //this->ui.lineEditFOVPos->text();

    auto strListFOV = strFOVGeom.split(",");
    if (strListFOV.count() == 3) {
      const auto FOV_DcmPosX = strListFOV.at(0).toFloat(); // mm
      const auto FOV_DcmPosY = strListFOV.at(1).toFloat();
      const auto FOV_Radius = strListFOV.at(2).toFloat();

      // Create Image using FixedImage sp

      // Image Pointer here
      UShortImageType::Pointer spRoiMask;
      AllocateByRef<UShortImageType, UShortImageType>(m_cbctrecon->m_spRawReconImg, spRoiMask);
      m_cbctregistration->m_pParent->GenerateCylinderMask(
          spRoiMask, FOV_DcmPosX, FOV_DcmPosY, FOV_Radius);

      auto writer3 = writerType::New();
      writer3->SetFileName(filePathROI.toLocal8Bit().constData());
      writer3->SetUseCompression(true);
      writer3->SetInput(spRoiMask);
      writer3->Update();
    }
  }

  auto filePathFixed_proc = filePathFixed;

  std::cout << "1: DoRegistrationDeform: CBCT pre-processing before deformable "
               "registration"
            << std::endl;
  // std::cout << "Air region and bubble will be removed" << std::endl;

  QFileInfo info1(m_cbctregistration->m_strPathCTSkin_autoRegi);
  QFileInfo info2(m_cbctregistration->m_strPathXFAutoRigid);

  if (!info1.exists() || !info2.exists()) {
    std::cout << "Fatal error! no CT skin is found or no XF auto file found. "
                 "Preprocessing will not be done. Proceeding."
              << std::endl;
  } else {
    filePathFixed_proc = m_cbctregistration->m_strPathPlastimatch + "/" +
                         "fixed_deform_proc.mha";
    // skin removal and bubble filling : output file = filePathFixed_proc
    // In Andreases code this was not checked, so we replace this with false (checkBoxFillBubbleCT)
    const auto bBubbleRemoval = false;//this->ui.checkBoxFillBubbleCT->isChecked();
    const auto skinExp = 8.0;//this->ui.lineEditCBCTSkinCropBfDIR->text().toDouble();

    const auto iBubThresholdUshort = 500;//this->ui.spinBoxBkDetectCT->value(); // 500
    const auto iBubFillUshort = 1000;//this->ui.spinBoxBubFillCT->value(); // 1000

    m_cbctregistration->ProcessCBCT_beforeDeformRegi(
        filePathFixed, m_cbctregistration->m_strPathCTSkin_autoRegi,
        filePathFixed_proc, m_cbctregistration->m_strPathXFAutoRigid,
        bBubbleRemoval, bPrepareMaskOnly, skinExp, iBubThresholdUshort,
        iBubFillUshort); // bubble filling yes

    if (bPrepareMaskOnly) {
      filePathFixed_proc = filePathFixed;
    }
  }

  std::cout << "2: DoRegistrationDeform: Creating a plastimatch command file"
            << std::endl;

  const auto fnCmdRegisterRigid = QString("cmd_register_deform.txt");
  // QString fnCmdRegisterDeform = "cmd_register_deform.txt";
  auto pathCmdRegister =
      m_cbctregistration->m_strPathPlastimatch + "/" + fnCmdRegisterRigid;

  auto strDeformableStage1 = QString("2,2,1,30,0.00001,0.005,5");//this->ui.lineEditArgument1->text(); // original param: 7, add output path
  auto strDeformableStage2 = QString("");//this->ui.lineEditArgument2->text();
  auto strDeformableStage3 = QString("");//this->ui.lineEditArgument3->text();

  strDeformableStage1.append(", ").append(filePathOutputStage1);
  strDeformableStage2.append(", ").append(filePathOutputStage2);
  strDeformableStage3.append(", ").append(filePathOutputStage3);

  const auto mse = true;//this->ui.radioButton_mse->isChecked();
  const auto cuda = false;//m_pParent->ui.radioButton_UseCUDA->isChecked();
  auto GradOptionStr = QString("0.7,0.7,0.7");//this->ui.lineEditGradOption->text();
  // For Cropped patients, FOV mask is applied.
  if (true){//this->ui.checkBoxUseROIForDIR->isChecked()) {
    m_cbctregistration->GenPlastiRegisterCommandFile(
        pathCmdRegister, filePathFixed_proc, filePathMoving, filePathOutput,
        filePathXform, PLAST_BSPLINE, strDeformableStage1, strDeformableStage2,
        strDeformableStage3, filePathROI, mse, cuda, GradOptionStr);
  } else {
    m_cbctregistration->GenPlastiRegisterCommandFile(
        pathCmdRegister, filePathFixed_proc, filePathMoving, filePathOutput,
        filePathXform, PLAST_BSPLINE, strDeformableStage1, strDeformableStage2,
        strDeformableStage3, QString(), mse, cuda, GradOptionStr);
  }

  std::string str_command_filepath = pathCmdRegister.toLocal8Bit().constData();

  m_cbctregistration->CallingPLMCommand(str_command_filepath);

  std::cout << "5: DoRegistrationDeform: Registration is done" << std::endl;
  std::cout << "6: DoRegistrationDeform: Reading output image" << std::endl;

  using readerType = itk::ImageFileReader<UShortImageType>;

  auto readerDefSt1 = readerType::New();

  auto tmpFileInfo = QFileInfo(filePathOutputStage1);
  if (tmpFileInfo.exists()) {
    readerDefSt1->SetFileName(filePathOutputStage1.toLocal8Bit().constData());
    readerDefSt1->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT1 = readerDefSt1->GetOutput();
  }

  auto readerDefSt2 = readerType::New();
  tmpFileInfo = QFileInfo(filePathOutputStage2);
  if (tmpFileInfo.exists()) {
    readerDefSt2->SetFileName(filePathOutputStage2.toLocal8Bit().constData());
    readerDefSt2->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT2 = readerDefSt2->GetOutput();
  }

  auto readerDefSt3 = readerType::New();
  tmpFileInfo = QFileInfo(filePathOutputStage3);
  if (tmpFileInfo.exists()) {
    readerDefSt3->SetFileName(filePathOutputStage3.toLocal8Bit().constData());
    readerDefSt3->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT3 = readerDefSt3->GetOutput();
  }

  auto readerFinCBCT = readerType::New();
  tmpFileInfo = QFileInfo(filePathFixed_proc); // cropped image Or not
  if (tmpFileInfo.exists()) {
    readerFinCBCT->SetFileName(filePathFixed_proc.toLocal8Bit().constData());
    readerFinCBCT->Update();
    m_cbctregistration->m_pParent->m_spRawReconImg = readerFinCBCT->GetOutput();
  } else {
    std::cout << "No filePathFixed_proc is available. Exit the function"
              << std::endl;
    return;
  }

  // Puncturing should be done for final Deform image

  auto strPathDeformCTFinal = filePathOutput;

  QFileInfo tmpBubFileInfo(m_cbctregistration->m_strPathMskCBCTBubble);

  if (false && tmpBubFileInfo.exists()) {// this->ui.checkBoxFillBubbleCT->isChecked() && tmpBubFileInfo.exists()) {
    std::cout << "6B: final puncturing according to the CBCT bubble"
              << std::endl;

    /*Mask_parms parms_fill;
    strPathDeformCTFinal = m_strPathPlastimatch + "/deformCTpuncFin.mha";

    parms_fill.mask_operation = MASK_OPERATION_FILL;
    parms_fill.input_fn = filePathOutput.toLocal8Bit().constData();
    parms_fill.mask_fn = m_strPathMskCBCTBubble.toLocal8Bit().constData();
    parms_fill.output_fn = strPathDeformCTFinal.toLocal8Bit().constData();*/

    strPathDeformCTFinal =
        m_cbctregistration->m_strPathPlastimatch + "/deformCTpuncFin.mha";

    const auto enMaskOp = MASK_OPERATION_FILL;
    auto input_fn = filePathOutput;
    auto mask_fn = m_cbctregistration->m_strPathMskCBCTBubble;
    auto output_fn = strPathDeformCTFinal;

    // int iBubblePunctureVal = this->ui.lineEditBkFillCT->text().toInt(); //0 =
    // soft tissue
    const auto iBubblePunctureVal = 0; // 0 = air. deformed CT is now already a USHORT image
    const auto mask_value = iBubblePunctureVal;
    m_cbctregistration->plm_mask_main(enMaskOp, input_fn, mask_fn, output_fn,
                                      static_cast<float>(mask_value));
  }

  auto readerDeformFinal = readerType::New();
  tmpFileInfo = QFileInfo(strPathDeformCTFinal);
  if (tmpFileInfo.exists()) {
    readerDeformFinal->SetFileName(
        strPathDeformCTFinal.toLocal8Bit().constData());
    readerDeformFinal->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT_Final =
        readerDeformFinal->GetOutput();
  } else {
    std::cout << "No final output is available. Exit the function" << std::endl;
    return;
  }

  std::cout << "7: DoRegistrationDeform: Reading is completed" << std::endl;

  const QFile xform_file(filePathXform);
  m_cbctregistration->m_pParent->m_structures->ApplyTransformTo<RIGID_CT>(
      xform_file);

  std::cout << "8: Contours deformed" << std::endl;

  /* Theese does nothing at the moment
  UpdateListOfComboBox(0); // combo selection signalis called
  UpdateListOfComboBox(1);


  SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  SelectComboExternal(1, REGISTER_DEFORM_FINAL);
  */
  std::cout << "FINISHED!: Deformable image registration. Proceed to scatter "
               "correction"
            << std::endl;
  /*
  // This is added by us:
  auto FirstString = QString("RAW_CBCT");
  auto SecondString = QString("DEFORM_FINAL");
  LoadImgFromComboBox(0, FirstString);
  LoadImgFromComboBox(1, SecondString);
  //
  */

  SLT_IntensityNormCBCT();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code this method is called when norm CBCT button is pushed. Find out where to implement this!
void ScatterCorrectThread::SLT_IntensityNormCBCT() {
  //This code is adde by us and is found from the callback from the comboboxes.
  m_parent->SLT_FixedImageSelected(QString("RAW_CBCT"));
  m_parent->SLT_MovingImageSelected(QString("DEFORMED_CT_FINAL"));
  const auto fROI_Radius = 30;//this->ui.lineEditNormRoiRadius->text().toFloat();

  std::cout << "Intensity is being analyzed...Please wait." << std::endl;

  float intensitySDFix = 0.0;
  float intensitySDMov = 0.0;
  const auto meanIntensityFix = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spRawReconImg, fROI_Radius, &intensitySDFix);
  const auto meanIntensityMov = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spDeformedCT_Final, fROI_Radius, &intensitySDMov);

  std::cout << "Mean/SD for Fixed = " << meanIntensityFix << "/"
            << intensitySDFix << std::endl;
  std::cout << "Mean/SD for Moving = " << meanIntensityMov << "/"
            << intensitySDMov << std::endl;

  AddConstHU(m_cbctrecon->m_spRawReconImg, static_cast<int>(meanIntensityMov - meanIntensityFix));
  // SLT_PassMovingImgForAnalysis();

  std::cout << "Intensity shifting is done! Added value = "
            << static_cast<int>(meanIntensityMov - meanIntensityFix)
            << std::endl;
  auto update_message =
      QString("Added_%1")
          .arg(static_cast<int>(meanIntensityMov - meanIntensityFix));

  //this->UpdateReconImage(m_cbctrecon->m_spRawReconImg, update_message);//m_pParent->UpdateReconImage(m_spFixedImg, update_message);
  //SelectComboExternal(0, REGISTER_RAW_CBCT);

  m_parent->SLT_FixedImageSelected(QString("RAW_CBCT"));

  SLT_DoScatterCorrection_APRIORI();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void ScatterCorrectThread::SLT_IntensityNormCBCT_COR_CBCT() {
  //This code is adde by us and is found from the callback from the comboboxes.
  m_parent->SLT_FixedImageSelected(QString("COR_CBCT"));
  m_parent->SLT_MovingImageSelected(QString("DEFORMED_CT_FINAL"));
  const auto fROI_Radius = 30;//this->ui.lineEditNormRoiRadius->text().toFloat();

  std::cout << "Intensity is being analyzed...Please wait." << std::endl;

  float intensitySDFix = 0.0;
  float intensitySDMov = 0.0;
  const auto meanIntensityFix = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spScatCorrReconImg, fROI_Radius, &intensitySDFix);
  const auto meanIntensityMov = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spDeformedCT_Final, fROI_Radius, &intensitySDMov);

  std::cout << "Mean/SD for Fixed = " << meanIntensityFix << "/"
            << intensitySDFix << std::endl;
  std::cout << "Mean/SD for Moving = " << meanIntensityMov << "/"
            << intensitySDMov << std::endl;

  AddConstHU(m_cbctrecon->m_spScatCorrReconImg, static_cast<int>(meanIntensityMov - meanIntensityFix));
  // SLT_PassMovingImgForAnalysis();

  std::cout << "Intensity shifting is done! Added value = "
            << static_cast<int>(meanIntensityMov - meanIntensityFix)
            << std::endl;
  auto update_message =
      QString("Added_%1")
          .arg(static_cast<int>(meanIntensityMov - meanIntensityFix));
  // Maybe this
  //SLT_FixedImageSelected("COR_CBCT");
  //SLT_MovingImageSelected("DEFORMED_CT_FINAL");
  //
  //this->UpdateReconImage(m_cbctrecon->m_spRawReconImg, update_message);//m_pParent->UpdateReconImage(m_spFixedImg, update_message);
  //SelectComboExternal(0, REGISTER_COR_CBCT);

  m_parent->SLT_FixedImageSelected(QString("RAW_CBCT"));
  emit SignalPassFixedImg(QString("RAW_CBCT"));
  //SLT_PassFixedImgForAnalysis(QString("RAW_CBCT"));
  m_parent->SLT_FixedImageSelected(QString("COR_CBCT"));
  //ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: rgba(47,212,75,60%);color: rgba(255,255,255,60%);font-size: 18px;border-width: 1.4px; border-color: rgba(0,0,0,60%);border-style: solid; border-radius: 7px;}");
}
/*
void ScatterCorrectThread::SLT_PassFixedImgForAnalysis(QString cur_fixed) {
  if (m_spFixedImg != nullptr) {
    //auto cur_fixed = this->ui.comboBoxImgFixed->currentText();
    m_pParent->UpdateReconImage(m_spFixedImg, cur_fixed);
  }
}
*/

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code this method is called when Scatter correct button is pushed. Find out where to implement this!
void ScatterCorrectThread::SLT_DoScatterCorrection_APRIORI() {

  if ((this->m_cbctrecon->m_spRefCTImg == nullptr &&
       m_parent->m_spMovingImg == nullptr) ||//m_dlgRegistration->m_spMovingImg == nullptr) ||
      m_cbctrecon->m_spRawReconImg ==nullptr){//m_dlgRegistration->m_spFixedImg == nullptr) {
    std::cerr
        << "Error!: No ref or no fixed image for forward projection is found."
        << "\n";
    return;
  }

  const auto bExportProj_Fwd = false;// this->ui.checkBox_ExportFwd->isChecked();
  const auto bExportProj_Scat = false;//this->ui.checkBox_ExportScat->isChecked();
  const auto bExportProj_Cor = false;//this->ui.checkBox_ExportCor->isChecked();

  // ForwardProjection(m_spRefCTImg, m_spCustomGeometry, m_spProjImgCT3D,
  // false); //final moving image
  /*
  if (bExportProj_Fwd) {
    this->m_cbctrecon->m_strPathPatientDir = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  }
    */
  // Overwrite the raw projections by re-projecting them from the potentially
  // modified CBCT
  std::cerr << "Custom Geometry: \n";
  m_cbctrecon->m_spCustomGeometry->Print(std::cerr);

  auto spProjImg3DFloat =
      this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
          m_cbctrecon->m_spRawReconImg, this->m_cbctrecon->m_spCustomGeometry, //m_dlgRegistration->m_spFixedImg, this->m_cbctrecon->m_spCustomGeometry,
          bExportProj_Fwd, false);//this->ui.radioButton_UseCUDA->isChecked());

  FloatImageType::Pointer p_projimg;
  if (m_parent->m_spMovingImg != nullptr){//m_dlgRegistration->m_spMovingImg != nullptr) {
    p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
        m_parent->m_spMovingImg, this->m_cbctrecon->m_spCustomGeometry,//m_dlgRegistration->m_spMovingImg, this->m_cbctrecon->m_spCustomGeometry,
        bExportProj_Fwd,
        false);//this->ui.radioButton_UseCUDA->isChecked()); // final moving image
  } else if (this->m_parent->m_spMovingImg != nullptr) {
    std::cerr << "No Moving image in Registration is found. Ref CT image will "
                 "be used instead"
              << "\n";
    p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
        this->m_parent->m_spMovingImg, this->m_cbctrecon->m_spCustomGeometry,
        bExportProj_Fwd,
        false);//this->ui.radioButton_UseCUDA->isChecked()); // final moving image
  }

  // YKTEMP
  auto projsize = p_projimg->GetBufferedRegion().GetSize();
  std::cout << "ProjImgCT Size = " << projsize[0] << ", " << projsize[1] << ", "
            << projsize[2] << "\n";
  std::cout << "ProjImgCT origin = " << p_projimg->GetOrigin()[0] << ", "
            << p_projimg->GetOrigin()[1] << ", " << p_projimg->GetOrigin()[2]
            << "\n";
  std::cout << "ProjImgCT spacing = " << p_projimg->GetSpacing()[0] << ", "
            << p_projimg->GetSpacing()[1] << ", " << p_projimg->GetSpacing()[2]
            << std::endl;

  // double scaResam = this->ui.lineEdit_scaResam->text().toDouble();
  const auto scaMedian = 12.0;//this->ui.lineEdit_scaMedian->text().toDouble();
  const auto scaGaussian = 0.05;//this->ui.lineEdit_scaGaussian->text().toDouble();

  std::cout << "Generating scatter map is ongoing..." << std::endl;

  this->m_cbctrecon->GenScatterMap_PriorCT(
      spProjImg3DFloat, p_projimg, this->m_cbctrecon->m_spProjImgScat3D,
      scaMedian, scaGaussian,
      bExportProj_Scat); // void GenScatterMap2D_PriorCT()

  std::cout << "To account for the mAs values, the intensity scale factor of "
            << GetRawIntensityScaleFactor(this->m_cbctrecon->m_strRef_mAs,
                                          this->m_cbctrecon->m_strCur_mAs)
            << "was multiplied during scatter correction to avoid negative "
               "scatter"
            << std::endl;
  // In Andreases code he sets text on the ui but we don't. Therefoe this is outcommented
  /*
  this->ui.lineEdit_CurmAs->setText(this->m_cbctrecon->m_strCur_mAs);
  this->ui.lineEdit_RefmAs->setText(this->m_cbctrecon->m_strRef_mAs);
  */

  p_projimg->Initialize(); // memory saving

  std::cout << "Scatter correction is in progress..." << std::endl;

  const auto postScatMedianSize = 3;//this->ui.lineEdit_scaPostMedian->text().toInt();

  this->m_cbctrecon->ScatterCorr_PrioriCT(spProjImg3DFloat,
                                          this->m_cbctrecon->m_spProjImgScat3D,
                                          this->m_cbctrecon->m_spProjImgCorr3D,
                                          postScatMedianSize, bExportProj_Cor);

  this->m_cbctrecon->m_spProjImgScat3D->Initialize(); // memory saving

  std::cout << "AfterCorrectionMacro is ongoing..." << std::endl;

  // In Andreases code he sets elements on his ui. therefore this is outcommented:
  // Update UI
  /*
  this->ui.pushButton_DoRecon->setEnabled(true);
  this->ui.spinBoxImgIdx->setMinimum(0);
  */
  const auto iSizeZ = this->m_cbctrecon->m_spProjImg3DFloat->GetBufferedRegion().GetSize()[2];
  /*
  this->ui.spinBoxImgIdx->setMaximum(iSizeZ - 1);
  this->ui.spinBoxImgIdx->setValue(0);
  */
  this->m_cbctrecon->SetMaxAndMinValueOfProjectionImage(); // update min max projection image
  SLT_InitializeGraphLim();
  //SLT_DrawProjImages(); // Update Table is called

  auto fdk_options = getFDKoptions();

  this->m_cbctrecon->AfterScatCorrectionMacro(false,true,false,fdk_options);
              /*
this->ui.radioButton_UseCUDA->isChecked(),
      this->ui.radioButton_UseOpenCL->isChecked(),
      this->ui.checkBox_ExportVolDICOM->isChecked(), fdk_options);
  */

  // Skin removal (using CT contour w/ big margin)
  std::cout
      << "Post  FDK reconstruction is done. Moving on to post skin removal"
      << std::endl;

  const auto voi_name = QString("");//this->m_dlgRegistration->ui.comboBox_VOItoCropBy->currentText();
  // m_cbctregistration->PostSkinRemovingCBCT(
  //    this->m_cbctrecon->m_spScatCorrReconImg, voi_name.toStdString());

  // 20151208 Removal of high intensity skin mask
  // Main issue: raw CBCT projection includes mask, deformed CT doesn't include
  // mask. In case of weight loss, mask signal is independent from skin contour,
  // but deformed CT cannot have that signal.  Therefore, after the subtraction
  // (CBCTcor projections), there is always a big peak. DIR quality doesn't
  // matter because it cannot 'create' mask signal anyway.  Assumption: near the
  // skin contour, this kind of discrepancy is not expected.
  // m_pDlgRegistration->ThermoMaskRemovingCBCT(m_spRawReconImg,
  // m_spScatCorrReconImg, threshold_HU);

  //m_dlgRegistration->UpdateListOfComboBox(0); // combo selection signalis
                                              // called
  //m_dlgRegistration->UpdateListOfComboBox(1);

  // Methods down below has been changed due to comboboxes
  // SLT_FixedImageSelected("RAW_CBCT");//m_dlgRegistration->SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  //m_dlgRegistration->SelectComboExternal(1, REGISTER_COR_CBCT);

  // m_dlgRegistration->SLT_DoLowerMaskIntensity(); // it will check the check
  // button.

  //SLT_DrawProjImages(); out commented by us

  std::cout << "Updating ReconImage..";
  auto updated_text = QString("Scatter corrected CBCT");

  // This has been commented out subce we don't want to update recon image
  //UpdateReconImage(this->m_cbctrecon->m_spScatCorrReconImg, updated_text); // main GUI update

  std::cout << "FINISHED!Scatter correction: CBCT DICOM files are saved"
            << std::endl;
  SLT_IntensityNormCBCT_COR_CBCT();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code he goes back to the registration UI. selects COR_CBCT and DEFORMED and then he clicks pass.
// Automatise that


// Is implemented in SLT_LoadSelectedProjFiles()
void ScatterCorrectThread::SLT_InitializeGraphLim() const {
  // In Andreases code this was checked so we keep this one (radioButton_graph_proj)
  // Set Max Min at graph
  if (true){//this->ui.radioButton_graph_proj->isChecked()) {
    if (this->m_cbctrecon->m_iImgCnt > 0) // if indep raw his images are loaded
    {
      const auto horLen = this->m_cbctrecon->m_dspYKImgProj->m_iWidth;
      // int verLen = m_dspYKImgProj->m_iHeight;

      // set edit maxium min
      const auto strXMin = QString("%1").arg(horLen);
      //this->ui.lineEditXMin->setText("0");
      //this->ui.lineEditXMax->setText(strXMin);

      const auto strYMin =
          QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMin, 0, 'f', 1);
      const auto strYMax =
          QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMax, 0, 'f', 1);

      //this->ui.lineEditYMin->setText(strYMin);
      //this->ui.lineEditYMax->setText(strYMax);
    }

    if (this->m_cbctrecon->m_spProjImg3DFloat == nullptr) {
      return;
    }

    const auto horLen =
        this->m_cbctrecon->m_spProjImg3DFloat->GetBufferedRegion().GetSize()[0];
    // int verLen = m_spProjImg3DFloat->GetBufferedRegion().GetSize()[1];

    // set edit maxium min
    const auto strXMin = QString("%1").arg(horLen);
    //this->ui.lineEditXMin->setText("0");
    //this->ui.lineEditXMax->setText(strXMin);

    const auto strYMin =
        QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMin, 0, 'f', 1);
    const auto strYMax =
        QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMax, 0, 'f', 1);

    //this->ui.lineEditYMin->setText(strYMin);
    //this->ui.lineEditYMax->setText(strYMax);

    // In Andreases code this was not checked so we outcommet this one (radioButton_graph_recon)
  } /*else if (this->ui.radioButton_graph_recon->isChecked()) {
    if (this->m_cbctrecon->m_spCrntReconImg == nullptr) {
      return;
    }

    const auto horLen =
        this->m_cbctrecon->m_spCrntReconImg->GetBufferedRegion().GetSize()[0];
    // int verLen = m_spCrntReconImg->GetBufferedRegion().GetSize()[1];

    // set edit maxium min

    const auto strXMax = QString("%1").arg(horLen);
    this->ui.lineEditXMin->setText("0");
    this->ui.lineEditXMax->setText(strXMax);

    const auto strYMin = QString("%1").arg(0.0, 0, 'f', 1);
    const auto strYMax = QString("%1").arg(2000.0, 0, 'f', 1);

    this->ui.lineEditYMin->setText(strYMin);
    this->ui.lineEditYMax->setText(strYMax);
  }
  */
}
FDK_options ScatterCorrectThread::getFDKoptions() const {
  FDK_options fdk_options;
  // In Andreases code this was initialized as 1.0
  fdk_options.TruncCorFactor = 1.0;//this->ui.lineEdit_Ramp_TruncationCorrection->text().toDouble();
  // In Andreases code this was initialized as 0.5
  fdk_options.HannCutX = 0.5;//this->ui.lineEdit_Ramp_HannCut->text().toDouble();
  // In Andreases code this was initialized as 0.5
  fdk_options.HannCutY = 0.5;//this->ui.lineEdit_Ramp_HannCutY->text().toDouble();
  // In Andreases code this was initialized as 0.0
  fdk_options.CosCut = 0.0;//this->ui.lineEdit_Ramp_CosineCut->text().toDouble();
  // In Andreases code this was initialized as 0.0
  fdk_options.HammCut = 0.0;//this->ui.lineEdit_Ramp_Hamming->text().toDouble();
  // In Andreases code this was aldready checked so we replace with true (checkBox_UseDDF)
  fdk_options.displacedDetectorFilter = true;//this->ui.checkBox_UseDDF->isChecked();
  // In Andreases code this was not checked so we replace with false (checkBox_UpdateAfterFiltering)
  fdk_options.updateAfterDDF = false;//this->ui.checkBox_UpdateAfterFiltering->isChecked();
  // In Andreases code this was aldready checked so we replace with true (checkBox_UsePSSF)
  fdk_options.ParkerShortScan = true;//this->ui.checkBox_UsePSSF->isChecked();

  // In Andreases code thesse three was initialized as 1
  fdk_options.ct_spacing[0] = 1;//this->ui.lineEdit_outImgSp_AP->text().toDouble();
  fdk_options.ct_spacing[1] = 1;//this->ui.lineEdit_outImgSp_SI->text().toDouble();
  fdk_options.ct_spacing[2] = 1;//this->ui.lineEdit_outImgSp_LR->text().toDouble();

  // In Andreases code thesse three was initialized as 400
  fdk_options.ct_size[0] = 400;//this->ui.lineEdit_outImgDim_AP->text().toInt();
  // In Andreases code thesse three was initialized as 200
  fdk_options.ct_size[1] = 200;//this->ui.lineEdit_outImgDim_SI->text().toInt();
  // In Andreases code thesse three was initialized as 400
  fdk_options.ct_size[2] = 200;//this->ui.lineEdit_outImgDim_LR->text().toInt();

  // In Andreases these three is set earlier in the code but the UI standard is implemented. Hope this works
  fdk_options.medianRadius[0] = 0;//this->ui.lineEdit_PostMedSizeX->text().toInt(); // radius along x
  fdk_options.medianRadius[1] = 0;//this->ui.lineEdit_PostMedSizeY->text().toInt(); // radius along y
  fdk_options.medianRadius[2] = 1;//this->ui.lineEdit_PostMedSizeZ->text().toInt(); // radius along z

  // In Andreases code this was aldready checked so we replace with true (checkBox_PostMedianOn)
  fdk_options.medianFilter = true;//this->ui.checkBox_PostMedianOn->isChecked();

  fdk_options.outputFilePath = QString("");// this->ui.lineEdit_OutputFilePath->text(); // In Andreases UI it says that this is optional

  return fdk_options;
}



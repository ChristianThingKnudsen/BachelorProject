#include "loadingthread.h"

#include <QThread>
#include <QtCore>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "mha_io.h"
#include "cbctrecon_io.h"
#include "mainwindow.h"




LoadingThread::LoadingThread(MainWindow *parent) : QThread(dynamic_cast<QObject*>(parent))
{
    this->m_cbctrecon = parent->m_cbctrecon.get();
    this->m_cbctregistration = parent->m_cbctregistration.get();
}

void LoadingThread::run(){
    this->SLT_SetHisDir();
}

void LoadingThread::SLT_SetHisDir() // Initialize all image buffer
{
  // Initializing..

   // When we are testing we don't want to use file dialogs and this has therefore been commented out.
  auto dirPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Acquisitions\\781076550");
  /*
  // Set folder --> then use RTK HIS Reader
  auto dirPath = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), "C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Acquisitions\\781076550",//this->m_cbctrecon->m_strPathDirDefault,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
      */

  if (dirPath.length() <= 1) {
    return;
  }

  this->m_cbctrecon->SetProjDir(dirPath);
  init_DlgRegistration(this->m_cbctrecon->m_strDCMUID);

  float kVp = 0.0;
  float mA = 0.0;
  float ms = 0.0;
  GetXrayParamFromINI(this->m_cbctrecon->m_strPathElektaINI, kVp, mA, ms);

  VEC3D couch_trans = {-999, -999,
                       -999}; // mm. In the text file, these values are in cm.
  VEC3D couch_rot = {-999, -999,
                     -999}; // mm. In the text file, these values are in cm.

  const auto res = GetCouchShiftFromINIXVI(
      this->m_cbctrecon->m_strPathElektaINIXVI2, &couch_trans, &couch_rot);

  if (res) {
    const auto strTransX = QString::number(couch_trans.x, 'f', 1);
    const auto strTransY = QString::number(couch_trans.y, 'f', 1);
    const auto strTransZ = QString::number(couch_trans.z, 'f', 1);
    const auto strTransAll = strTransX + "," + strTransY + "," + strTransZ;

    const auto strRotX = QString::number(couch_rot.x, 'f', 1);
    const auto strRotY = QString::number(couch_rot.y, 'f', 1);
    const auto strRotZ = QString::number(couch_rot.z, 'f', 1);

    const auto strRotAll = strRotX + "," + strRotY + "," + strRotZ;
  }

  this->m_cbctrecon->m_vSelectedFileNames.clear();

  std::cout << "Push Load button to load projection images" << std::endl;
  SLT_LoadSelectedProjFiles(dirPath);
}

void LoadingThread::init_DlgRegistration(QString &str_dcm_uid) const// init dlgRegistrations
{
    QString strDCMUID = str_dcm_uid;
    m_cbctregistration->SetPlmOutputDir(strDCMUID);

    const UShortImageType::Pointer spNull;
    // unlink all of the pointers
    auto &p_parent = m_cbctrecon;
    p_parent->m_spRefCTImg = spNull;
    p_parent->m_spManualRigidCT =
        spNull; // copied from RefCTImg; ID: RefCT --> Moving Img, cloned
    p_parent->m_spAutoRigidCT = spNull; // ID: AutoRigidCT
    p_parent->m_spDeformedCT1 = spNull; // Deformmation will be carried out based
                                        // on Moving IMage of GUI //AutoDeformCT1
    p_parent->m_spDeformedCT2 = spNull; // AutoDeformCT2
    p_parent->m_spDeformedCT3 = spNull; // AutoDeformCT3
    p_parent->m_spDeformedCT_Final = spNull; // AutoDeformCT3

}
/*
QString getBowtiePath(QWidget *parent, const QDir &calDir) {
  return QFileDialog::getOpenFileName(
      parent, "Find air(+bowtie) filter image for subtraction",
      calDir.absolutePath(), "Projection (*.xim)", nullptr, nullptr);
}
*/

//Is needed for the next method SLT_LoadSelectedProjFiles()
std::tuple<bool, bool> LoadingThread::probeUser(const QString &guessDir) {
  // When we are testing we don't want to use file dialogs and this has therefore been commented out.
  auto dirPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\Plan CT\\E_PT1 plan");
  /*
  auto dirPath = QFileDialog::getExistingDirectory(
      this, tr("Open CT DICOM Directory"), guessDir,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
      */

  auto dcm_success = false;
  if (!(dirPath.length() <= 1)) {

    if (this->m_cbctrecon->ReadDicomDir(dirPath)) {

      this->m_cbctrecon->RegisterImgDuplication(REGISTER_REF_CT,
                                                REGISTER_MANUAL_RIGID);
      dcm_success = true;
    }
  }

  auto instaRecon = true;

  return std::make_tuple(instaRecon, dcm_success);
}

FilterReaderType::Pointer
LoadingThread::ReadBowtieFileWhileProbing(const QString &proj_path,
                                            std::tuple<bool, bool> &answers) {

  auto bowtiereader =
      FilterReaderType::New(); // we use is because we need the projections to
                               // be in the same unit (order of magnitude)

  QDir guessDir(proj_path + QString("/../"));

  const auto calDir(proj_path + QString("/Calibrations/"));

  QString bowtiePath;

  switch (this->m_cbctrecon->m_projFormat) {
  case XIM_FORMAT:
    bowtiePath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Calibrations\\AIR-Full-Bowtie-100KV-Scattergrid-SAD-SID_0\\Current\\FilterBowtie.xim");//getBowtiePath(this, calDir);
    if (bowtiePath.length() > 1) {

      std::cout << "loading bowtie-filter..." << std::endl;
      std::vector<std::string> filepath;
      filepath.push_back(bowtiePath.toStdString());
      bowtiereader->SetFileNames(filepath);
      emit Signal_UpdateProgressBarLoad(20);
      std::thread calc_thread_bowtie(
          [&bowtiereader] { bowtiereader->Update(); });
      answers = probeUser(guessDir.absolutePath());
      calc_thread_bowtie.join();

    } else {
      answers = probeUser(
          guessDir.absolutePath()); // looks ugly, but allows threading
    }
    break;
  default:
    answers = probeUser(guessDir.absolutePath()); // ^^^
    break;
  }
  if (bowtiePath.length() > 1) {
    return bowtiereader;
  }
  return nullptr;
}

void LoadingThread::SLT_LoadSelectedProjFiles(QString &path) // main loading fuction for projection images
{
  auto dirPath = path;

  if (!QFile::exists(dirPath)) {
    std::cout << "Projection file directory was not found. Retry." << std::endl;
    return;
  }

  auto names = this->m_cbctrecon->GetProjFileNames(dirPath);

  if (this->m_cbctrecon->m_projFormat == HIS_FORMAT &&
      !this->m_cbctrecon->IsFileNameOrderCorrect(names)) {
    std::cout << "Check the file name order" << std::endl;
    //QMessageBox::warning(this, "warning", "Error on File Name Sorting!");
    //return;
    emit SignalMessageBox(1,"warning","Error on File Name Sorting!");
  }

  std::cout << "File name order was cross-checked and found to be OK!"
            << std::endl;

  const auto fullCnt = names.size();
  if (fullCnt <= 0) {
    std::cout << "No projection file was found. Retry." << std::endl;
    return;
  }

  std::cout << fullCnt << "  projection files were found." << std::endl;

  // 2) Elekta Geometry file
  const auto geomPath = this->m_cbctrecon->m_strPathGeomXML;//this->ui.lineEdit_ElektaGeomPath->text();
  QFileInfo geomFileInfo(geomPath);
  //! QFile::exists(geomPath)

  if (!this->m_cbctrecon->LoadGeometry(geomFileInfo, names)) {
    if (!this->m_cbctrecon->m_strError.isEmpty()) {
      //QMessageBox::critical(this, "LoadXVIGeometryFile",
                            //this->m_cbctrecon->m_strError, QMessageBox::Ok);
    }
  }
  const auto &p_geometry = this->m_cbctrecon->m_spFullGeometry;
  const auto iFullGeoDataSize = p_geometry->GetGantryAngles().size();
  if (iFullGeoDataSize < 1) {
    std::cout << "Not enough projection image (should be > 0)" << std::endl;
    return;
  }

  if (iFullGeoDataSize != fullCnt) {
    if (this->m_cbctrecon->m_projFormat != XIM_FORMAT) {
      std::cout << "Size of geometry data and file numbers are not same! Check "
                   "and retry"
                << std::endl;
      return;
    }

    //const auto reply = QMessageBox::question(
      //  this, "Mismatch in number of files and Geometry information!",
        //"Mismatch in number of files and Geometry information!\nHowever, Xim "
       // "detected, so it may be safe to continue anyway?",
      //  QMessageBox::Yes | QMessageBox::No);
    //if (reply == QMessageBox::Yes) {
    //  std::cout << "continuing despite warning..." << std::endl;
    //} else {
    //  return;
    //}
  }

  auto angle_gaps = p_geometry->GetAngularGaps(p_geometry->GetSourceAngles());

  auto sum_gap =
      std::accumulate(std::begin(angle_gaps), std::end(angle_gaps), 0.0);
  sum_gap /= itk::Math::pi * 180.0;
  const auto mean_gap = sum_gap / angle_gaps.size();

  std::cout << "AngularGaps Sum (deg):" << sum_gap
            << ", Mean (deg): " << mean_gap << std::endl;
  double gantryAngelIntervalValue = 30;
  const auto gantryAngleInterval = gantryAngelIntervalValue; //this->ui.lineEdit_ManualProjAngleGap->text().toDouble();

  // In Andreases code this was not checked so we outcommet this one (Radio_KeepOriginalAngles)
  // if (this->ui.Radio_KeepOriginalAngles->isChecked())
  /*
  if (this->ui.Radio_ManualProjAngleGap->isChecked()) {
    // bManualGap = true;
    // std::cout << "Input angle gap in deg: " ;
    // cin >> gantryAngleInterval;

    if (gantryAngleInterval < mean_gap) {
      std::cout << "Angle gap size is too small. Terminating the app"
                << std::endl;
      return;
      // bManualGap = false;
    }
  }
  */

  const auto exclude_ids = this->m_cbctrecon->GetExcludeProjFiles(false, gantryAngleInterval);
      //this->ui.Radio_ManualProjAngleGap->isChecked(), gantryAngleInterval);

  this->m_cbctrecon->LoadSelectedProj(exclude_ids, names);

  // Reads the cone beam projections
  using ReaderType = rtk::ProjectionsReader<FloatImageType>;
  auto reader = ReaderType::New();
  reader->SetFileNames(this->m_cbctrecon->m_vSelectedFileNames);
  std::thread calc_thread([&reader]() { reader->Update(); });

  std::cout << "Reader detached from main thread" << std::endl;

  // After reading the whole file, HIS header should be saved
  this->m_cbctrecon->saveHisHeader();

  //  Insta Recon, Dcm read
  const auto geopath = geomFileInfo.absolutePath();
  std::tuple<bool, bool> answers;
  auto bowtie_reader = ReadBowtieFileWhileProbing(geopath, answers);

  calc_thread.join();
  emit Signal_UpdateProgressBarLoad(40);
  std::cout << "Reader re-attached to main thread" << std::endl;

  this->m_cbctrecon->m_spProjImg3DFloat =
      reader->GetOutput(); // 1024 1024, line integ image
  auto &proj = this->m_cbctrecon->m_spProjImg3DFloat;

  const auto bowtie_proj = bowtie_reader->GetOutput();
  if (bowtie_reader != nullptr) {
    ApplyBowtie(proj, bowtie_proj);
  }

  if (this->m_cbctrecon->m_projFormat == HND_FORMAT) {
    std::cout << "Fitted bowtie-filter correction ongoing..." << std::endl;
    SLT_DoBowtieCorrection();
  }

  saveImageAsMHA<FloatImageType>(this->m_cbctrecon->m_spProjImg3DFloat);
  auto res_factor = 0.5;//this->ui.lineEdit_DownResolFactor->text().toDouble();
  if (!this->m_cbctrecon->ResampleProjections(res_factor)) { // 0.5
    // reset factor if image was not resampled
    auto res_factor =1.0;//this->ui.lineEdit_DownResolFactor->setText("1.0");
  }

  this->m_cbctrecon->m_spProjImgRaw3D =
      this->m_cbctrecon->ConvertLineInt2Intensity(
          this->m_cbctrecon->m_spProjImg3DFloat);

  this->m_cbctrecon
      ->SetMaxAndMinValueOfProjectionImage(); // update min max projection image

  SLT_InitializeGraphLim();

  //this->SLT_DrawProjImages(); // Update Table is called Maybe comment this in later on
   emit Signal_UpdateProgressBarLoad(60);
  if (!std::get<0>(answers)) { // instaRecon
    std::cout
        << "FINISHED!: Loading projection files. Proceed to reconstruction"
        << std::endl;
  } else {
    SLT_DoReconstruction();
  }

  if (std::get<0>(answers) && std::get<1>(answers)) { // CT DCM dir was found
    //SLT_ViewRegistration();
  }
}
// Is implemented in SLT_LoadSelectedProjFiles()
void LoadingThread::SLT_DoBowtieCorrection() {
  if (this->m_cbctrecon->m_spProjImg3DFloat == nullptr) {
    return;
  }

  if (this->m_cbctrecon->m_projFormat != HND_FORMAT) {
    std::cout
        << "Bow tie filtering should not be used for His data or Xim data!!"
        << std::endl;
    return;
  }
  // In Andreases UI he uses this string. Hope this works
  QString comboBox_fBTcor_String = "1.4571;2.4506;2.6325;0.0095;4.1181";
  const auto strList = comboBox_fBTcor_String.split(';'); //this->ui.comboBox_fBTcor->currentText().split(';');

  this->m_cbctrecon->BowtieByFit(false,strList);//this->ui.checkBox_Fullfan->isChecked(),strList);

  this->m_cbctrecon->SetMaxAndMinValueOfProjectionImage();
  //SLT_DrawProjImages();
  std::cout << "Bow-tie correction done." << std::endl;
}

// Is implemented in SLT_LoadSelectedProjFiles()
void LoadingThread::SLT_InitializeGraphLim() const {
  // In Andreases code this was checked so we keep this one (radioButton_graph_proj)
  // Set Max Min at graph
  if (true){//this->ui.radioButton_graph_proj->isChecked()) {
    if (this->m_cbctrecon->m_iImgCnt > 0) // if indep raw his images are loaded
    {
      const auto horLen = this->m_cbctrecon->m_dspYKImgProj->m_iWidth;
      const auto strXMin = QString("%1").arg(horLen);
      const auto strYMin =
          QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMin, 0, 'f', 1);
      const auto strYMax =
          QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMax, 0, 'f', 1);
    }

    if (this->m_cbctrecon->m_spProjImg3DFloat == nullptr) {
      return;
    }

    const auto horLen =
        this->m_cbctrecon->m_spProjImg3DFloat->GetBufferedRegion().GetSize()[0];

    // set edit maxium min
    const auto strXMin = QString("%1").arg(horLen);

    const auto strYMin =
        QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMin, 0, 'f', 1);
    const auto strYMax =
        QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMax, 0, 'f', 1);


    // In Andreases code this was not checked so we outcommet this one (radioButton_graph_recon)
  } else if (false){//this->ui.radioButton_graph_recon->isChecked()) {
    if (this->m_cbctrecon->m_spCrntReconImg == nullptr) {
      return;
    }

    const auto horLen =
        this->m_cbctrecon->m_spCrntReconImg->GetBufferedRegion().GetSize()[0];
    // int verLen = m_spCrntReconImg->GetBufferedRegion().GetSize()[1];

    // set edit maxium min

    const auto strXMax = QString("%1").arg(horLen);
    //this->ui.lineEditXMin->setText("0");
    //this->ui.lineEditXMax->setText(strXMax);

    const auto strYMin = QString("%1").arg(0.0, 0, 'f', 1);
    const auto strYMax = QString("%1").arg(2000.0, 0, 'f', 1);

    //this->ui.lineEditYMin->setText(strYMin);
    //this->ui.lineEditYMax->setText(strYMax);
  }
}
// Is called by SLT_DoReconstruction()
FDK_options LoadingThread::getFDKoptions() const {
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
// Is implemented in SLT_LoadSelectedProjFiles()
void LoadingThread::SLT_DoReconstruction() {
  const auto fdk_options = getFDKoptions();
  itk::TimeProbe reconTimeProbe;
  reconTimeProbe.Start();
  // In Andreases UI he uses Cuda, but we use OpenCL.
  this->m_cbctrecon->DoReconstructionFDK<OPENCL_DEVT>(REGISTER_RAW_CBCT, fdk_options);
  emit Signal_UpdateProgressBarLoad(80);
  /*
  if (this->ui.radioButton_UseCUDA->isChecked()) {
    this->m_cbctrecon->DoReconstructionFDK<CUDA_DEVT>(REGISTER_RAW_CBCT,
                                                      fdk_options);
  } else if (this->ui.radioButton_UseOpenCL->isChecked()) {
    this->m_cbctrecon->DoReconstructionFDK<OPENCL_DEVT>(REGISTER_RAW_CBCT,
                                                        fdk_options);
  } else {
    this->m_cbctrecon->DoReconstructionFDK<CPU_DEVT>(REGISTER_RAW_CBCT,                                                     fdk_options);
  }
  */

  reconTimeProbe.Stop();
  std::cout << "It took " << reconTimeProbe.GetMean() << ' '
            << reconTimeProbe.GetUnit() << std::endl;

  QString update_text("RAW_CBCT");
  UpdateReconImage(this->m_cbctrecon->m_spCrntReconImg, update_text);

  m_dlgRegistration->UpdateListOfComboBox(0); // combo selection
                                              // signalis called
  m_dlgRegistration->UpdateListOfComboBox(1);

  emit Signal_UpdateProgressBarLoad(100);
  emit Signal_SetButtonsAfterLoad();

}

// Is called by SLT_DoReconstruction()
void LoadingThread::UpdateReconImage(UShortImageType::Pointer &spNewImg,
                                       QString &fileName) {
  this->m_cbctrecon->m_spCrntReconImg = spNewImg;

  const auto &p_curimg = this->m_cbctrecon->m_spCrntReconImg;
  const auto origin_new = p_curimg->GetOrigin();
  const auto spacing_new = p_curimg->GetSpacing();
  const auto size_new = p_curimg->GetBufferedRegion().GetSize();

  std::cout << "New Origin" << origin_new << std::endl;
  std::cout << "New spacing" << spacing_new << std::endl;
  std::cout << "New size" << size_new << std::endl;

  auto size = p_curimg->GetBufferedRegion().GetSize();

  this->m_cbctrecon->m_dspYKReconImage->CreateImage(size[0], size[1], 0); // maybe 100 instead of 0.

  emit Signal_DisconnectSlider();

  emit Signal_UpdateSlider(size[2] - 1);

  const auto initVal = qRound((size[2] - 1) / 2.0);

  //SLT_InitializeGraphLim();

  emit Signal_ReConnectSlider(initVal);
}

// Is implemented in SLT_DrawProjImages() in MainWindow
void LoadingThread::SLT_UpdateTable() {

  // In Andreases code this was aldready checked so we outcommet this one (radioButton_graph_proj)
    /*
  if (this->m_cbctrecon->m_spCrntReconImg == nullptr) {
    this->ui.radioButton_graph_proj->setChecked(true);
  }
  */

  // std::cout << "check 1" << std::endl;
  YK16GrayImage *pYKImg;
  auto fMultiPlyFactor = 1.0;
  auto fMinValue = 0.0;

  // In Andreases code this was aldready checked so we replace with true (radioButton_graph_proj)
  if (true){//this->ui.radioButton_graph_proj->isChecked()) {
    pYKImg = this->m_cbctrecon->m_dspYKImgProj.get(); // you may look, but no touching!

    if (this->m_cbctrecon->m_iImgCnt > 0) { // if indep image
      fMultiPlyFactor = 1.0;
    } else {
      fMultiPlyFactor = this->m_cbctrecon->m_multiplyFactor;
      fMinValue = this->m_cbctrecon->m_fProjImgValueMin;
    }
  } else {
    pYKImg = this->m_cbctrecon->m_dspYKReconImage.get();
    fMinValue = 0.0;
  }
  if (pYKImg == nullptr) {
    return;
  }

  auto columnSize = 2;
  auto rowSize = pYKImg->m_iHeight;

  /// int rowSize = pYKImg->m_iWidth;
  // In Andreases code this was aldready checked so we replace with true (radioButton_Profile_Hor)
  if (true){//this->ui.radioButton_Profile_Hor->isChecked()) {
    // columnSize = 2;
    rowSize = pYKImg->m_iWidth;
  }

  // std::cout << "check 4" << std::endl;
  m_pTableModel = std::make_unique<QStandardItemModel>(
      rowSize, columnSize, this); // 2 Rows and 3 Columns

  auto pos_item = std::make_unique<QStandardItem>(QString("Position(mm)"));
  auto val_item = std::make_unique<QStandardItem>(QString("Value"));

  m_pTableModel->setHorizontalHeaderItem(0, pos_item.release());
  m_pTableModel->setHorizontalHeaderItem(1, val_item.release());

  auto originX = 0.0;
  auto originY = 0.0;
  auto spacingX = 1.0;
  auto spacingY = 1.0;
  // In Andreases code this is used for the graph so therefore we out comment this section
  /*
  if (!this->ui.radioButton_graph_proj->isChecked()) {
    if (this->m_cbctrecon->m_spCrntReconImg != nullptr) {
      auto tmpOrigin = this->m_cbctrecon->m_spCrntReconImg->GetOrigin();
      auto tmpSpacing = this->m_cbctrecon->m_spCrntReconImg->GetSpacing();
      originX = tmpOrigin[0];
      originY = tmpOrigin[1];
      spacingX = tmpSpacing[0];
      spacingY = tmpSpacing[1];
    }
  }
  */

  // std::cout << "check 6" << std::endl;
  // In Andreases code this was aldready checked so we replace with true (radioButton_Profile_Hor)
  QVector<qreal> vPos;
  if (true){//this->ui.radioButton_Profile_Hor->isChecked()) {
    for (auto i = 0; i < rowSize; i++) {
      vPos.push_back(originX + i * spacingX);
    }
  } else {
    for (auto i = 0; i < rowSize; i++) {
      vPos.push_back(originY + i * spacingY);
    }
  }
  // In Andreases code this was aldready checked so we replace with true (radioButton_Profile_Hor)
  QVector<qreal> vProfile;
  if (true){//this->ui.radioButton_Profile_Hor->isChecked()) {
    pYKImg->GetProfileData(vProfile, DIRECTION_HOR);
  } else {
    pYKImg->GetProfileData(vProfile, DIRECTION_VER);
  }

  // int i = fixedY;
  for (auto i = 0; i < rowSize; i++) {
    const auto tmpVal1 = vPos[i];
    auto xpos_item =
        std::make_unique<QStandardItem>(QString("%1").arg(tmpVal1));
    m_pTableModel->setItem(i, 0, xpos_item.release());

    const auto tmpVal2 = vProfile[i] / fMultiPlyFactor + fMinValue;
    auto profval_item =
        std::make_unique<QStandardItem>(QString("%1").arg(tmpVal2));
    m_pTableModel->setItem(i, 1, profval_item.release());
  }
  // In Andreases code this is for visualising dose and this is therefore out commented
  /*
  this->ui.tableViewReconImgProfile->setModel(
      m_pTableModel.get()); // also for proj
      */

  // In Andreases code this is for visualising dose and this is therefore out commented
  //SLT_DrawGraph();
}


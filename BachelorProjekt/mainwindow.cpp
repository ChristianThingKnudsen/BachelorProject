#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qyklabel.h"
#include "progressbar.h"
#include "informationwindow.h"

// QT
#include <qfiledialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Head-Neck");
    ui->comboBox->addItem("Prostata");
    ui->comboBox->addItem("Thorax");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLoadCT_clicked()
{
    Progressbar progressbar;
    progressbar.setModal(true);
    progressbar.exec();
}

void MainWindow::on_btnInfo_clicked()
{
    InformationWindow infoWindow;
    infoWindow.setModal(true);
    infoWindow.exec();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void MainWindow::foo()
{
    this->ui->btnLoadCT->setText("Hello");
}

void MainWindow::SLT_SetHisDir() // Initialize all image buffer
{
    /*
  // Initializing..

  // Set folder --> then use RTK HIS Reader
  auto dirPath = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), this->m_cbctrecon->m_strPathDirDefault,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (dirPath.length() <= 1) {
    return;
  }

  this->ui.lineEdit_HisDirPath->setText(dirPath);

  this->m_cbctrecon->SetProjDir(dirPath);
  init_DlgRegistration(this->m_cbctrecon->m_strDCMUID);

  this->ui.lineEdit_ElektaGeomPath->setText(
      this->m_cbctrecon->m_strPathGeomXML);

  float kVp = 0.0;
  float mA = 0.0;
  float ms = 0.0;
  GetXrayParamFromINI(this->m_cbctrecon->m_strPathElektaINI, kVp, mA, ms);

  if (fabs(kVp * mA * ms) > 0.001) {
    // update GUI
    std::cout << "Updating current mAs setting from INI file: "
              << "kVp= " << kVp << ", mA= " << mA << ", ms= " << ms
              << std::endl;
  }
  this->ui.lineEdit_CurmAs->setText(QString("%1, %2").arg(mA).arg(ms));

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

    this->ui.lineEdit_CouchTrans->setText(strTransAll);
    this->ui.lineEdit_CouchRot->setText(strRotAll);
  } else {
    this->ui.lineEdit_CouchTrans->setText("Not available");
    this->ui.lineEdit_CouchRot->setText("Not available");
  }

  this->m_cbctrecon->m_vSelectedFileNames.clear();

  std::cout << "Push Load button to load projection images" << std::endl;
  */
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
void MainWindow::SLT_LoadSelectedProjFiles() // main loading fuction for projection images
{
  this->ui->btnLoadCT->setDisabled(true);
  // 1) Get all projection file names
  auto dirPath = this->ui.lineEdit_HisDirPath->text();
  //.toLocal8Bit().constData();

  if (!QFile::exists(dirPath)) {
    std::cout << "Projection file directory was not found. Retry." << std::endl;
    return;
  }

  auto names = this->m_cbctrecon->GetProjFileNames(dirPath);

  if (this->m_cbctrecon->m_projFormat == HIS_FORMAT &&
      !this->m_cbctrecon->IsFileNameOrderCorrect(names)) {
    std::cout << "Check the file name order" << std::endl;
    QMessageBox::warning(this, "warning", "Error on File Name Sorting!");
    return;
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
  const auto geomPath = this->ui.lineEdit_ElektaGeomPath->text();
  QFileInfo geomFileInfo(geomPath);
  //! QFile::exists(geomPath)

  if (!this->m_cbctrecon->LoadGeometry(geomFileInfo, names)) {
    if (!this->m_cbctrecon->m_strError.isEmpty()) {
      QMessageBox::critical(this, "LoadXVIGeometryFile",
                            this->m_cbctrecon->m_strError, QMessageBox::Ok);
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

    const auto reply = QMessageBox::question(
        this, "Mismatch in number of files and Geometry information!",
        "Mismatch in number of files and Geometry information!\nHowever, Xim "
        "detected, so it may be safe to continue anyway?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      std::cout << "continuing despite warning..." << std::endl;
    } else {
      return;
    }
  }

  auto angle_gaps = p_geometry->GetAngularGaps(p_geometry->GetSourceAngles());

  auto sum_gap =
      std::accumulate(std::begin(angle_gaps), std::end(angle_gaps), 0.0);
  sum_gap /= itk::Math::pi * 180.0;
  const auto mean_gap = sum_gap / angle_gaps.size();

  std::cout << "AngularGaps Sum (deg):" << sum_gap
            << ", Mean (deg): " << mean_gap << std::endl;

  const auto gantryAngleInterval =
      this->ui.lineEdit_ManualProjAngleGap->text().toDouble();

  // if (this->ui.Radio_KeepOriginalAngles->isChecked())
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

  const auto exclude_ids = this->m_cbctrecon->GetExcludeProjFiles(
      this->ui.Radio_ManualProjAngleGap->isChecked(), gantryAngleInterval);

  this->m_cbctrecon->LoadSelectedProj(exclude_ids, names);

  // Reads the cone beam projections
  using ReaderType = rtk::ProjectionsReader<FloatImageType>;
  auto reader = ReaderType::New();
  reader->SetFileNames(this->m_cbctrecon->m_vSelectedFileNames);
  // TRY_AND_EXIT_ON_ITK_EXCEPTION(
  // std::thread calc_thread(read_projections, reader);
  std::thread calc_thread([&reader]() { reader->Update(); });
  // calc_thread.detach();

  std::cout << "Reader detached from main thread" << std::endl;

  // After reading the whole file,
  // HIS header should be saved
  this->m_cbctrecon->saveHisHeader();

  //  Insta Recon, Dcm read
  const auto geopath = geomFileInfo.absolutePath();
  std::tuple<bool, bool> answers;
  auto bowtie_reader = ReadBowtieFileWhileProbing(geopath, answers);

  calc_thread.join();
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
  auto res_factor = this->ui.lineEdit_DownResolFactor->text().toDouble();
  if (!this->m_cbctrecon->ResampleProjections(res_factor)) { // 0.5
    // reset factor if image was not resampled
    this->ui.lineEdit_DownResolFactor->setText("1.0");
  }

  this->m_cbctrecon->m_spProjImgRaw3D =
      this->m_cbctrecon->ConvertLineInt2Intensity(
          this->m_cbctrecon->m_spProjImg3DFloat);
  // if X not 1024 == input size: out_offset =
  // in_offset + (1024*res_f - X*res_f)*out_spacing     <- will still
  // break down at fw_projection

  this->ui.pushButton_DoRecon->setEnabled(true);

  this->ui.spinBoxImgIdx->setMinimum(0);
  this->ui.spinBoxImgIdx->setMaximum(
      this->m_cbctrecon->m_vSelectedFileNames.size() - 1);
  this->ui.spinBoxImgIdx->setValue(0); // it doesn't call Draw Event .. don't
                                       // know why.

  this->m_cbctrecon
      ->SetMaxAndMinValueOfProjectionImage(); // update min max projection image

  SLT_InitializeGraphLim();

  this->SLT_DrawProjImages(); // Update Table is called

  if (!std::get<0>(answers)) { // instaRecon
    std::cout
        << "FINISHED!: Loading projection files. Proceed to reconstruction"
        << std::endl;
  } else {
    SLT_DoReconstruction();
  }

  if (std::get<0>(answers) && std::get<1>(answers)) { // CT DCM dir was found
    SLT_ViewRegistration();
  }
}
*/

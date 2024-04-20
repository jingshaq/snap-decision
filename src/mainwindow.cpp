#include "snapdecision/mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMimeData>
#include <QSettings>

#include "./ui_about.h"
#include "./ui_mainwindow.h"
#include "./ui_program_settings.h"
#include "qdialog.h"
#include "snapdecision/imagetreemodel.h"
#include "snapdecision/program_settings.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->action_About, &QAction::triggered, this, &MainWindow::showAbout);
  connect(ui->action_Preferences, &QAction::triggered, this, &MainWindow::showSettings);
  connect(ui->action_Open_Directory, &QAction::triggered, this, &MainWindow::openDialog);

  QSettings qsettings("JasonIMercer", "SnapDecision");
  restoreGeometry(qsettings.value("geometry").toByteArray());
  restoreState(qsettings.value("windowState").toByteArray());

  ui->hsplitter->restoreState(qsettings.value("hsplitState").toByteArray());
  ui->splitter->restoreState(qsettings.value("splitState").toByteArray());


  ui->toolContainerWidget->deleteLater();
  //ui->statusbar->addPermanentWidget(ui->toolContainerWidget);
}

MainWindow::~MainWindow()
{
  QSettings qsettings("JasonIMercer", "SnapDecision");
  qsettings.setValue("geometry", saveGeometry());
  qsettings.setValue("windowState", saveState());
  qsettings.setValue("hsplitState", ui->hsplitter->saveState());
  qsettings.setValue("splitState", ui->splitter->saveState());

  delete ui;
}

void MainWindow::initialize()
{
  connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
          &MainWindow::onTreeSelectionChanged);

  const auto setImage = [](auto* lbl, const auto& path)
  {
    QPixmap originalPixmap(path);
    int labelHeight = lbl->fontMetrics().height();
    QPixmap scaledPixmap = originalPixmap.scaledToHeight(labelHeight, Qt::SmoothTransformation);
    lbl->setPixmap(scaledPixmap);
  };

  setImage(ui->lblECGfx, ":/images/ec.png");
  setImage(ui->lblZoomGfx, ":/images/zoom.png");
  setImage(ui->lblFGfx, ":/images/aperture.png");
  setImage(ui->lblTimeGfx, ":/images/clock.png");
  setImage(ui->lblSpeedGfx, ":/images/timer.png");
  setImage(ui->lblIsoGfx, ":/images/iso.png");

  const auto copyTip = [](auto* dest, auto* src) { dest->setToolTip(src->toolTip()); };

  copyTip(ui->lblECVal, ui->lblECGfx);
  copyTip(ui->lblZoomVal, ui->lblZoomGfx);
  copyTip(ui->lblFval, ui->lblFGfx);
  copyTip(ui->lblTime, ui->lblTimeGfx);
  copyTip(ui->lblSpeedVal, ui->lblSpeedGfx);
  copyTip(ui->lblIsoVal, ui->lblIsoGfx);

  auto f = ui->lblTime->font();
  f.setBold(false);

  ui->lblTime->setFont(f);
  ui->lblECVal->setFont(f);
  ui->lblFval->setFont(f);
  ui->lblIsoVal->setFont(f);
  ui->lblSpeedVal->setFont(f);
  ui->lblMode->setFont(f);
  ui->lblZoomVal->setFont(f);
}

void MainWindow::openDialog()
{
  QString filter = "Image Files (*.png *.jpg *.jpeg *.bmp *.gif)";
  QString fileName = QFileDialog::getOpenFileName(nullptr, "Select an Image", QDir::homePath(), filter);

  if (!fileName.isEmpty())
  {
    emit resourceDropped(fileName);
  }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
  if (event->mimeData()->hasUrls())
  {
    event->acceptProposedAction();
  }
}

void MainWindow::dropEvent(QDropEvent* event)
{
  const QMimeData* mimeData = event->mimeData();

  if (mimeData->hasUrls())
  {
    QList<QUrl> urlList = mimeData->urls();

    if (!urlList.isEmpty())
    {
      QUrl url = urlList.first();
      if (url.isLocalFile())
      {
        QString path = url.toLocalFile();
        QFileInfo fileInfo(path);

        if (fileInfo.isFile() || fileInfo.isDir())
        {
          emit resourceDropped(path);
        }
      }
    }
  }
}

void MainWindow::setImageProperties(const os& time, const os& mode, const os& zoom, const os& speed, const os& f,
                                    const os& ec, const os& iso)
{
  const auto qs = [](const auto& txt) { return QString::fromStdString(txt.value_or("")); };

  ui->lblTime->setText(qs(time));
  ui->lineTime->setVisible(mode.has_value());
  ui->lblTimeGfx->setVisible(mode.has_value());

  ui->lblMode->setText(qs(mode));
  ui->lineMode->setVisible(mode.has_value());

  ui->lblZoomVal->setText(qs(zoom));
  ui->lblZoomGfx->setEnabled(zoom.has_value());

  ui->lblSpeedVal->setText(qs(speed));
  ui->lblSpeedGfx->setVisible(speed.has_value());
  ui->lineSpeed->setVisible(speed.has_value());

  ui->lblFval->setText(qs(f));
  ui->lblFGfx->setEnabled(f.has_value());
  ui->lineF->setVisible(f.has_value());

  ui->lblECVal->setText(qs(ec));
  ui->lblECGfx->setEnabled(ec.has_value());

  ui->lblIsoVal->setText(qs(iso));
  ui->lblIsoGfx->setEnabled(iso.has_value());
}

void MainWindow::showAbout()
{
  QDialog dialog(this);

  Ui::AboutDialog about_ui;

  about_ui.setupUi(&dialog);

  dialog.exec();
}

void MainWindow::showSettings()
{
  ProgramSettings program_settings(this);  // switch to non-ui version so we can have custom code

  program_settings.setSettings(get_settings_());

  if (program_settings.exec() == QDialog::Rejected)
  {
    return;
  }

  set_settings_(program_settings.getSettings());
}

void MainWindow::onTreeSelectionChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
  if (selected.indexes().isEmpty())
  {
    return;
  }

  QModelIndex currentIndex = selected.indexes().first();

  auto imageDesc = static_cast<ImageTreeModel*>(ui->treeView->model())->nodeFromIndex(currentIndex);

  if (currentIndex.isValid())
  {
    emit imageFocused(QString::fromStdString(imageDesc->full_path));
  }
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <optional>

#include "qitemselectionmodel.h"
#include "snapdecision/settings.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void initialize();

  Ui::MainWindow* ui;

  SetSettings set_settings_;
  GetSettings get_settings_;

  void dragEnterEvent(QDragEnterEvent* event);

  void dropEvent(QDropEvent* event);

  using os = std::optional<std::string>;
  void setImageProperties(const os& time, const os& mode, const os& zoom, const os& speed, const os& f, const os& ec,
                          const os& iso);

signals:
  void imageClassified(const QString& file_path, int classification);
  void imageFocused(const QString& full_path);
  void resourceDropped(const QString& path);

private slots:
  void showAbout();
  void showSettings();
  void openDialog();

private:
  void onTreeSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
};
#endif  // MAINWINDOW_H

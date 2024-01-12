#ifndef PROGRAM_SETTINGS_H
#define PROGRAM_SETTINGS_H

#include <QDialog>
#include "snapdecision/settings.h"

namespace Ui {
class ProgramSettings;
}

class ProgramSettings : public QDialog
{
  Q_OBJECT

public:
  explicit ProgramSettings(QWidget *parent = nullptr);
  ~ProgramSettings();

  void setSettings(const Settings& s);
  Settings getSettings() const;
private:
  Ui::ProgramSettings *ui;
};

#endif // PROGRAM_SETTINGS_H

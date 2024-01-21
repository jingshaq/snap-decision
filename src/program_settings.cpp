#include "snapdecision/program_settings.h"

#include <QPushButton>

#include "./ui_program_settings.h"

ProgramSettings::ProgramSettings(QWidget* parent) : QDialog(parent), ui(new Ui::ProgramSettings)
{
  ui->setupUi(this);

  connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this,
          [this]() { setSettings(Settings{}); });
}

ProgramSettings::~ProgramSettings()
{
  delete ui;
}

void ProgramSettings::setSettings(const Settings& s)
{
  ui->spinBurst->setValue(s.burst_threshold_ms_ / 1000.0);
  ui->spinLocation->setValue(s.location_theshold_ms_ / (1000.0 * 60.0));
  ui->txtDeleteFolderName->setText(s.delete_foler_name_);
  ui->spinCache->setValue(s.cache_memory_mb_);
  ui->checkConsole->setChecked(s.show_debug_console_);

  const auto f = [&](auto* key, const auto& seq) { key->setKeySequence(seq); };

  f(ui->keyNextImage, s.key_next_image_);
  f(ui->keyPrevImage, s.key_prev_image_);
  f(ui->keyKeepAndNext, s.key_keep_and_next_);
  f(ui->keyDeleteAndNext, s.key_delete_and_next_);

  f(ui->keyUnclassifiedToDeleteAndNext, s.key_unclassified_to_delete_and_next_);
  f(ui->keyUnclassifiedToKeepAndNext, s.key_unclassified_to_keep_and_next_);

  f(ui->keyPanLeft, s.key_pan_left_);
  f(ui->keyPanRight, s.key_pan_right_);
  f(ui->keyPanUp, s.key_pan_up_);
  f(ui->keyPanDown, s.key_pan_down_);

  f(ui->keyResetView, s.key_reset_view_);
  f(ui->keyZoom1, s.key_zoom_1_);
  f(ui->keyZoom2, s.key_zoom_2_);
  f(ui->keyZoom3, s.key_zoom_3_);
  f(ui->keyZoom4, s.key_zoom_4_);
  f(ui->keyZoom5, s.key_zoom_5_);

  f(ui->keyVoteUp, s.key_vote_up_);
  f(ui->keyVoteDown, s.key_vote_down_);

  f(ui->keyPreferences, s.key_preferences_);
  f(ui->keyDeleteMarked, s.key_delete_marked_);

  ui->groupBox1->setChecked(s.tool[0].enabled);
  ui->groupBox2->setChecked(s.tool[1].enabled);
  ui->groupBox3->setChecked(s.tool[2].enabled);
  ui->groupBox4->setChecked(s.tool[3].enabled);

  ui->txtToolName1->setText(s.tool[0].name);
  ui->txtToolName2->setText(s.tool[1].name);
  ui->txtToolName3->setText(s.tool[2].name);
  ui->txtToolName4->setText(s.tool[3].name);

  ui->txtTool1->setText(s.tool[0].command);
  ui->txtTool2->setText(s.tool[1].command);
  ui->txtTool3->setText(s.tool[2].command);
  ui->txtTool4->setText(s.tool[3].command);

  f(ui->keyTool1, s.tool[0].sequence);
  f(ui->keyTool2, s.tool[1].sequence);
  f(ui->keyTool3, s.tool[2].sequence);
  f(ui->keyTool4, s.tool[3].sequence);
}

Settings ProgramSettings::getSettings() const
{
  Settings s;
  s.burst_threshold_ms_ = ui->spinBurst->value() * 1000;
  s.location_theshold_ms_ = ui->spinLocation->value() * 1000 * 60;
  s.delete_foler_name_ = ui->txtDeleteFolderName->text();
  if (s.delete_foler_name_.isEmpty())
  {
    s.delete_foler_name_ = "delete";
  }
  s.cache_memory_mb_ = ui->spinCache->value();
  s.show_debug_console_ = ui->checkConsole->isChecked();
  s.key_next_image_ = ui->keyNextImage->keySequence();

  const auto f = [&](const auto* key, auto& seq) { seq = key->keySequence(); };

  f(ui->keyNextImage, s.key_next_image_);
  f(ui->keyPrevImage, s.key_prev_image_);
  f(ui->keyKeepAndNext, s.key_keep_and_next_);
  f(ui->keyDeleteAndNext, s.key_delete_and_next_);

  f(ui->keyUnclassifiedToDeleteAndNext, s.key_unclassified_to_delete_and_next_);
  f(ui->keyUnclassifiedToKeepAndNext, s.key_unclassified_to_keep_and_next_);

  f(ui->keyPanLeft, s.key_pan_left_);
  f(ui->keyPanRight, s.key_pan_right_);
  f(ui->keyPanUp, s.key_pan_up_);
  f(ui->keyPanDown, s.key_pan_down_);

  f(ui->keyResetView, s.key_reset_view_);
  f(ui->keyZoom1, s.key_zoom_1_);
  f(ui->keyZoom2, s.key_zoom_2_);
  f(ui->keyZoom3, s.key_zoom_3_);
  f(ui->keyZoom4, s.key_zoom_4_);
  f(ui->keyZoom5, s.key_zoom_5_);

  f(ui->keyVoteUp, s.key_vote_up_);
  f(ui->keyVoteDown, s.key_vote_down_);

  f(ui->keyPreferences, s.key_preferences_);
  f(ui->keyDeleteMarked, s.key_delete_marked_);

  s.tool[0].enabled = ui->groupBox1->isChecked();
  s.tool[1].enabled = ui->groupBox2->isChecked();
  s.tool[2].enabled = ui->groupBox3->isChecked();
  s.tool[3].enabled = ui->groupBox4->isChecked();

  s.tool[0].name = ui->txtToolName1->text();
  s.tool[1].name = ui->txtToolName2->text();
  s.tool[2].name = ui->txtToolName3->text();
  s.tool[3].name = ui->txtToolName4->text();

  s.tool[0].command = ui->txtTool1->text();
  s.tool[1].command = ui->txtTool2->text();
  s.tool[2].command = ui->txtTool3->text();
  s.tool[3].command = ui->txtTool4->text();

  f(ui->keyTool1, s.tool[0].sequence);
  f(ui->keyTool2, s.tool[1].sequence);
  f(ui->keyTool3, s.tool[2].sequence);
  f(ui->keyTool4, s.tool[3].sequence);

  return s;
}

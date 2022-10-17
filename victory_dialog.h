#ifndef VICTORY_DIALOG_H
#define VICTORY_DIALOG_H

#include <QCloseEvent>
#include <QDialog>

#include "style_helper.h"
#include "ui_victory_dialog.h"

QT_BEGIN_NAMESPACE
namespace InSearchOfTreasure {

class VictoryDialog : public QDialog {
  Q_OBJECT

 public:
  explicit VictoryDialog(QWidget *parent = nullptr)
      : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    this->setStyleSheet(StyleHelper::getDialogWindowStyle());
    ui->pushButton_restart->setStyleSheet(
        StyleHelper::getStartGameAboutGameButtonStyle());
    ui->pushButton_end->setStyleSheet(
        StyleHelper::getStartGameAboutGameButtonStyle());
    ui->label->setStyleSheet(StyleHelper::getStyleLabel());
  }
  ~VictoryDialog() { delete ui; }

  auto closeEvent(QCloseEvent *event) -> void {
    emit end();
    event->accept();
  }

 signals:
  void restart();
  void end();

 private slots:
  void on_pushButton_end_clicked() { emit end(); }
  void on_pushButton_restart_clicked() { emit restart(); }

 private:
  Ui::Dialog *ui;
};
}  // namespace InSearchOfTreasure
#endif  // VICTORY_DIALOG_H

#ifndef VICTORY_DIALOG_H
#define VICTORY_DIALOG_H

#include <QCloseEvent>
#include <QDialog>

#include "style_helper.h"
#include "ui_victory_dialog.h"

QT_BEGIN_NAMESPACE
namespace InSearchOfTreasure_2_0 {

class MessageWindow : public QDialog {
  Q_OBJECT

 public:
  explicit MessageWindow(QString const &messege,
                         QString const &text_but1,
                         QString const &text_but2,
                         QWidget *parent = nullptr)
      : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    this->setStyleSheet(StyleHelper::getDialogWindowStyle());

    ui->pushButton_1->setStyleSheet(
        StyleHelper::getUndoButtonStyle());
    ui->pushButton_1->setText(text_but1);


    ui->pushButton_2->setStyleSheet(
        StyleHelper::getUndoButtonStyle());
    ui->pushButton_2->setText(text_but2);

    ui->label->setStyleSheet(StyleHelper::getStyleLabelAboutGame());
    ui->label->setText(messege);
  }
  ~MessageWindow() { delete ui; }


  auto closeEvent(QCloseEvent *event) -> void {
       emit close_dialog();
       event->accept();
  }

 signals:
  void button1();
  void button2();
  void close_dialog();

 private slots:
  void on_pushButton_1_clicked() {
      emit button1();
      this->close();
  }
  void on_pushButton_2_clicked() {
      emit button2();
      this->close();
  }

 private:
  Ui::Dialog *ui;
};

}  // namespace InSearchOfTreasure_2_0
#endif  // VICTORY_DIALOG_H

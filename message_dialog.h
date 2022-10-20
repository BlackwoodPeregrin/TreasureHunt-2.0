#ifndef MESSAGE_DIALOG_H
#define MESSAGE_DIALOG_H

#include <QCloseEvent>
#include <QDialog>

#include "style_helper.h"
#include "ui_message_dialog.h"

QT_BEGIN_NAMESPACE
namespace InSearchOfTreasure_2_0 {

class MessageWindow : public QDialog {
  Q_OBJECT

 public:
  explicit MessageWindow(QString const &messege, QString const &title, QIcon button1, QSize size1,
                         QIcon button2, QSize size2, QWidget *parent = nullptr)
      : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);

    this->setWindowTitle(title);
    this->setStyleSheet(StyleHelper::getBackgroundPlayingField());

    ui->pushButton_1->setStyleSheet(
        StyleHelper::getUndoButtonStyle());
    ui->pushButton_1->setIcon(button1);
    ui->pushButton_1->setIconSize(size1);

    ui->pushButton_2->setStyleSheet(
        StyleHelper::getUndoButtonStyle());
    ui->pushButton_2->setIcon(button2);
    ui->pushButton_2->setIconSize(size2);

    ui->label->setStyleSheet(StyleHelper::getStyleLabelCloseGame());
    ui->label->setText(messege);

    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(reject()));
  }
  ~MessageWindow() { delete ui; }


 private:
  Ui::Dialog *ui;
};

}  // namespace InSearchOfTreasure_2_0
#endif  // MESSAGE_DIALOG_H

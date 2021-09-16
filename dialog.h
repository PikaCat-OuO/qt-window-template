#pragma once

#include <QCloseEvent>
#include <QDialog>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog {
  Q_OBJECT
  Q_PROPERTY(QColor mColorClose READ getColorClose WRITE setColorClose)
  Q_PROPERTY(QColor mColorMin READ getColorMin WRITE setColorMin)

public:
  Dialog(QWidget *parent = nullptr);
  ~Dialog();

protected:
  void closeEvent(QCloseEvent *);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  bool eventFilter(QObject *watched, QEvent *event);

private slots:
  void on_CloseButton_clicked();

  void on_MinButton_clicked();

private:
  Ui::Dialog *ui;
  bool mCloseCheck{false};
  bool mOnDialog{false};
  QPoint mMouseStartPoint;
  QPoint mDialogStartPoint;
  QColor getColorClose() const;
  void setColorClose(const QColor color);
  QColor mColorClose{QColor(212, 64, 39, 0)};
  QColor getColorMin() const;
  void setColorMin(const QColor color);
  QColor mColorMin{QColor(38, 169, 218, 0)};
};

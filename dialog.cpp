#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
  ui->setupUi(this);
  //安装标题栏按钮事件监控
  ui->CloseButton->installEventFilter(this);
  ui->MinButton->installEventFilter(this);
  //设置无边框窗口
  setWindowFlag(Qt::FramelessWindowHint);
  //设置窗口透明
  setAttribute(Qt::WA_TranslucentBackground);
  //设置窗口在屏幕正中央显示
  auto desk = QApplication::primaryScreen()->geometry();
  move((desk.width() - this->width()) / 2,
       (desk.height() - this->height()) / 2);
  //窗口启动动画
  QPropertyAnimation *Ani = new QPropertyAnimation(this, "windowOpacity");
  Ani->setDuration(600);
  Ani->setStartValue(0);
  Ani->setEndValue(0.94);
  Ani->setEasingCurve(QEasingCurve::InOutSine);
  Ani->start(QPropertyAnimation::DeleteWhenStopped);
  //设置窗口阴影
  QGraphicsDropShadowEffect *Shadow = new QGraphicsDropShadowEffect;
  Shadow->setOffset(0, 0);
  Shadow->setColor(QColor(0, 0, 0, 50));
  Shadow->setBlurRadius(10);
  ui->frame->setGraphicsEffect(Shadow);
}

Dialog::~Dialog() { delete ui; }
//重写关闭事件实现窗口关闭动画
void Dialog::closeEvent(QCloseEvent *event) {
  if (this->mCloseCheck == false) {
    this->mCloseCheck = true;
    event->ignore();
    //窗口关闭动画
    QPropertyAnimation *Ani = new QPropertyAnimation(this, "windowOpacity");
    Ani->setDuration(600);
    Ani->setStartValue(0.94);
    Ani->setEndValue(0);
    Ani->setEasingCurve(QEasingCurve::InOutSine);
    Ani->start();
    connect(Ani, &QPropertyAnimation::finished, this, &Dialog::close);
  } else {
    event->accept();
  }
}

//重写鼠标移动事件，实现点击窗口任意处移动窗口的功能
void Dialog::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    this->mOnDialog = true;
    this->mMouseStartPoint = event->globalPosition().toPoint();
    this->mDialogStartPoint = this->frameGeometry().topLeft();
  }
}

void Dialog::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton and this->mOnDialog) {
    QPoint MouseMoveDelta =
        event->globalPosition().toPoint() - this->mMouseStartPoint;
    this->move(this->mDialogStartPoint + MouseMoveDelta);
  } else {
    //单击按住按钮后离开按钮区域，按钮回复原样
    this->mColorClose = QColor(212, 64, 39, 0);
    QString qssClose = QString("#CloseButton{border-image:url(:/Images/"
                               "close.ico);border-radius:5px;background: "
                               "rgba(212,64,39,0);}#CloseButton:pressed{border-"
                               "image:url(:/Images/close_press.ico);}");
    ui->CloseButton->setStyleSheet(qssClose);
    this->mColorMin = QColor(38, 169, 218, 0);
    QString qssMin = QString("#MinButton{border-image:url(:/Images/"
                             "min.ico);border-radius:5px;background: "
                             "rgba(38,169,218,0);}#MinButton:pressed{border-"
                             "image:url(:/Images/min_press.bmp);}");
    ui->MinButton->setStyleSheet(qssMin);
  }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    this->mOnDialog = false;
  }
}

//实现按Esc键关闭窗口时也能播放动画
void Dialog::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    this->close();
  }
}

//标题栏按钮动画
bool Dialog::eventFilter(QObject *watched, QEvent *event) {
  //关闭按钮点燃效果
  if (watched == ui->CloseButton) {
    if (event->type() == QEvent::Enter) {
      QPropertyAnimation *Ani = new QPropertyAnimation(this, "ColorClose");
      Ani->setDuration(100);
      Ani->setStartValue(getColorClose());
      Ani->setEndValue(QColor(212, 64, 39, 230));
      Ani->start(QPropertyAnimation::DeleteWhenStopped);
      return true;
    }
    if (event->type() == QEvent::Leave) {
      QPropertyAnimation *Ani = new QPropertyAnimation(this, "ColorClose");
      Ani->setDuration(100);
      Ani->setStartValue(getColorClose());
      Ani->setEndValue(QColor(212, 64, 39, 0));
      Ani->start(QPropertyAnimation::DeleteWhenStopped);
      return true;
    }
  }
  //最小化按钮点燃效果
  if (watched == ui->MinButton) {
    if (event->type() == QEvent::Enter) {
      QPropertyAnimation *Ani = new QPropertyAnimation(this, "ColorMin");
      Ani->setDuration(100);
      Ani->setStartValue(getColorMin());
      Ani->setEndValue(QColor(38, 169, 218, 230));
      Ani->start(QPropertyAnimation::DeleteWhenStopped);
      return true;
    }
    if (event->type() == QEvent::Leave) {
      QPropertyAnimation *Ani = new QPropertyAnimation(this, "ColorMin");
      Ani->setDuration(100);
      Ani->setStartValue(getColorMin());
      Ani->setEndValue(QColor(38, 169, 218, 0));
      Ani->start(QPropertyAnimation::DeleteWhenStopped);
      return true;
    }
  }
  return false;
}
//获取关闭按钮颜色
QColor Dialog::getColorClose() const { return this->mColorClose; }
//设置关闭按钮颜色
void Dialog::setColorClose(const QColor color) {
  mColorClose = color;
  QString qss =
      QString("#CloseButton{border-image:url(:/Images/"
              "close.ico);border-radius:5px;background: rgba(%1, %2, %3, "
              "%4);}#CloseButton:pressed{border-image:url(:/Images/"
              "close_press.ico);}")
          .arg(color.red())
          .arg(color.green())
          .arg(color.blue())
          .arg(color.alpha());
  ui->CloseButton->setStyleSheet(qss);
}
//获得最小化按钮颜色
QColor Dialog::getColorMin() const { return this->mColorMin; }
//设置最小化按钮颜色
void Dialog::setColorMin(const QColor color) {
  mColorMin = color;
  QString qss =
      QString(
          "#MinButton{border-image:url(:/Images/"
          "min.ico);border-radius:5px;background: rgba(%1, %2, %3, "
          "%4);}#MinButton:pressed{border-image:url(:/Images/min_press.bmp);}")
          .arg(color.red())
          .arg(color.green())
          .arg(color.blue())
          .arg(color.alpha());
  ui->MinButton->setStyleSheet(qss);
}
//窗口按钮事件
void Dialog::on_CloseButton_clicked() { this->close(); }

void Dialog::on_MinButton_clicked() { this->showMinimized(); }

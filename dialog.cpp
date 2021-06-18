#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
  ui->setupUi(this);
  //��װ��������ť�¼����
  ui->CloseButton->installEventFilter(this);
  ui->MinButton->installEventFilter(this);
  //�����ޱ߿򴰿�
  setWindowFlag(Qt::FramelessWindowHint);
  //���ô���͸��
  setAttribute(Qt::WA_TranslucentBackground);
  //���ô�������Ļ��������ʾ
  auto desk = QApplication::primaryScreen()->geometry();
  move((desk.width() - this->width()) / 2,
       (desk.height() - this->height()) / 2);
  //������������
  QPropertyAnimation *Ani = new QPropertyAnimation(this, "windowOpacity");
  Ani->setDuration(600);
  Ani->setStartValue(0);
  Ani->setEndValue(0.94);
  Ani->setEasingCurve(QEasingCurve::InOutSine);
  Ani->start(QPropertyAnimation::DeleteWhenStopped);
  //���ô�����Ӱ
  QGraphicsDropShadowEffect *Shadow = new QGraphicsDropShadowEffect;
  Shadow->setOffset(0, 0);
  Shadow->setColor(QColor(0, 0, 0, 50));
  Shadow->setBlurRadius(10);
  ui->frame->setGraphicsEffect(Shadow);
}

Dialog::~Dialog() { delete ui; }
//��д�ر��¼�ʵ�ִ��ڹرն���
void Dialog::closeEvent(QCloseEvent *event) {
  if (this->mCloseCheck == false) {
    this->mCloseCheck = true;
    event->ignore();
    //���ڹرն���
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

//��д����ƶ��¼���ʵ�ֵ���������⴦�ƶ����ڵĹ���
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
    //������ס��ť���뿪��ť���򣬰�ť�ظ�ԭ��
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

//ʵ�ְ�Esc���رմ���ʱҲ�ܲ��Ŷ���
void Dialog::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    this->close();
  }
}

//��������ť����
bool Dialog::eventFilter(QObject *watched, QEvent *event) {
  //�رհ�ť��ȼЧ��
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
  //��С����ť��ȼЧ��
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
//��ȡ�رհ�ť��ɫ
QColor Dialog::getColorClose() const { return this->mColorClose; }
//���ùرհ�ť��ɫ
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
//�����С����ť��ɫ
QColor Dialog::getColorMin() const { return this->mColorMin; }
//������С����ť��ɫ
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
//���ڰ�ť�¼�
void Dialog::on_CloseButton_clicked() { this->close(); }

void Dialog::on_MinButton_clicked() { this->showMinimized(); }

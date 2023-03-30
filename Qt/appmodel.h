#ifndef APPMODEL_H
#define APPMODEL_H

#include "EgoInterface.h"
#include "ImageProvider.h"
#include <QObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui>
#include <QtQml/qqml.h>

class CarData : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString uid READ uid WRITE setUid NOTIFY dataChanged)
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY dataChanged)
  Q_PROPERTY(QString type READ type WRITE setType NOTIFY dataChanged)
  Q_PROPERTY(QString state READ state WRITE setState NOTIFY dataChanged)

public:
  explicit CarData(QObject *parent = 0);
  CarData(const CarData &other);

  QString uid() const;
  QString name() const;
  QString type() const;
  QString state() const;

  void setUid(const QString &value);
  void setName(const QString &value);
  void setType(const QString &value);
  void setState(const QString &value);

signals:
  void dataChanged();

private:
  QString m_uid;
  QString m_name;
  QString m_type;
  QString m_state;
};
Q_DECLARE_METATYPE(CarData)

class AppModelPrivate;

class AppModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
  Q_PROPERTY(QQmlListProperty<CarData> car READ car NOTIFY carChanged)
  Q_PROPERTY(int32_t radar1 READ radar1 WRITE setRadar1 NOTIFY radar1Changed)
  Q_PROPERTY(int32_t radar2 READ radar2 WRITE setRadar2 NOTIFY radar2Changed)
  Q_PROPERTY(int32_t radar3 READ radar3 WRITE setRadar3 NOTIFY radar3Changed)
  Q_PROPERTY(int32_t radar4 READ radar4 WRITE setRadar4 NOTIFY radar4Changed)
  Q_PROPERTY(int32_t radar5 READ radar5 WRITE setRadar5 NOTIFY radar5Changed)
  Q_PROPERTY(int32_t radar6 READ radar6 WRITE setRadar6 NOTIFY radar6Changed)
  Q_PROPERTY(int32_t ping READ ping WRITE setPing NOTIFY pingChanged)
  Q_PROPERTY(int32_t imux READ imux WRITE setImux NOTIFY imuxChanged)
  Q_PROPERTY(int32_t imuy READ imuy WRITE setImuy NOTIFY imuyChanged)
  Q_PROPERTY(int32_t alertSound READ alertSound NOTIFY playAlertSound)
  Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
  Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

public:
  explicit AppModel(IEgoNotify *n = 0, IEgoControl *c = 0, QObject *parent = 0);
  ~AppModel();

  Q_INVOKABLE void userLogin(QString useName, QString password);
  Q_INVOKABLE void connectCar(int id);
  Q_INVOKABLE void leaveCar();
  Q_INVOKABLE void logout();
  Q_INVOKABLE void loadUser();
  bool ready() const;

  int radar1() const;
  int radar2() const;
  int radar3() const;
  int radar4() const;
  int radar5() const;
  int radar6() const;
  int ping() const;
  int alertSound() const;
  int imux() const;
  int imuy() const;
  QString username() const;
  QString password() const;

  void setReady(const bool value);
  void setRadar1(const int32_t &value);
  void setRadar2(const int32_t &value);
  void setRadar3(const int32_t &value);
  void setRadar4(const int32_t &value);
  void setRadar5(const int32_t &value);
  void setRadar6(const int32_t &value);
  void setPing(const int32_t &value);
  void setImux(const int32_t &value);
  void setImuy(const int32_t &value);
  void setUsername(const QString &value);
  void setPassword(const QString &value);
  QQmlListProperty<CarData> car() const;

public slots:
  void onLoginRst(bool rst);
  void onCarAppend(const UserCamera &info);
  void onRadarUpadte(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4,
                     int32_t r5);
  void onPing(int32_t value);
  void onImuUpdate(int32_t x, int32_t y);
  void onNotifyDel();
  void onKickoff();

signals:
  void readyChanged();
  void carChanged();
  void radar1Changed();
  void radar2Changed();
  void radar3Changed();
  void radar4Changed();
  void radar5Changed();
  void radar6Changed();
  void pingChanged();
  void imuxChanged();
  void imuyChanged();
  void usernameChanged();
  void passwordChanged();
  void playAlertSound();
  void carConnected();
  void closeWin();

private:
  IEgoNotify *_notify;
  IEgoControl *_control;

  AppModelPrivate *d;

  int32_t m_radar1;
  int32_t m_radar2;
  int32_t m_radar3;
  int32_t m_radar4;
  int32_t m_radar5;
  int32_t m_radar6;
  int32_t m_ping;
  int32_t m_alert;
  int32_t m_imux;
  int32_t m_imuy;
  bool m_ready;
  QString m_username;
  QString m_password;
};

#endif // APPMODEL_H

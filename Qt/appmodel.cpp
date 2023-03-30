#include "appmodel.h"
#include "Ego.h"

// extern std::array<IRender *, 3> ar;

CarData::CarData(QObject *parent) : QObject(parent) {}

CarData::CarData(const CarData &other)
    : QObject(nullptr), m_uid(other.m_uid), m_name(other.m_name),
      m_type(other.m_type), m_state(other.m_state) {}

QString CarData::uid() const { return m_uid; }

QString CarData::name() const { return m_name; }

QString CarData::type() const { return m_type; }

QString CarData::state() const { return m_state; }

void CarData::setUid(const QString &value) {
  m_uid = value;
  emit dataChanged();
}

void CarData::setName(const QString &value) {
  m_name = value;
  emit dataChanged();
}

void CarData::setState(const QString &value) {
  m_state = value;
  emit dataChanged();
}

void CarData::setType(const QString &value) {
  m_type = value;
  emit dataChanged();
}

class AppModelPrivate {
public:
  QList<CarData *> car;
  QQmlListProperty<CarData> *carProp = nullptr;
};

static void carAppend(QQmlListProperty<CarData> *prop, CarData *val) {
  Q_UNUSED(val);
  Q_UNUSED(prop);
}

static CarData *carAt(QQmlListProperty<CarData> *prop, qsizetype index) {
  AppModelPrivate *d = static_cast<AppModelPrivate *>(prop->data);
  return d->car.at(index);
}

static qsizetype carCount(QQmlListProperty<CarData> *prop) {
  AppModelPrivate *d = static_cast<AppModelPrivate *>(prop->data);
  return d->car.size();
}

static void carClear(QQmlListProperty<CarData> *prop) {
  static_cast<AppModelPrivate *>(prop->data)->car.clear();
}

AppModel::AppModel(IEgoNotify *n, IEgoControl *c, QObject *parent)
    : _notify(n), _control(c), QObject(parent), d(new AppModelPrivate) {
  d->carProp = new QQmlListProperty<CarData>(this, d, carAppend, carCount,
                                             carAt, carClear);

  connect(dynamic_cast<QObject *>(_notify), SIGNAL(egoLoginResult(bool)), this,
          SLOT(onLoginRst(bool)));

  connect(dynamic_cast<QObject *>(_notify), SIGNAL(egoCarAppend(UserCamera)),
          this, SLOT(onCarAppend(UserCamera)));

  connect(
      dynamic_cast<QObject *>(_notify),
      SIGNAL(egoRadar(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t)),
      this,
      SLOT(
          onRadarUpadte(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t)));

  connect(dynamic_cast<QObject *>(_notify), SIGNAL(egoPing(int32_t)), this,
          SLOT(onPing(int32_t)));

  connect(dynamic_cast<QObject *>(_notify), SIGNAL(egoImu(int32_t, int32_t)),
          this, SLOT(onImuUpdate(int32_t, int32_t)));

  connect(dynamic_cast<QObject *>(_notify), SIGNAL(egoNotifyDel()), this,
          SLOT(onNotifyDel()));

  connect(dynamic_cast<QObject *>(_notify), SIGNAL(egoKickoff()), this,
          SLOT(onKickoff()));
}

AppModel::~AppModel() {
  if (d->carProp)
    delete d->carProp;

  if (d)
    delete d;
}

bool AppModel::ready() const { return m_ready; }

int AppModel::radar1() const { return m_radar1; }

int AppModel::radar2() const { return m_radar2; }

int AppModel::radar3() const { return m_radar3; }

int AppModel::radar4() const { return m_radar4; }

int AppModel::radar5() const { return m_radar5; }

int AppModel::radar6() const { return m_radar6; }

int AppModel::ping() const { return m_ping; }

int AppModel::imux() const { return m_imux; }

int AppModel::imuy() const { return m_imuy; }

QString AppModel::username() const { return m_username; }

QString AppModel::password() const { return m_password; }

int AppModel::alertSound() const { return m_alert; }

void AppModel::setReady(const bool value) {
  m_ready = value;
  emit readyChanged();
}

void AppModel::setRadar1(const int32_t &value) {
  m_radar1 = value;
  emit radar1Changed();
}

void AppModel::setRadar2(const int32_t &value) {

  m_radar2 = value;
  emit radar2Changed();
}

void AppModel::setRadar3(const int32_t &value) {

  m_radar3 = value;
  emit radar3Changed();
}

void AppModel::setRadar4(const int32_t &value) {

  m_radar4 = value;
  emit radar4Changed();
}

void AppModel::setRadar5(const int32_t &value) {

  m_radar5 = value;
  emit radar5Changed();
}

void AppModel::setRadar6(const int32_t &value) {

  m_radar6 = value;
  emit radar6Changed();
}

void AppModel::setPing(const int32_t &value) {

  m_ping = value;
  emit pingChanged();
}

void AppModel::setImux(const int32_t &value) {

  m_imux = value;
  emit imuxChanged();
}

void AppModel::setUsername(const QString &value) {

  m_username = value;
  emit usernameChanged();
}

void AppModel::setPassword(const QString &value) {

  m_password = value;
  emit passwordChanged();
}

void AppModel::setImuy(const int32_t &value) {

  m_imuy = value;
  emit imuyChanged();
}

void AppModel::onKickoff() { emit closeWin(); }

QQmlListProperty<CarData> AppModel::car() const { return *(d->carProp); }

void AppModel::userLogin(QString useName, QString password) {
  _control->Login(useName.toStdString(), password.toStdString());
}

void AppModel::leaveCar() {
  _control->OnCarLeave();
  //  setReady(false);
}

void AppModel::connectCar(int id) {
  _control->OnCarConnect(id);
  if (!m_ready) {
    setReady(true);
  }
}

void AppModel::onLoginRst(bool rst) {
  qDebug() << "AppModel::loginResult >>";
  if (rst) {
    // delete previous carList
    //    foreach (CarData *inf, d->car)
    //      delete inf;
    //    d->car.clear();

    // req new list
    //_control->ReqCarList();

  } else {
    qDebug() << "AppModel::loginResult >>" << rst;
  }
}

void AppModel::onNotifyDel() { d->car.clear(); }

void AppModel::onCarAppend(const UserCamera &info) {
  CarData users;
  users.setUid(QString::number(info.uid));
  users.setType(QString::number(info.type));
  users.setName(QString::fromStdString(info.name));
  users.setState(QString::number(info.state));
  d->car.append(&users);
  emit carChanged();
}

void AppModel::onRadarUpadte(int32_t r0, int32_t r1, int32_t r2, int32_t r3,
                             int32_t r4, int32_t r5) {

  if (r0 > 0)
    this->setRadar1(r0);
  if (r1 > 0)
    this->setRadar2(r1);
  if (r2 > 0)
    this->setRadar3(r2);
  if (r3 > 0)
    this->setRadar4(r3);
  if (r4 > 0)
    this->setRadar5(r4);
  if (r5 > 0)
    this->setRadar6(r5);

  int32_t distance = 2000;

  if ((r0 > 0 && r0 < distance) || (r1 > 0 && r1 < distance) ||
      (r2 > 0 && r2 < distance) || (r3 > 0 && r3 < distance) ||
      (r4 > 0 && r4 < distance) || (r5 > 0 && r5 < distance)) {
    emit playAlertSound();
  }
}

void AppModel::onPing(int32_t value) { this->setPing(value); }

void AppModel::onImuUpdate(int32_t x, int32_t y) {
  this->setImux(x);
  this->setImuy(y);
}

void AppModel::logout() {
  d->car.clear();
  m_ready = false;
}

void AppModel::loadUser() {
    std::string usr;
    std::string pwd;
    QString iniFilePath = QString(QCoreApplication::applicationDirPath()) + "/user.ini";
    QSettings ini(iniFilePath, QSettings::IniFormat);
    if(!QFile::exists(iniFilePath)) {
      ini.setValue("user/Name", "");
      ini.setValue("user/Password", "");
    } else {
      QString usr = ini.value("user/Name").toString();
      this->setUsername(usr);
      QString pwd = ini.value("user/Password").toString();
      this->setPassword(pwd);
    }
}

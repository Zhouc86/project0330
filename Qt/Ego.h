#ifndef EGOD_H
#define EGOD_H
#include "EgoInterface.h"
#include <QObject>
#include <QtGui>

Q_DECLARE_METATYPE(UserCamera);

class CEgoNotify : public QObject, public IEgoNotify {
  Q_OBJECT

public:
  void OnRobot(std::unique_ptr<UserCamera> &info) override;
  void OnSigin(bool bRet) override;
  void OnNotifyDel(int32_t peer) override;
  void OnNotifyRadar(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4,
                     int32_t r5) override;
  void OnNotifyImu(int32_t x, int32_t y) override;
  void OnNotifyPing(int32_t value) override;
  void OnNotifyKickOff() override;

  //#ifdef LIDAR_SENSOR
  //  void OnLidarData(bool isLeft, bool isDense, int32_t seq, PointXYZI *data,
  //                   int32_t length) override;
  //#endif

signals:
  void egoLoginResult(bool rst);
  void egoCarAppend(const UserCamera &info);
  void egoRadar(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4,
                int32_t r5);

  void egoImu(int32_t x, int32_t y);
  void egoPing(int32_t value);
  void egoNotifyDel();
  void egoKickoff();
};
#endif

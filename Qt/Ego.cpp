#include "Ego.h"

// start -> login -> OnSigin -> OnCarConnect -> OnRobot -> OnCarLeave
// -> OnNotifyDel

void CEgoNotify::OnSigin(bool bRet) {
  if (bRet) {
    emit egoLoginResult(true);
  } else {
    emit egoLoginResult(false);
  }
}

void CEgoNotify::OnRobot(std::unique_ptr<UserCamera> &info) {
  UserCamera users;
  users.uid = info->uid;
  users.name = info->name;
  users.type = info->type;
  users.state = info->state;
  emit egoCarAppend(users);
}

void CEgoNotify::OnNotifyDel(int32_t peer) {
  qDebug() << "OnNotifyDel";
  emit egoNotifyDel();
}

void CEgoNotify::OnNotifyRadar(int32_t r0, int32_t r1, int32_t r2, int32_t r3,
                               int32_t r4, int32_t r5) {

  emit egoRadar(r0, r1, r2, r3, r4, r5);
}

void CEgoNotify::OnNotifyImu(int32_t x, int32_t y) { emit egoImu(x, y); }

void CEgoNotify::OnNotifyPing(int32_t value) { emit egoPing(value); }

void CEgoNotify::OnNotifyKickOff() {
  qDebug() << "kick off";
  emit egoKickoff();
}

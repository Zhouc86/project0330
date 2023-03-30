#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include "EgoInterface.h"
#include <QImage>
#include <QPainter>
#include <QQuickImageProvider>
#include <mutex>

class ImageProvider : public QQuickImageProvider {
public:
  ImageProvider();

  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize);

  QPixmap requestPixmap(const QString &id, QSize *size,
                        const QSize &requestedSize);

  QImage img;
};

class ShowImage : public QObject, public IRender {
  Q_OBJECT

public:
  explicit ShowImage(QObject *parent = 0);

  ImageProvider *m_pImgProvider;

  void OnRender(std::unique_ptr<uint8_t> &pBuffer, int32_t width,
                int32_t height) override;

  RenderPosition campos;

public slots:
  void sendImage(QImage);
  void sendBackImage(QImage);
  void sendLeftImage(QImage);
  void sendRightImage(QImage);
  void sendPanelImage(QImage);

signals:
  void callQmlRefeshImg();
  void callQmlRefeshBackImg();
  void callQmlRefeshRightImg();
  void callQmlRefeshLeftImg();
  void callQmlRefeshPanelImg();
  void sendPic(QImage image);

private:
  QImage m_image;
  std::mutex lock_;
};
#endif // IMAGEPROVIDER_H

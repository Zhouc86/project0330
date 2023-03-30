#include "ImageProvider.h"
#include <QDebug>

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage ImageProvider::requestImage(const QString &id, QSize *size,
                                   const QSize &requestedSize) {
  return this->img;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size,
                                     const QSize &requestedSize) {
  return QPixmap::fromImage(this->img);
}

ShowImage::ShowImage(QObject *parent) : QObject(parent) {
  m_pImgProvider = new ImageProvider();
}

void ShowImage::sendImage(QImage sendimage) {
  m_pImgProvider->img = sendimage;
  emit callQmlRefeshImg();
}

void ShowImage::sendBackImage(QImage sendimage) {
  m_pImgProvider->img = sendimage;
  emit callQmlRefeshBackImg();
}

void ShowImage::sendLeftImage(QImage sendimage) {
  m_pImgProvider->img = sendimage;
  emit callQmlRefeshLeftImg();
}

void ShowImage::sendRightImage(QImage sendimage) {
  m_pImgProvider->img = sendimage;
  emit callQmlRefeshRightImg();
}

void ShowImage::sendPanelImage(QImage sendimage) {
  m_pImgProvider->img = sendimage;
  emit callQmlRefeshPanelImg();
}

void ShowImage::OnRender(std::unique_ptr<uint8_t> &pBuffer, int32_t width,
                         int32_t height) {

  if (campos == RenderPosition::FRONT) {
    m_image = QImage(pBuffer.get(), 1280, 720, QImage::Format_RGB32);
    this->sendImage(m_image);
  }

  if (campos == RenderPosition::BACK) {
    m_image = QImage(pBuffer.get(), 1280, 720, QImage::Format_RGB32);
    this->sendBackImage(m_image);
  }

  if (campos == RenderPosition::LEFT) {
    m_image = QImage(pBuffer.get(), 720, 1280, QImage::Format_RGB32);
    this->sendLeftImage(m_image);
  }

  if (campos == RenderPosition::RIGHT) {
    m_image = QImage(pBuffer.get(), 720, 1280, QImage::Format_RGB32);
    this->sendRightImage(m_image);
  }

  if (campos == RenderPosition::DASHBOARD) {
    m_image = QImage(pBuffer.get(), 1280, 720, QImage::Format_RGB32);
    this->sendPanelImage(m_image);
  }
}

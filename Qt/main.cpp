#include "Ego.h"
#include "EgoInterface.h"
#include "ImageProvider.h"
#include "appmodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/QDir>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlEngine>
#include <QtQuick>

int main(int argc, char *argv[]) {

  qputenv("QSG_RHI_BACKEND", "opengl");
  QGuiApplication app(argc, argv);

  QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

  QQuickView viewer;

  std::array<IRender *, RenderPosition::ALL> ar;

  IEgoNotify *_notify;
  IEgoControl *_control;

  for (int i = 0; i < RenderPosition::ALL; i++) {
    ar[i] = new ShowImage();
  }

  _notify = new CEgoNotify();

  _control = GetEgoController(_notify);

  _control->Start(ar);

  AppModel *appModel = new AppModel(_notify, _control);

  viewer.rootContext()->setContextProperty("appModel", appModel);

  QQmlEngine *engine = viewer.engine();

  // Front Cam
  ShowImage *FrontImage = static_cast<ShowImage *>(ar[0]);
  FrontImage->campos = RenderPosition::FRONT;
  engine->rootContext()->setContextProperty("FrontImage", FrontImage);
  engine->addImageProvider(QLatin1String("FrontImage"),
                           FrontImage->m_pImgProvider);

  // Back Cam
  ShowImage *BackImage = static_cast<ShowImage *>(ar[1]);
  BackImage->campos = RenderPosition::BACK;
  engine->rootContext()->setContextProperty("BackImage", BackImage);
  engine->addImageProvider(QLatin1String("BackImage"),
                           BackImage->m_pImgProvider);

  // Left Cam
  ShowImage *LeftImage = static_cast<ShowImage *>(ar[2]);
  LeftImage->campos = RenderPosition::LEFT;
  engine->rootContext()->setContextProperty("LeftImage", LeftImage);
  engine->addImageProvider(QLatin1String("LeftImage"),
                           LeftImage->m_pImgProvider);

  // Right Cam
  ShowImage *RightImage = static_cast<ShowImage *>(ar[3]);
  RightImage->campos = RenderPosition::RIGHT;
  engine->rootContext()->setContextProperty("RightImage", RightImage);
  engine->addImageProvider(QLatin1String("RightImage"),
                           RightImage->m_pImgProvider);

  // Panel Cam
  ShowImage *PanelImage = static_cast<ShowImage *>(ar[4]);
  PanelImage->campos = RenderPosition::DASHBOARD;
  engine->rootContext()->setContextProperty("PanelImage", PanelImage);
  engine->addImageProvider(QLatin1String("PanelImage"),
                           PanelImage->m_pImgProvider);

  //  // ForwardLeft Cam
  //  ShowImage *ForwardLeftImage = static_cast<ShowImage *>(ar[4]);
  //  ForwardLeftImage->campos = CAM_POS::FORWARD_LEFT;
  //  engine->rootContext()->setContextProperty("ForwardLeftImage",
  //                                            ForwardLeftImage);
  //  engine->addImageProvider(QLatin1String("ForwardLeftImage"),
  //                           ForwardLeftImage->m_pImgProvider);

  //  // ForwardRight Cam
  //  ShowImage *ForwardRightImage = static_cast<ShowImage *>(ar[5]);
  //  ForwardRightImage->campos = CAM_POS::FORWARD_RIGHT;
  //  engine->rootContext()->setContextProperty("ForwardRightImage",
  //                                            ForwardRightImage);
  //  engine->addImageProvider(QLatin1String("ForwardRightImage"),
  //                           ForwardRightImage->m_pImgProvider);

#ifdef Q_OS_WIN
  QString extraImportPath(QStringLiteral("%1/../../../../%2"));
#else
  QString extraImportPath(QStringLiteral("%1/../../../%2"));
#endif
  viewer.engine()->addImportPath(extraImportPath.arg(
      QGuiApplication::applicationDirPath(), QString::fromLatin1("qml")));

  QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer,
                   &QWindow::close);

  viewer.setTitle(QStringLiteral("Hyper vision remote control System"));

  viewer.setSource(QUrl("qrc:///main.qml"));

  viewer.setResizeMode(QQuickView::SizeRootObjectToView);

  QObject *item = viewer.rootObject();

  QObject::connect(item, SIGNAL(qmlSignalMaxWindow()), &viewer, SLOT(show()));

  QObject::connect(item, SIGNAL(qmlSignalQuit()), &viewer, SLOT(close()));

  viewer.setFlags(Qt::FramelessWindowHint | Qt::Window);

  viewer.showMinimized();

  return app.exec();
}

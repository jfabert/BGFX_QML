#include "bgfxitem.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

int main(int argc, char* argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGL);

  QGuiApplication app(argc, argv);
  qmlRegisterType<BgfxItem>("BGFX", 1, 0, "BgfxItem");

  QQmlApplicationEngine engine;
  const QUrl            url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject* obj, const QUrl& objUrl) {
        if ( !obj && url == objUrl )
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);
  return app.exec();
}

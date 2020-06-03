#pragma once

#include <QQuickWindow>
#include <bgfx/bgfx.h>

class BgfxRenderer : public QObject
{
  Q_OBJECT
public:
  BgfxRenderer(QQuickWindow* window);
  ~BgfxRenderer();

  void setViewportSize(const QSize& size)
  {
    m_viewportSize = size;
  }
  void setWindow(QQuickWindow* window)
  {
    m_window = window;
    m_window->setClearBeforeRendering(false);
  }

public slots:
  void init();
  void paint();

private:
  void init_api_specifics(bgfx::Init& init);

  bool          m_BgfxInitialized;
  QSize         m_viewportSize;
  QQuickWindow* m_window;
};

#include "bgfxitem.h"

#include <QRunnable>
#include <thread>

BgfxItem::BgfxItem()
  : m_renderer(nullptr)
{
  connect(
      this, &QQuickItem::windowChanged, this, &BgfxItem::handleWindowChanged);
}

// -----------------------------------------------------------------------------

BgfxItem::~BgfxItem()
{
}

// -----------------------------------------------------------------------------

void BgfxItem::handleWindowChanged(QQuickWindow* win)
{
  if ( win )
  {
    connect(
        win, &QQuickWindow::beforeSynchronizing, this, &BgfxItem::sync,
        Qt::DirectConnection);
    connect(
        win, &QQuickWindow::sceneGraphInvalidated, this, &BgfxItem::cleanup,
        Qt::DirectConnection);

    win->setColor(Qt::lightGray);
  }
}

// -----------------------------------------------------------------------------

void BgfxItem::cleanup()
{
  delete m_renderer;
  m_renderer = nullptr;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class CleanupJob : public QRunnable
{
public:
  CleanupJob(BgfxRenderer* renderer)
    : m_renderer(renderer)
  {
  }
  void run() override
  {
    delete m_renderer;
  }

private:
  BgfxRenderer* m_renderer;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void BgfxItem::releaseResources()
{
  window()->scheduleRenderJob(
      new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
  m_renderer = nullptr;
}

// -----------------------------------------------------------------------------

void BgfxItem::sync()
{
  if ( !m_renderer )
  {
    m_renderer = new BgfxRenderer(window());

    connect(
        window(), &QQuickWindow::beforeRendering, m_renderer,
        &BgfxRenderer::init);
    connect(
        window(), &QQuickWindow::beforeRenderPassRecording, m_renderer,
        &BgfxRenderer::paint);
  }
  m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
  m_renderer->setWindow(window());
}

// -----------------------------------------------------------------------------

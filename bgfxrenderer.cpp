#include "bgfxrenderer.h"

#include "bgfx/platform.h"

#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QVulkanInstance>

BgfxRenderer::BgfxRenderer(QQuickWindow* window)
  : m_BgfxInitialized(false)
  , m_window(window)
{
  m_window->setClearBeforeRendering(false);
}

// -----------------------------------------------------------------------------

BgfxRenderer::~BgfxRenderer()
{
  bgfx::shutdown();
}

// -----------------------------------------------------------------------------

void BgfxRenderer::init()
{
  if ( m_BgfxInitialized )
  {
    return;
  }

  auto winId = reinterpret_cast<void*>(m_window->winId());

  const auto width  = m_viewportSize.width();
  const auto height = m_viewportSize.height();

  const auto debug = BGFX_DEBUG_NONE;
  const auto reset = BGFX_RESET_NONE;

  bgfx::Init init;

  init_api_specifics(init);

  init.vendorId          = BGFX_PCI_ID_NONE;
  init.profile           = true;
  init.debug             = debug;
  init.resolution.width  = unsigned(width);
  init.resolution.height = unsigned(height);
  init.resolution.reset  = reset;
  init.platformData.nwh  = winId;

  bgfx::renderFrame();
  m_BgfxInitialized = bgfx::init(init);

  if ( !m_BgfxInitialized )
  {
    throw std::runtime_error("Initialization of BGFX failed");
  }

  auto w = static_cast<uint16_t>(width);
  auto h = static_cast<uint16_t>(height);

  bgfx::setViewRect(0, 0, 0, w, h);
  bgfx::setViewClear(
      0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, uint32_t(0x443355FF));
}

// -----------------------------------------------------------------------------

void BgfxRenderer::init_api_specifics(bgfx::Init& init)
{
  auto rif = m_window->rendererInterface();
  auto api = rif->graphicsApi();

  if ( api == QSGRendererInterface::GraphicsApi::OpenGLRhi
       || api == QSGRendererInterface::GraphicsApi::OpenGL )
  {
    init.type                 = bgfx::RendererType::OpenGL;
    init.platformData.context = rif->getResource(
        m_window, QSGRendererInterface::Resource::OpenGLContextResource);
  }
  else if ( api == QSGRendererInterface::GraphicsApi::Direct3D11Rhi )
  {
    init.type                 = bgfx::RendererType::Direct3D11;
    init.platformData.context = rif->getResource(
        m_window, QSGRendererInterface::Resource::DeviceResource);
  }
  else if ( api == QSGRendererInterface::GraphicsApi::VulkanRhi )
  {
      init.type                 = bgfx::RendererType::Vulkan;
      init.platformData.context = static_cast<VkDevice*>(rif->getResource(
          m_window, QSGRendererInterface::Resource::DeviceContextResource));
      //https://doc-snapshots.qt.io/qt5-dev/qsgrendererinterface.html#getResource
  }
  else
  {
    qDebug("API unknown");
    throw std::runtime_error("unkown graphics API");
  }
}

// -----------------------------------------------------------------------------

void BgfxRenderer::paint()
{
  m_window->beginExternalCommands();

  const auto width  = m_viewportSize.width();
  const auto height = m_viewportSize.height();

  auto w = static_cast<uint16_t>(width);
  auto h = static_cast<uint16_t>(height);

  bgfx::setViewRect(0, 0, 0, w, h);
  bgfx::touch(0);
  bgfx::frame();

  m_window->endExternalCommands();
}

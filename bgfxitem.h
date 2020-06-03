#pragma once

#include "bgfxrenderer.h"

#include <QQuickItem>

class BgfxItem : public QQuickItem
{
  Q_OBJECT

public:
  BgfxItem();
  ~BgfxItem();

public slots:
  void sync();
  void cleanup();

private slots:
  void handleWindowChanged(QQuickWindow* win);

private:
  void releaseResources() override;

  bool          m_BgfxInitialized;
  BgfxRenderer* m_renderer;
};

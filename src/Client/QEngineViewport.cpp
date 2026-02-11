#include <QEngineViewport.hpp>

QEngineViewportRenderer::QEngineViewportRenderer() {}
QOpenGLFramebufferObject* QEngineViewportRenderer::createFramebufferObject(
    const QSize& size) {}
void QEngineViewportRenderer::render() {}

QQuickFramebufferObject::Renderer* QEngineViewport::createRenderer() const {
  return new QEngineViewportRenderer;
}
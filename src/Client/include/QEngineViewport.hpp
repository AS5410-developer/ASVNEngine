#ifndef CLIENT_ENGINEVIEWPORT
#define CLIENT_ENGINEVIEWPORT

#include <QQuickFramebufferObject>

class QEngineViewportRenderer : public QQuickFramebufferObject::Renderer {
 public:
  QEngineViewportRenderer();
  QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
  void render() override;
  virtual ~QEngineViewportRenderer() = default;
};

class QEngineViewport : public QQuickFramebufferObject {
 public:
  Q_OBJECT
  QML_ELEMENT
  virtual Renderer* createRenderer() const override;
  virtual ~QEngineViewport() = default;
};

#endif
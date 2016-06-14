#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "Entities/Game.hpp"
#include "SceneGraph/Window.hpp"

class Window;

class Environment : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY
                 fullscreenChanged)
  Q_PROPERTY(QString system READ system CONSTANT)

  Window *m_view;

 public:
  Environment(Window *view);

  inline Window *view() const { return m_view; }

  QString system() const;

  bool fullscreen() const;
  void setFullscreen(bool);

 signals:
  void fullscreenChanged();
};

class Window : public SceneGraph::Window {
 private:
  Game m_game;
  Environment m_environment;

 protected:
  void resizeEvent(QResizeEvent *);

 public:
  Window(QWindow * = nullptr);
};

#endif  // WINDOW_HPP

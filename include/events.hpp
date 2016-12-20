#ifndef EVENTS_HPP
#define EVENTS_HPP

struct wPress { };
struct aPress { };
struct sPress { };
struct dPress { };
struct rPress { };
struct ePress { };
struct qPress { };
struct pPress { };
struct tPress { };
struct MouseEvent { const double x, y; };
struct DrawGridToggle { };
struct CursorEnterWindow { const double x, y; };
struct CursorExitWindow { const double x, y; };

#endif //EVENTS_HPP
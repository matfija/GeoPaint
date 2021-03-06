#include "crtanje.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QStyle>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "geom_graphics_view.hpp"
#include "Boje.hpp"

// Element graficke scene koji nam predstavlja primitivu (tacku)
crtanje::ctacka::ctacka()
    : odgovarajuca_tacka(geom::tacka(0.0,0.0)), boja(PODRAZUMEVANA_BOJA), kliknuto(KLIKNUTO_BOJA) {};

crtanje::ctacka::ctacka(const geom::tacka& t)
    : odgovarajuca_tacka(geom::tacka(t)), boja(PODRAZUMEVANA_BOJA), kliknuto(KLIKNUTO_BOJA) {};

crtanje::ctacka::ctacka(const geom::Element x, const geom::Element y)
    : odgovarajuca_tacka(geom::tacka(x,y)), boja(PODRAZUMEVANA_BOJA), kliknuto(KLIKNUTO_BOJA) {};


/***** OVO SAMO AKO BAS BUDE VREMENA! *****
// TODO: Dodali bismo i varijantu u kojoj korisnik moze definisati sopstvenu boju tacke
   Moze biti korisno ako korisnik zeli da prati niz geometrijskih transformacija za odredjenu tacku
crtanje::ctacka::ctacka(const geom::tacka& t, QColor marker)
    : odgovoarajuca_tacka(geom::tacka(t), boja(marker), kliknuto(marker) {};
crtanje::ctacka::ctacka(const geom::Elem x, const geom::Elem y, QColor marker)
    : odgovarajuca_tacka(geom::tacka(t)), boja(marker), kliknuto(marker) {};
*******************************************/

// Pomocna funkcija za iscrtavanje (nasledjena iz klase elementa graficke scene)
QRectF crtanje::ctacka::boundingRect() const  {
    return QRect(-15,-15,15,15);
}

// Crtanje elementa klase clinija (nasledjena iz klase elementa graficke scene)
void crtanje::ctacka::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget)

    QBrush brush;
    QPen pen;

    brush.setStyle(Qt::SolidPattern);

    if (option->state & QStyle::State_Sunken) {
        brush.setColor(kliknuto);
        pen.setColor(kliknuto);
    } else {
        brush.setColor(boja);
        pen.setColor(boja);
    }

    painter->setBrush(brush);
    painter->setPen(pen);

    painter->drawEllipse(-10, -10, 10, 10);
}

// Pomocna funkcija za detekciju kolizije (nasledjena iz klase elementa graficke scene)
QPainterPath crtanje::ctacka::shape() const {
    QPainterPath path;
    path.addEllipse(-10,-10,10,10);
    return path;
}

// Postavljamo zeljenu boju tacke
void crtanje::ctacka::setColor(QColor boja_, QColor kliknuto_) {
    boja = boja_;
    kliknuto = kliknuto_;
}

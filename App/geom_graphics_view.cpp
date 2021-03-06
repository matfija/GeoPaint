#include <vector>

#include "geom_graphics_view.hpp"
#include "crtanje.hpp"
#include "../Cons/geom.hpp"
#include "../Cons/util.hpp"

#include "mainwindow.h"
#include <sstream>

// geom_graphics_view : Nasa podklasa grafickog pogleda na scenu
geom_graphics_view::geom_graphics_view(QWidget* parent)
    : QGraphicsView(parent)
{
   // Podesavamo graficke parametre scene.
   QGraphicsScene* scena = new QGraphicsScene(this);

   scena->setSceneRect(X_BOUND_GRAPHIC_SCENE, Y_BOUND_GRAPHIC_SCENE,
                          WIDTH_GRAPHIC_SCENE, HEIGHT_GRAPHIC_SCENE);

   setScene(scena);

   // Skaliranje kako bi y osa isla nagore
   scale(1, -1);

   nacrtaj_koordinatne_ose();

   /***** Ovde samo proveravamo nase metode *****
   nacrtaj_tacku();
   nacrtaj_tacku(geom::tacka (70,80));
   geom::tacka t (100,100);
   geom::elipsa e (t,300,100);
   geom::tacka centar (300,300);
   geom::krug k (centar,200);
   geom::trans translacija (-300,-300,false);
   geom::poly p{{100, 100}, {200, 200}, {-100, 50}};
   p.zatvori();
   k.transformisi(translacija);
   nacrtaj_elipsu(e);
   nacrtaj_krug(k);
   nacrtaj_poligon(p);
   ***** ********************************** *****/
}

void geom_graphics_view::drawBackground(QPainter* painter, const QRectF& pozadina_)
{
    Q_UNUSED(pozadina_)

    QRectF pozadina = this->sceneRect();
    painter->setOpacity(0.8);
    painter->setBrush(QColor(159,189,223));


    painter->drawRect(pozadina);
    /*nacrtaj_mrezu();*/

}

void geom_graphics_view::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);

    auto sc = mapToScene(event->pos());

    std::ostringstream s;
    s << "Trenutni položaj miša: (" << sc.rx() << ", " << sc.ry() << ").";

    auto prozor = qobject_cast<MainWindow*>(parentWidget()->parentWidget());

    prozor->posalji_poruku(QString::fromStdString(s.str()));
}

void geom_graphics_view::wheelEvent(QWheelEvent* event)
{
    static auto broj = 0;
    static const auto faktor = 1.1;

    if (event->delta() > 0){
        if (broj < 12){
            scale(faktor, faktor);
            broj++;
        }
    } else if (broj > -12){
        scale(1/faktor, 1/faktor);
        broj--;
    }
}


void geom_graphics_view::nacrtaj_tacku()
{
    crtanje::ctacka* nova_tacka = new crtanje::ctacka();
    nova_tacka->setPos(0, 0);
    scene()->addItem(nova_tacka);
    sve_tacke.push_back(geom::Vektor{0, 0});
}

void geom_graphics_view::nacrtaj_tacku(const geom::tacka& t) {
    crtanje::ctacka* nova_tacka = new crtanje::ctacka(t);
    nova_tacka->setPos(t[0], t[1]);
    scene()->addItem(nova_tacka);
    sve_tacke.push_back(geom::Vektor{t[0], t[1]});
}

void geom_graphics_view::nacrtaj_liniju (const geom::tacka& pocetak, const geom::tacka& kraj) {
    crtanje::clinija* linija = new crtanje::clinija(pocetak[0], pocetak[1],
                             kraj[0], kraj[1]);
    scene()->addItem(linija);

}

void geom_graphics_view::nacrtaj_x_osu() {
    const auto x_osa = crtanje::cxosa::osa();
    scene()->addItem(x_osa);
}


void geom_graphics_view::nacrtaj_y_osu() {
    const auto y_osa = crtanje::cyosa::osa();
    scene()->addItem(y_osa);
}


// FIXME: Vidljivost mreze
void geom_graphics_view::nacrtaj_mrezu() {
    for (auto i= X_BOUND_GRAPHIC_SCENE; i<= (X_BOUND_GRAPHIC_SCENE + WIDTH_GRAPHIC_SCENE); i+= MESH_CELL)
    {
        const geom::tacka pocetak_ (i,Y_BOUND_GRAPHIC_SCENE);
        const geom::tacka kraj_   (i, (Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE));
        nacrtaj_liniju(pocetak_, kraj_);
    }

    for (auto i= Y_BOUND_GRAPHIC_SCENE; i<= (Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE); i+= MESH_CELL)
    {
        const geom::tacka pocetak_ (X_BOUND_GRAPHIC_SCENE, i);
        const geom::tacka kraj_   ((X_BOUND_GRAPHIC_SCENE + WIDTH_GRAPHIC_SCENE), i);
        nacrtaj_liniju(pocetak_, kraj_);
    }
}


void geom_graphics_view::nacrtaj_elipsu(geom::elipsa& e) {
    crtanje::celipsa* nova_elipsa = new crtanje::celipsa(e);
    scene()->addItem(nova_elipsa);
}

void geom_graphics_view::nacrtaj_krug(geom::krug& k) {
    crtanje::ckrug* novi_krug = new crtanje::ckrug(k);
    scene()->addItem(novi_krug);
}

void geom_graphics_view::nacrtaj_poligon(geom::poly& p) {
    crtanje::cpoligon* novi_krug = new crtanje::cpoligon(p);
    scene()->addItem(novi_krug);
}

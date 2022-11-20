#include "QuadTree.hpp"


float distSqrd(sf::Vector2f p1, sf::Vector2f p2) {
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);  // return squared distance
}

bool collideRectAndRect(RectByCenter rect1, RectByCenter rect2) {
    float tlx1 = rect1.center.x - rect1.radius.x;
    float tly1 = rect1.center.y - rect1.radius.y;
    float brx1 = rect1.center.x + rect1.radius.x;
    float bry1 = rect1.center.y + rect1.radius.y;

    float tlx2 = rect2.center.x - rect2.radius.x;
    float tly2 = rect2.center.y - rect2.radius.y;
    float brx2 = rect2.center.x + rect2.radius.x;
    float bry2 = rect2.center.y + rect2.radius.y;

    if ((tlx1 <= brx2) && (brx1 >= tlx2) && (tly1 <= bry2) && (bry1 >= tly2)) {
        return true;
    }
    return false;
}

void QuadTree::del() {
    points.clear();
    if (northWest != nullptr) {
        northWest->del();
        northEast->del();
        southEast->del();
        southWest->del();
    }
    delete northWest;
    delete northEast;
    delete southEast;
    delete southWest;

    northWest = nullptr;
    northEast = nullptr;
    southEast = nullptr;
    southWest = nullptr;

    return;
}

void QuadTree::display(sf::RenderTexture& window){
    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color(((int) boundary.center.x*12663)%200+50, ((int)boundary.center.y * 33851)%200+50, ((int)boundary.radius.x * 9846935) % 200 + 50));
    rect.setOutlineThickness(4);
    rect.setFillColor(sf::Color(0, 255, 0, 0));
    sf::Vector2f pos;

    pos = substractVectors(boundary.center, boundary.radius);

    rect.setPosition(pos);
    rect.setSize(multiplyVector(boundary.radius, 2));
    window.draw(rect);
    if (northWest == nullptr) {
        return;
    }
    else {
        northWest->display(window);
        northEast->display(window);
        southEast->display(window);
        southWest->display(window);
        return;
    }
}

void QuadTree::subdivide() {
    //subdivide the previous rect in 4 equal parts
    RectByCenter r;
    r.radius = multiplyVector(boundary.radius, 0.5);
    r.center.x = boundary.center.x - boundary.radius.x / 2;
    r.center.y = boundary.center.y - boundary.radius.y / 2;
    northWest = new QuadTree(r);

    r.center.x = boundary.center.x + boundary.radius.x / 2;
    r.center.y = boundary.center.y - boundary.radius.y / 2;
    northEast = new QuadTree(r);

    r.center.x = boundary.center.x + boundary.radius.x / 2;
    r.center.y = boundary.center.y + boundary.radius.y / 2;
    southEast = new QuadTree(r);

    r.center.x = boundary.center.x - boundary.radius.x / 2;
    r.center.y = boundary.center.y + boundary.radius.y / 2;
    southWest = new QuadTree(r);
};

bool QuadTree::insert(Cell* p) {
    // Insérer un point dans le QuadTree
    // Ignorer les objets qui n'appartiennent pas a ce quadtree
    if ((abs(boundary.center.x - p->getPosition().x) >= boundary.radius.x) || (abs(boundary.center.y - p->getPosition().y) >= boundary.radius.y)) {
        return false; // l'objet ne doit pas être ajoute
    }

    // S'il reste de la place dans ce quadtree, y ajouter l'objet
    if (points.size() < QT_NODE_CAPACITY && northWest == nullptr) {
        points.push_back(p);
        return true;
    }

    // Sinon, subdiviser le quadtree, puis ajouter le point au nœud qui l'acceptera
    if (northWest == nullptr) {
        subdivide();

        //distribute the points to the childs
        for (int pointMoved = 0; pointMoved < points.size(); pointMoved++) {
            if (northWest->insert(points[pointMoved])) continue;
            else if (northEast->insert(points[pointMoved])) continue;
            else if (southWest->insert(points[pointMoved])) continue;
            else if (southEast->insert(points[pointMoved])) continue;
        }

        points.clear();
    }

    if (northWest->insert(p)) { return true; }
    if (northEast->insert(p)) { return true; }
    if (southWest->insert(p)) { return true; }
    if (southEast->insert(p)) { return true; }

    // Sinon, le point ne peut etre insere, pour une raison inconnue (cela ne devrait jamais arriver)
    return false;
}

void QuadTree::queryRangeRect(RectByCenter range, std::vector<Cell*> &pointsInRange) {
    // skip if the quadtree isn't concerned
    if (!collideRectAndRect(range, boundary)) {
        return;
    }

    //if not child, return the node points
    if (northWest == nullptr) {
        for (int p = 0; p < points.size(); p++) {
            if ((abs(points.at(p)->getPosition().x - range.center.x) < range.radius.x) &&
                (abs(points.at(p)->getPosition().y - range.center.y) < range.radius.y)) {
                pointsInRange.push_back(points.at(p));
            }
        }
        return;
    }

    // if it have childs, ask them
    northWest->queryRangeRect(range, pointsInRange);
    northEast->queryRangeRect(range, pointsInRange);
    southEast->queryRangeRect(range, pointsInRange);
    southWest->queryRangeRect(range, pointsInRange);
    return;
}

void QuadTree::queryRangeCircle(RectByCenter range, std::vector<Cell*>& pointsInRange) {
    // skip if the quadtree isn't concerned
    if (!collideRectAndRect(range, boundary)) {
        return;
    }

    //if not child, return the node points
    if (northWest == nullptr) {
        for (int p = 0; p < points.size(); p++) {
            if ((abs(points.at(p)->getPosition().x - range.center.x) < range.radius.x) &&
                (abs(points.at(p)->getPosition().y - range.center.y) < range.radius.y)) {

                //in rect, check if in circle
                if (distSqrd(points[p]->getPosition(), range.center) < range.radius.x * range.radius.x + range.radius.y * range.radius.y) {
                    pointsInRange.push_back(points.at(p));
                }
            }
        }
        return;
    }

    // if it have childs, ask them
    northWest->queryRangeCircle(range, pointsInRange);
    northEast->queryRangeCircle(range, pointsInRange);
    southEast->queryRangeCircle(range, pointsInRange);
    southWest->queryRangeCircle(range, pointsInRange);
    return;
}
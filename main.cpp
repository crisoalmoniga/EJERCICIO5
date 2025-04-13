#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "ParteCuerpo.h"

#define SCALE 100.0f

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ejercicio 5 - Mario Ragdoll");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 0.0f);
    b2World world(gravity);

    // Crear el piso
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.f, 0.f);
    b2Body* ground = world.CreateBody(&groundBodyDef);

    b2EdgeShape edge;
    b2FixtureDef fixture;
    fixture.shape = &edge;

    edge.SetTwoSided(b2Vec2(0, 6), b2Vec2(8, 6));
    ground->CreateFixture(&fixture);

    // === Crear partes del cuerpo ===
    ParteCuerpo cabeza(world, 400, 200, 80, 80);        // x, y, ancho, alto
    ParteCuerpo torso(world, 400, 300, 100, 100);
    ParteCuerpo brazoIzq(world, 330, 300, 60, 20);
    ParteCuerpo brazoDer(world, 470, 300, 60, 20);
    ParteCuerpo piernaIzq(world, 370, 420, 25, 90);
    ParteCuerpo piernaDer(world, 430, 420, 25, 90);

    // === Joints ===
    b2DistanceJointDef jointDef;

    // Cabeza - Torso
    jointDef.bodyA = cabeza.getBody();
    jointDef.bodyB = torso.getBody();
    jointDef.localAnchorA.Set(0, 40 / SCALE);  // abajo de la cabeza
    jointDef.localAnchorB.Set(0, -50 / SCALE); // arriba del torso
    jointDef.length = 0.01f;
    world.CreateJoint(&jointDef);

    // Brazo Izquierdo - Torso
    jointDef.bodyA = brazoIzq.getBody();
    jointDef.bodyB = torso.getBody();
    jointDef.localAnchorA.Set(30 / SCALE, 0);     // derecha del brazo
    jointDef.localAnchorB.Set(-50 / SCALE, 0);    // izquierda del torso
    jointDef.length = 0.01f;
    world.CreateJoint(&jointDef);

    // Brazo Derecho - Torso
    jointDef.bodyA = brazoDer.getBody();
    jointDef.bodyB = torso.getBody();
    jointDef.localAnchorA.Set(-30 / SCALE, 0);     // izquierda del brazo
    jointDef.localAnchorB.Set(50 / SCALE, 0);      // derecha del torso
    jointDef.length = 0.01f;
    world.CreateJoint(&jointDef);

    // Pierna Izquierda - Torso
    jointDef.bodyA = piernaIzq.getBody();
    jointDef.bodyB = torso.getBody();
    jointDef.localAnchorA.Set(0, -45 / SCALE);    // arriba de la pierna
    jointDef.localAnchorB.Set(-20 / SCALE, 50 / SCALE); // abajo izquierda del torso
    jointDef.length = 0.01f;
    world.CreateJoint(&jointDef);

    // Pierna Derecha - Torso
    jointDef.bodyA = piernaDer.getBody();
    jointDef.bodyB = torso.getBody();
    jointDef.localAnchorA.Set(0, -45 / SCALE);    // arriba de la pierna
    jointDef.localAnchorB.Set(20 / SCALE, 50 / SCALE); // abajo derecha del torso
    jointDef.length = 0.01f;
    world.CreateJoint(&jointDef);

    // Dibujar joints
    auto drawJoint = [&](b2Body* a, b2Body* b) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(a->GetPosition().x * SCALE, a->GetPosition().y * SCALE), sf::Color::Green),
            sf::Vertex(sf::Vector2f(b->GetPosition().x * SCALE, b->GetPosition().y * SCALE), sf::Color::Green)
        };
        window.draw(line, 2, sf::Lines);
        };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(1.f / 60.f, 8, 3);

        window.clear();

        cabeza.draw(window);
        torso.draw(window);
        brazoIzq.draw(window);
        brazoDer.draw(window);
        piernaIzq.draw(window);
        piernaDer.draw(window);

        drawJoint(cabeza.getBody(), torso.getBody());
        drawJoint(brazoIzq.getBody(), torso.getBody());
        drawJoint(brazoDer.getBody(), torso.getBody());
        drawJoint(piernaIzq.getBody(), torso.getBody());
        drawJoint(piernaDer.getBody(), torso.getBody());

        window.display();
    }

    return 0;
}
#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <iostream>
#include "particle.h"
#include "geometry.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setVersion(3, 1);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);


//    float dt = 0.01f;  //simulation time
//    float tini = 0.0f;
//    float tfinal = 6.0f; //final time of simulation
//    // particle inicialization
//    Particle p(0.0f, 5.0f, 0.0f); //initial position of the particle

//    p.setLifetime(5.0f);
//    std::cout << "Lifetime =" << p.getLifetime() << std::endl;
//    p.setBouncing(0.8f);
//    p.addForce(0, -9.8f, 0);
////	p.setFixed(true);

//    // define a floor plane for collision
//    glm::vec3 punt(0.0f);
//    glm::vec3 normal(0, 1, 0);
//    Plane plane(punt, normal);
//    // simulation loop
//    int count = 0;
//    float disact, disant;
//    disact = plane.distPoint2Plane(p.getCurrentPosition());
//    float time = tini;
//    while (time <= tfinal) {
//        if (p.getLifetime() > 0) {
//            p.updateParticle(dt, Particle::UpdateMethod::EulerOrig);
//            std::cout << "position = " << p.getCurrentPosition().x << "  " << p.getCurrentPosition().y
//                << "  " << p.getCurrentPosition().z << "  time = " << time << std::endl;
//            //Check for floor collisions
//            disant = disact;
//            p.setLifetime(p.getLifetime() - dt);
//            disact = plane.distPoint2Plane(p.getCurrentPosition());
//            if (disant*disact < 0.0f) {
//                //VERY IMPORTANT: only valid for the plane y=0 (floor plane)
//                //Must be addapted to a general plane,
//                p.setPosition(p.getCurrentPosition().x, -p.getCurrentPosition().y, p.getCurrentPosition().z);
//                p.setVelocity(p.getVelocity().x, -p.getVelocity().y, p.getVelocity().z);
//                std::cout << "Bounce = " << count++ << std::endl;
//                disact = -disact; //
//                system("PAUSE");
//            }
//        }
//        time = time + dt; //increase time counter
//    }
//    system("PAUSE");


	w.show();

	return a.exec();
}

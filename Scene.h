#ifndef SCENE_H
#define SCENE_H

// Définition de la classe Scene

#include <gl-matrix.h>
#include <vector>

#include "Light.h"

#include "Duck.h"
#include "Ground.h"

#include "Communication.h"

class Scene
{
private:

    //Client réseau
    Communication::Client client;

    // objets de la scène
    std::vector<Duck*> ducks;
    Ground* m_Ground;

    // lampes
    Light* m_Light;

    // matrices de transformation des objets de la scène
    mat4 m_MatP;
    mat4 m_MatV;
    mat4 m_MatVM;
    mat4 m_MatTMP;

    // caméra table tournante
    float m_Azimut;
    float m_Elevation;
    float m_Distance;
    vec3 m_Center;

    // souris
    bool m_Clicked;
    double m_MousePrecX;
    double m_MousePrecY;


public:

    /** constructeur, crée les objets 3D à dessiner */
    Scene();

    /** destructeur, libère les ressources */
    ~Scene();

    /**
     * appelée quand la taille de la vue OpenGL change
     * @param width : largeur en nombre de pixels de la fenêtre
     * @param height : hauteur en nombre de pixels de la fenêtre
     */
    void onSurfaceChanged(int width, int height);


    /**
     * appelée quand on enfonce un bouton de la souris
     * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
     * @param x coordonnée horizontale relative à la fenêtre
     * @param y coordonnée verticale relative à la fenêtre
     */
    void onMouseDown(int btn, double x, double y);

    /**
     * appelée quand on relache un bouton de la souris
     * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
     * @param x coordonnée horizontale relative à la fenêtre
     * @param y coordonnée verticale relative à la fenêtre
     */
    void onMouseUp(int btn, double x, double y);

    /**
     * appelée quand on bouge la souris
     * @param x coordonnée horizontale relative à la fenêtre
     * @param y coordonnée verticale relative à la fenêtre
     */
    void onMouseMove(double x, double y);

    /**
     * appelée quand on appuie sur une touche du clavier
     * @param code : touche enfoncée
     */
    void onKeyDown(unsigned char code);

    /** Dessine l'image courante */
    void onDrawFrame();

    /**
     * @brief Initialise un canard
     *
     */
    void createDuck(int, float, float, float, float, float, float);

    /**
     * @brief Met à jour les canards
     *
     */
    void updateDucks(mat4&, vec4&);

    /**
     * @brief Dessine les canards à l'écran
     *
     */
    void drawDucks();

    /**
     * @brief Libère l'espace mémoires alloué au canards
     *
     */
    void destroyDucks();

    /**
     * @brief Verifie la queue de communication avec le thread client pour la création de canards
     *
     */
    void handleDuckCreationRequest();

    /**
     * @brief Demande au thread reseau l'envoi d'un message de canard trouvé au serveur
     *
     */
    void sendDuckFoundMessage(int);
};

#endif

#include <iostream>
#include "SFML/Graphics.hpp"
#include "Button.h"
#include "Textbox.h"
#include "SFML/Audio.hpp"
#include <fstream>;
#include <cstring>

using namespace sf;
using namespace std;

//Variables
bool isPressed;
float pozx, pozy;
int piesa_aleasa;
int nr_piese_noi = 0;
int last_piesa1, last_piesa2;
int rama_x1 = 150, rama_x2 = 1616, rama_y1 = 100, rama_y2 = 1000;
int v[200];

int nr_nod = 0;
int nod1, nod2;
int nr_linii = 0;
int l1[1001], l2[1001];
int nr_pasi = 0;
int nr_pasi_redo = 0;

int nr_nod_special = 0;
//Window
sf::RenderWindow* window;
sf::VideoMode videoMode;
sf::Event ev;

//Mouse positions
sf::Vector2i mousePosWindow;

//Game objects
struct piese{
    sf::Texture texturaPiesa;
    sf::RectangleShape piesa;
    float x1, x2, y1, y2;
    int nr_piesa;
}p[13];

struct piese_noi {
    int tip;
    sf::RectangleShape piese_noi;
    float x1, x2, y1, y2;
    float size_x, size_y;

    int nr_rotatii = 1;
    float nod_x1, nod_x2, nod_y1, nod_y2;
    sf::CircleShape nod;
    int nrNodSpecial;
    int nrPiesa;
    //edit
    string  Nume = "", Voltaj = "", Rezistenta = "";

}p_n[250];

struct piesa_al {
    sf::RectangleShape square;
}p_a;

struct size {
    sf::Texture texturaSize;
    sf::CircleShape cs;
    float x1, x2, y1, y2;

}cs[5];
struct rama_button
{
    sf::RectangleShape square;
}s[13];
struct savedata {
    double rx, ry;
    double  rnodx1, rnodx2, rnody1, rnody2,rnodx3,rnody3;
    double rwidht, rheight;
    double rtip;
    double rnr_rotatii;
    int rnr_linii,rnrNoduri;
    int rnr_nod_special,rnr_piesa;
    double rl1[1001], rl2[1001];
};
struct undo {
    int tip_actiune;
    int nr_piesa;
    float x, y;
}pas[1001];

struct redo {
    int tip_actiune;
    int nr_piesa;
    float x, y;
}redo[1001];


//function
void updateMousePos()
{
    mousePosWindow = sf::Mouse::getPosition(*window);
}
void initpiesa()
{
    for (int i = 0; i < 9; i++)
    {
        p[i].piesa.setPosition(3, 75 * i + 225);
        p[i].piesa.setSize(sf::Vector2f(90.f, 75.f));
        p[i].piesa.setOutlineColor(sf::Color(147,8,253));
        p[i].piesa.setOutlineThickness(2.5);
        p[i].x1 = 3;
        p[i].x2 = 90;
        p[i].y1 = 75 * i + 225;
        p[i].y2 = p[i].y1 + 75;
        p[i].nr_piesa = i;
    }
    p[0].texturaPiesa.loadFromFile("Image/electrical_cell_paint.png");
    p[1].texturaPiesa.loadFromFile("Image/resistor_paint.png");
    p[2].texturaPiesa.loadFromFile("Image/bulb_paint.png");
    p[3].texturaPiesa.loadFromFile("Image/diode_paint.png");
    p[4].texturaPiesa.loadFromFile("Image/ammeter_paint.png");
    p[5].texturaPiesa.loadFromFile("Image/voltmeter_paint.png");
    p[6].texturaPiesa.loadFromFile("Image/capacitor_paint.png");
    p[7].texturaPiesa.loadFromFile("Image/transistor_paint.png");
    p[8].texturaPiesa.loadFromFile("Image/fuse_paint.png");
    for(int i=0; i<9; i++)
        p[i].piesa.setTexture(&p[i].texturaPiesa);

    //size --
    cs[1].cs.setPosition(1920 - 225, 1080 - 110);
    cs[1].x1 = 1920 - 225;
    cs[1].x2 = cs[1].x1 + 50;   
    cs[1].y1 = 1080 - 110;
    cs[1].y2 = cs[1].y1 + 50;

    //size ++
    cs[2].cs.setPosition(1920 - 150, 1080 - 110);
    cs[2].x1 = 1920 - 150;
    cs[2].x2 = cs[2].x1 + 50;
    cs[2].y1 = 1080 - 110;
    cs[2].y2 = cs[2].y1 + 50;

    //delete
    cs[3].cs.setPosition(515, 1040);
    cs[3].x1 = 515;
    cs[3].x2 = cs[3].x1 + 250;
    cs[3].y1 = 1040;
    cs[3].y2 = cs[3].y1 + 35;

    //rotate
    cs[4].cs.setPosition(265, 1040);
    cs[4].x1 =  265;
    cs[4].x2 = cs[4].x1 + 250;
    cs[4].y1 = 1040;
    cs[4].y2 = cs[4].y1 + 35;

    for (int i = 1; i <= 2; i++)
    {
        cs[i].cs.setRadius(28);
        cs[i].cs.setOutlineColor(sf::Color(147, 8, 253));
        cs[i].cs.setOutlineThickness(2.5);
    }
    cs[1].texturaSize.loadFromFile("Image/Zoom_Out.png");
    cs[2].texturaSize.loadFromFile("Image/Zoom_In.png");
    for (int i = 1; i < 3; i++)
        cs[i].cs.setTexture(&cs[i].texturaSize);
}
void spawnP(int x)
{
    nr_piese_noi++;
   
    nr_pasi++;
    pas[nr_pasi].tip_actiune = 2;
    pas[nr_pasi].nr_piesa = nr_piese_noi;

    pozx = rand() % (1920 - 600) + 150;
    pozy = rand() % (1080 - 400) + 150;
    p_n[nr_piese_noi].x1 = pozx;
    p_n[nr_piese_noi].y1 = pozy;
    p_n[nr_piese_noi].y2 = pozy + 75;
    p_n[nr_piese_noi].x2 = pozx + 90;
    p_n[nr_piese_noi].size_x = 90;
    p_n[nr_piese_noi].size_y = 75;
    p_n[nr_piese_noi].tip = x;

    p_n[nr_piese_noi].piese_noi.setPosition(pozx + 45, pozy + 37.5);
    p_n[nr_piese_noi].piese_noi.setSize(sf::Vector2f(90.f, 75.f));

    p_n[nr_piese_noi].piese_noi.setOutlineColor(sf::Color::Black);
    p_n[nr_piese_noi].piese_noi.setOutlineThickness(1.f);
    p_n[nr_piese_noi].piese_noi.setOrigin(45.f, 37.5f);
    p_n[nr_piese_noi].piese_noi.setTexture(&p[x].texturaPiesa);
    
    p_n[nr_piese_noi].nrPiesa = x;
    
    if (x != 7)
    {
        nr_nod = nr_nod + 2; //
        p_n[nr_piese_noi].nrNodSpecial = nr_nod_special;
        //nodul 1
        p_n[nr_nod - 1 ].nod_x1 = pozx - 10;
        p_n[nr_nod - 1 ].nod_x2 = pozx + 10;
        p_n[nr_nod - 1 ].nod_y1 = pozy + 37.5 - 10;
        p_n[nr_nod - 1 ].nod_y2 = pozy + 37.5 + 10;
        //nodul 2
        p_n[nr_nod ].nod_x1 = pozx + 90 - 10;
        p_n[nr_nod ].nod_x2 = pozx + 90 + 10;
        p_n[nr_nod ].nod_y1 = pozy + 37.5 - 10;
        p_n[nr_nod ].nod_y2 = pozy + 37.5 + 10;

        p_n[nr_nod - 1 ].nod.setPosition(pozx, pozy + 37.5);
        p_n[nr_nod - 1 ].nod.setRadius(7);
        p_n[nr_nod - 1 ].nod.setFillColor(sf::Color(147, 8, 253));
        p_n[nr_nod - 1 ].nod.setOutlineThickness(1.5);

        p_n[nr_nod ].nod.setPosition(pozx + 90, pozy + 37.5);
        p_n[nr_nod ].nod.setRadius(7);
        p_n[nr_nod ].nod.setFillColor(sf::Color(147, 8, 253));
        p_n[nr_nod ].nod.setOutlineThickness(1.5);

        p_n[nr_nod - 1 ].nod.setOrigin(7, 7);
        p_n[nr_nod ].nod.setOrigin(7, 7);
    }
    else
    {
        nr_nod = nr_nod + 3; 
        nr_nod_special++;
        p_n[nr_piese_noi].nrNodSpecial= nr_nod_special;
        //nodul 1  
        p_n[nr_nod - 2].nod_x1 = pozx - 10;
        p_n[nr_nod - 2].nod_x2 = pozx + 10;
        p_n[nr_nod - 2].nod_y1 = pozy + 37.5 - 10;
        p_n[nr_nod - 2].nod_y2 = pozy + 37.5 + 10;

        //nodul 2
        p_n[nr_nod - 1].nod_x1 = pozx + 62 - 10;
        p_n[nr_nod - 1].nod_x2 = pozx + 62 + 10;
        p_n[nr_nod - 1].nod_y1 = pozy  - 10;
        p_n[nr_nod -1].nod_y2 = pozy  + 10;

        //nodul 3
        p_n[nr_nod].nod_x1 = pozx + 62 - 10;
        p_n[nr_nod].nod_x2 = pozx + 62 + 10;
        p_n[nr_nod].nod_y1 = pozy + 75 - 10;
        p_n[nr_nod].nod_y2 = pozy  + 75 + 10;

        p_n[nr_nod - 2].nod.setPosition(pozx, pozy + 37.5);
        p_n[nr_nod - 2].nod.setRadius(7);
        p_n[nr_nod - 2].nod.setFillColor(sf::Color(147, 8, 253));
        p_n[nr_nod - 2].nod.setOutlineThickness(1.5);

        p_n[nr_nod - 1].nod.setPosition(pozx + 62, pozy);
        p_n[nr_nod -1].nod.setRadius(7);
        p_n[nr_nod -1].nod.setFillColor(sf::Color(147, 8, 253));
        p_n[nr_nod -1].nod.setOutlineThickness(1.5);

        p_n[nr_nod].nod.setPosition(pozx + 62, pozy + 75);
        p_n[nr_nod].nod.setRadius(7);
        p_n[nr_nod].nod.setFillColor(sf::Color(147, 8, 253));
        p_n[nr_nod].nod.setOutlineThickness(1.5);

        p_n[nr_nod - 2].nod.setOrigin(7, 7);
        p_n[nr_nod - 1].nod.setOrigin(7, 7);
        p_n[nr_nod ].nod.setOrigin(7, 7);
    }
   
}
void set_size1(int x)
{
    if (p_n[x].size_x > 60)
    {
        p_n[x].size_x = p_n[x].size_x - 10;
        p_n[x].size_y = p_n[x].size_y - 10;
        p_n[x].x1 = p_n[x].x1 + 5;
        p_n[x].y1 = p_n[x].y1 + 5;
        p_n[x].y2 = p_n[x].y2 - 5;
        p_n[x].x2 = p_n[x].x2 - 5;
        if (p_n[x].nr_rotatii % 2) {
            p_n[x].piese_noi.setOrigin(p_n[x].size_x / 2, p_n[x].size_y / 2);
            p_n[x].piese_noi.setSize(sf::Vector2f(p_n[x].size_x, p_n[x].size_y));//
        }
        else
        {
            p_n[x].piese_noi.setOrigin(p_n[x].size_y / 2, p_n[x].size_x / 2);
            p_n[x].piese_noi.setSize(sf::Vector2f(p_n[x].size_y, p_n[x].size_x));
        }
        if (p_n[x].nrPiesa != 7)
        {
            if (p_n[x].nr_rotatii % 4 == 1)
            {
                p_n[x*2+p_n[x].nrNodSpecial- 1 ].nod.setPosition(p_n[x].x1, p_n[x].y1 + p_n[x].size_y / 2);
                p_n[x*2+p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2, p_n[x].y1 + p_n[x].size_y / 2);

                p_n[x*2+p_n[x].nrNodSpecial   - 1 ].nod_x1 = p_n[x].x1 - 10;
                p_n[x*2+p_n[x].nrNodSpecial   - 1 ].nod_x2 = p_n[x].x1 + 10;
                p_n[x*2+p_n[x].nrNodSpecial   ].nod_x1 = p_n[x].x2 - 10;
                p_n[x*2+p_n[x].nrNodSpecial   ].nod_x2 = p_n[x].x2 + 10;
            }
            if (p_n[x].nr_rotatii % 4 == 2)
            {
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1 + p_n[x].size_y);
                p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1);

                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y1 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y1 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y2 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = p_n[x].y2 + 10;

            }
            if (p_n[x].nr_rotatii % 4 == 3)
            {
                p_n[x*2+p_n[x].nrNodSpecial   - 1].nod.setPosition(p_n[x].x2, p_n[x].y1 + p_n[x].size_y / 2);
                p_n[x*2+p_n[x].nrNodSpecial   ].nod.setPosition(p_n[x].x1, p_n[x].y1 + p_n[x].size_y / 2);

                p_n[x*2+p_n[x].nrNodSpecial   - 1 ].nod_x1 = p_n[x].x2 - 10;
                p_n[x*2+p_n[x].nrNodSpecial   - 1 ].nod_x2 = p_n[x].x2 + 10;
                p_n[x*2+p_n[x].nrNodSpecial   ].nod_x1 = p_n[x].x1 - 10;
                p_n[x*2+p_n[x].nrNodSpecial   ].nod_x2 = p_n[x].x1 + 10;

            }
            if (p_n[x].nr_rotatii % 4 == 0)
            {
                p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1 + p_n[x].size_y);
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1);

                p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y1 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = p_n[x].y1 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y2 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y2 + 10;

            }
        }
        else
        {
            if (p_n[x].nr_rotatii % 4 == 1)
            {
                p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1, p_n[x].y1 + p_n[x].size_y / 2);
                p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2-(p_n[x].size_x/4)-10, p_n[x].y1);
                p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2 - (p_n[x].size_x / 4)-10, p_n[x].y2);

                p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x1 - 10;
                p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x1 + 10;
                p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x2 - (p_n[x].size_x / 4) -10 - 10;
                p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x2 - (p_n[x].size_x / 4) - 10 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x2 - (p_n[x].size_x / 4) - 10 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x2 - (p_n[x].size_x / 4) - 10 - 10;
            }
            if (p_n[x].nr_rotatii % 4 == 2)
            {
                p_n[x * 2 + p_n[x].nrNodSpecial -2].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1);
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x2, p_n[x].y2 -(p_n[x].size_y/4)-10);
                p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1, p_n[x].y2 - (p_n[x].size_y / 4)-10);

                p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y1 = p_n[x].y1 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y2 = p_n[x].y1 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y2 - (p_n[x].size_y / 4) - 10 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y2 - (p_n[x].size_y / 4) - 10 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y2 - (p_n[x].size_y / 4) - 10 - 10;
                p_n[x * 2+ p_n[x].nrNodSpecial].nod_y2 = p_n[x].y2 - (p_n[x].size_y / 4) - 10 + 10;

            }
            if (p_n[x].nr_rotatii % 4 == 3)
            {
                p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2, p_n[x].y2 - p_n[x].size_y / 2);
                p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + (p_n[x].size_x/4)+10, p_n[x].y2);
                p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + (p_n[x].size_x/4)+10, p_n[x].y1);

                p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x2 - 10;
                p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x2 + 10;
                p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x1 + (p_n[x].size_x / 4) + 10 - 10;
                p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x1 + (p_n[x].size_x / 4) + 10 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x1 + (p_n[x].size_x / 4) + 10 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x1 + (p_n[x].size_x / 4) + 10 + 10;
            }
            if (p_n[x].nr_rotatii % 4 == 0)
            {
                p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y2);
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x1, p_n[x].y1 + (p_n[x].size_y / 4)+10);
                p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2, p_n[x].y1 + (p_n[x].size_y / 4) + 10);

                p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y1 = p_n[x].y2 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y2 = p_n[x].y2 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y1 + (p_n[x].size_y / 4) + 10 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y1 + (p_n[x].size_y / 4) + 10 + 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y1 + (p_n[x].size_y / 4) + 10 - 10;
                p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = p_n[x].y1 + (p_n[x].size_y / 4) + 10 + 10;
            }
        }
    }
  

}
void set_size2(int x)
{
    if (p_n[x].size_x < 140)
        if (p_n[x].x2 + 5 < rama_x2 && p_n[x].y2 + 5 < rama_y2 && p_n[x].x1 - 5 > rama_x1 && p_n[x].y1 - 5 > rama_y1)
        {
            p_n[x].size_x = p_n[x].size_x + 10;
            p_n[x].size_y = p_n[x].size_y + 10;
            p_n[x].x1 = p_n[x].x1 - 5;
            p_n[x].y1 = p_n[x].y1 - 5;
            p_n[x].y2 = p_n[x].y2 + 5;
            p_n[x].x2 = p_n[x].x2 + 5;
            if (p_n[x].nr_rotatii % 2) {
                p_n[x].piese_noi.setOrigin(p_n[x].size_x / 2, p_n[x].size_y / 2);
                p_n[x].piese_noi.setSize(sf::Vector2f(p_n[x].size_x, p_n[x].size_y));
            }
            else
            {
                p_n[x].piese_noi.setOrigin(p_n[x].size_y / 2, p_n[x].size_x / 2);
                p_n[x].piese_noi.setSize(sf::Vector2f(p_n[x].size_y, p_n[x].size_x));

            }
            if (p_n[x].nrPiesa != 7)
            {
                if (p_n[x].nr_rotatii % 4 == 1)
                {
                    p_n[x*2+p_n[x].nrNodSpecial   - 1].nod.setPosition(p_n[x].x1, p_n[x].y1 + p_n[x].size_y / 2);
                    p_n[x*2+p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2, p_n[x].y1 + p_n[x].size_y / 2);

                    p_n[x*2+p_n[x].nrNodSpecial   - 1].nod_x1 = p_n[x].x1 - 10;
                    p_n[x*2+p_n[x].nrNodSpecial   - 1].nod_x2 = p_n[x].x1 + 10;
                    p_n[x*2+p_n[x].nrNodSpecial].nod_x1 = p_n[x].x2 - 10;
                    p_n[x*2+p_n[x].nrNodSpecial].nod_x2 = p_n[x].x2 + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 2)
                {
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1 + p_n[x].size_y);
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1);

                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y1 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y1 + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y2 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = p_n[x].y2 + 10;

                }
                if (p_n[x].nr_rotatii % 4 == 3)
                {
                    p_n[x*2+p_n[x].nrNodSpecial   - 1].nod.setPosition(p_n[x].x2, p_n[x].y1 + p_n[x].size_y / 2);
                    p_n[x*2+p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1, p_n[x].y1 + p_n[x].size_y / 2);

                    p_n[x*2+p_n[x].nrNodSpecial   - 1].nod_x1 = p_n[x].x2 - 10;
                    p_n[x*2+p_n[x].nrNodSpecial   - 1].nod_x2 = p_n[x].x2 + 10;
                    p_n[x*2+p_n[x].nrNodSpecial].nod_x1 = p_n[x].x1 - 10;
                    p_n[x*2+p_n[x].nrNodSpecial].nod_x2 = p_n[x].x1 + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 0)
                {
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1 + p_n[x].size_y);
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1);

                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y1 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = p_n[x].y1 + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y2 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y2 + 10;

                }
            }
            else
            {
                if (p_n[x].nr_rotatii % 4 == 1)
                {
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1, p_n[x].y1 + p_n[x].size_y / 2);
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2 - (p_n[x].size_x/4)-10, p_n[x].y1);
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2 - (p_n[x].size_x/4)-10, p_n[x].y2);

                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x1 + 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x1 + 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].size_x - (p_n[x].size_x/4) + 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].size_x - (p_n[x].size_x/4) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x2 - (p_n[x].size_x/4) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x2 - (p_n[x].size_x/4) + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 2)
                {
                    p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y1);
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x2, p_n[x].y2 - (p_n[x].size_y / 4)-10);
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1, p_n[x].y2 - (p_n[x].size_y / 4)-10);

                    p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y1 = p_n[x].y1 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y2 = p_n[x].y1 + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y1 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y1 + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y2 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = p_n[x].y2 + 10;

                }
                if (p_n[x].nr_rotatii % 4 == 3)
                {
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2, p_n[x].y2 - p_n[x].size_y / 2);
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + (p_n[x].size_x/4)+10, p_n[x].y2);
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x1 + (p_n[x].size_x/4)+10, p_n[x].y1);

                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x2 + 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x2 + 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x1 + (p_n[x].size_x/4) + 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x1 + (p_n[x].size_x/4) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = p_n[x].x1 + (p_n[x].size_x/4) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = p_n[x].x1 + (p_n[x].size_x/4) + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 0)
                {
                    p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod.setPosition(p_n[x].x1 + p_n[x].size_x / 2, p_n[x].y2);
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod.setPosition(p_n[x].x1, p_n[x].y1 + (p_n[x].size_y / 4)+10);
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(p_n[x].x2, p_n[x].y1 + (p_n[x].size_y / 4)+10);

                    p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y1 = p_n[x].y1 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 2].nod_y2 = p_n[x].y1 + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y1 = p_n[x].y1 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial - 1].nod_y2 = p_n[x].y1 + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = p_n[x].y2 - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = p_n[x].y2 + 10;
                }
            }
        } 
     
}
void rotate(int x,int grade)
{
     p_n[x].piese_noi.rotate(grade);
 
    float centrux = p_n[x].x2 - p_n[x].size_x / 2;
    float centruy = p_n[x].y2 - p_n[x].size_y / 2;
    float aux = p_n[x].size_x;
    p_n[x].size_x = p_n[x].size_y;
    p_n[x].size_y = aux;
 
    if (p_n[x].nr_rotatii % 2)
    {
        p_n[x].x1 = centrux - p_n[x].size_x / 2;
        p_n[x].x2 = centrux + p_n[x].size_x / 2;
        p_n[x].y1 = centruy - p_n[x].size_y / 2;
        p_n[x].y2 = centruy + p_n[x].size_y / 2;
    }
    else
    {
        p_n[x].x1 = centrux - p_n[x].size_x / 2;
        p_n[x].x2 = centrux + p_n[x].size_x / 2;
        p_n[x].y1 = centruy - p_n[x].size_y / 2;
        p_n[x].y2 = centruy + p_n[x].size_y / 2;
    }
 
    if (p_n[x].nrPiesa != 7)
    {
        if (p_n[x].nr_rotatii % 4 == 1)
        {
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod.setPosition(centrux, centruy - p_n[x].size_y / 2);
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod.setPosition(centrux, centruy + p_n[x].size_y / 2);
        
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x1 = centrux - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x2 = centrux + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y1 = centruy - p_n[x].size_y / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y2 = centruy - p_n[x].size_y / 2 + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x1 = centrux - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x2 = centrux + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_y1 = centruy + p_n[x].size_y / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_y2 = centruy + p_n[x].size_y / 2 + 10;
        }
        if (p_n[x].nr_rotatii % 4 == 2)
        {
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod.setPosition(centrux + p_n[x].size_x / 2, centruy);
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod.setPosition(centrux - p_n[x].size_x / 2, centruy);
 
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x1 = centrux + p_n[x].size_x / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x2 = centrux + p_n[x].size_x / 2 + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y1 = centruy - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y2 = centruy + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x1 = centrux - p_n[x].size_x / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x2 = centrux - p_n[x].size_x / 2 + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_y1 = centruy - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_y2 = centruy + 10;
        }
        if (p_n[x].nr_rotatii % 4 == 3)
        {
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod.setPosition(centrux, centruy + p_n[x].size_y / 2);
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod.setPosition(centrux, centruy - p_n[x].size_y / 2);
 
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x1 = centrux - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x2 = centrux + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y1 = centruy + p_n[x].size_y / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y2 = centruy + p_n[x].size_y / 2 + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x1 = centrux - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x2 = centrux + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_y1 = centruy - p_n[x].size_y / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_y2 = centruy - p_n[x].size_y / 2 + 10;
        }
        if (p_n[x].nr_rotatii % 4 == 0)
        {
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod.setPosition(centrux - p_n[x].size_x / 2, centruy);
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod.setPosition(centrux + p_n[x].size_x / 2, centruy);
 
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x1 = centrux - p_n[x].size_x / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_x2 = centrux - p_n[x].size_x / 2 + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y1 = centruy - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial  - 1  ].nod_y2 = centruy + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x1 = centrux + p_n[x].size_x / 2 - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_x2 = centrux + p_n[x].size_x / 2 + 10;
            p_n[x * 2   +p_n[x].nrNodSpecial    ].nod_y1 = centruy - 10;
            p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y2 = centruy + 10;
        }
    }
    else
    {
        if (p_n[x].nr_rotatii % 4 == 1)
        {
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux , centruy - (p_n[x].size_y / 2));
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(centrux + (p_n[x].size_x / 2) , centruy + (p_n[x].size_y / 5));
            p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux - (p_n[x].size_x / 2), centruy + (p_n[x].size_y / 5));
 
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = centrux  - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = centrux  + 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = centruy - (p_n[x].size_y / 2) - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = centruy - (p_n[x].size_y / 2) + 10;
 
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = centrux + (p_n[x].size_x / 2) - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = centrux + (p_n[x].size_x / 2) + 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = centruy + (p_n[x].size_y / 5) - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = centruy + (p_n[x].size_y / 5) + 10;
 
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = centrux - (p_n[x].size_x / 2) - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = centrux - (p_n[x].size_x / 2) + 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = centruy + (p_n[x].size_y / 5) - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = centruy + (p_n[x].size_y / 5) + 10;
        }
        if (p_n[x].nr_rotatii % 4 == 2)
        {
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux + (p_n[x].size_x / 2), centruy);
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(centrux - (p_n[x].size_x / 5), centruy + (p_n[x].size_y) / 2);
            p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux - (p_n[x].size_x / 5), centruy - (p_n[x].size_y) / 2);
 
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = centrux + (p_n[x].size_x / 2) - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = centrux + (p_n[x].size_x / 2) + 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = centruy  - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = centruy  + 10;
 
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = centrux - (p_n[x].size_x / 5) - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = centrux - (p_n[x].size_x / 5) + 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = centruy + (p_n[x].size_y / 2) - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = centruy + (p_n[x].size_y / 2) + 10;
 
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = centrux - (p_n[x].size_x / 5) - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = centrux - (p_n[x].size_x / 5) + 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = centruy - (p_n[x].size_y / 2) - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = centruy - (p_n[x].size_y / 2) + 10;
        }
        if (p_n[x].nr_rotatii % 4 == 3)
        {
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux, centruy + (p_n[x].size_y / 2));
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(centrux - (p_n[x].size_x / 2), centruy - (p_n[x].size_y / 5));
            p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux + (p_n[x].size_x / 2), centruy - (p_n[x].size_y / 5));
 
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = centrux  - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = centrux  + 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = centruy + (p_n[x].size_y / 2) - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = centruy + (p_n[x].size_y / 2) + 10;
 
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = centrux - (p_n[x].size_x / 2) - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = centrux - (p_n[x].size_x / 2) + 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = centruy - (p_n[x].size_y / 5) - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = centruy - (p_n[x].size_y / 5) + 10;
 
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = centrux + (p_n[x].size_x / 2) - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = centrux + (p_n[x].size_x / 2) + 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = centruy - (p_n[x].size_y / 5) - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = centruy - (p_n[x].size_y / 5) + 10;
        }
        if (p_n[x].nr_rotatii % 4 == 0)
        {
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux - (p_n[x].size_x / 2), centruy );
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(centrux + (p_n[x].size_x / 5), centruy - (p_n[x].size_y) / 2);
            p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(centrux + (p_n[x].size_x / 5), centruy + (p_n[x].size_y) / 2);
 
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = centrux - (p_n[x].size_x / 2) - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = centrux - (p_n[x].size_x / 2) + 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = centruy  - 10;
            p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = centruy  + 10;
 
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = centrux + (p_n[x].size_x / 5) - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = centrux + (p_n[x].size_x / 5) + 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = centruy - (p_n[x].size_y) / 2 - 10;
            p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = centruy - (p_n[x].size_y) / 2 + 10;
 
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = centrux + (p_n[x].size_x / 5) - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = centrux + (p_n[x].size_x / 5) + 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = centruy + (p_n[x].size_y) / 2 - 10;
            p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = centruy + (p_n[x].size_y) / 2 + 10;
        }
    }
    p_n[x].nr_rotatii++;
}
void delete_p(int x)
{
    p_n[x].piese_noi.setPosition(2000, 2000);
    p_n[x].x1 = 2000;
    p_n[x].x2 = 2000;
    p_n[x].y1 = 2000;
    p_n[x].y2 = 2000;
    last_piesa2 = 0;
    if (p_n[x].nrPiesa != 7)
    {
        p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod.setPosition(2000, 2000);
        p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x1 = 2000;
        p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x2 = 2000;
        p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y1 = 2000;
        p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y2 = 2000;
        p_n[x * 2   +p_n[x].nrNodSpecial ] .nod.setPosition(2000, 2000);
        p_n[x * 2   +p_n[x].nrNodSpecial ] .nod_x1 = 2000;
        p_n[x * 2   +p_n[x].nrNodSpecial ] .nod_x2 = 2000;
        p_n[x * 2   +p_n[x].nrNodSpecial ] .nod_y1 = 2000;
        p_n[x * 2   +p_n[x].nrNodSpecial ] .nod_y2 = 2000;
    }
    else
    {
        p_n[x * 2 -2+ p_n[x].nrNodSpecial].nod.setPosition(2000, 2000);
        p_n[x * 2 -2+ p_n[x].nrNodSpecial].nod_x1 = 2000;
        p_n[x * 2 -2+ p_n[x].nrNodSpecial].nod_x2 = 2000;
        p_n[x * 2 -2+ p_n[x].nrNodSpecial].nod_y1 = 2000;
        p_n[x * 2 -2+ p_n[x].nrNodSpecial].nod_y2 = 2000;
        p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(2000, 2000);
        p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = 2000;
        p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = 2000;
        p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = 2000;
        p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = 2000;
        p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(2000, 2000);
        p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = 2000;
        p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = 2000;
        p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = 2000;
        p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = 2000;
    }
    for (int i = 1; i <= nr_linii; i++)
    {
        if (l1[i] == x * 2 || l2[i] == x * 2) l2[i] = -1, l1[i] = -1;
        if (l1[i] == x * 2 - 1 || l2[i] == x * 2 - 1) l2[i] = -1, l1[i] = -1;

    }
}
//update
void updateP(bool isPressed, int x)
{
    if (isPressed == true && x != -1)
        if (mousePosWindow.x > rama_x1 + p_n[x].size_x / 2 && mousePosWindow.x < rama_x2 - p_n[x].size_x / 2 && mousePosWindow.y >rama_y1 + p_n[x].size_y / 2 && mousePosWindow.y < rama_y2 - p_n[x].size_y / 2)
        {
            p_n[x].piese_noi.setPosition(mousePosWindow.x, mousePosWindow.y);
            p_n[x].x1 = mousePosWindow.x - (p_n[x].size_x / 2);
            p_n[x].y1 = mousePosWindow.y - (p_n[x].size_y / 2);
            p_n[x].x2 = p_n[x].x1 + p_n[x].size_x;
            p_n[x].y2 = p_n[x].y1 + p_n[x].size_y;
          
            if (p_n[x].nrPiesa != 7)
            {
               cout<< x * 2 - 1 + p_n[x].nrNodSpecial << " " << x * 2 + p_n[x].nrNodSpecial << "\n";
                if (p_n[x].nr_rotatii % 4 == 1)
                {
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod.setPosition(mousePosWindow.x - (p_n[x].size_x / 2), mousePosWindow.y);
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod.setPosition(mousePosWindow.x + (p_n[x].size_x / 2), mousePosWindow.y);

                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x1 = mousePosWindow.x - (p_n[x].size_x / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x2 = mousePosWindow.x - (p_n[x].size_x / 2) + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y1 = mousePosWindow.y - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y2 = mousePosWindow.y + 10;

                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x1 = mousePosWindow.x + (p_n[x].size_x / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x2 = mousePosWindow.x + (p_n[x].size_x / 2) + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y1 = mousePosWindow.y - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y2 = mousePosWindow.y + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 2)
                {
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod.setPosition(mousePosWindow.x, mousePosWindow.y - (p_n[x].size_y / 2));
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod.setPosition(mousePosWindow.x, mousePosWindow.y + (p_n[x].size_y / 2));

                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x1 = mousePosWindow.x - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x2 = mousePosWindow.x + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y1 = mousePosWindow.y - (p_n[x].size_y / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y2 = mousePosWindow.y - (p_n[x].size_y / 2) + 10;

                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x1 = mousePosWindow.x - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x2 = mousePosWindow.x + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y1 = mousePosWindow.y + (p_n[x].size_y / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y2 = mousePosWindow.y + (p_n[x].size_y / 2) + 10;

                }
                if (p_n[x].nr_rotatii % 4 == 3)
                {
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod.setPosition(mousePosWindow.x + (p_n[x].size_x / 2), mousePosWindow.y);
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod.setPosition(mousePosWindow.x - (p_n[x].size_x / 2), mousePosWindow.y);

                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x1 = mousePosWindow.x + (p_n[x].size_x / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x2 = mousePosWindow.x + (p_n[x].size_x / 2) + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y1 = mousePosWindow.y - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y2 = mousePosWindow.y + 10;

                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x1 = mousePosWindow.x - (p_n[x].size_x / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x2 = mousePosWindow.x - (p_n[x].size_x / 2) + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y1 = mousePosWindow.y - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y2 = mousePosWindow.y + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 0)
                {
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod.setPosition(mousePosWindow.x, mousePosWindow.y + (p_n[x].size_y / 2));
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod.setPosition(mousePosWindow.x, mousePosWindow.y - (p_n[x].size_y / 2));

                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x1 = mousePosWindow.x - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_x2 = mousePosWindow.x + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y1 = mousePosWindow.y + (p_n[x].size_y / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial - 1].nod_y2 = mousePosWindow.y + (p_n[x].size_y / 2) + 10;

                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x1 = mousePosWindow.x - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_x2 = mousePosWindow.x + 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y1 = mousePosWindow.y - (p_n[x].size_y / 2) - 10;
                    p_n[x * 2   +p_n[x].nrNodSpecial ].nod_y2 = mousePosWindow.y - (p_n[x].size_y / 2) + 10;

                }
            }
            else
            {
                cout << x * 2 - 2 + p_n[x].nrNodSpecial << " " << x * 2 - 1 + p_n[x].nrNodSpecial << " " << x * 2 + p_n[x].nrNodSpecial << "\n";
                if (p_n[x].nr_rotatii % 4 == 1)
                {
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x - (p_n[x].size_x / 2), mousePosWindow.y);
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x + (p_n[x].size_x / 4)-10, mousePosWindow.y - (p_n[x].size_y /2));
                    p_n[x * 2  + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x + (p_n[x].size_x / 4)-10, mousePosWindow.y + (p_n[x].size_y / 2));

                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x - (p_n[x].size_x / 2) - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x - (p_n[x].size_x / 2) + 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y + 10;

                    p_n[x * 2 - 1+ p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x + (p_n[x].size_x / 5) - 10;
                    p_n[x * 2 - 1+ p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x + (p_n[x].size_x / 5) + 10;
                    p_n[x * 2 - 1+ p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y - (p_n[x].size_y / 2) - 10;
                    p_n[x * 2 - 1+ p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y - (p_n[x].size_y / 2) + 10;

                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x + (p_n[x].size_x / 5) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x + (p_n[x].size_x / 5) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y + (p_n[x].size_y / 2) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y + (p_n[x].size_y / 2) + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 2)
                {
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x, mousePosWindow.y - (p_n[x].size_y)/2);
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x + (p_n[x].size_x / 2), mousePosWindow.y + ((p_n[x].size_y)/4)-5);
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x - (p_n[x].size_x / 2), mousePosWindow.y + ((p_n[x].size_y) / 4)-5);

                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x  - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x  + 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y - (p_n[x].size_y) / 2 - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y - (p_n[x].size_y) / 2 + 10;

                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x + (p_n[x].size_x / 2) - 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x + (p_n[x].size_x / 2) + 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y + (p_n[x].size_y / 5) - 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y + (p_n[x].size_y / 5) + 10;

                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x - (p_n[x].size_x / 2) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x - (p_n[x].size_x / 2) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y + (p_n[x].size_y / 5) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y + (p_n[x].size_y / 5 )+ 10;
                }
                if (p_n[x].nr_rotatii % 4 == 3)
                {
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x + (p_n[x].size_x / 2), mousePosWindow.y);
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x - (p_n[x].size_x / 4)+10, mousePosWindow.y + (p_n[x].size_y / 2));
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x - (p_n[x].size_x / 4)+10, mousePosWindow.y - (p_n[x].size_y / 2));

                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x + (p_n[x].size_x / 2) - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x + (p_n[x].size_x / 2) + 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y + 10;

                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x - (p_n[x].size_x / 5) - 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x - (p_n[x].size_x / 5) + 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y + (p_n[x].size_y / 2) - 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y + (p_n[x].size_y / 2) + 10;

                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x - (p_n[x].size_x / 5) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x - (p_n[x].size_x / 5) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y - (p_n[x].size_y / 2) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y - (p_n[x].size_y / 2) + 10;
                }
                if (p_n[x].nr_rotatii % 4 == 0)
                {
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x, mousePosWindow.y + (p_n[x].size_y) / 2);
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x - (p_n[x].size_x / 2), mousePosWindow.y - ((p_n[x].size_y) / 4)+5);
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod.setPosition(mousePosWindow.x + (p_n[x].size_x / 2), mousePosWindow.y - ((p_n[x].size_y) / 4)+5);

                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x + 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y + (p_n[x].size_y) / 2 - 10;
                    p_n[x * 2 - 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y + (p_n[x].size_y) / 2 + 10;

                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x - (p_n[x].size_x / 2) - 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x - (p_n[x].size_x / 2) + 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y - (p_n[x].size_y / 5) - 10;
                    p_n[x * 2 - 1 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y - (p_n[x].size_y / 5) + 10;

                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x1 = mousePosWindow.x + (p_n[x].size_x / 2) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_x2 = mousePosWindow.x + (p_n[x].size_x / 2) + 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y1 = mousePosWindow.y - (p_n[x].size_y / 5) - 10;
                    p_n[x * 2 + p_n[x].nrNodSpecial].nod_y2 = mousePosWindow.y - (p_n[x].size_y / 5) + 10;
                }

            }
        }
}
void spawnP_A(int x)
{
    if (last_piesa1 != last_piesa2)
    {
        p_n[last_piesa1].piese_noi.setOutlineColor(sf::Color::Black);
        p_n[last_piesa1].piese_noi.setOutlineThickness(2.5);

    }

    p_n[x].piese_noi.setOutlineColor(sf::Color(147, 8, 253));
    p_n[x].piese_noi.setOutlineThickness(1.f);

    p_a.square.setPosition(1920 - 200, 1080 - 200);
    p_a.square.setSize(sf::Vector2f(90.f, 75.f));
    p_a.square.setOutlineColor(sf::Color(147, 8, 253));
    p_a.square.setOutlineThickness(1.f);
    if (last_piesa2 != 0) {
        p_a.square.setTexture(&p[p_n[x].tip].texturaPiesa);
        p_a.square.setFillColor(sf::Color::White);
    }
    else p_a.square.setFillColor(sf::Color::Black);
    //adauga o textura blank
}
void conex(int x, int y)
{
    if (x > y)
        swap(x, y);

    if (x != 0 || (x % 2 && x != y - 1) || (x % 3 && x != y - 1))
    {
        nr_linii++;
        l1[nr_linii] = x;
        l2[nr_linii] = y;      
        p_n[l1[nr_linii]].nod.setFillColor(sf::Color(147, 8, 253));
        p_n[l2[nr_linii]].nod.setFillColor(sf::Color(147, 8, 253));
    }
}
void spawn_linie(int x1, int x3,int x2, int y1, int y2)
{

    sf::Vertex linie1[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1)),
        sf::Vertex(sf::Vector2f(x2, y1))
    };

    sf::Vertex linie2[] =
    {
        sf::Vertex(sf::Vector2f(x2, y1)),
        sf::Vertex(sf::Vector2f(x2, y2))
    };

    sf::Vertex linie3[] =
    {
        sf::Vertex(sf::Vector2f(x2,y2)),
        sf::Vertex(sf::Vector2f(x3,y2))
    };
    window->draw(linie1, 2, sf::Lines);
    window->draw(linie2, 2, sf::Lines);
    window->draw(linie3, 2, sf::Lines);
}
void linie_trasa(bool P, int x)
{
    if (P == 1 && x != 0)
        spawn_linie(p_n[x].nod_x1 + 10, mousePosWindow.x, (p_n[x].nod_x1 + 10+ mousePosWindow.x)/2 ,p_n[x].nod_y1 + 10, mousePosWindow.y);
}
//render
void renderP()
{
    for (int i = 0; i < 9; i++)
        window->draw(p[i].piesa);

    for (int i = 1; i <= nr_piese_noi; i++)
        window->draw(p_n[i].piese_noi);

    for (int i = 1; i <= 2; i++)
        window->draw(cs[i].cs);

    for (int i = 1; i <= nr_nod; i++) 
        window->draw(p_n[i].nod);

    for (int i = 1; i <= nr_linii; i++) 
        spawn_linie(p_n[l1[i]].nod_x1 + 10, p_n[l2[i]].nod_x1 + 10, (p_n[l1[i]].nod_x1 + 10+p_n[l2[i]].nod_x1 + 10)/2, p_n[l1[i]].nod_y1 + 10, p_n[l2[i]].nod_y1 + 10);


    window->draw(p_a.square);
}
void rama()
{
    RectangleShape background;
    background.setSize(Vector2f(1460,896));
    background.setPosition(Vector2f(152,103));
    background.setFillColor(Color::Black);

    sf::Vertex line1[] =
    {
        sf::Vertex(sf::Vector2f(rama_x1, rama_y1),Color(147, 8, 253)),
        sf::Vertex(sf::Vector2f(rama_x2,rama_y1))
    };

    sf::Vertex line2[] =
    {
        sf::Vertex(sf::Vector2f(rama_x2, rama_y1),Color(147, 8, 253)),
        sf::Vertex(sf::Vector2f(rama_x2,rama_y2))
    };

    sf::Vertex line3[] =
    {
        sf::Vertex(sf::Vector2f(rama_x2,rama_y2),Color(147, 8, 253)),
        sf::Vertex(sf::Vector2f(rama_x1, rama_y2))
    };

    sf::Vertex line4[] =
    {
        sf::Vertex(sf::Vector2f(rama_x1, rama_y2),Color(147, 8, 253)),
        sf::Vertex(sf::Vector2f(rama_x1, rama_y1))
    };
    window->draw(line1, 2, sf::Lines);
    window->draw(line2, 2, sf::Lines);
    window->draw(line3, 2, sf::Lines);
    window->draw(line4, 2, sf::Lines);

    window->draw(background);
}
//game engine
void update()
{
    updateP(isPressed, piesa_aleasa);
    updateMousePos();
}
void render()
{
    //draw game obj
    rama();
    renderP();
    spawnP_A(last_piesa2);
    linie_trasa(isPressed, nod1);
}
//saving
void saveToFile(const std::string file_name)
{
    std::ofstream out_file;

    out_file.open(file_name ,std::ios::binary);
    
    if (out_file.is_open())
    {
        savedata data;
        data.rnrNoduri = nr_nod;
        cout << "Nr noduri" << nr_nod<<"\n";
        data.rnr_linii = nr_linii;
        cout << nr_linii << " ";
        for (int i = 1; i <= nr_linii; i++)
        {
            data.rl1[i] = l1[i];
            data.rl2[i] = l2[i];
        }
        for (int i = 1; i <= nr_piese_noi; i++)
        {         
            data.rx = p_n[i].piese_noi.getPosition().x;
            data.ry = p_n[i].piese_noi.getPosition().y;
            data.rwidht = p_n[i].piese_noi.getSize().x;
            data.rheight = p_n[i].piese_noi.getSize().y;
            data.rtip = p_n[i].tip;
            data.rnr_piesa = p_n[i].nrPiesa;

            data.rnr_rotatii = p_n[i].nr_rotatii;
            if (p_n[i].nrPiesa != 7)
            { 
                data.rnodx1 = p_n[2 * i -1  + p_n[i].nrNodSpecial].nod.getPosition().x;
                data.rnody1 = p_n[2 * i -1 + p_n[i].nrNodSpecial].nod.getPosition().y;
                data.rnodx2 = p_n[2 * i + p_n[i].nrNodSpecial].nod.getPosition().x;
                data.rnody2 = p_n[2 * i + p_n[i].nrNodSpecial].nod.getPosition().y;    
            }
            else
            {
                
                data.rnodx1 = p_n[2 * i - 2 + p_n[i].nrNodSpecial].nod.getPosition().x;
                data.rnody1 = p_n[2 * i - 2 + p_n[i].nrNodSpecial].nod.getPosition().y;
                data.rnodx2 = p_n[2 * i - 1 + p_n[i].nrNodSpecial].nod.getPosition().x;
                data.rnody2 = p_n[2 * i - 1 + p_n[i].nrNodSpecial].nod.getPosition().y;
                data.rnodx3 = p_n[2 * i + p_n[i].nrNodSpecial].nod.getPosition().x;
                data.rnody3 = p_n[2 * i + p_n[i].nrNodSpecial].nod.getPosition().y;
            }
            out_file.write((const char*)&data, sizeof(data));
        }
    }
    else
    {
        std::cout << "ERROR::COULD NOT SAVE TO FILE::FILENAME: " << file_name<<"\n";
    }

    out_file.close();
}
void loadFromFile(const std::string file_name,int &nr_piese_noi,int &nr_nod,int &nr_linii)
{
    std::ifstream in_file;

    in_file.open(file_name,std::ios::binary);

    if (in_file.is_open())
    {
        savedata data;
         int i = 1,jnod=0;
        while (!in_file.eof())
        {
            in_file.read((char*)&data, sizeof(savedata));
            nr_nod = data.rnrNoduri;
            cout << nr_nod;
            nr_linii = data.rnr_linii;
            cout << nr_linii << " ";
            for (int i = 1; i <= nr_linii; i++)
            {
                l1[i] = data.rl1[i];
                l2[i] = data.rl2[i];
            }
            p_n[i].x1 = data.rx-data.rwidht/2;
            p_n[i].y1 = data.ry-data.rheight/2;
            p_n[i].y2 = data.ry + data.rheight/2;
            p_n[i].x2 = data.rx + data.rwidht/2;
            p_n[i].size_x = data.rwidht;
            p_n[i].size_y = data.rheight;
            p_n[i].piese_noi.setPosition(data.rx, data.ry);
            p_n[i].piese_noi.setSize(sf::Vector2f(data.rwidht, data.rheight));
            p_n[i].tip = data.rtip;
            p_n[i].piese_noi.setOutlineColor(sf::Color::Black);
            p_n[i].piese_noi.setOutlineThickness(1.f);
            p_n[i].piese_noi.setOrigin(data.rwidht/2, data.rheight/2);
            p_n[i].piese_noi.setTexture(&p[p_n[i].tip].texturaPiesa);

            p_n[i].nr_rotatii = data.rnr_rotatii;
        
            p_n[i].nrPiesa = data.rnr_piesa;

            if (p_n[i].nrPiesa != 7)
            {
                jnod++;
                //nodl 1
                p_n[jnod].nod_x1 = data.rnodx1 - 10;
                p_n[jnod].nod_x2 = data.rnodx1 + 10;
                p_n[jnod].nod_y1 = data.rnody1 - 10;
                p_n[jnod].nod_y2 = data.rnody1 + 10;

                p_n[jnod].nod.setPosition(data.rnodx1, data.rnody1);
                p_n[jnod].nod.setRadius(7);
                p_n[jnod].nod.setFillColor(sf::Color(147, 8, 253));
                p_n[jnod].nod.setOutlineThickness(1.5);
                p_n[jnod].nod.setOrigin(7,7);

                jnod++;
                //nodul 2
                p_n[jnod].nod_x1 = data.rnodx2 - 10;
                p_n[jnod].nod_x2 = data.rnodx2 + 10;
                p_n[jnod].nod_y1 = data.rnody2 - 10;
                p_n[jnod].nod_y2 = data.rnody2 + 10;

                p_n[jnod].nod.setPosition(data.rnodx2, data.rnody2);
                p_n[jnod].nod.setRadius(7);
                p_n[jnod].nod.setFillColor(sf::Color(147, 8, 253));
                p_n[jnod].nod.setOutlineThickness(1.5);
                p_n[jnod].nod.setOrigin(7,7);
            }
            else
            {
                jnod++;
                //nodul1
                p_n[jnod].nod_x1 = data.rnodx1 - 10;
                p_n[jnod].nod_x2 = data.rnodx1 + 10;
                p_n[jnod].nod_y1 = data.rnody1 - 10;
                p_n[jnod].nod_y2 = data.rnody1 + 10;
                p_n[jnod].nod.setPosition(data.rnodx1, data.rnody1);
                p_n[jnod].nod.setRadius(7);
                p_n[jnod].nod.setFillColor(sf::Color(147, 8, 253));
                p_n[jnod].nod.setOutlineThickness(1.5);
                p_n[jnod].nod.setOrigin(7,7);
                jnod++;
                //nodul2
                p_n[jnod].nod_x1 = data.rnodx2 - 10;
                p_n[jnod].nod_x2 = data.rnodx2 + 10;
                p_n[jnod].nod_y1 = data.rnody2 - 10;
                p_n[jnod].nod_y2 = data.rnody2 + 10;
                p_n[jnod].nod.setPosition(data.rnodx2, data.rnody2);
                p_n[jnod].nod.setRadius(7);
                p_n[jnod].nod.setFillColor(sf::Color(147, 8, 253));
                p_n[jnod].nod.setOutlineThickness(1.5);
                p_n[jnod].nod.setOrigin(7,7);
                jnod++;
                //nodul3
                p_n[jnod].nod_x1 = data.rnodx3 - 10;
                p_n[jnod].nod_x2 = data.rnodx3 + 10;
                p_n[jnod].nod_y1 = data.rnody3 - 10;
                p_n[jnod].nod_y2 = data.rnody3 + 10;

                p_n[jnod].nod.setPosition(data.rnodx3, data.rnody3);
                p_n[jnod].nod.setRadius(7);
                p_n[jnod].nod.setFillColor(sf::Color(147, 8, 253));
                p_n[jnod].nod.setOutlineThickness(1.5);
                p_n[jnod].nod.setOrigin(7,7);
            }
            i++;
        }
        nr_piese_noi = i - 2;
        cout << nr_piese_noi<<"noduri="<<jnod;
        nr_linii = data.rnr_linii;
    }
    else
    {
        std::cout << "ERROR::COULD NOT SAVE TO FILE::FILENAME: " << file_name << "\n";
    }
    in_file.close();
}

void PositionPiesa(int x)
{
    if (x > 0)
    {
        nr_pasi++;

        pas[nr_pasi].tip_actiune = 1;
        pas[nr_pasi].nr_piesa = x;
        pas[nr_pasi].x = p_n[x].piese_noi.getPosition().x;
        pas[nr_pasi].y = p_n[x].piese_noi.getPosition().y;



    }
   
}
void muta_piesa_pozitie(int piesa, int x, int y, int nr_rotatii)
{
    p_n[piesa].piese_noi.setPosition(x, y);

    p_n[piesa].x1 = x - (p_n[piesa].size_x / 2);
    p_n[piesa].y1 = y - (p_n[piesa].size_y / 2);
    p_n[piesa].x2 = p_n[piesa].x1 + p_n[piesa].size_x;
    p_n[piesa].y2 = p_n[piesa].y1 + p_n[piesa].size_y;

    if (p_n[piesa].nrPiesa != 7)
    {
        if (nr_rotatii % 4 == 1)
        {
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod.setPosition(x - (p_n[piesa].size_x / 2), y);
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x + (p_n[piesa].size_x / 2), y);

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x1 = x - (p_n[piesa].size_x / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x2 = x - (p_n[piesa].size_x / 2) + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y1 = y - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y2 = y + 10;

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x1 = x + (p_n[piesa].size_x / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x2 = x + (p_n[piesa].size_x / 2) + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y1 = y - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y2 = y + 10;
        }
        if (nr_rotatii % 4 == 2)
        {
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod.setPosition(x, y - (p_n[piesa].size_y / 2));
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x, y + (p_n[piesa].size_y / 2));

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x1 = x - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x2 = x + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y1 = y - (p_n[piesa].size_y / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y2 = y - (p_n[piesa].size_y / 2) + 10;

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x1 = x - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x2 = x + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y1 = y + (p_n[piesa].size_y / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y2 = y + (p_n[piesa].size_y / 2) + 10;

        }
        if (nr_rotatii % 4 == 3)
        {
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod.setPosition(x + (p_n[piesa].size_x / 2), y);
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x - (p_n[piesa].size_x / 2), y);

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x1 = x + (p_n[piesa].size_x / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x2 = x + (p_n[piesa].size_x / 2) + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y1 = y - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y2 = y + 10;

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x1 = x - (p_n[piesa].size_x / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x2 = x - (p_n[piesa].size_x / 2) + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y1 = y - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y2 = y + 10;
        }
        if (nr_rotatii % 4 == 0)
        {
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod.setPosition(x, y + (p_n[piesa].size_y / 2));
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x, y - (p_n[piesa].size_y / 2));

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x1 = x - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_x2 = x + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y1 = y + (p_n[piesa].size_y / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial - 1].nod_y2 = y + (p_n[piesa].size_y / 2) + 10;

            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x1 = x - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_x2 = x + 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y1 = y - (p_n[piesa].size_y / 2) - 10;
            p_n[piesa * 2 + p_n[piesa].nrNodSpecial].nod_y2 = y - (p_n[piesa].size_y / 2) + 10;

        }
    }
    else
    {
        if (p_n[piesa].nr_rotatii % 4 == 1)
        {
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x - (p_n[piesa].size_x / 2), y);
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod.setPosition(x + (p_n[piesa].size_x / 4) - 10, y - (p_n[piesa].size_y / 2));
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod.setPosition(x + (p_n[piesa].size_x / 4) - 10, y + (p_n[piesa].size_y / 2));

            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x1 = x - (p_n[piesa].size_x / 2) - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x2 = x - (p_n[piesa].size_x / 2) + 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y1 = y - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y2 = y + 10;

            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x1 = x + (p_n[piesa].size_x / 5) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x2 = x + (p_n[piesa].size_x / 5) + 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y1 = y - (p_n[piesa].size_y / 2) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y2 = y - (p_n[piesa].size_y / 2) + 10;

            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x1 = x + (p_n[piesa].size_x / 5) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x2 = x + (p_n[piesa].size_x / 5) + 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y1 = y + (p_n[piesa].size_y / 2) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y2 = y + (p_n[piesa].size_y / 2) + 10;
        }
        if (p_n[piesa].nr_rotatii % 4 == 2)
        {
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x, y - (p_n[piesa].size_y) / 2);
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod.setPosition(x + (p_n[piesa].size_x / 2), y + ((p_n[piesa].size_y) / 4) - 5);
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod.setPosition(x - (p_n[piesa].size_x / 2), y + ((p_n[piesa].size_y) / 4) - 5);

            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x1 = x - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x2 = x + 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y1 = y - (p_n[piesa].size_y) / 2 - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y2 = y - (p_n[piesa].size_y) / 2 + 10;

            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x1 = x + (p_n[piesa].size_x / 2) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x2 = x + (p_n[piesa].size_x / 2) + 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y1 = y + (p_n[piesa].size_y / 5) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y2 = y + (p_n[piesa].size_y / 5) + 10;

            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x1 = x - (p_n[piesa].size_x / 2) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x2 = x - (p_n[piesa].size_x / 2) + 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y1 = y + (p_n[piesa].size_y / 5) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y2 = y + (p_n[piesa].size_y / 5) + 10;
        }
        if (p_n[piesa].nr_rotatii % 4 == 3)
        {
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x + (p_n[piesa].size_x / 2), y);
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod.setPosition(x - (p_n[piesa].size_x / 4) + 10, y + (p_n[piesa].size_y / 2));
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod.setPosition(x - (p_n[piesa].size_x / 4) + 10, y - (p_n[piesa].size_y / 2));

            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x1 = x + (p_n[piesa].size_x / 2) - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x2 = x + (p_n[piesa].size_x / 2) + 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y1 = y - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y2 = y + 10;

            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x1 = x - (p_n[piesa].size_x / 5) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x2 = x - (p_n[piesa].size_x / 5) + 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y1 = y + (p_n[piesa].size_y / 2) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y2 = y + (p_n[piesa].size_y / 2) + 10;

            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x1 = x - (p_n[piesa].size_x / 5) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x2 = x - (p_n[piesa].size_x / 5) + 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y1 = y - (p_n[piesa].size_y / 2) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y2 = y - (p_n[piesa].size_y / 2) + 10;
        }
        if (p_n[piesa].nr_rotatii % 4 == 0)
        {
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod.setPosition(x, y + (p_n[piesa].size_y) / 2);
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod.setPosition(x - (p_n[piesa].size_x / 2), y - ((p_n[piesa].size_y) / 4) + 5);
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod.setPosition(x + (p_n[piesa].size_x / 2), y - ((p_n[piesa].size_y) / 4) + 5);

            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x1 = x - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_x2 = x + 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y1 = y + (p_n[piesa].size_y) / 2 - 10;
            p_n[piesa*2 - 2 + p_n[piesa].nrNodSpecial].nod_y2 = y + (p_n[piesa].size_y) / 2 + 10;

            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x1 = x - (p_n[piesa].size_x / 2) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_x2 = x - (p_n[piesa].size_x / 2) + 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y1 = y - (p_n[piesa].size_y / 5) - 10;
            p_n[piesa*2 - 1 + p_n[piesa].nrNodSpecial].nod_y2 = y - (p_n[piesa].size_y / 5) + 10;

            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x1 = x + (p_n[piesa].size_x / 2) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_x2 = x + (p_n[piesa].size_x / 2) + 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y1 = y - (p_n[piesa].size_y / 5) - 10;
            p_n[piesa*2 + p_n[piesa].nrNodSpecial].nod_y2 = y - (p_n[piesa].size_y / 5) + 10;
        }
    }
}
void Undo()
{  
    if (nr_pasi >= 0)
    {


        if (pas[nr_pasi].tip_actiune == 1)
        {
            nr_pasi_redo++;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;
            redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;
            redo[nr_pasi_redo].x = p_n[pas[nr_pasi].nr_piesa].piese_noi.getPosition().x;
            redo[nr_pasi_redo].y = p_n[pas[nr_pasi].nr_piesa].piese_noi.getPosition().y;

            muta_piesa_pozitie(pas[nr_pasi].nr_piesa, pas[nr_pasi].x, pas[nr_pasi].y, p_n[pas[nr_pasi].nr_piesa].nr_rotatii);
        }


        if (pas[nr_pasi].tip_actiune == 2)
        {
            nr_pasi_redo++;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;
            redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;
            redo[nr_pasi_redo].x = p_n[pas[nr_pasi].nr_piesa].piese_noi.getPosition().x;
            redo[nr_pasi_redo].y = p_n[pas[nr_pasi].nr_piesa].piese_noi.getPosition().y;

            delete_p(pas[nr_pasi].nr_piesa);
        }


        if (pas[nr_pasi].tip_actiune == 3)
        {
            nr_pasi_redo++;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;
            redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;

            set_size2(pas[nr_pasi].nr_piesa);
        }


        if (pas[nr_pasi].tip_actiune == 4)
        {
            nr_pasi_redo++;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;
            redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;

            set_size1(pas[nr_pasi].nr_piesa);
        }


        if (pas[nr_pasi].tip_actiune == 5)
        {
            nr_pasi_redo++;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;
            redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;

            for (int i = 1; i <= 3; i++)
                rotate(pas[nr_pasi].nr_piesa, 90);
        }


        if (pas[nr_pasi].tip_actiune == 6)
        {
            nr_pasi_redo++;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;

            for (int i = 1; i <= nr_linii; i++)
                if (l1[i] == pas[nr_pasi].nr_piesa || l2[i] == pas[nr_pasi].nr_piesa)
                {
                    redo[nr_pasi_redo].x = l1[i];
                    redo[nr_pasi_redo].y = l2[i];
                    l2[i] = -1;
                    l1[i] = -1;
                }

        }


        if (pas[nr_pasi].tip_actiune == 7)
        {
            nr_pasi_redo++;
            redo[nr_pasi_redo].tip_actiune = pas[nr_pasi].tip_actiune;

            redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].x;

            conex(pas[nr_pasi].x, pas[nr_pasi].y);
        }


        nr_pasi--;

    }

    /*
        if (!pieseStack.empty())
        {
            if (actiuniStack.top() == 1)
            {
                if (nrActiuniPiesa[pieseStack.top()] > 1)
                {
                    float x = -1.00, y = -1.00, nodx1 = -1, nody1 = -1, nodx2 = -1, nody2 = -1, sizex = -1, sizey = -1;
                    int i = -1;

                    //size pop
                    //nodul1,nodul2:x,y
                    //mystack[pieseStack.top()].pop();
                    //mystack[pieseStack.top()].pop();
                    
                    //Redo
                    //mystackRedo[pieseStack.top()].push(mystack[pieseStack.top()].top());
                    mystack[pieseStack.top()].pop();
                    //Redo
                    //mystackRedo[pieseStack.top()].push(mystack[pieseStack.top()].top());
                    mystack[pieseStack.top()].pop();
                    //Redo
                    //mystackRedo[pieseStack.top()].push(mystack[pieseStack.top()].top());
                    mystack[pieseStack.top()].pop();
                    //Redo
                    //mystackRedo[pieseStack.top()].push(mystack[pieseStack.top()].top());
                    mystack[pieseStack.top()].pop();
                    //Redo
                    //mystackRedo[pieseStack.top()].push(mystack[pieseStack.top()].top());
                    mystack[pieseStack.top()].pop();
                    //Redo
                    //mystackRedo[pieseStack.top()].push(mystack[pieseStack.top()].top());
                    mystack[pieseStack.top()].pop();
                    //Redo
                    //nrActiuniPiesaRedo[pieseStack.top()] = nrActiuniPiesa[pieseStack.top()];
                    nrActiuniPiesa[pieseStack.top()]--;
                    
                    cout << " Undo_piesa = " << pieseStack.top() << " nrActiuniPiesa = " << nrActiuniPiesa[pieseStack.top()] << "\n";

                    // sizey = mystack[pieseStack.top()].top();
                      //  mystack[pieseStack.top()].pop();
                     //sizex = mystack[pieseStack.top()].top();
                       // mystack[pieseStack.top()].pop();
                     //nodul1,nodul2:x,y
                    nody2 = mystack[pieseStack.top()].top();
                    mystack[pieseStack.top()].pop();
                    nodx2 = mystack[pieseStack.top()].top();
                    mystack[pieseStack.top()].pop();
                    nody1 = mystack[pieseStack.top()].top();
                    mystack[pieseStack.top()].pop();
                    nodx1 = mystack[pieseStack.top()].top();
                    mystack[pieseStack.top()].pop();

                    y = mystack[pieseStack.top()].top();
                    cout << " \n y= " << y;
                    mystack[pieseStack.top()].pop();

                    x = mystack[pieseStack.top()].top();
                    cout << " \n x= " << x;
                    mystack[pieseStack.top()].pop();

                    mystack[pieseStack.top()].push(x);
                    mystack[pieseStack.top()].push(y);
                    mystack[pieseStack.top()].push(nodx1);
                    mystack[pieseStack.top()].push(nody1);
                    mystack[pieseStack.top()].push(nodx2);
                    mystack[pieseStack.top()].push(nody2);
                    // mystack[pieseStack.top()].push(sizex);
                    //mystack[pieseStack.top()].push(sizey);
                    i = pieseStack.top();

                    p_n[i].piese_noi.setPosition(x, y);
                    // p_n[i].piese_noi.setSize(sf::Vector2f(sizex, sizey));
                    p_n[2 * i - 1].nod.setPosition(nodx1, nody1);
                    p_n[2 * i].nod.setPosition(nodx2, nody2);
                    
                    //Elimin actiunea
                    //actiuniStackRedo.push(actiuniStack.top());
                    actiuniStack.pop();
                   // pieseStackRedo.push(pieseStack.top());
                    pieseStack.pop();

                }
                else
                {
                    while (!mystack[pieseStack.top()].empty())
                    {
                       
                        mystack[pieseStack.top()].pop();
                    }
                    nrActiuniPiesa[pieseStack.top()]--;
                    cout << " Undo_Else_piesa= " << pieseStack.top() << " \n";
                    delete_p(pieseStack.top());
                    //Elimin actiunea
                    actiuniStack.pop();
                    pieseStack.pop();

                }
            }
            else
                if (actiuniStack.top() == 2)
                {
                   
                    l1[nr_linii] = -1;
                    l2[nr_linii] = -1;
                    nr_linii--;
                    actiuniStack.pop();
                }
                else
                    if (actiuniStack.top() == 3)
                    {        
                        rotate(last_piesa2, 270);
                        cout << "aolo";              
                        actiuniStack.pop();
                    }
        }
        else {
            std::cout << "ERROR::Stack is empty \n";
           // nrPieseStack[i] = 0;
           
        }
   */
}
void Redo()
{

    if (nr_pasi_redo > 0)
    {
        if (redo[nr_pasi_redo].tip_actiune == 1)
        {
            nr_pasi++;
            pas[nr_pasi].tip_actiune = redo[nr_pasi_redo].tip_actiune;
            pas[nr_pasi].nr_piesa = redo[nr_pasi_redo].nr_piesa;
            pas[nr_pasi].x = p_n[redo[nr_pasi_redo].nr_piesa].piese_noi.getPosition().x;
            pas[nr_pasi].y = p_n[redo[nr_pasi_redo].nr_piesa].piese_noi.getPosition().y;

            muta_piesa_pozitie(redo[nr_pasi_redo].nr_piesa, redo[nr_pasi_redo].x, redo[nr_pasi_redo].y, p_n[redo[nr_pasi_redo].nr_piesa].nr_rotatii);
        }

        if (redo[nr_pasi_redo].tip_actiune == 2)
        {
            nr_pasi++;
            pas[nr_pasi].tip_actiune = redo[nr_pasi_redo].tip_actiune;
            pas[nr_pasi].nr_piesa = redo[nr_pasi_redo].nr_piesa;
            pas[nr_pasi].x = p_n[redo[nr_pasi_redo].nr_piesa].piese_noi.getPosition().x;
            pas[nr_pasi].y = p_n[redo[nr_pasi_redo].nr_piesa].piese_noi.getPosition().y;

            muta_piesa_pozitie(redo[nr_pasi_redo].nr_piesa, redo[nr_pasi_redo].x, redo[nr_pasi_redo].y, p_n[redo[nr_pasi_redo].nr_piesa].nr_rotatii);
        }

        if (redo[nr_pasi_redo].tip_actiune == 3)
        {
            nr_pasi++;
            pas[nr_pasi].tip_actiune = redo[nr_pasi_redo].tip_actiune;
            pas[nr_pasi].nr_piesa = redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;

            set_size1(redo[nr_pasi_redo].nr_piesa);
        }

        if (redo[nr_pasi_redo].tip_actiune == 4)
        {
            nr_pasi++;
            pas[nr_pasi].tip_actiune = redo[nr_pasi_redo].tip_actiune;
            pas[nr_pasi].nr_piesa = redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;

            set_size2(redo[nr_pasi_redo].nr_piesa);
        }

        if (redo[nr_pasi_redo].tip_actiune == 5)
        {
            nr_pasi++;
            pas[nr_pasi].tip_actiune = redo[nr_pasi_redo].tip_actiune;
            pas[nr_pasi].nr_piesa = redo[nr_pasi_redo].nr_piesa = pas[nr_pasi].nr_piesa;

            rotate(redo[nr_pasi_redo].nr_piesa, 90);
        }
        if (redo[nr_pasi_redo].tip_actiune == 6)
        {
            nr_pasi++;
            pas[nr_pasi].tip_actiune = redo[nr_pasi_redo].tip_actiune;
            pas[nr_pasi].x = redo[nr_pasi_redo].nr_piesa;

            conex(redo[nr_pasi_redo].x, redo[nr_pasi_redo].y);
        }
        if (redo[nr_pasi_redo].tip_actiune == 7)
        {
            nr_pasi++;
            pas[nr_pasi].tip_actiune = redo[nr_pasi_redo].tip_actiune;

            for (int i = 1; i <= nr_linii; i++)
                if (l1[i] == redo[nr_pasi_redo].nr_piesa || l2[i] == redo[nr_pasi_redo].nr_piesa)
                {
                    pas[nr_pasi].x = l1[i];
                    pas[nr_pasi].y = l2[i];
                    l2[i] = -1;
                    l1[i] = -1;
                }
        }

        nr_pasi_redo--;
    }
}

void colision_window()
{
    RenderWindow window(sf::VideoMode(1920, 1080),"SFML");
    window.setFramerateLimit(120);

    float dt;
    Clock dt_clock;
    
    const float gridSize = 50.f;

    //Piesa
    const float movementSpeed = 200.f;
    Vector2f viteza;
    RectangleShape piesa;
    piesa.setFillColor(Color::Green);
    piesa.setSize(Vector2f(gridSize, gridSize));
    
    while (window.isOpen())
    {
        dt = dt_clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //Piesa movement
        viteza.x = 0.f;
        viteza.y = 0.f;
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            viteza.y += -movementSpeed *dt;
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            viteza.y += movementSpeed * dt;
        }
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            viteza.x += -movementSpeed * dt;
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            viteza.x += movementSpeed * dt;
        }
        piesa.move(viteza);
        
        //Collision screen window
        if (piesa.getPosition().x < 0)
            piesa.setPosition(0, piesa.getPosition().y);
        if (piesa.getPosition().y < 0)
            piesa.setPosition(piesa.getPosition().x, 0);
        if(piesa.getPosition().x > 1980-gridSize)
            piesa.setPosition(1980-gridSize, piesa.getPosition().y);
        if (piesa.getPosition().y > 1080-gridSize)
            piesa.setPosition(piesa.getPosition().x, 1080-gridSize);
            
        //Render
        window.clear();

        window.draw(piesa);

        window.display();
    }
}
void info()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Meniu", sf::Style::Fullscreen);
    window.setFramerateLimit(144);

    //sound
    SoundBuffer buffer,buffer2;
    buffer.loadFromFile("sounds/button.wav");
    Sound sound;
    sound.setBuffer(buffer);

    //back_ground
    sf::Texture back_ground;
    back_ground.loadFromFile("Image/Black.jpg");
    sf::Sprite s(back_ground);
    
    //Font
    Font font;
    font.loadFromFile("Font/arial.ttf");
    Text text("", font);
    text.setFillColor(Color(255, 0, 0));
    text.setFont(Font(font));
    text.setOutlineColor(Color(0, 0, 0));
    text.setOutlineThickness(5);
    
    //Title
    Text title("Components Information", font);
    title.setCharacterSize(60);
    title.setFillColor(Color(100, 100, 100));
    title.setOutlineColor(Color(224, 224, 224));
    title.setOutlineThickness(4);
    title.setPosition(600, 100);
    title.setStyle(Text::Italic);

    //buton
    Button btn("Back", { 250,50 }, 30, sf::Color::Red, sf::Color::Red);
    btn.setPosition({ 5, 1040 });
    btn.setFont(font);
    btn.setTextColor(Color(100, 100, 100));

    //rama button back
    sf::RectangleShape square1;
    square1.setSize(sf::Vector2f(250, 35));
    square1.setPosition(sf::Vector2f(5, 1040));
    square1.setFillColor(sf::Color::Transparent);
    square1.setOutlineThickness(5);
    square1.setOutlineColor(sf::Color(100, 100, 100));
    
    //componentes
    Texture electrical_cell;
    electrical_cell.loadFromFile("Image/Electrical-cell.png");
    Sprite c1;
    c1.setTexture(electrical_cell);
    c1.scale(Vector2f(0.125, 0.125));
    c1.setPosition(Vector2f(100, 250));

    Texture resistor;
    resistor.loadFromFile("Image/Resistor.png");
    Sprite c2;
    c2.setTexture(resistor);
    c2.setScale(0.098, 0.2);
    c2.setPosition(Vector2f(100,350));
    
    Texture bulb;
    bulb.loadFromFile("Image/bulb.png");
    Sprite c3;
    c3.setTexture(bulb);
    c3.scale(Vector2f(0.27, 0.4));
    c3.setPosition(Vector2f(100, 450));

    Texture diode;
    diode.loadFromFile("Image/diode.png");
    Sprite c4;
    c4.setTexture(diode);
    c4.scale(Vector2f(0.25, 0.25));
    c4.setPosition(Vector2f(100, 550));

    Texture ammeter;
    ammeter.loadFromFile("Image/ammeter.png");
    Sprite c5;
    c5.setTexture(ammeter);
    c5.scale(Vector2f(0.25, 0.25));
    c5.setPosition(Vector2f(100, 650));

    Texture voltmeter;
    voltmeter.loadFromFile("Image/voltmeter.png");
    Sprite c6;
    c6.setTexture(voltmeter);
    c6.scale(Vector2f(0.25, 0.25));
    c6.setPosition(Vector2f(1200, 250)); 
   
    Texture capacitor;
    capacitor.loadFromFile("Image/capacitor.png");
    Sprite c7;
    c7.setTexture(capacitor);
    c7.scale(Vector2f(0.33, 0.25));
    c7.setPosition(Vector2f(1200, 350));

    Texture transistor;
    transistor.loadFromFile("Image/transistor.png");
    Sprite c8;
    c8.setTexture(transistor);
    c8.scale(Vector2f(0.118, 0.095));
    c8.setPosition(Vector2f(1200, 450));

    Texture fuse;
    fuse.loadFromFile("Image/fuse.png");
    Sprite c9;
    c9.setTexture(fuse);
    c9.scale(Vector2f(0.5, 0.4));
    c9.setPosition(Vector2f(1200, 550));

    //text
    Text c1_text("An electrical cell is a power supply for a circuit,"
         "\nletting current flow from the positive terminal around the circuit"
         "\nto the negative terminal.The cell creates a potential "
         "\ndifference across itself by supplying energy to charges.", font);
    c1_text.setCharacterSize(13);
    c1_text.setPosition(250, 250);
    c1_text.setFillColor(Color(255, 255, 255));

    Text c2_text("A resistor is a component that restricts flow of curent\nin a circuit.The voltage Vacross the resistor\nis directly proportional to the current i,i through a resistor.", font);
    c2_text.setCharacterSize(13);
    c2_text.setPosition(250, 350);
    c2_text.setFillColor(Color(255, 255, 255));
    
    Text c3_text("A bulb is a component that restircts flow of current\nin a circuit.The brightness of the bulb depends on its power\nP,which has the units of watts,W.", font);
    c3_text.setCharacterSize(13);
    c3_text.setPosition(250, 450);
    c3_text.setFillColor(Color(255, 255, 255));
    
    Text c4_text("An ideal diode is a component that only allows current in one\ndirection.This is because the ideal diode has an\ninfinite resistance in the reverse direction and\nzero resistance in the forward direction", font);
    c4_text.setCharacterSize(13);
    c4_text.setPosition(250, 550);
    c4_text.setFillColor(Color(255, 255, 255));

    Text c5_text("An ammeter is a device that measures current flowing\nbetween its ends.Current is that rate of flow of charge\nand is measured in amps with symbol A.", font);
    c5_text.setCharacterSize(13);
    c5_text.setPosition(250, 650);
    c5_text.setFillColor(Color(255, 255, 255));

    Text c6_text("An voltmeter is a device that measures the potential\ndifference between its ends.Potential difference is often\ncalled voltage and is measured in volts with symbol V.", font);
    c6_text.setCharacterSize(13);
    c6_text.setPosition(1350, 250);
    c6_text.setFillColor(Color(255, 255, 255));

    Text c7_text("A capacitor is a component that consists of two\nconducting plates separated by an insulator.It is used to\ntemporarily store electrical charge and energy.", font);
    c7_text.setCharacterSize(13);
    c7_text.setPosition(1350, 350);
    c7_text.setFillColor(Color(255, 255, 255));

    Text c8_text("A transistor acts as a switch or as an amplifier\nin electrical circuits.The three pins are the\nCollector C,Base B and Emitter E.When there is zero\nvoltage between the base and ground the transistor does not\nallow any current to flow though it from its C lin to its E pin.", font);
    c8_text.setCharacterSize(13);
    c8_text.setPosition(1350, 450);
    c8_text.setFillColor(Color(255, 255, 255));

    Text c9_text("A fuse is a device that allows current to flow while it is below\na certain value,which depends on the rating of fuse.When the\ncurrent exceeds this value,the fuse will blow(melt or break),\nwhich breaks the circuit and stops current flowing altogether.", font);
    c9_text.setCharacterSize(13);
    c9_text.setPosition(1350, 550);
    c9_text.setFillColor(Color(255, 255, 255));
    bool ok = false;
    
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case sf::Event::MouseMoved:
                if (btn.isMouseOver(window))
                {
                  
                    btn.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }   
                }
                else
                {
                    btn.setBackColor(sf::Color::Black);
                    ok = false;
                }
            case sf::Event::MouseButtonPressed:
                if (btn.isMouseOver(window) && Mouse::isButtonPressed(Mouse::Left))
                    window.close();
            }
        }
        window.clear();
        window.draw(s);
        window.draw(c1_text);
        window.draw(c1);
        window.draw(c2_text);
        window.draw(c2);
        window.draw(c3_text);
        window.draw(c3);
        window.draw(c4_text);
        window.draw(c4);
        window.draw(c5_text);
        window.draw(c5);
        window.draw(c6_text);
        window.draw(c6);
        window.draw(c7_text);
        window.draw(c7);
        window.draw(c8_text);
        window.draw(c8);
        window.draw(c9_text);
        window.draw(c9);
        btn.drawTo(window);
        window.draw(square1);
        window.draw(title);
        window.display();
    }
}

void tabla()
{
    //Sound
    SoundBuffer buffer;
    buffer.loadFromFile("sounds/button.wav");
    Sound sound;
    sound.setBuffer(buffer);

    //Font
    Font font;
    font.loadFromFile("Font/arial.ttf");
    Text text("", font);
    text.setFillColor(Color(255, 0, 0));
    text.setFont(Font(font));
    text.setOutlineColor(Color(0, 0, 0));
    text.setOutlineThickness(5);

    //bordura buttoane de sus
    s[1].square.setSize(sf::Vector2f(200, 50));
    s[1].square.setPosition(sf::Vector2f(5, 5));
    s[2].square.setSize(sf::Vector2f(400, 50));
    s[2].square.setPosition(sf::Vector2f(215, 5));
    s[3].square.setSize(sf::Vector2f(400, 50));
    s[3].square.setPosition(sf::Vector2f(625, 5));
    s[4].square.setSize(sf::Vector2f(400, 50));
    s[4].square.setPosition(sf::Vector2f(1035, 5));
    s[5].square.setSize(sf::Vector2f(400, 50));
    s[5].square.setPosition(sf::Vector2f(1445, 5));
    s[6].square.setSize(sf::Vector2f(400, 50));
    s[6].square.setPosition(sf::Vector2f(1855, 5));

    //bordura buttoane de jos
    s[7].square.setSize(sf::Vector2f(250, 35));
    s[7].square.setPosition(sf::Vector2f(5, 1040));
    s[8].square.setSize(sf::Vector2f(250, 35));
    s[8].square.setPosition(sf::Vector2f(265, 1040));
    s[9].square.setSize(sf::Vector2f(250, 35));
    s[9].square.setPosition(sf::Vector2f(525, 1040));
    s[10].square.setSize(sf::Vector2f(250, 35));
    s[10].square.setPosition(sf::Vector2f(785, 1040));
    for (int i = 1; i <= 10; i++)
    {
        s[i].square.setFillColor(sf::Color::Transparent);
        s[i].square.setOutlineThickness(5);
        s[i].square.setOutlineColor(sf::Color(100, 100, 100));
    }

    //textBox
    Textbox textbox1(15, sf::Color::White, false);
    textbox1.setFont(font);
    textbox1.setPosition({ 970,480 });
    textbox1.setLimit(true, 10);

    Textbox textbox2(15, sf::Color::White, false);
    textbox2.setFont(font);
    textbox2.setPosition({ 970,520 });
    textbox2.setLimit(true, 10);

    Textbox textbox3(15, sf::Color::White, false);
    textbox3.setFont(font);
    textbox3.setPosition({ 970,560 });
    textbox3.setLimit(true, 10);

    //edit
    //button chenar
    Button btnedit("", { 400,300 }, 0, sf::Color::Black, sf::Color::Red);
    btnedit.setPosition({ 760,390 });
    btnedit.setOutlineColor({ 147, 8, 253 }, 5);

    Button btnedit_close("", { 50,50 }, 10, sf::Color::Black, sf::Color::Red);
    btnedit_close.setPosition({ 1110,390 });
    btnedit_close.setOutlineColor({ 147, 8, 253 }, 5);
    Button btn_save("", { 50,50 }, 0, sf::Color::Black, sf::Color::Red);
    btn_save.setPosition({ 760,390 });
    btn_save.setOutlineColor({ 147, 8, 253 }, 5);

    sf::Text close_text("X", font);
    close_text.setCharacterSize(30);
    close_text.setPosition({ 1125,400 });
    sf::Text save_text("Save", font);
    save_text.setCharacterSize(20);
    save_text.setPosition({ 762,400 });

    sf::Text title1("Parameters", font), title2("Nume", font), title3("Voltaj", font), title4("Rezistenta", font);
    title1.setCharacterSize(30);
    title1.setPosition({ 870, 400 });
    title1.setStyle(Text::Italic);
    title2.setCharacterSize(30);
    title2.setPosition({ 810, 470 });
    title2.setStyle(Text::Italic);
    title3.setCharacterSize(30);
    title3.setPosition({ 810,510 });
    title3.setStyle(Text::Italic);
    title4.setCharacterSize(30);
    title4.setPosition({ 810, 550 });
    title4.setStyle(Text::Italic);

    Button btnedit1("", { 150,22 }, 0, sf::Color::Black, sf::Color::Red);
    btnedit1.setPosition({ 970,480 });
    btnedit1.setOutlineColor({ 128, 128, 128 }, 1);
    Button btnedit2("", { 150,22 }, 0, sf::Color::Black, sf::Color::Red);
    btnedit2.setPosition({ 970,520 });
    btnedit2.setOutlineColor({ 128, 128, 128 }, 1);
    Button btnedit3("", { 150,22 }, 0, sf::Color::Black, sf::Color::Red);
    btnedit3.setPosition({ 970,560 });
    btnedit3.setOutlineColor({ 128, 128, 128 }, 1);


    //butoane tabla
    Button btn("Back", { 200,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn.setPosition({ 5,5 });
    btn.setFont(font);
    btn.setTextColor(Color(100, 100, 100));
    btn.setTextPos({ 50,10 });

    Button btn1("Save", { 400,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn1.setPosition({ 215,5 });
    btn1.setFont(font);
    btn1.setTextColor(Color(100, 100, 100));
    btn1.setTextPos({ 350,10 });

    Button btn2("Edit", { 400,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn2.setPosition({ 625,5 });
    btn2.setFont(font);
    btn2.setTextColor(Color(100, 100, 100));
    btn2.setTextPos({ 750,10 });

    Button btn3("Open", { 400,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn3.setPosition({ 1035,5 });
    btn3.setFont(font);
    btn3.setTextColor(Color(100, 100, 100));
    btn3.setTextPos({ 1150,10 });

    Button btn4("Reset", { 400,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn4.setPosition({ 1445,5 });
    btn4.setFont(font);
    btn4.setTextColor(Color(100, 100, 100));
    btn4.setTextPos({ 1550,10 });

    Button btn5("", { 400,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn5.setPosition({ 1855,5 });
    btn5.setFont(font);
    btn5.setTextColor(Color(100, 100, 100));
    btn5.setTextPos({ 1550,10 });

    //button de jos
    Button btn7("Undo", { 250,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn7.setPosition({ 5,1040 });
    btn7.setFont(font);
    btn7.setTextColor(Color(100, 100, 100));

    Button btn8("Delete", { 250,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn8.setPosition({ 265,1040 });
    btn8.setFont(font);
    btn8.setTextColor(Color(100, 100, 100));

    Button btn9("Rotate", { 250,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn9.setPosition({ 525,1040 });
    btn9.setFont(font);
    btn9.setTextColor(Color(100, 100, 100));

    Button btn10("Redo", { 250,50 }, 30, sf::Color::Black, sf::Color::Red);
    btn10.setPosition({ 785,1040 });
    btn10.setFont(font);
    btn10.setTextColor(Color(100, 100, 100));

    //back_ground
    sf::Texture back_ground;
    back_ground.loadFromFile("Image/Black.jpg");
    sf::Sprite BackGround(back_ground);


    //init game engine
    initpiesa();
    for (int i = 0; i < 9; i++)
        std::cout << p[i].x1 << " " << p[i].x2 << " " << p[i].y1 << " " << p[i].y2 << "\n ";


    //game loop
    window = nullptr;
    videoMode.height = 1080;
    videoMode.width = 1920;

    window = new sf::RenderWindow(videoMode, "Electron", sf::Style::Fullscreen);

    window->setFramerateLimit(144);
    bool ok = false;
    int select = 0;
    bool selected = false, save_select = false;
    while (window->isOpen())
    {
        sf::Event ev;
        while (window->pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::Closed: {
                window->close(); }
                                  break;

            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape) {
                    for (int i = 1; i <= nr_piese_noi; i++)
                        delete_p(i);
                    window->close();
                }
                break;
            case sf::Event::MouseMoved:
                if (btn.isMouseOver(*window))
                {
                    btn.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn1.isMouseOver(*window))
                {
                    btn1.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn1.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn2.isMouseOver(*window))
                {
                    btn2.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn2.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn3.isMouseOver(*window))
                {
                    btn3.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn3.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn4.isMouseOver(*window))
                {
                    btn4.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn4.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn5.isMouseOver(*window))
                {
                    btn5.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn5.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn7.isMouseOver(*window))
                {
                    btn7.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn7.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn8.isMouseOver(*window))
                {
                    btn8.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn9.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn8.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn9.isMouseOver(*window))
                {
                    btn9.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn10.isMouseOver(*window))
                    {
                        btn9.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn10.isMouseOver(*window))
                {
                    btn10.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(*window) && !btn2.isMouseOver(*window) && !btn3.isMouseOver(*window) && !btn4.isMouseOver(*window) && !btn5.isMouseOver(*window) && !btn7.isMouseOver(*window) && !btn8.isMouseOver(*window) && !btn.isMouseOver(*window) && !btn9.isMouseOver(*window))
                    {
                        btn10.setBackColor(sf::Color::Black);
                        ok = false;
                    }

                break;
            case sf::Event::TextEntered: {
                textbox1.typedOn(ev);
                textbox2.typedOn(ev);
                textbox3.typedOn(ev);
            }
           break;
            case sf::Event::MouseButtonPressed:
            {
                //Button case:
                if (btn.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    for (int i = 1; i <= nr_piese_noi; i++)
                        delete_p(i);
                    window->close();
                }
                if (btn4.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    for (int i = 1; i <= nr_piese_noi; i++)
                        delete_p(i);
                }
                if (btn1.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    saveToFile("Save_Test.bn");
                }
                if (btn3.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left))
                    loadFromFile("Save_Test.bn", nr_piese_noi, nr_nod, nr_linii);

               
                if (btnedit1.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    textbox1.setSelected(true);
                }

                if ((!btnedit1.isMouseOver(*window)) && Mouse::isButtonPressed(Mouse::Left)) {
                    textbox1.setSelected(false);
                }

                if (btnedit2.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    textbox2.setSelected(true);                 
                }
                if ((!btnedit2.isMouseOver(*window)) && Mouse::isButtonPressed(Mouse::Left)) {
                    textbox2.setSelected(false);
                }
                if (btnedit3.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    textbox3.setSelected(true);
                }
                if ((!btnedit3.isMouseOver(*window)) && Mouse::isButtonPressed(Mouse::Left)) {
                    textbox3.setSelected(false);
                }

                //spawn case
                isPressed = true;
                for (int i = 0; i < 9; i++)
                    if (mousePosWindow.x >= p[i].x1 && mousePosWindow.x <= p[i].x2 && mousePosWindow.y >= p[i].y1 && mousePosWindow.y <= p[i].y2)
                        spawnP(p[i].nr_piesa);


                for (int j = 1; j <= nr_piese_noi; j++)
                    if (mousePosWindow.x >= p_n[j].x1 + 10 && mousePosWindow.x <= p_n[j].x2 - 10 && mousePosWindow.y >= p_n[j].y1 + 10 && mousePosWindow.y <= p_n[j].y2 - 10)
                    {
                        piesa_aleasa = j;
                        PositionPiesa(piesa_aleasa);
                        last_piesa1 = j;
                        if (last_piesa1 != last_piesa2)
                        {
                            last_piesa1 = last_piesa2;
                            last_piesa2 = j;
                        }
                    }
                if (btn7.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    Undo();
                }
                if (btn10.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left)) {
                    Redo();
                }
                
                if (btn2.isMouseOver(*window) && Mouse::isButtonPressed(Mouse::Left) && last_piesa2 > 0)
                {
                    selected = true;
                    save_select = false;
                }
                if (mousePosWindow.x >= 1110 && mousePosWindow.x <= 1160 && mousePosWindow.y >= 390 && mousePosWindow.y <= 440 && Mouse::isButtonPressed(Mouse::Left))
                {
                    selected = false;
                    save_select = false;
                }
                if (mousePosWindow.x >= 760 && mousePosWindow.x <= 810 && mousePosWindow.y >= 390 && mousePosWindow.y <= 440 && Mouse::isButtonPressed(Mouse::Left))
                    save_select = true;
                    
                for (int z = 1; z <= nr_nod; z++)
                    if (mousePosWindow.x >= p_n[z].nod_x1 && mousePosWindow.x <= p_n[z].nod_x2 && mousePosWindow.y >= p_n[z].nod_y1 && mousePosWindow.y <= p_n[z].nod_y2)
                    {
                        nod1 = z;
                        for (int i = 1; i <= nr_linii; i++)
                            if (l1[i] == nod1 || l2[i] == nod1)
                            {
                                nr_pasi++;
                                pas[nr_pasi].tip_actiune = 7;
                                pas[nr_pasi].x = l1[i];
                                pas[nr_pasi].y = l2[i];

                                l2[i] = -1;
                                l1[i] = -1;

                            }
                    }

                if (mousePosWindow.x >= cs[1].x1 && mousePosWindow.x <= cs[1].x2 && mousePosWindow.y >= cs[1].y1 && mousePosWindow.y <= cs[1].y2)
                {
                    nr_pasi++;
                    pas[nr_pasi].tip_actiune = 3;
                    pas[nr_pasi].nr_piesa = last_piesa2;
                    set_size1(last_piesa2);
                }
                if (mousePosWindow.x >= cs[2].x1 && mousePosWindow.x <= cs[2].x2 && mousePosWindow.y >= cs[2].y1 && mousePosWindow.y <= cs[2].y2)
                {
                    nr_pasi++;
                    pas[nr_pasi].tip_actiune = 4;
                    pas[nr_pasi].nr_piesa = last_piesa2;
                    set_size2(last_piesa2);
                }
                if (mousePosWindow.x >= cs[3].x1 && mousePosWindow.x <= cs[3].x2 && mousePosWindow.y >= cs[3].y1 && mousePosWindow.y <= cs[3].y2)
                {
                    nr_pasi++;
                    pas[nr_pasi].tip_actiune = 5;
                    pas[nr_pasi].nr_piesa = last_piesa2;
                    rotate(last_piesa2, 90);

                }

                if (mousePosWindow.x >= cs[4].x1 && mousePosWindow.x <= cs[4].x2 && mousePosWindow.y >= cs[4].y1 && mousePosWindow.y <= cs[4].y2)
                {
                    PositionPiesa(last_piesa2);
                    delete_p(last_piesa2);
                }
                nod2 = 0;


                break;

            case sf::Event::MouseButtonReleased:
            {
                for (int z = 1; z <= nr_nod; z++)
                    if (mousePosWindow.x >= p_n[z].nod_x1 && mousePosWindow.x <= p_n[z].nod_x2 && mousePosWindow.y >= p_n[z].nod_y1 && mousePosWindow.y <= p_n[z].nod_y2)
                    {
                        nod2 = z;

                        for (int i = 1; i <= nr_linii; i++)
                            if (l1[i] == nod2 || l2[i] == nod2)
                            {
                                nr_pasi++;
                                pas[nr_pasi].tip_actiune = 7;
                                pas[nr_pasi].x = l1[i];
                                pas[nr_pasi].y = l2[i];

                                l2[i] = -1;
                                l1[i] = -1;
                            }

                        if (nod1 != 0 && nod2 != 0)
                        {
                            nr_pasi++;
                            pas[nr_pasi].tip_actiune = 6;
                            pas[nr_pasi].nr_piesa = nod1;

                            conex(nod1, nod2);
                        }
                    }

                nod1 = 0;

                isPressed = false;
                save_select = false;
                piesa_aleasa = -1;
            }
            break;
            }

            }
            window->clear();
            //update

            update();

            //render
            window->draw(BackGround);
            render();
            btn.drawTo(*window);
            btn1.drawTo(*window);
            btn2.drawTo(*window);
            btn3.drawTo(*window);
            btn4.drawTo(*window);
            btn7.drawTo(*window);
            btn8.drawTo(*window);
            btn9.drawTo(*window);
            btn10.drawTo(*window);
            //Rama butoane:
            for (int i = 1; i <= 10; i++) {
                if (i != 6)
                    window->draw(s[i].square);
            }
            if (selected == true) {
                btnedit.drawTo(*window);
                btnedit1.drawTo(*window);
                btnedit2.drawTo(*window);
                btnedit3.drawTo(*window);
                btnedit_close.drawTo(*window);
                btn_save.drawTo(*window);
                window->draw(title1);
                window->draw(title2);
                window->draw(title3);
                window->draw(title4);
                window->draw(close_text);
                window->draw(save_text);
                textbox1.drawTo(*window);
                textbox2.drawTo(*window);
                textbox3.drawTo(*window);
            }     
            window->display();
        }
    }
}

int main()
{
    sf::RenderWindow window_main(sf::VideoMode(1920, 1080), "Meniu", sf::Style::Fullscreen);
    window_main.setFramerateLimit(144);

    int width = sf::VideoMode::getDesktopMode().width;
    int height = sf::VideoMode::getDesktopMode().height;
    sf::Texture back_ground;
    back_ground.loadFromFile("Image/Black.jpg");
    sf::Sprite s(back_ground);

    
    //sound
    SoundBuffer buffer;
    buffer.loadFromFile("sounds/button.wav");
    Sound sound;
    sound.setBuffer(buffer);

    //Font
    Font font;
    font.loadFromFile("Font/arial.ttf");
    Text text("", font);
    text.setFillColor(Color(255, 0, 0));
    text.setFont(Font(font));
    text.setOutlineColor(Color(0, 0, 0));
    text.setOutlineThickness(5);

    //Button
    Button btn1("New Project", { 250,100 }, 35, sf::Color::Black, sf::Color::Red);
    Button btn2("Info", { 250,100 }, 40, Color::Blue, Color::Red);
    Button btn3("Exit", { 250,100 }, 40, sf::Color::Black, sf::Color::Red);

    //titlu
    Text title("ELECTRON", font);
    title.setCharacterSize(80);
    title.setFillColor(Color(100, 100, 100));
    title.setOutlineColor(Color(224, 224, 224));
    title.setOutlineThickness(4);
    title.setPosition(760, 300);
    title.setStyle(Text::Italic);


    btn1.setPosition({ 860,540 });
    btn1.setFont(font);
    btn1.setTextColor(Color(100, 100, 100));

    btn2.setPosition({ 860,680 });
    btn2.setFont(font);
    btn2.setTextColor(Color(100, 100, 100));

    btn3.setPosition({ 860,820 });
    btn3.setFont(font);
    btn3.setTextColor(Color(100, 100, 100));
    //square
    sf::RectangleShape square1;
    square1.setSize(sf::Vector2f(250, 100));
    square1.setPosition(sf::Vector2f(width / 2 - 100, height / 2));
    square1.setFillColor(sf::Color::Transparent);
    square1.setOutlineThickness(5);
    square1.setOutlineColor(sf::Color(100, 100, 100));

    sf::RectangleShape square2;
    square2.setSize(sf::Vector2f(250, 100));
    square2.setPosition(sf::Vector2f(width / 2 - 100, height / 2 + 140));
    square2.setFillColor(sf::Color::Transparent);
    square2.setOutlineThickness(5);
    square2.setOutlineColor(sf::Color(100, 100, 100));

    sf::RectangleShape square3;
    square3.setSize(sf::Vector2f(250, 100));
    square3.setPosition(sf::Vector2f(width / 2 - 100, height / 2 + 280));
    square3.setFillColor(sf::Color::Transparent);
    square3.setOutlineThickness(5);
    square3.setOutlineColor(sf::Color(100, 100, 100));
    bool ok = false;

    while (window_main.isOpen())
    {
        sf::Event event;
        while (window_main.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window_main.close();
                break;
            case sf::Event::MouseMoved:
                if (btn1.isMouseOver(window_main))
                {
                    btn1.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn2.isMouseOver(window_main) && !btn3.isMouseOver(window_main))
                    {
                        btn1.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn2.isMouseOver(window_main))
                {
                    btn2.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(window_main) && !btn3.isMouseOver(window_main))
                    {
                        btn2.setBackColor(sf::Color::Black);
                        ok = false;
                    }
                if (btn3.isMouseOver(window_main))
                {
                    btn3.setBackColor(sf::Color::White);
                    if (ok == false) {
                        sound.play();
                        ok = true;
                    }
                }
                else
                    if (!btn1.isMouseOver(window_main) && !btn2.isMouseOver(window_main))
                    {
                        btn3.setBackColor(sf::Color::Black);
                        ok = false;
                    }
            case sf::Event::MouseButtonPressed:
                if (btn1.isMouseOver(window_main) && Mouse::isButtonPressed(Mouse::Left))
                    tabla();
               
                if (btn2.isMouseOver(window_main) && Mouse::isButtonPressed(Mouse::Left))
                    info();
                if (btn3.isMouseOver(window_main) && Mouse::isButtonPressed(Mouse::Left))
                    window_main.close();
            }
        }
        window_main.clear();
        window_main.draw(s);
        window_main.draw(title);
        window_main.draw(square1);
        window_main.draw(square2);
        window_main.draw(square3);
        btn1.drawTo(window_main);
        btn2.drawTo(window_main);
        btn3.drawTo(window_main);
        window_main.display();
    }
    return 0;
}
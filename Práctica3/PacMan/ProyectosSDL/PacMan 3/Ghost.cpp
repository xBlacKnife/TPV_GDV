//Dependencias
#include "Ghost.h"
#include "Game.h" 
#include <vector>
#include <cstdlib>
#include <iostream>
#include "PlayState.h"

using namespace std;

//Constructoras
Ghost::Ghost() :GameCharacter(), ghostNo(0){}

Ghost::Ghost(Texture* texture, Game* juego, PlayState* playState, unsigned int ghostNo)
: GameCharacter(juego, texture, playState, 0, ghostNo*2), ghostNo(ghostNo){}

Ghost::Ghost(Texture* texture, Game* juego, PlayState* playState, unsigned int ghostNo, unsigned int posX, unsigned int posY)
	: GameCharacter(juego, texture, playState, 0, ghostNo * 2, posX, posY), ghostNo(ghostNo){}


void Ghost::update()
{
	//1.Creamos el vector con las 4 direcciones
	//Izquierda, derecha, arriba y abajo (en ese orden)
	vector<Par>dirs(4);
	dirs[0].x = dirs[2].y = -1;
	dirs[1].x = dirs[3].y = 1;
	
	//2.Filtramos para quedarnos con las posibles (y quitando la de atr�s si se da el caso)
	filtraDirs(dirs);

	//3.Elegimos una direcci�n
	if (dirs.size() > 0)
	{
		//Elegimos la direcci�n aleatoriamente 
		int random = rand() % dirs.size();
		dirX = dirs[random].x;
		dirY = dirs[random].y;
	}
	//Si no hay ninguna direcci�n disponible, no se mueve
	else
		dirX = dirY = 0;

	//4.Movemos al fantasma con la seguridad de que esa casilla es posible
	posX += dirX;
	posY += dirY;
	playState->compruebaLimites(posX, posY); //Tenemos en cuenta el toroide

	//5.Animamos al fantasma
	animate();
}

void const Ghost::filtraDirs(vector<Par> &dirs)
{
	//Quitamos las posiciones a las que el fantasma se pueda mover
	unsigned int cont = 0;
	list<GameCharacter*>::iterator it;
	for (int i = 0; i < 4; i++)
	{
		int nx = posX + dirs[cont].x;
		int ny = posY + dirs[cont].y;
		if (posPosible(nx, ny))
			cont++;
		else
			dirs.erase(dirs.begin() + cont);
	}

	//Si hay m�s de una direcci�n posible, quitamos la contraria a la actual
	if (dirs.size() > 1)
		deleteBackDir(dirs);
}

bool Ghost::posPosible(int x, int y)
{
	list<GameObject*>::iterator it;
	playState->compruebaLimites(x, y);
	return (playState->nextCell(x, y) && !playState->hayFantasma(x, y, it));
}

void const Ghost::deleteBackDir(vector<Par> &dirs)
{
	unsigned int j = 0;
	Par dirActual;
	dirActual.x = dirX;
	dirActual.y = dirY;

	//Buscamos la direcci�n contraria
	while (j < dirs.size() && !Game::ParesOpuestos(dirActual, dirs[j]))
		j++;

	//Si la direcci�n contraria se encuentra, la borramos
	if (j < dirs.size())
		dirs.erase(dirs.begin() + j);
}


void Ghost::animate()
{
	//Cambiamos las animaciones dependiendo del tiempo de power-up que quede
	//1.HAY POWER-UP
	if (playState->powerUpActive())
	{
		//a)Queda tiempo todav�a
		if (playState->timeLeft() > ENERGY_TIME / 4)
		{
			frameRow = 0;
			if (!juego->alterna(frameCol, 12, 13))
				frameCol = 12;
		}
		//b)Queda poco tiempo
		else
		{
			//Columna
			juego->alterna(frameCol, 12, 13);
			//Fila
			juego->alterna(frameRow, 0, 1);
		}
	}
	//2.NO HAY POWER-UP
	else
	{
		//Columna(cambia en cada frame)
		if (!juego->alterna(frameCol, ghostNo * 2, ghostNo * 2 + 1))
			frameCol = ghostNo * 2;

		//Fila (depende de la direcci�n): derecha, izquierda, arriba o abajo (en ese orden)
		if (dirX == 1)
			frameRow = 0;
		else if (dirX == -1)
			frameRow = 2;
		else if (dirY == -1)
			frameRow = 3;
		else
			frameRow = 1;
	}
}


Ghost::~Ghost()
{

}




#include "Pacman.h"
#include "Game.h"
#include <iostream>

Pacman::Pacman()
//Secuencia de inicialización
	:posX(0), posY(0), iniX(0), iniY(0), numLives(0), energyLeft(0), dirX(0), dirY(0),
	bufferX(1), bufferY(0), textRow(0), textCol(0), juego(nullptr), texture(nullptr){}


Pacman::Pacman(Texture* texture, Game* juego, unsigned int posX, unsigned int posY)
//Secuencia de inicialización
	:posX(posX), posY(posY), iniX(posX), iniY(posY), numLives(3), energyLeft(0), dirX(1), dirY(0),
	bufferX(0), bufferY(0), textRow(0), textCol(11), juego(juego), texture(texture){}


void const Pacman::render()
{
	//Creamos el rectángulo destino
	SDL_Rect destRec;
	destRec.x = posX * juego->getCellSize(0);
	destRec.y = posY * juego->getCellSize(1);
	destRec.w = juego->getCellSize(0);
	destRec.h = juego->getCellSize(1);
	
	texture->renderFrame(juego->getRenderer(), destRec, textRow, textCol);
}

void Pacman::update()
{
	//1.RESTAMOS ENERGÍA
	if (energyLeft > 0)
		energyLeft--;

	//2.MOVEMOS A PACMAN
	//Miramos si podemos coger la dirección del buffer
	int nx = posX + bufferX;
	int ny = posY + bufferY;
	if (juego->nextCell(nx, ny, true) && !bufferVacio())
	{
		//Cogemos la dirección
		dirX = bufferX;
		dirY = bufferY;
		//Vaciamos el buffer
		bufferX = bufferY = 0;
		//Movemos
		move(nx, ny);
	}

	//Si no, cogemos la actual
	else
	{
		nx = posX + dirX;
		ny = posY + dirY;
		if (juego->nextCell(nx, ny, true))
			move(nx, ny);
	}
}

void Pacman::move(unsigned int x, unsigned int y)
{
	posX = x;
	posY = y;
	animation();
}

void Pacman::die()
{
	//Le quitamos una vida y llamamos a GameOver si es necesario
	numLives--;
	if (numLives < 1)
		juego->gameOver();
	else
	{
		//Lo movemos a la posición y dirección de inicio
		posX = iniX;
		posY = iniY;
		dirX = 1;
		dirY = 0;
		textRow = 0;
		textCol = 11;
	}
}

void Pacman::setDir(int x, int y)
{
	bufferX = x;
	bufferY = y;
}

void Pacman::animation()
{
	//Columna (cambia en cada frame)
	if (textCol == 10)
		textCol++;
	else
		textCol--;

	//Fila (depende de la dirección)
	if (dirX == 1)
		textRow = 0;
	else if (dirX == -1)
		textRow = 2;
	else if (dirY == -1)
		textRow = 3;
	else
		textRow = 1;
}

Pacman::~Pacman()
{

}



#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	

#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

#include "Enemigo.h"
#include "Nave.h"
#include "Disparo.h"
#include "Global.h"
#include "BackGround.h"

#include "Highscores.h"

using namespace std;

//GLOBALS
enum KEYS { LEFT, RIGHT, UP, DOWN, SPACE };
bool keys[5] = { false,false,false };
int estadoJuego = 1;
extern int width;
extern int height;
int numConVida = NUM_COLUMNAS*NUM_FILAS;
int EnemyWaveCount = 0;
int input = 0;	
int output = 0;	
int x = 20;	
int marcador = 0;
char *nombreString;
bool startAnim = false;
bool isHighscore = false;
int lowScore = 0;
const int FPS = 90;
bool done = false;
bool redraw = true;
int contadorDestruidos = 0;

//metodos
void setEnemigo();
void reiniciarJuego();
void colisionEnemigo(int&);
void colisionJugador();
void disparoEnemigo();
void actualizarDisparo();
bool reachEnd();
void InitBackground(BackGround &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image);
void UpdateBackground(BackGround &back);
void DrawBackground(BackGround &back);
void Reactivar_Enemigos();
void DrawAnimation(ALLEGRO_BITMAP *pic, int &y);
void writeScore();
void readScores();

void actualizarContadorEnemigos();
void verificarPosicionEnemigo();


//inicial
Nave jugador(width / 5, height / 2);
Disparo	jugadorDisparo(jugador.x_pos, jugador.y_pos, 5, true);
Disparo	enemigoDisparo(0, 0, 10, false);
Enemigo	arrEnem[NUM_COLUMNAS][NUM_FILAS];	//array de objetos
Highscores arrScores[11];
BackGround BG;
BackGround MG;
BackGround FG;
BackGround MM;



/*
menú del juego
*/
int main(void)
{
	readScores();
	setEnemigo();
	enemigoDisparo.isFriendly = false;
	srand((unsigned)time(NULL));


	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize allegro", NULL, NULL);
		return -1;
	}

	//Allegro variables
	ALLEGRO_DISPLAY *DISPLAY = NULL;
	ALLEGRO_BITMAP *imagVida[7];
	ALLEGRO_BITMAP *imagNave = NULL;
	ALLEGRO_BITMAP *imagBala = NULL;
	ALLEGRO_BITMAP *imagGreenLazer = NULL;
	ALLEGRO_BITMAP *imagEnemigo = NULL;
	ALLEGRO_BITMAP *Game = NULL;
	ALLEGRO_BITMAP *MENU = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *mgImage = NULL;
	ALLEGRO_BITMAP *fgImage = NULL;
	ALLEGRO_BITMAP *animNave;
	ALLEGRO_BITMAP *logo;
	ALLEGRO_BITMAP *State3 = NULL;
	ALLEGRO_BITMAP *State4 = NULL;

	ALLEGRO_EVENT_QUEUE *TestQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font38 = NULL;
	ALLEGRO_FONT *starFont = NULL;
	ALLEGRO_FONT *starOutline = NULL;
	ALLEGRO_FONT *fontName = NULL;
	ALLEGRO_SAMPLE *blaster = NULL;
	ALLEGRO_SAMPLE *explosion = NULL;
	ALLEGRO_SAMPLE *startGame = NULL;
	ALLEGRO_USTR* str = al_ustr_new("Ingrese su nombre");

	//Allegro Module 
	al_init_image_addon();
	al_set_new_window_position(400, 200);
	DISPLAY = al_create_display(width, height);
	al_hide_mouse_cursor(DISPLAY);
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();

	if (!DISPLAY)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize display", NULL, NULL);
		return -1;
	}

	//Sonido
	al_reserve_samples(7);
	blaster = al_load_sample("Sounds/XWing-Laser.ogg");
	explosion = al_load_sample("Sounds/Blast.ogg");
	startGame = al_load_sample("Sounds/xwing.ogg");
	State3 = al_load_bitmap("Imagenes/EARTH.png");
	State4 = al_load_bitmap("Imagenes/DeathStar.png");

	//Imagenes
	imagBala = al_load_bitmap("Imagenes/Lazer.png");
	imagGreenLazer = al_load_bitmap("Imagenes/GreenLazer1.png");
	imagNave = al_load_bitmap("Imagenes/player3.png");
	imagEnemigo = al_load_bitmap("Imagenes/enemy.png");
	MENU = al_load_bitmap("Imagenes/starBG.png");

	imagVida[0] = al_load_bitmap("Imagenes/1.png");
	imagVida[1] = al_load_bitmap("Imagenes/2.png");
	imagVida[2] = al_load_bitmap("Imagenes/3.png");
	imagVida[3] = al_load_bitmap("Imagenes/4.png");
	imagVida[4] = al_load_bitmap("Imagenes/5.png");
	imagVida[5] = al_load_bitmap("Imagenes/6.png");
	imagVida[6] = al_load_bitmap("Imagenes/blank1.png");
	for (int i = 0; i < 7; i++)
		al_convert_mask_to_alpha(imagVida[i], al_map_rgb(0, 0, 0));

	bgImage = al_load_bitmap("Imagenes/starBG.png");
	mgImage = al_load_bitmap("Imagenes/starMG.jpg");
	fgImage = al_load_bitmap("Imagenes/starFG.png");
	animNave = al_load_bitmap("Imagenes/player3.png");
	logo = al_load_bitmap("Imagenes/logo.png");

	al_convert_mask_to_alpha(imagNave, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(imagEnemigo, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(imagBala, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(mgImage, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(animNave, al_map_rgb(0, 0, 0));

	InitBackground(BG, 0, 0, 1, 0, 800, 600, -1, 1, bgImage);
	InitBackground(MG, 0, 0, 3, 0, 2000, 768, -1, 1, mgImage);
	InitBackground(FG, 0, 0, 5, 0, 800, 600, -1, 1, fgImage); 
	InitBackground(MM, 0, 0, 1, 0, 800, 600, -1, 1, MENU);

	timer = al_create_timer(1.0 / FPS);

	al_set_display_icon(DISPLAY, imagNave);

	TestQueue = al_create_event_queue();
	al_register_event_source(TestQueue, al_get_keyboard_event_source());
	al_register_event_source(TestQueue, al_get_display_event_source(DISPLAY));
	al_register_event_source(TestQueue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	int frameCount = 0;
	int pos = (int)al_ustr_size(str);
	font38 = al_load_font("Fonts/Legacy.ttf", 38, 0);
	starFont = al_load_font("Fonts/STARWARS.TTF", 80, 0);
	starOutline = al_load_font("Fonts/STARWARS.TTF", 81, 0);
	fontName = al_load_font("Fonts/Legacy.ttf", 50, 0);

	while (!done)
	{
		ALLEGRO_EVENT GETKEY;
		al_wait_for_event(TestQueue, &GETKEY);

		if (GETKEY.type == ALLEGRO_EVENT_DISPLAY_CLOSE)	// permite hacer clic en el botón X para cerrar el programa
		{
			done = true;
		}

		else if (GETKEY.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			UpdateBackground(MM);
			frameCount++;
			if (keys[LEFT] && estadoJuego == 2)
				jugador.MoverNaveIzq();
			if (keys[RIGHT] && estadoJuego == 2)
				jugador.MoverNaveDer();
			if (keys[UP] && estadoJuego == 2)
				jugador.MoverNaveArriba();
			if (keys[DOWN] && estadoJuego == 2)
				jugador.MoverNaveAbajo();
			if (keys[SPACE])
			{
				if (estadoJuego == 1) {
					al_play_sample(startGame, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					startAnim = true;
				}
				else if (estadoJuego == 2)
				{
					if (jugadorDisparo.status == 0)
					{
						al_play_sample(blaster, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						jugadorDisparo.status = 1;
					}
				}

			}
			if (estadoJuego == 2)
			{
				int temp = marcador;

				verificarPosicionEnemigo();   // reinicia los enemigos una vez que recorren toda la pantalla
				UpdateBackground(BG);
				UpdateBackground(MG);
				UpdateBackground(FG);
				colisionEnemigo(marcador);
				actualizarContadorEnemigos();
				colisionJugador();
				disparoEnemigo();
				actualizarDisparo();

				if (temp != marcador)
					al_play_sample(explosion, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); // sonido de explocion de los enemigos
			}
		}


		else if (GETKEY.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (GETKEY.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:  //esc para terminar el juego
				if (estadoJuego == 2)
					estadoJuego = 3;
				else if (estadoJuego == 3)
					estadoJuego = 4;
				else
					done = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				break;
			case ALLEGRO_KEY_H:
				if (estadoJuego == 1)
					estadoJuego = 4;
				break;
			case ALLEGRO_KEY_BACKSPACE:	
				if (estadoJuego == 3)
					if (al_ustr_prev(str, &pos))
						al_ustr_truncate(str, pos);
				break;
			case ALLEGRO_KEY_ENTER:	
				if (estadoJuego == 3)
				{
					estadoJuego = 4;
					break;
				}

				if (estadoJuego == 4)
				{
					reiniciarJuego();
					al_ustr_free(str);
					str = al_ustr_new("Ingrese su nombre");
					break;
				}
				break;
			}
		}

		if (GETKEY.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (GETKEY.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}

		else if (GETKEY.type == ALLEGRO_EVENT_KEY_CHAR && estadoJuego == 3 && isHighscore)
		{
			if (GETKEY.keyboard.unichar >= 32)
			{
				if (input == 0)
				{
					al_ustr_remove_range(str, 0, al_ustr_length(str));
					pos = 0;
					input = 1;
				}
				pos += al_ustr_append_chr(str, GETKEY.keyboard.unichar);
			}
		}

		if (redraw && al_is_event_queue_empty(TestQueue)) 
		{
			redraw = false;
			if (estadoJuego == 1)  //menú
			{
				DrawBackground(MM);

				if (startAnim)
				{
					DrawAnimation(animNave, x);
				}

				al_draw_text(font38, al_map_rgb(255, 40, 78), width / 2, height - 750, ALLEGRO_ALIGN_CENTRE, "Shotter Espacial");
				al_draw_bitmap(logo, 140, (height)-690, 0);
				al_draw_text(font38, al_map_rgb(255, 40, 78), (width / 2), (height)-250, ALLEGRO_ALIGN_CENTRE, "Espacion para iniciar");
				al_draw_text(font38, al_map_rgb(255, 40, 78), (width / 2), (height - 200), ALLEGRO_ALIGN_CENTRE, "H para ver el top puntuaciones");
				al_draw_text(font38, al_map_rgb(255, 40, 78), (width / 2), (height - 150), ALLEGRO_ALIGN_CENTRE, "ESC para salir");
			}

			else if (estadoJuego == 2)	//menú juego
			{
				DrawBackground(BG);
				DrawBackground(MG);
				DrawBackground(FG);


				if (jugadorDisparo.status == 1 && jugador.active)
				{
					jugadorDisparo.Increment();	
					al_draw_bitmap(imagGreenLazer, jugadorDisparo.x_pos, jugadorDisparo.y_pos, 0);
					if (jugadorDisparo.y_pos < 20)
					{
						jugadorDisparo.status = 0;
						actualizarDisparo();
					}
				}

				if (enemigoDisparo.status == 1)
				{
					enemigoDisparo.Increment();
					al_draw_bitmap(imagBala, enemigoDisparo.x_pos, enemigoDisparo.y_pos, 0);
				}

				switch (jugador.health)
				{
				case 60:
					al_draw_bitmap(imagVida[0], 10, 40, 0);
					break;
				case 50:
					al_draw_bitmap(imagVida[1], 10, 40, 0);
					break;
				case 40:
					al_draw_bitmap(imagVida[2], 10, 40, 0);
					break;
				case 30:
					al_draw_bitmap(imagVida[3], 10, 40, 0);
					break;
				case 20:
					al_draw_bitmap(imagVida[4], 10, 40, 0);
					break;
				case 10:
					al_draw_bitmap(imagVida[5], 10, 40, 0);
					break;
				case 0:
					al_draw_bitmap(imagVida[6], 10, 40, 0);
					break;
				}

				if (jugador.health > 0)
				{
					al_draw_bitmap(imagNave, jugador.x_pos - 45, jugador.y_pos, 0);
				}

				bool test = reachEnd();
				for (int i = 0; i < NUM_COLUMNAS; i++)
				{


					for (int j = 0; j < NUM_FILAS; j++)
					{

						if (arrEnem[i][j].active)
						{
							int mov = rand() % 100;
							if (mov%2 == 0)
							{
								arrEnem[i][j].MoverUp(test);
								al_draw_bitmap(imagEnemigo, arrEnem[i][j].x_pos, arrEnem[i][j].y_pos, 0);
							}
							else
							{
								arrEnem[i][j].MoverDOWN(test);
								al_draw_bitmap(imagEnemigo, arrEnem[i][j].x_pos, arrEnem[i][j].y_pos, 0);
							}
						}
					}

				}

				al_draw_textf(font38, al_map_rgb(255, 0, 0), 10, 0, 0, "PUNTOS: %i", marcador);
				al_draw_textf(font38, al_map_rgb(255, 0, 0), 250, 0, 0, "Destridos: %i", contadorDestruidos);
				
			}

			else if (estadoJuego == 3) //fin del juego
			{
				al_draw_bitmap(State3, 0, 0, ALLEGRO_ALIGN_CENTRE);
				DrawAnimation(animNave, x);
				if (jugador.health == 0)
				{
					al_play_sample(explosion, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					jugador.health = 1;
				}

				if (marcador>lowScore)
					isHighscore = true;

				nombreString = al_cstr_dup(str);
				al_draw_textf(starFont, al_map_rgb(45,119,166), width / 2, 50, ALLEGRO_ALIGN_CENTRE, "Puntos: %i", marcador);
				
				if (isHighscore)
					al_draw_text(fontName, al_map_rgb_f(1, 1, 1), width / 2, 120, ALLEGRO_ALIGN_CENTRE, nombreString);
			}

			else if (estadoJuego == 4)	//marcadores
			{
				al_draw_bitmap(State4, 0, 0, 0);
				al_draw_text(font38, al_map_rgb(255, 255, 0), width/2, height-40, ALLEGRO_ALIGN_CENTRE, "ENTER PARA VOLVER");
				
				if (isHighscore && output == 0)
				{
					writeScore();
					output = 1;
				}

				al_draw_textf(starFont, al_map_rgb(255, 255, 0), width / 2, 40, ALLEGRO_ALIGN_CENTRE, "TOP 10");

				for (int i = 0; i < 10; i++)
				{
					al_draw_textf(font38, al_map_rgb(255, 0, 0), width / 2 - 60, 120 + i * 60, ALLEGRO_ALIGN_RIGHT, "%i", arrScores[i].marc);
					al_draw_textf(font38, al_map_rgb(255, 0, 0), width / 2 + 60, 120 + i * 60, ALLEGRO_ALIGN_LEFT, "%s", arrScores[i].nombre.c_str());
				}

			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));	
		}
	}

	al_destroy_sample(blaster);
	al_destroy_sample(explosion);
	al_destroy_sample(startGame);

	al_destroy_event_queue(TestQueue);
	al_destroy_timer(timer);
	al_destroy_display(DISPLAY);

	al_destroy_font(font38);
	al_destroy_font(starFont);
	al_destroy_font(fontName);

	al_destroy_bitmap(Game);
	al_destroy_bitmap(State3);
	al_destroy_bitmap(State4);
	al_destroy_bitmap(MENU);
	al_destroy_bitmap(imagEnemigo);
	al_destroy_bitmap(imagNave);
	al_destroy_bitmap(imagBala);
	al_destroy_bitmap(imagGreenLazer);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(mgImage);
	al_destroy_bitmap(fgImage);
	al_destroy_bitmap(animNave);
	al_destroy_bitmap(logo);
	for (int i = 0; i < 7; i++)
		al_destroy_bitmap(imagVida[i]);

	return 0;
}

/*
	Verifica si la nave del jugador es goleada por una bala enemiga mediante el uso de coordenadas de ambos elementos
*/
void colisionJugador()
{
	if (enemigoDisparo.status)
	{
		if (enemigoDisparo.y_pos  < jugador.y_pos + jugador.boxheight && enemigoDisparo.y_pos > jugador.y_pos - jugador.boxheight
			&& enemigoDisparo.x_pos < jugador.x_pos + jugador.boxright && enemigoDisparo.x_pos > jugador.x_pos - jugador.boxleft)
		{
			enemigoDisparo.status = 0;	
			jugador.health -= 10;
		}
	}

	if (jugador.health == 0)
	{
		jugador.active = false;
		estadoJuego = 3;
	}
}

/*
	Realiza el posicionamiento de los enemigos en lugares de incio fijop para que puede inciar su movimiento
*/
void setEnemigo()
{

	arrEnem[0][0].set(width - sp, 15);	
	arrEnem[1][0].set(width, 15);
	arrEnem[2][0].set(width + sp, 15);	
	arrEnem[3][0].set(width - 2 * sp, 15);
	arrEnem[4][0].set(width + 2 * sp, 15);
	arrEnem[5][0].set(width - 3 * sp, 15);
	

	arrEnem[0][1].set(width - sp, 140);	
	arrEnem[1][1].set(width, 140);	
	arrEnem[2][1].set(width + sp, 140);
	arrEnem[3][1].set(width - 2 * sp, 140);
	arrEnem[4][1].set(width + 2 * sp, 140);
	arrEnem[5][1].set(width - 3 * sp, 140);
	

	arrEnem[0][2].set(width - sp, 235);	
	arrEnem[1][2].set(width, 235);	
	arrEnem[2][2].set(width + sp, 235);	 
	arrEnem[3][2].set(width - 2 * sp, 235);
	arrEnem[4][2].set(width + 2 * sp, 235);
	arrEnem[5][2].set(width - 3 * sp, 235);
	

	arrEnem[0][3].set(width - sp, 355);
	arrEnem[1][3].set(width, 355);
	arrEnem[2][3].set(width + sp, 355);
	arrEnem[3][3].set(width - 2 * sp, 355);
	arrEnem[4][3].set(width + 2 * sp, 355);
	arrEnem[5][3].set(width - 3 * sp, 355);
	
}

/*
	verifica cuando los enemigos ha avandonado la pantalla por el margen izquierdo 
	para ser nuevamente inicializadas 
*/
void verificarPosicionEnemigo()
{
	for (int i = 0; i < NUM_COLUMNAS; i++)
	{
		for (int j = 0; j < NUM_FILAS; j++)
		{
			if (arrEnem[i][j].x_pos <= -650)
			{
				Reactivar_Enemigos();
				setEnemigo();
				actualizarContadorEnemigos();
				
			}
		}
	}

}

/*
	verifica la colisión de una nave enemiga con la bala del jugador
*/
void colisionEnemigo(int &score)
{
	if (jugadorDisparo.status)
	{
		for (int i = 0; i < NUM_COLUMNAS; i++)
		{
			for (int j = 0; j < NUM_FILAS; j++)	
			{
				if (arrEnem[i][j].active)
				{
					if (jugadorDisparo.y_pos - 25 < arrEnem[i][j].y_pos + arrEnem[i][j].boxheight && jugadorDisparo.y_pos > arrEnem[i][j].y_pos - arrEnem[i][j].boxheight
						&& jugadorDisparo.x_pos < arrEnem[i][j].x_pos + arrEnem[i][j].boxright && jugadorDisparo.x_pos > arrEnem[i][j].x_pos - arrEnem[i][j].boxleft)
					{
						jugadorDisparo.status = 0;	
						arrEnem[i][j].active = false;
						contadorDestruidos++;

						switch (j)
						{
						case 0:
							score += 40;
							break;
						case 1:
							score += 20;
							break;
						case 2:
							score += 20;
							break;
						default:
							score += 10;
							break;
						}
						numConVida--;
					}
					
				}
			}
		}
	}
}

/*
	realiza los disparos de las naves enemigas segun las posiciónes en que se encuentran sus coordenadas
*/
void disparoEnemigo()
{
	if (!enemigoDisparo.status)
	{
		for (int i = 0; i < NUM_COLUMNAS; i++)
		{
			for (int j = 0; j < NUM_FILAS; j++)
			{
				if (arrEnem[i][j].active)
				{
					int randCol = rand() % NUM_COLUMNAS;
					int randRow = rand() % NUM_FILAS;
					if (randCol == i && randRow == j)
					{
						enemigoDisparo.UpdatebulletPos(arrEnem[i][j]);
						enemigoDisparo.status = 1;
						break;
					}
				}
			}
		}
	}
}

/*
	muestra el proyectil en pantalla 
*/
void actualizarDisparo()
{
	if (jugadorDisparo.status == 0)	// mientras que la bala no está activa establece la posición de la nave como posición inicial de la bala
		jugadorDisparo.UpdateBulletPos(jugador);
}

/*
	verifica los limites de la pantalla para las naves enemigas
*/
bool reachEnd()	//returns true si algún enemigo golpea cualquiera de los lados
{
	for (int i = 0; i < NUM_COLUMNAS; i++)
	{
		for (int j = 0; j < NUM_FILAS; j++)
		{
			if (arrEnem[i][j].active)
			{
				if (width - 80 < arrEnem[i][j].x_pos && arrEnem[i][j].speed > 0)
				{
					return true;
					break;
				}
				else if (10 > arrEnem[i][j].x_pos && arrEnem[i][j].speed < 0)
				{
					return true;
					break;
				}
			}
		}
	}
	return false;
}

/*
	Funciónes de posicion y moviminto del fonde de pantalla
*/
void InitBackground(BackGround &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image)
{
	back.x = x;
	back.y = y;
	back.velX = velx;
	back.velY = vely;
	back.WIDTH = width;
	back.HEIGHT = height;
	back.dirX = dirX;
	back.dirY = dirY;
	back.image = image;
}

void UpdateBackground(BackGround &back)
{
	back.x += back.velX * back.dirX;

	if (back.WIDTH < 1024 && back.x + back.WIDTH <= 100)
		back.x = 0;
	else if (back.WIDTH > 1024 && back.x + back.WIDTH <= 0)
		back.x = 0;
}

void DrawBackground(BackGround &back)
{
	al_draw_bitmap(back.image, back.x, back.y, 0);
	al_draw_bitmap(back.image, back.x + back.WIDTH, back.HEIGHT, 0);
	al_draw_bitmap(back.image, back.x, back.HEIGHT, 0);
	al_draw_bitmap(back.image, back.x + back.WIDTH, back.y, 0);
}

/*
	crea nuevamente a los enemigos que han sido destruidos
*/
void Reactivar_Enemigos()
{
	for (int i = 0; i < NUM_COLUMNAS; i++)
	{
		for (int j = 0; j < NUM_FILAS; j++)
		{
			arrEnem[i][j].active = true;
		}
	}
}

/*
	atualiza el contador de enemigos en pantalla 
	en el momento que son eliminados todos un una ola de enemigos se crea una nueva ola de enemigos
*/
void actualizarContadorEnemigos()
{
	if (numConVida == 0)
	{
		EnemyWaveCount++;
		numConVida = NUM_COLUMNAS*NUM_FILAS;
		Reactivar_Enemigos();
		setEnemigo();

		switch (jugador.health)
		{
		case 50:
			jugador.health += 10;
			break;
		case 40:
			jugador.health += 20;
			break;
		case 30:
			jugador.health += 20;
			break;
		case 20:
			jugador.health += 20;
			break;
		case 10:
			jugador.health += 20;
			break;
		}
	}
}

/*
	realiza una pequeña animacion al finalizar la partida de una nave volando infinitamente
*/
void DrawAnimation(ALLEGRO_BITMAP *pic, int &x)
{
	al_draw_bitmap(pic, x, 350, 0);
	

	if (estadoJuego == 1)
	{
		estadoJuego = 2;
	}
	else if (estadoJuego == 3)
	{
		x = x + 3;
		if (x > width - 5)
			x = -10;
	}
}

/*
	Lee el archivo de texto donde estan almacenadas las puntuaciónes
*/
void readScores()
{
	ifstream myfile("scores.txt");
	string line;
	Highscores temp;
	int count = 0;

	while (myfile.good())
	{
		getline(myfile, line); 
		temp.marc = stoi(line.substr(0, (int)line.find(' ')));
		temp.nombre = line.substr((int)line.find(' ') + 1);

		arrScores[count].nombre = temp.nombre;
		arrScores[count].marc = temp.marc;
		count++;
	}

	myfile.close();
	lowScore = arrScores[9].marc;
}

/*
	Escribe nuevos puntajes en el archivo de texto siempre y cuando se encuentren entre el Top 10 de mejores puntuacioes
*/
void writeScore()
{
	int pos = 9 - 1;
	int i = 0;

	while (marcador > arrScores[pos].marc && pos >= 0)
	{
		arrScores[pos + 1] = arrScores[pos];
		pos--;
	}
	arrScores[pos + 1].marc = marcador;
	arrScores[pos + 1].nombre = nombreString;

	ofstream OutputFile("scores.txt");
	for (i = 0; i < 9; i++)
	{
		if (arrScores[i].nombre == " ")
			arrScores[i].nombre = "NO NAME";
		OutputFile << arrScores[i].marc << " " << arrScores[i].nombre << endl;
	}

	if (arrScores[i].nombre == " ")
		arrScores[i].nombre = "NO NAME";
	OutputFile << arrScores[i].marc << " " << arrScores[i].nombre;

	OutputFile.close();
}

/*
	reinicia nuevamente el juego con los valores restaurados
*/
void reiniciarJuego()
{
	estadoJuego = 1;
	readScores();	//lee los marcadores del archivo de texto
	Reactivar_Enemigos();
	setEnemigo();
	jugador.active = true;
	jugador.health = 60;
	marcador = 0;
	numConVida = NUM_COLUMNAS*NUM_FILAS;
	EnemyWaveCount = 0;
	input = 0;
	output = 0;	
	x = 20;	
	startAnim = false;
	isHighscore = false;
	contadorDestruidos = 0;
}


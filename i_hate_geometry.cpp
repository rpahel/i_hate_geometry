#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <unordered_map>
#include <time.h>
#include "windows.h"
#include "maths.h"
#include "struct.h"
#include "updates.h"
#include "spawns.h"

std::string getAppPath();
std::string getAssetsPath(std::string appPath);

int main()
{
	srand(time(NULL)); // Génère la seed pour les rand()

	sf::RenderWindow window(sf::VideoMode(1200, 900),"I hate geometry"); // Taille et nom de la fenêtre
	window.setVerticalSyncEnabled(true); // Bloque les fps à 60

	int wallThickness = 20; // Largeur des murs

	// Début génération et placement des murs
	sf::RectangleShape wallSouth(sf::Vector2f(1200, wallThickness));
	wallSouth.setFillColor(sf::Color(100,100,100));
	wallSouth.setPosition(0, 900 - wallThickness);

	sf::RectangleShape wallNorth(sf::Vector2f(1200, wallThickness));
	wallNorth.setFillColor(sf::Color(100, 100, 100));

	sf::RectangleShape wallEast(sf::Vector2f(wallThickness, 900));
	wallEast.setFillColor(sf::Color(100, 100, 100));
	wallEast.setPosition(1200 - wallThickness, 0);

	sf::RectangleShape wallWest(sf::Vector2f(wallThickness, 900));
	wallWest.setFillColor(sf::Color(100, 100, 100));
	// Fin génération et placement des murs

	sf::FloatRect boundingBoxes[4] {wallSouth.getGlobalBounds(), wallNorth.getGlobalBounds(), wallEast.getGlobalBounds(), wallWest.getGlobalBounds()}; // On prend les hitbox des murs

	Game game; // On créer la struct game contenant toutes les listes et autres paramètres de la partie.
	Player player; // On créer la struct player
	Mouse mouse; // On créer la struct mouse

	game.currentLevel = 1; // Niveau actuel. Début = 1
	game.numberOfEnemies = 1; // Nombre d'ennemis au premier niveau
	game.numberOfItems = 2; // Nombre d'items
	game.isNewRoom = true; // Est-ce que c'est une nouvelle pièce ? Début = oui
	game.isPaused = false; // Le jeu démarre pas en pause.

	game.font.loadFromFile(getAssetsPath(getAppPath()) + "arial.ttf"); // On charge le style de font voulu
	game.levelText.setFont(game.font); // On assigne le font au texte
	game.levelText.setPosition(1080,50); // On assigne une position au texte
	game.levelText.setCharacterSize(24); // On assigne une taille de police
	game.levelText.setString("level " + std::to_string(game.currentLevel)); //On affecte un texte au text

	game.pauseText.setFont(game.font); // On assigne le font au texte
	game.pauseText.setPosition(600, 350); // On assigne une position au texte
	game.pauseText.setCharacterSize(50); // On assigne une taille de police

	game.button1.setSize(sf::Vector2f(100, 40)); // On assigne une taille au bouton
	game.button1.setFillColor(sf::Color::Black); // On le rend transparent
	game.button1.setOutlineColor(sf::Color::White); // On rend ces contours blancs
	game.button1.setOutlineThickness(2); // On assigne une épaisseur aux contours
	game.button1.setOrigin(game.button1.getSize().x / 2, game.button1.getSize().y / 2); // On met son origine au centre de la forme
	game.button1.setPosition(500, 450); // On le place

	game.button2.setSize(sf::Vector2f(100, 40)); // On assigne une taille au bouton
	game.button2.setFillColor(sf::Color::Black); // On le rend transparent
	game.button2.setOutlineColor(sf::Color::White); // On rend ces contours blancs
	game.button2.setOutlineThickness(2); // On assigne une épaisseur aux contours
	game.button2.setOrigin(game.button1.getSize().x / 2, game.button1.getSize().y / 2); // On met son origine au centre de la forme
	game.button2.setPosition(700, 450); // On le place

	game.restartText.setFont(game.font); // On assigne le font au texte
	game.restartText.setCharacterSize(24); // On assigne une taille de police
	game.restartText.setString("Restart"); // On affecte un texte au text
	game.restartText.setOrigin(game.restartText.getLocalBounds().width / 2, game.restartText.getLocalBounds().height - 2); // On met l'origine du texte en son centre
	game.restartText.setPosition(game.button1.getPosition()); // On assigne une position au texte

	game.quitText.setFont(game.font); // On assigne le font au texte
	game.quitText.setCharacterSize(24); // On assigne une taille de police
	game.quitText.setString("Quit"); // On affecte un texte au text
	game.quitText.setOrigin(game.quitText.getLocalBounds().width / 2, game.quitText.getLocalBounds().height - 2); // On met l'origine du texte en son centre
	game.quitText.setPosition(game.button2.getPosition()); // On assigne une position au texte

	player = SpawnPlayer();

	while (window.isOpen())
	{
		// Inputs
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::A)         // Si on appuie sur A, qqchose se passe (utile pour les tests)
				{
					std::cout << "A" << std::endl;
					game.enemies.clear();
					game.bosses.clear();
					game.currentLevel = 4;
				}

				if (event.key.code == sf::Keyboard::Escape && !player.isDead)         // Si on appuie sur échap, pause le jeu.
				{
					if(!game.isPaused)
					{
						game.isPaused = true;
					}
					else
					{
						game.isPaused = false;
					}
				}

				break;

			default:
				break;
			}
		}

		// Début setup
		if(game.isNewRoom) // Quand c'est une nouvelle salle, on initialise le joueur et les ennemis.
		{
			if(game.currentLevel % 5 != 0)
			{
				SpawnEnemies(game, wallThickness);
				SpawnItems(game, wallThickness);
			}
			else
			{
				SpawnBoss(game);
				SpawnItems(game, wallThickness);
			}

			game.isNewRoom = false;
		}

		if (game.enemies.empty() && game.bosses.empty()) // Si on a tué tout les ennemis, charge une nouvelle room
		{
			game.enemies.clear();
			game.bosses.clear();
			game.bullets.clear();
			game.enemyBullet.clear();
			game.items.clear();
			game.particles.clear();
			player.playerSpeed = 300.f;
			player.shape.setPosition(600, 450);
			game.currentLevel++;
			game.levelText.setString("level " + std::to_string(game.currentLevel));
			game.timeSinceStartLevel = 0;
			game.isNewRoom = true;
		}
		// Fin setup

		// Début logique
		game.deltaTime = game.clock.restart(); // Calcul du temps ecoule depuis la derniere boucle
		game.timeSinceStartLevel += game.deltaTime.asSeconds(); // Calcul du temps depuis le début du level

		if(game.isPaused)
		{
			game.deltaTime = game.deltaTime.Zero;
		}

		if(!player.isDead)
		{
			PlayerMovement(player, game.deltaTime.asSeconds()); // On update la position du joueur en fonction des inputs
		}
		else
		{
			player.shape.setRadius(0);
		}

		for (auto it = game.enemies.begin(); it != game.enemies.end(); ++it) // Pour chaque ennemi...
		{
			UpdateEnemyState(*it, game.deltaTime.asSeconds()); // On l'update

			if (it->moveCD <= 0) // Si moveCD est inférieur ou égal à 0...
			{
				it->direction = RandomDirection(); // On change la direction de l'ennemi
				it->moveCD = it->moveDuration;
			}

			MoveEnemies(*it, game.deltaTime.asSeconds()); // On déplace l'ennemi

			if (it->fireCD <= 0 && !player.isDead) // Si fireCD est inférieur ou égal à 0...
			{
				switch (it->type) // En fonction du type de l'ennemi...
				{
					case 0:
						SpawnEnemiesBullet(game, *it, player.shape, 0); // On fait apparaitre une balle ennemie
						break;
					case 1:
						for(int i=0; i < 2; ++i)
						{
							SpawnEnemiesBullet(game, *it, player.shape, i); // i représente le numéro de la balle, utile pour determiner sa direction
						}
						break;
					default:
						SpawnEnemiesBullet(game, *it, player.shape, 0);
						break;
				}

				it->fireCD = it->fireRate;
			}
		}

		for (auto it = game.bullets.begin(); it != game.bullets.end(); ++it) // Pour chaque balle...
		{
			MoveBullets(*it, game.deltaTime.asSeconds()); // On déplace la balle
		}

		for (auto it = game.enemyBullet.begin(); it != game.enemyBullet.end(); ++it) // Pour chaque balle ennemie...
		{
			MoveEnemyBullets(*it, game.deltaTime.asSeconds()); // On déplace la balle
		}

		for (auto it = game.particles.begin(); it != game.particles.end();) // Pour chaque particule...
		{
			MoveParticles(*it, game.deltaTime.asSeconds()); // On déplace la particule

			it->lifeTime -= game.deltaTime.asSeconds(); // On réduit deltaTime de lifeTime

			if (it->lifeTime <= 0) // Quand lifeTime atteint 0...
			{
				it = game.particles.erase(it); // On efface la particule de la liste et on se met sur la nouvelle particule
			}
			else
			{
				++it; // On passe à la prochaine particule
			}
		}

		for (auto it = game.particles.begin(); it != game.particles.end(); ++it) // Pour chaque particule...
		{
			MoveParticles(*it, game.deltaTime.asSeconds()); // On déplace la particule
		}

		for (auto it = game.bosses.begin(); it != game.bosses.end(); ++it)
		{
			UpdateBossState(*it, game.deltaTime.asSeconds(), game, player);
		//
		//	if (it->isMoving)
		//	{
		//		MoveBoss(*it, player.shape, game.deltaTime.asSeconds()); // On déplace le(s) boss
		//	}
		//
		//	else if (it->isShooting)
		//	{
		//		UpdateBossState(*it, game.deltaTime.asSeconds());
		//		std::cout << it->fireCD << std::endl;
		//
		//		if (it->myState == it->isMoving)
		//		{
		//			MoveBoss(*it, player.shape, game.deltaTime.asSeconds()); // On déplace le(s) boss
		//			std::cout << "moving" << std::endl;
		//		}
		//
		//		else if (it->myState == it->isShooting)
		//		{
		//			std::cout << "shooting" << std::endl;
		//			if (it->fireCD <= 0)
		//			{
		//				SpawnBossBullet(game, *it, player.shape, rand() % 20 + 5);
		//				it->fireCD = it->fireRate;
		//			}
		//		}
		//
		//		else if (it->myState == it->isBlocking)
		//		{
		//			//Fais apparaître le shield du boss
		//			std::cout << "blocking" << std::endl;
		//
		//			if (it->name == "shield_")
		//			{
		//				window.draw(it->shape);
		//				RotateShield(*it, game.deltaTime.asSeconds());
		//			}
		//		}
		//	}
		}

		UpdatePlayerState(player, game.deltaTime.asSeconds()); // On update les valeurs du player à update

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // Si on clique sur LMB...
		{
			UpdateMousePos(mouse, window); // On update les coordonnées de la souris

			if(game.isPaused || player.isDead)
			{
				if(mouse.x >= game.button1.getPosition().x - (game.button1.getSize().x/2) && mouse.x <= game.button1.getPosition().x + (game.button1.getSize().x / 2)
					&& mouse.y >= game.button1.getPosition().y - (game.button1.getSize().y / 2) && mouse.y <= game.button1.getPosition().y + (game.button1.getSize().y / 2)) // Si la souris est dans l'espace occupé par le rectangle...
				{
					RestartGame(game, player); // On appelle la fonction RestartGame
					player = SpawnPlayer();
					game.isPaused = false; // On dépause le jeu
					player.fireCD = player.fireRate; // Sert juste à pas tirer quand on clique sur le bouton
				}

				if (mouse.x >= game.button2.getPosition().x - (game.button2.getSize().x / 2) && mouse.x <= game.button2.getPosition().x + (game.button2.getSize().x / 2)
					&& mouse.y >= game.button2.getPosition().y - (game.button2.getSize().y / 2) && mouse.y <= game.button2.getPosition().y + (game.button2.getSize().y / 2)) // Si la souris est dans l'espace occupé par le rectangle...
				{
					window.close();
				}
			}

			if (player.fireCD <= 0.f && !player.isDead && !game.isPaused) // Si fireCD est inférieur ou égal à 0 et que le joueur n'est pas mort et que le jeu n'est pas en pause...
			{
				SpawnBullet(game, player, mouse); // On spawn une balle
				player.fireCD = player.fireRate; // On remet fireCoolDown à fireRate
			}
		}
			
		CheckAllTheCollisions(player, game, boundingBoxes); // On check toutes les collisions (sauf entre les ennemis)
		// Fin logique
		
		// Rendu
		window.clear(); // On nettoie la fenêtre

		for(auto it = game.enemies.begin(); it != game.enemies.end(); ++it) // Pour chaque enemy...
		{
			window.draw(it->shape); // On l'affiche
		}

		for (auto it = game.bullets.begin(); it != game.bullets.end(); ++it) // Pour chaque bullet...
		{
			window.draw(it->shape); // On l'affiche
		}

		for (auto it = game.enemyBullet.begin(); it != game.enemyBullet.end(); ++it) // Pour chaque enemyBullet...
		{
			window.draw(it->shape); // On l'affiche
		}

		for (auto it = game.items.begin(); it != game.items.end(); ++it) // Pour chaque item...
		{
			window.draw(it->shape); // On l'affiche
		}

		for (auto it = game.particles.begin(); it != game.particles.end(); ++it) // Pour chaque particle...
		{
			window.draw(it->shape); // On l'affiche
		}

		for (auto it = game.bosses.begin(); it != game.bosses.end(); ++it) // Pour chaque boss...
		{
			window.draw(it->shape);
		}

		for (auto it = game.bossShields.begin(); it != game.bossShields.end(); ++it) // Pour chaque shield du boss;
		{
			window.draw(it->shape);
		}

		for (auto it = game.bossCacs.begin(); it != game.bossCacs.end(); ++it) // Pour chaque corp à corp du boss;
		{
			window.draw(it->shape);
		}

		if(!player.isDead) // Si le joueur n'est pas mort...
		{
			window.draw(player.shape); // On le dessine
		}

		// Début affichage des murs
		window.draw(wallNorth);
		window.draw(wallEast);
		window.draw(wallSouth);
		window.draw(wallWest);
		// Fin affichage des murs

		window.draw(game.levelText); // On affiche le texte

		if(player.isDead)
		{
			game.pauseText.setString("Game Over."); //On affecte un texte au text
			game.pauseText.setOrigin(game.pauseText.getLocalBounds().width / 2, game.pauseText.getLocalBounds().height / 2); // On met l'origine du texte en son centre
			window.draw(game.pauseText); // On affiche le texte
			window.draw(game.button1); // On affiche le texte
			window.draw(game.button2); // On affiche le texte
			window.draw(game.restartText); // On affiche le texte
			window.draw(game.quitText); // On affiche le texte
		}

		if (game.isPaused)
		{
			game.pauseText.setString("Paused."); //On affecte un texte au text
			game.pauseText.setOrigin(game.pauseText.getLocalBounds().width / 2, game.pauseText.getLocalBounds().height / 2); // On met l'origine du texte en son centre
			window.draw(game.pauseText); // On affiche le texte
			window.draw(game.button1); // On affiche le texte
			window.draw(game.button2); // On affiche le texte
			window.draw(game.restartText); // On affiche le texte
			window.draw(game.quitText); // On affiche le texte
		}

		// Fin Rendu

		window.display(); // On affiche à l'écran tous les draw
	}
}

std::string getAppPath() // On va chercher l'endroit où se trouve l'appli
{
	char cExeFilePath[256];
	GetModuleFileNameA(NULL, cExeFilePath, 256);
	std::string exeFilePath = cExeFilePath;
	int exeNamePos = exeFilePath.find_last_of("\\/");
	std::string appPath = exeFilePath.substr(0, exeNamePos + 1);
	return appPath;
}

std::string getAssetsPath(std::string appPath) // On va chercher l'endroit où se trouve les assets
{
	std::string assetsPath = appPath + "Assets\\";
	return assetsPath;
}
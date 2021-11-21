#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
	soundManager sound; //On créer la struct sound
	
	sound.principalBuffer.loadFromFile(getAssetsPath(getAppPath()) + "song1.wav");
	sound.principalSound.setBuffer(sound.principalBuffer);
	sound.principalSound.setVolume(75.f);
	sound.principalSound.play();

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

	player = SpawnPlayer(); // On fait spawn le joueur

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
					LoadLevel(game, player, 5); // On va directement au level 5
				}

				if (event.key.code == sf::Keyboard::Escape && !player.isDead)         // Si on appuie sur échap, pause le jeu.
				{
					if (!game.isPaused)
					{
						game.isPaused = true;
					}
					else
					{
						game.isPaused = false;
						sound.principalSound.play(); //On relance la musique principale
					}

					break;

			default:
				break;
				}
			}
		}

		// Début setup
		if(game.isNewRoom) // Quand c'est une nouvelle salle, on initialise le joueur et les ennemis.
		{
			if (game.currentLevel % 5 == 0)
			{
				SpawnBoss(game);
				SpawnItems(game, wallThickness);
				game.isNewRoom = false;
			}
			else
			{
				SpawnEnemies(game, wallThickness);
				SpawnItems(game, wallThickness);
				game.isNewRoom = false;
			}
		}

		if (game.enemies.empty() && game.bosses.empty()) // Si on a tué tout les ennemis, charge une nouvelle room
		{
			LoadLevel(game, player, game.currentLevel + 1);
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
			player.shape.setRadius(0); // On mets son rayon à 0 pour qu'ils n'entre plus en collision avec quoi que ce soit
		}

		for (auto it = game.enemies.begin(); it != game.enemies.end(); ++it) // Pour chaque ennemi...
		{
			UpdateEnemyState(*it, game.deltaTime.asSeconds(), player.shape, game); // On l'update

			if (it->moveCD <= 0) // Si moveCD est inférieur ou égal à 0...
			{
				it->direction = RandomDirection(); // On change la direction de l'ennemi
				it->moveCD = it->moveDuration;
			}

			MoveEnemies(*it, game.deltaTime.asSeconds()); // On déplace l'ennemi
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

		for (auto it = game.bosses.begin(); it != game.bosses.end();) // Pour chaque boss..
		{
			UpdateBossState(*it, game.deltaTime.asSeconds(), game, player); // On update le boss

			if(it->health <= 0) // Si le boss n'a plus de vie..
			{
				it = game.bosses.erase(it); // On le supprime de la liste
			}
			else
			{
				++it; // On passe au prochain boss
			}
		}

		UpdatePlayerState(player, game.deltaTime.asSeconds()); // On update les valeurs du player à update

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // Si on clique sur LMB...
		{
			UpdateMousePos(mouse, window); // On update les coordonnées de la souris

			if(game.isPaused || player.isDead) // Si le jeu est en pause ou si le joueur est mort...
			{
				if(mouse.x >= game.button1.getPosition().x - (game.button1.getSize().x/2) && mouse.x <= game.button1.getPosition().x + (game.button1.getSize().x / 2)
					&& mouse.y >= game.button1.getPosition().y - (game.button1.getSize().y / 2) && mouse.y <= game.button1.getPosition().y + (game.button1.getSize().y / 2)) // Si la souris est dans l'espace occupé par le rectangle...
				{
					sound.principalSound.play(); //On relance la musique principale
					LoadLevel(game, player, 1); // On appelle la fonction RestartGame
					player = SpawnPlayer(); // On respawn le joueur
					game.isPaused = false; // On dépause le jeu
					player.fireCD = player.fireRate; // Sert juste à pas tirer quand on clique sur le bouton
				}

				if (mouse.x >= game.button2.getPosition().x - (game.button2.getSize().x / 2) && mouse.x <= game.button2.getPosition().x + (game.button2.getSize().x / 2)
					&& mouse.y >= game.button2.getPosition().y - (game.button2.getSize().y / 2) && mouse.y <= game.button2.getPosition().y + (game.button2.getSize().y / 2)) // Si la souris est dans l'espace occupé par le rectangle...
				{
					window.close(); // On ferme la fenêtre
				}
			}

			if (player.fireCD <= 0.f && !player.isDead && !game.isPaused) // Si fireCD est inférieur ou égal à 0 et que le joueur n'est pas mort et que le jeu n'est pas en pause...
			{
				SpawnBullet(game, player, mouse); // On spawn une balle
				//LoadSong(game, getAssetsPath(getAppPath()) + "playerShoot.wav", game.deltaTime.asSeconds(), 0);
				player.fireCD = player.fireRate; // On remet fireCoolDown à fireRate
			}
		}

		for (std::list<soundManager>::iterator it = game.playerShootSound.begin(); it != game.playerShootSound.end();)
		{
			it->playerShootSoundLifetime -= game.deltaTime.asSeconds(); // On réduit deltaTime de lifeTime du son du tir du joueur

			if (it->playerShootSoundLifetime <= 0) // Quand lifeTime atteint 0...
			{
				it = game.playerShootSound.erase(it); // On efface le son de la liste et on se met sur le nouveau son
			}
			else
			{
				++it; // On passe au prochain son
			}
		}

		for (std::list<soundManager>::iterator it = game.enemyShootSound.begin(); it != game.enemyShootSound.end();)
		{
			it->enemyShootLifetime -= game.deltaTime.asSeconds(); // On réduit deltaTime de lifeTime du son du tir de l'ennemi

			if (it->enemyShootLifetime <= 0) // Quand lifeTime atteint 0...
			{
				it = game.enemyShootSound.erase(it); // On efface le son de la liste et on se met sur le nouveau son
			}
			else
			{
				++it; // On passe au prochain son
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
			window.draw(it->itemText);
		}

		for (auto it = game.particles.begin(); it != game.particles.end(); ++it) // Pour chaque particle...
		{
			window.draw(it->shape); // On l'affiche
		}

		for (auto it = game.bosses.begin(); it != game.bosses.end(); ++it) // Pour chaque boss...
		{
			window.draw(it->shape); // On l'affiche
			window.draw(it->shapeHealthBar); // On affiche sa barre de vie
			window.draw(it->shapeContenerHealthBar); // On affiche le conteneur de la barre de vie
		}

		for (auto it = game.bossShields.begin(); it != game.bossShields.end(); ++it) // Pour chaque shield du boss
		{
			window.draw(it->shape); // On l'affiche
		}

		for (auto it = game.bossCacs.begin(); it != game.bossCacs.end(); ++it) // Pour chaque corp à corp du boss
		{
			window.draw(it->shape); // On l'affiche
		}

		if(!player.isDead) // Si le joueur n'est pas mort...
		{
			window.draw(player.shape); // On l'affiche
		}

		// Début affichage des murs
		window.draw(wallNorth);
		window.draw(wallEast);
		window.draw(wallSouth);
		window.draw(wallWest);
		// Fin affichage des murs

		window.draw(game.levelText); // On affiche le texte

		if(player.isDead) // Si le joueur est mort ...
		{
			sound.principalSound.pause(); //On arrete la musique principale
			game.pauseText.setString("Game Over."); //On affecte un texte au text
			game.pauseText.setOrigin(game.pauseText.getLocalBounds().width / 2, game.pauseText.getLocalBounds().height / 2); // On met l'origine du texte en son centre
			window.draw(game.pauseText); // On affiche le game over
			window.draw(game.button1); // On affiche le button 1
			window.draw(game.button2); // On affiche le button 2
			window.draw(game.restartText); // On affiche le texte du button 1
			window.draw(game.quitText); // On affiche le texte du button 2
		}

		if (game.isPaused)
		{
			sound.principalSound.pause(); //On arrete la musique principale
			game.pauseText.setString("Paused."); //On affecte un texte au text
			game.pauseText.setOrigin(game.pauseText.getLocalBounds().width / 2, game.pauseText.getLocalBounds().height / 2); // On met l'origine du texte en son centre
			window.draw(game.pauseText); // On affiche le pause
			window.draw(game.button1); // On affiche le button 1
			window.draw(game.button2); // On affiche le button 2
			window.draw(game.restartText); // On affiche le texte du button 1
			window.draw(game.quitText); // On affiche le texte du button 2
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
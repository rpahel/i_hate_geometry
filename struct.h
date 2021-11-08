#pragma once

struct Player {

	int life;
	int speed;
	int damage;
};

enum PlayerState {
	Alive,
	Dead,
	Moving,
	Attacking,
};

struct Enemy{

	int life;
	int speed;
	int damage;
};

enum EnemyState {
	Alive,
	Dead,
	Moving,
	Attacking,
};

struct Item {
	int heal;
	int damage;
	int gainSpeed;
	int lossSpeed;
};

struct bullet {
	int bulletSpeed;
	int bulletDamage;
	int bulletSize;
	int bulletNumber;
};


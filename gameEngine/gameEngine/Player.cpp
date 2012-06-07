#include "Player.h"


Player::Player(void)
{
}


Player::~Player(void)
{
}


string Player::getName(){
	return name;
}

void Player::setName(string name){
	Player::name = name;
}

vector<int> Player::getScores(){
	return scores
}

void Player::addNewScore(int score){
	scores.push_back(score);
}

void Player::updateScore(int score){
	scores.back() = score;
}
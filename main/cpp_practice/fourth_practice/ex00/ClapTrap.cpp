#include "ClapTrap.hpp"

ClapTrap::ClapTrap(string name_c): name(name_c), health(10),energy(10),
    damage(0){
cout    << "ClapTrap " << name << " is spawned." << endl;
}

ClapTrap::ClapTrap(ClapTrap &copy){    
*this = copy;
cout    << "ClapTrap copy constructor called" << endl;
}

ClapTrap &ClapTrap::operator=(ClapTrap &assignment){
this->name = assignment.name;
this->health = assignment.health;
this->energy = assignment.energy;
this->damage = assignment.damage;
cout    << "ClapTrap copy assignment operator called" << endl;
return *this;
}

ClapTrap::~ClapTrap(){
cout    << "ClapTrap " << name << " is disappeared." << endl;
}

void ClapTrap::attack(const string &target){
if (health <= 0){
    cout    << "ClapTrap " << name << " is dead." << endl;
    return;
}
if (energy <= 0){
    cout    << "ClapTrap " << name << " is not have energy." << endl;
    return ;
}
cout    << "ClapTrap " << name << " attacks " << target << ", causing "
        << damage << " points of damage!" << endl;
energy--;
if (energy == 0){
    cout    << "ClapTrap " << name << "'s energy is over." << endl;
}
}

void ClapTrap::takeDamage(unsigned int amount){
if (health <= 0){
    cout    << "ClapTrap " << name << " is dead" << endl;
    return ;
}
health -= amount;
cout    << "ClapTrap " << name << " is took " << amount << " damage."
        << endl;
if (health <= 0)
    cout    << "ClapTrap " << name << " is died." << endl;
}

// max health;
void ClapTrap::beRepaired(unsigned int amount){
if (health <= 0){
    cout    << "ClapTrap " << name << " is dead." << endl;
    return ;
}
if (energy <= 0){
    cout    << "ClapTrap " << name << " is not have energy." << endl;
    return ;
}
health += amount;
cout    << "ClapTrap " << name << " is " << amount << " unit healed."
        << endl;
energy--;
if (energy == 0){
    cout    << "ClapTrap " << name << "'s energy is over." << endl;
}
}
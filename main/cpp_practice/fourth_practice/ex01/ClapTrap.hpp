#ifndef CLAPTRAP_H
# define CLAPTRAP_H
# if defined(__linux__)
/* Linux. --------------------------------------------------- */

# elif defined(__APPLE__)
/* OSX. --------------------------------------------------- */

# endif

# define RED				"\x1B[31m"
# define GREEN				"\x1B[32m"
# define YELLOW				"\x1B[33m"
# define BLUE				"\x1B[34m"
# define RESET				"\x1B[0m"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

class ClapTrap
{
private:
    string  name;
    int     health;
    int     energy;
    int     damage;
public:
    ClapTrap(string name_c);
    ClapTrap(ClapTrap &copy);
    ClapTrap &operator=(ClapTrap &assignment);
    ~ClapTrap();
    void attack(const string& target);
    void takeDamage(unsigned int amount);
    void beRepaired(unsigned int amount);
};



#endif

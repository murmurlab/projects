# include "./depend.hpp"
# include "./PhoneBook.hpp"

PhoneBook::PhoneBook(){
cout << WELCOME << endl;
cout << CMD_LST << endl;
older = 0;}

int PhoneBook::add( void ){
string  tmp[5];

if (older > MAX_CONTACTS - 1)
    older = 0;
cout << PROMPT_FNAME;
cin >> tmp[0];
if (cin.eof()){
    return -1;}
cout << PROMPT_LNAME;
cin >> tmp[1];
if (cin.eof()){
    return -1;}
cout << PROMPT_NNAME;
cin >> tmp[2];
if (cin.eof()){
    return -1;}
cout << PROMPT_PHONE;
cin >> tmp[3];
if (cin.eof()){
    return -1;}
cout << PROMPT_SEC;
cin >> tmp[4];
if (cin.eof()){
    return -1;}
contacts[older].set_first_name(tmp[0]);
contacts[older].set_last_name(tmp[1]);
contacts[older].set_nick_name(tmp[2]);
contacts[older].set_phone_num(tmp[3]);
contacts[older].set_secret(tmp[4]);
older++;
return (0);}

int PhoneBook::printVertical(Contact &contact, size_t idx){
cout    << GREEN NORMAL_LEDGE << std::NORMAL_ALIGN << setw(NORMAL_KEY_SIZE) << std::setfill(NORMAL_BLANK) << idx
        << NORMAL_SEPARATOR << std::NORMAL_ALIGN << setw(NORMAL_KEY_SIZE) << std::setfill(NORMAL_BLANK) << truncate(contact.get_first_name(), NORMAL_KEY_SIZE, true)
        << NORMAL_SEPARATOR << std::NORMAL_ALIGN << setw(NORMAL_KEY_SIZE) << std::setfill(NORMAL_BLANK) << truncate(contact.get_last_name(), NORMAL_KEY_SIZE, true)
        << NORMAL_SEPARATOR << std::NORMAL_ALIGN << setw(NORMAL_KEY_SIZE) << std::setfill(NORMAL_BLANK) << truncate(contact.get_nick_name(), NORMAL_KEY_SIZE, true)
        << NORMAL_REDGE RESET;
return 0;}

int PhoneBook::printHorizontal(Contact &contact, int swicth){
switch (swicth){
case 1:
    cout    << GREEN
            << DETAIL_LEDGE << std::DETAIL_ALIGN << setw(DETAIL_KEY_SIZE) << std::setfill(DETAIL_BLANK) << FNAME
            << DETAIL_SEPARATOR << std::DETAIL_ALIGN << setw(DETAIL_VALUE_SIZE) << std::setfill(DETAIL_BLANK) << truncate(contact.get_first_name(), DETAIL_VALUE_SIZE, true)
            << DETAIL_REDGE RESET;
break;
case 2:
    cout    << GREEN
            << DETAIL_LEDGE << std::DETAIL_ALIGN << setw(DETAIL_KEY_SIZE) << std::setfill(DETAIL_BLANK) << LNAME
            << DETAIL_SEPARATOR << std::DETAIL_ALIGN << setw(DETAIL_VALUE_SIZE) << std::setfill(DETAIL_BLANK) << truncate(contact.get_last_name(), DETAIL_VALUE_SIZE, true)
            << DETAIL_REDGE RESET;
break;
case 3:
    cout    << GREEN
            << DETAIL_LEDGE << std::DETAIL_ALIGN << setw(DETAIL_KEY_SIZE) << std::setfill(DETAIL_BLANK) << NNAME
            << DETAIL_SEPARATOR << std::DETAIL_ALIGN << setw(DETAIL_VALUE_SIZE) << std::setfill(DETAIL_BLANK) << truncate(contact.get_nick_name(), DETAIL_VALUE_SIZE, true)
            << DETAIL_REDGE RESET;
break;
case 4:
    cout    << GREEN
            << DETAIL_LEDGE << std::DETAIL_ALIGN << setw(DETAIL_KEY_SIZE) << std::setfill(DETAIL_BLANK) << PNUME
            << DETAIL_SEPARATOR << std::DETAIL_ALIGN << setw(DETAIL_VALUE_SIZE) << std::setfill(DETAIL_BLANK) << truncate(contact.get_phone_num(), DETAIL_VALUE_SIZE, true)
            << DETAIL_REDGE RESET;
break;
case 5:
    cout    << GREEN
            << DETAIL_LEDGE << std::DETAIL_ALIGN << setw(DETAIL_KEY_SIZE) << std::setfill(DETAIL_BLANK) << sec
            << DETAIL_SEPARATOR << std::DETAIL_ALIGN << setw(DETAIL_VALUE_SIZE) << std::setfill(DETAIL_BLANK) << truncate(contact.get_secret(), DETAIL_VALUE_SIZE, true)
            << DETAIL_REDGE RESET;
break;
default:
    cerr << INVALID_SELECTION << endl << CMD_LST;
break;}return 0;}

int PhoneBook::search( void ){
string	search_inp;
char    *left;
int     idx;

printCeiling();
for (size_t i = 0; i < MAX_CONTACTS - 1; i++){
    printVertical(contacts[i], i);
    wall_wextra2();}
printVertical(contacts[MAX_CONTACTS - 1], MAX_CONTACTS - 1);
cout<<endl;
printFloor();
cout << ASK_INDEX << endl;
cin >> search_inp;
if (cin.eof()){
    return 1;}
idx = strtol(search_inp.c_str(), &left, 10);
if (*left != '\0')
    idx = -1;
if (idx == 0 && search_inp[0] != '0')
    return cout << INVALID_SEARCH << ": " << idx << endl, 1;
if (idx < 0 || idx > MAX_CONTACTS - 1)
    return cout << INVALID_SEARCH << ": " << idx << endl, 1;
printCeiling2();
for (size_t i = 1; i <= MAX_PROPERTY - 1; i++){
    printHorizontal(contacts[idx], i);
    wall_wextra();}
printHorizontal(contacts[idx], MAX_PROPERTY);
cout<<endl;
printFloor2();
return (0);}

int PhoneBook::exit( void ){
cout << SUCC_EXIT << endl;
return (0);}